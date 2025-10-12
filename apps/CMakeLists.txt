cmake_minimum_required(VERSION 3.30)

add_executable(barevk)
file(GLOB_RECURSE EXE_BAREVK_SRC "bare/*.cpp" "bare/*.h")
target_sources(barevk PRIVATE ${EXE_BAREVK_SRC})

#add_executable(qtvk)
#file(GLOB_RECURSE EXE_QTVK_SRC "qt/*.cpp" "qt/*.h")
#target_sources(qtvk PRIVATE ${EXE_QTVK_SRC})

