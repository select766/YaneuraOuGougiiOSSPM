import re
from pathlib import Path

def get_encoding(path: Path):
    if path.name == "halfkp_vm_256x2-32-32.h":
        return "cp932"
    else:
        return "utf-8-sig" # BOMを削除して読む

def special_patch(source: str, path: Path):
    if path.name == "types.h":
        source = source.replace("::to_usi_string", "YANEURAOU_GOUGI_NAMESPACE::to_usi_string")
    elif path.name == "key128.h":
        """
        テンプレートの特殊化がグローバルスコープでないとエラーになる
        template <>
        struct std::hash<Key128>
        """
        source = source.replace("""template <>
struct std::hash<Key128> {
	size_t operator()(const Key128& k) const {
		// 下位bit返すだけで良いのでは？
		return (size_t)(k.extract64<0>());
	}
};
""", """}
template <>
struct std::hash<YANEURAOU_GOUGI_NAMESPACE::Key128> {
	size_t operator()(const YANEURAOU_GOUGI_NAMESPACE::Key128& k) const {
		// 下位bit返すだけで良いのでは？
		return (size_t)(k.extract64<0>());
	}
};
namespace YANEURAOU_GOUGI_NAMESPACE {
""")
        source = source.replace("""template <>
struct std::hash<Key256> {
	size_t operator()(const Key256& k) const {
		// 下位bit返すだけで良いのでは？
		return (size_t)(k.extract64<0>());
	}
};
""", """}
template <>
struct std::hash<YANEURAOU_GOUGI_NAMESPACE::Key256> {
	size_t operator()(const YANEURAOU_GOUGI_NAMESPACE::Key256& k) const {
		// 下位bit返すだけで良いのでは？
		return (size_t)(k.extract64<0>());
	}
};
namespace YANEURAOU_GOUGI_NAMESPACE {
""")
    # elif path.name.startswith("makebook"):
    #     # グローバルスコープのmate_inなどを置換
    #     source = re.sub("(?<!\w)::(mate_in|mated_in)", "YANEURAOU_GOUGI_NAMESPACE::\\1", source)
    elif path.name == "config.h":
        # makebook*.cppに書き換え箇所があるのでそもそもビルド対象から外す
        source = source.replace("#define ENABLE_MAKEBOOK_CMD", "")
    elif path.name == "thread.h":
        source = source + """
void register_iostream_thread(int engine_id);
#ifndef REGISTER_IOSTREAM_THREAD
#define REGISTER_IOSTREAM_THREAD
inline void register_iostream_thread()
{
#if defined(YANEURAOU_ENGINE_DEEP)
register_iostream_thread(0);
#elif defined(YANEURAOU_ENGINE_NNUE)
register_iostream_thread(1);
#else
#error YANEURAOU_ENGINE_DEEP nor YANEURAOU_ENGINE_NNUE is defined.
#endif
}
#endif
"""
    elif path.name == "thread.cpp":
        source = source.replace("void Thread::idle_loop() {", """void Thread::idle_loop() {
register_iostream_thread();
""")
    elif path.name == "yaneuraou-search.cpp":
        source = source.replace("::search<Root>", "YANEURAOU_GOUGI_NAMESPACE::search<Root>")
        # FV_SCALEを水匠5に合わせた24にする
        source = source.replace('o["FV_SCALE"] << Option(16, 1, 128);', 'o["FV_SCALE"] << Option(24, 1, 128);')
    elif path.name == "evaluate_nnue.cpp":
        # FV_SCALEを水匠5に合わせた24にする
        source = source.replace("int FV_SCALE = 16;", "int FV_SCALE = 24;")
        source = source.replace("namespace Eval {", "extern std::string nnue_file_path;\nnamespace Eval {")
        source = source.replace("const std::string file_path = Path::Combine(dir_name, file_name);", "const std::string file_path = nnue_file_path;")
    elif path.name == "YaneuraOu_dlshogi_bridge.cpp":
        # DNN_Model1を空欄にする
        source = source.replace('R"(model.mlmodel)"', '""')

    return source

def modify_source(path: Path):
    OPEN_LINE = "namespace YANEURAOU_GOUGI_NAMESPACE {"
    CLOSE_LINE = "}"
    source_lines = path.read_text(encoding=get_encoding(path)).splitlines()
    out_lines = []
    out_lines.append(OPEN_LINE)
    for line in source_lines:
        if line == OPEN_LINE:
            raise ValueError("The source is already modified. Run 'git checkout .' in yaneuraou repository.")
        if re.match("\\s*#include", line):
            out_lines.append(CLOSE_LINE)
            out_lines.append(line)
            out_lines.append(OPEN_LINE)
        else:
            out_lines.append(line)
    out_lines.append(CLOSE_LINE)
    dst_source = "\n".join(out_lines)
    dst_source = special_patch(dst_source, path)
    path.write_text(dst_source, encoding="utf-8")

def main():
    source_dir = Path('YaneuraOu/source')
    source_paths = []
    # YaneuraOu/source/eval/deep/nn_coreml.mm は使用しない
    for pattern in ["**/*.cpp", "**/*.h", "**/*.hpp"]:
        source_paths.extend(source_dir.glob(pattern))
    for source_path in source_paths:
        print(str(source_path))
        modify_source(source_path)

if __name__ == '__main__':
    main()
