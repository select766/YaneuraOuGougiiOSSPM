//#include <iostream>
//#include "bitboard.h"
//#include "position.h"
#include "../../YaneuraOu/source/search.h"
#include "../../YaneuraOu/source/thread.h"
#include "../../YaneuraOu/source/tt.h"
#include "../../YaneuraOu/source/usi.h"
#include "../../YaneuraOu/source/misc.h"

// cin/coutをsocketにリダイレクト
#include <iostream>
#include <mutex>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


namespace yaneuraou_gougi_deep {
	extern std::string modelc_url_cache;
	// 使用デバイス
	// MLComputeUnitsCPUOnly = 0,
	// MLComputeUnitsCPUAndGPU = 1,
	// MLComputeUnitsAll = 2
	// Allで損をする事例は見つかっていないが、選べるようにすることも考えられる。
	extern int coreml_compute_units_cache;
}

#if defined(YANEURAOU_ENGINE_DEEP)
namespace YANEURAOU_GOUGI_NAMESPACE {
	std::string modelc_url_cache;
	// 使用デバイス
	// MLComputeUnitsCPUOnly = 0,
	// MLComputeUnitsCPUAndGPU = 1,
	// MLComputeUnitsAll = 2
	// Allで損をする事例は見つかっていないが、選べるようにすることも考えられる。
	int coreml_compute_units_cache;
}
#endif


namespace yaneuraou_gougi_nnue {
	extern std::string nnue_file_path;
}

#if defined(YANEURAOU_ENGINE_NNUE)
namespace YANEURAOU_GOUGI_NAMESPACE {
	std::string nnue_file_path;
}
#endif

namespace yaneuraou_gougi_deep {
	void yaneuraou_ios_thread_main();
}

namespace yaneuraou_gougi_nnue {
	void yaneuraou_ios_thread_main();
}

namespace YANEURAOU_GOUGI_NAMESPACE {
	void yaneuraou_ios_thread_main() {
		register_iostream_thread();
		// --- 全体的な初期化
		int argc = 1;
		char *argv[] = {"yaneuraou"};
		CommandLine::init(argc,argv);
		USI::init(Options);
		Bitboards::init();
		Position::init();
		Search::init();

		// エンジンオプションの"Threads"があるとは限らないので…。
		size_t thread_num = Options.count("Threads") ? (size_t)Options["Threads"] : 1;
		Threads.set(thread_num);

		//Search::clear();
		Eval::init();

	#if !defined(__EMSCRIPTEN__)
		// USIコマンドの応答部

		USI::loop(argc, argv);

		// 生成して、待機させていたスレッドの停止

		Threads.set(0);
	#else
		// yaneuraOu.wasm
		// ここでループしてしまうと、ブラウザのメインスレッドがブロックされてしまうため、コメントアウト
	#endif
	}
}

// DEEP側のビルド時にエントリポイント関数をビルドする
#if defined(YANEURAOU_ENGINE_DEEP)
static const int n_engines = 2;
static int socket_fds[n_engines] = {0};
static std::mutex mutex_thread_engine_map;
static std::map<std::thread::id, int> thread_engine_map; // スレッドID -> エンジン番号(DEEP=0, NNUE=1)

static int socket_connect(const char* server_ip, int server_port) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(server_ip);
    sin.sin_port = htons(server_port);

    int ret = connect(socket_fd, (const struct sockaddr*)&sin, sizeof(sin));
    if (ret == -1) {
        std::cerr << "Failed to connect tcp server" << std::endl;
        return -1;
    }

    return socket_fd;
}

// static int socket_disconnect() {
//     if (socket_fd > 0) {
//         close(socket_fd);
//         socket_fd = -1;
//     }
// }

int get_socket_for_thread()
{
	auto id = std::this_thread::get_id();
	std::lock_guard<std::mutex> lock(mutex_thread_engine_map);
	auto it = thread_engine_map.find(id);
	if (it != thread_engine_map.end())
	{
		return socket_fds[it->second];
	}
	else
	{
		std::cerr << "get_socket_for_thread: thread " << id << " is not registered" << std::endl;
		return socket_fds[0];
	}
}

class myoutstreambuf : public std::streambuf {
public:
    myoutstreambuf() {
    }

protected:
    int overflow(int nCh = EOF) {
		if (nCh >= 0) {
			char c = (char)nCh;
			if (send(get_socket_for_thread(), &c, 1, 0) < 1) {
				return EOF;
			}
		}
        return nCh;
    }
};

class myinstreambuf : public std::streambuf {
public:
    myinstreambuf() {
    }

protected:
    int uflow() {
		char buf;
		if (recv(get_socket_for_thread(), &buf, 1, 0)) {
			return buf;
		} else {
			return EOF;
		}
    }
};

void register_iostream_thread(int engine_id)
{
	std::lock_guard<std::mutex> lock(mutex_thread_engine_map);
	thread_engine_map[std::this_thread::get_id()] = engine_id;
}


extern "C" int yaneuraou_ios_main(const char* deep_server_ip, int deep_server_port, const char* deep_modelc_url, int deep_coreml_compute_units, const char* nnue_server_ip, int nnue_server_port, const char* nnue_file_path) {
	// stdioを独自に切り替え
	myoutstreambuf *outs = new myoutstreambuf();
	myinstreambuf *ins = new myinstreambuf();
	std::cout.rdbuf(outs);
	std::cin.rdbuf(ins);
    yaneuraou_gougi_deep::modelc_url_cache = deep_modelc_url;
    yaneuraou_gougi_deep::coreml_compute_units_cache = deep_coreml_compute_units;
	yaneuraou_gougi_nnue::nnue_file_path = nnue_file_path;
	int ok_flag = 0;
	if ((socket_fds[0] = socket_connect(deep_server_ip, deep_server_port)) >= 0)
	{
		std::thread thread_deep(yaneuraou_gougi_deep::yaneuraou_ios_thread_main);
		thread_deep.detach();
		ok_flag |= 1 << 0;
	}
	if ((socket_fds[1] = socket_connect(nnue_server_ip, nnue_server_port)) >= 0)
	{
		std::thread thread_nnue(yaneuraou_gougi_nnue::yaneuraou_ios_thread_main);
		thread_nnue.detach();
		ok_flag |= 1 << 1;
	}
    return ok_flag;
}

#endif
