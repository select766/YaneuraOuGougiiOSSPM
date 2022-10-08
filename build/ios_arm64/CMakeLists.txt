cmake_minimum_required(VERSION 3.15)
project(yaneuraou)

set(CMAKE_CXX_STANDARD 17)

set (SRC
    ../src/ios_main.cpp
    # not including main.cpp
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
    ../../YaneuraOu/source/engine/yaneuraou-engine/yaneuraou-search.cpp
)
add_library(yaneuraou STATIC ${SRC})

target_compile_options(yaneuraou PUBLIC -fno-exceptions -fno-rtti -Wextra -MMD -MP -fpermissive -Ofast -DNDEBUG -D_LINUX -DUNICODE -DNO_EXCEPTIONS -DMATERIAL_LEVEL=1 -DIS_64BIT -DUSE_NEON -DYANEURAOU_ENGINE_MATERIAL)
