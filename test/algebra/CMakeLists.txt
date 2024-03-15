cmake_minimum_required(VERSION 3.28)

set(SOURCES vector.cpp matrix.cpp quaternion.cpp)

set(WORK_DIR "/mnt/main_disk/code")
set(BOOST_LOCAL "${WORK_DIR}/extern_libs/boost_1_84_0-clang")
set(ENV{LD_LIBRARY_PATH} ${LD_LIBRARY_PATH}:${BOOST_LOCAL}/lib)
include_directories (${BOOST_LOCAL})
link_directories(${BOOST_LOCAL}/lib)

add_executable(algebra_test ${SOURCES})
