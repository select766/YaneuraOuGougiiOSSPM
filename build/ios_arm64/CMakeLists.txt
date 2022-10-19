cmake_minimum_required(VERSION 3.15)
project(yaneuraou)

set(CMAKE_CXX_STANDARD 17)

set (SRC
    ../src/ios_main.cpp
    ../src/nn_coreml_ios.mm
    # not including main.cpp, nn_coreml.mm
    ../../YaneuraOu/source/types.cpp
    ../../YaneuraOu/source/bitboard.cpp
    ../../YaneuraOu/source/misc.cpp
    ../../YaneuraOu/source/movegen.cpp
    ../../YaneuraOu/source/position.cpp
    ../../YaneuraOu/source/usi.cpp
    ../../YaneuraOu/source/usi_option.cpp
    ../../YaneuraOu/source/thread.cpp
    ../../YaneuraOu/source/tt.cpp
    ../../YaneuraOu/source/movepick.cpp
    ../../YaneuraOu/source/timeman.cpp
    ../../YaneuraOu/source/book/book.cpp
    ../../YaneuraOu/source/book/apery_book.cpp
    ../../YaneuraOu/source/extra/bitop.cpp
    ../../YaneuraOu/source/extra/long_effect.cpp
    ../../YaneuraOu/source/extra/sfen_packer.cpp
    ../../YaneuraOu/source/extra/super_sort.cpp
    ../../YaneuraOu/source/mate/mate.cpp
    ../../YaneuraOu/source/mate/mate1ply_without_effect.cpp
    ../../YaneuraOu/source/mate/mate1ply_with_effect.cpp
    ../../YaneuraOu/source/mate/mate_solver.cpp
    ../../YaneuraOu/source/eval/evaluate_bona_piece.cpp
    ../../YaneuraOu/source/eval/evaluate.cpp
    ../../YaneuraOu/source/eval/evaluate_io.cpp
    ../../YaneuraOu/source/eval/evaluate_mir_inv_tools.cpp
    ../../YaneuraOu/source/eval/material/evaluate_material.cpp
    ../../YaneuraOu/source/testcmd/unit_test.cpp
    ../../YaneuraOu/source/testcmd/mate_test_cmd.cpp
    ../../YaneuraOu/source/testcmd/normal_test_cmd.cpp
    ../../YaneuraOu/source/testcmd/benchmark.cpp
    ../../YaneuraOu/source/book/makebook.cpp
    ../../YaneuraOu/source/book/makebook2015.cpp
    ../../YaneuraOu/source/book/makebook2019.cpp
    ../../YaneuraOu/source/book/makebook2021.cpp
    ../../YaneuraOu/source/learn/learner.cpp
    ../../YaneuraOu/source/learn/learning_tools.cpp
    ../../YaneuraOu/source/learn/multi_think.cpp
    ../../YaneuraOu/source/eval/deep/nn_types.cpp
    ../../YaneuraOu/source/eval/deep/nn.cpp
    ../../YaneuraOu/source/eval/deep/nn_onnx_runtime.cpp
    ../../YaneuraOu/source/eval/deep/nn_tensorrt.cpp
    ../../YaneuraOu/source/engine/dlshogi-engine/dlshogi_searcher.cpp
    ../../YaneuraOu/source/engine/dlshogi-engine/PrintInfo.cpp
    ../../YaneuraOu/source/engine/dlshogi-engine/UctSearch.cpp
    ../../YaneuraOu/source/engine/dlshogi-engine/Node.cpp
    ../../YaneuraOu/source/engine/dlshogi-engine/YaneuraOu_dlshogi_bridge.cpp
)
add_library(yaneuraou STATIC ${SRC})

target_compile_options(yaneuraou PUBLIC -fno-exceptions -fno-rtti -Wextra -MMD -MP -fpermissive -Ofast -DNDEBUG -D_LINUX -DUNICODE -DNO_EXCEPTIONS -DIS_64BIT -DUSE_NEON -fobjc-arc -DYANEURAOU_ENGINE_DEEP -DCOREML)
target_link_options(yaneuraou PUBLIC -framework Foundation -framework CoreML)
