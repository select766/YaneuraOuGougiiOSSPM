cmake_minimum_required(VERSION 3.15)
project(yaneuraou)

set(CMAKE_CXX_STANDARD 17)


set (SRC
    ../../src/ios_main.cpp
    # not including main.cpp
    ../../../YaneuraOu/source/types.cpp
    ../../../YaneuraOu/source/bitboard.cpp
    ../../../YaneuraOu/source/misc.cpp
    ../../../YaneuraOu/source/movegen.cpp
    ../../../YaneuraOu/source/position.cpp
    ../../../YaneuraOu/source/usi.cpp
    ../../../YaneuraOu/source/usi_option.cpp
    ../../../YaneuraOu/source/thread.cpp
    ../../../YaneuraOu/source/tt.cpp
    ../../../YaneuraOu/source/movepick.cpp
    ../../../YaneuraOu/source/timeman.cpp
    ../../../YaneuraOu/source/book/book.cpp
    ../../../YaneuraOu/source/book/apery_book.cpp
    ../../../YaneuraOu/source/extra/bitop.cpp
    ../../../YaneuraOu/source/extra/long_effect.cpp
    ../../../YaneuraOu/source/extra/sfen_packer.cpp
    ../../../YaneuraOu/source/extra/super_sort.cpp
    ../../../YaneuraOu/source/mate/mate.cpp
    ../../../YaneuraOu/source/mate/mate1ply_without_effect.cpp
    ../../../YaneuraOu/source/mate/mate1ply_with_effect.cpp
    ../../../YaneuraOu/source/mate/mate_solver.cpp
    ../../../YaneuraOu/source/eval/evaluate_bona_piece.cpp
    ../../../YaneuraOu/source/eval/evaluate.cpp
    ../../../YaneuraOu/source/eval/evaluate_io.cpp
    ../../../YaneuraOu/source/eval/evaluate_mir_inv_tools.cpp
    ../../../YaneuraOu/source/eval/material/evaluate_material.cpp
    ../../../YaneuraOu/source/testcmd/unit_test.cpp
    ../../../YaneuraOu/source/testcmd/mate_test_cmd.cpp
    ../../../YaneuraOu/source/testcmd/normal_test_cmd.cpp
    ../../../YaneuraOu/source/testcmd/benchmark.cpp
    ../../../YaneuraOu/source/engine/yaneuraou-engine/yaneuraou-search.cpp
# NNUE
    ../../../YaneuraOu/source/eval/nnue/evaluate_nnue.cpp
    ../../../YaneuraOu/source/eval/nnue/evaluate_nnue_learner.cpp
    ../../../YaneuraOu/source/eval/nnue/nnue_test_command.cpp
    ../../../YaneuraOu/source/eval/nnue/features/k.cpp
    ../../../YaneuraOu/source/eval/nnue/features/p.cpp
    ../../../YaneuraOu/source/eval/nnue/features/half_kp.cpp
    ../../../YaneuraOu/source/eval/nnue/features/half_kp_vm.cpp
    ../../../YaneuraOu/source/eval/nnue/features/half_relative_kp.cpp
    ../../../YaneuraOu/source/eval/nnue/features/half_kpe9.cpp
    ../../../YaneuraOu/source/eval/nnue/features/pe9.cpp
    ../.dl/embedded_nnue.cpp
)
add_library(yaneuraou STATIC ${SRC})

# target_compile_options(yaneuraou PUBLIC -fno-exceptions -fno-rtti -Wextra -MMD -MP -fpermissive -Ofast -DNDEBUG -D_LINUX -DUNICODE -DNO_EXCEPTIONS -DMATERIAL_LEVEL=1 -DIS_64BIT -DUSE_AVX2 -DUSE_BMI2 -mbmi -mbmi2 -mavx2 -mpopcnt -DYANEURAOU_ENGINE_MATERIAL)
target_compile_options(yaneuraou PUBLIC -fno-exceptions -fno-rtti -Wextra -MMD -MP -fpermissive -Ofast -DNDEBUG -D_LINUX -DUNICODE -DNO_EXCEPTIONS -DIS_64BIT -DUSE_AVX2 -DUSE_BMI2 -mbmi -mbmi2 -mavx2 -mpopcnt -DYANEURAOU_ENGINE_NNUE -DEVAL_NNUE_KP256 -DEVAL_EMBEDDING -DYANEURAOU_GOUGI_NAMESPACE=yaneuraou_gougi_nnue)
# TODO: FV_SCALE=24
