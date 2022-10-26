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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static int socket_fd;

std::string modelc_url_cache;
// 使用デバイス
// MLComputeUnitsCPUOnly = 0,
// MLComputeUnitsCPUAndGPU = 1,
// MLComputeUnitsAll = 2
// Allで損をする事例は見つかっていないが、選べるようにすることも考えられる。
int coreml_compute_units_cache;

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

class myoutstreambuf : public std::streambuf {
public:
    myoutstreambuf(int _socket_fd): socket_fd(_socket_fd) {
    }

protected:
    int overflow(int nCh = EOF) {
		if (nCh >= 0) {
			char c = (char)nCh;
			if (send(socket_fd, &c, 1, 0) < 1) {
				return EOF;
			}
		}
        return nCh;
    }
private:
	int socket_fd;
};

class myinstreambuf : public std::streambuf {
public:
    myinstreambuf(int _socket_fd): socket_fd(_socket_fd) {
    }

protected:
    int uflow() {
		char buf;
		if (recv(socket_fd, &buf, 1, 0)) {
			return buf;
		} else {
			return EOF;
		}
    }

private:
	int socket_fd;
};

static void yaneuraou_ios_thread_main() {
	myoutstreambuf outs(socket_fd);
	myinstreambuf ins(socket_fd);
	auto default_out = std::cout.rdbuf(&outs);
	auto default_in = std::cin.rdbuf(&ins);

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

	std::cout.rdbuf(default_out);
	std::cin.rdbuf(default_in);
}

extern "C" int yaneuraou_ios_main(const char* server_ip, int server_port, const char* modelc_url, int coreml_compute_units) {
    modelc_url_cache = modelc_url;
    coreml_compute_units_cache = coreml_compute_units;
	socket_fd = socket_connect(server_ip, server_port);
	if (socket_fd < 0) {
		return 1;
	}
    std::thread thread(yaneuraou_ios_thread_main);
    thread.detach();
    return 0;
}
