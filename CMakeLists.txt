cmake_minimum_required(VERSION 3.17)
project(term_paper)

set(CMAKE_CXX_STANDARD 14)
include_directories(${CMAKE_SOURCE_DIR}/libs/gmp-6.2.1)

if (WIN32)
    add_executable(term_paper main.cpp include/WSEML.h src/WSEML.cpp src/parser.cpp include/parser.h include/pointers.h src/pointers.cpp include/dllconfig.h include/misc.h src/misc.cpp src/arch/win32/funcCall.cpp src/helpFunc.cpp include/helpFunc.h include/lists.h)
endif (WIN32)
if (UNIX)
    add_executable(term_paper main.cpp include/WSEML.h src/WSEML.cpp src/parser.cpp include/parser.h include/pointers.h src/pointers.cpp include/dllconfig.h include/misc.h src/misc.cpp src/arch/unix/funcCall.cpp src/helpFunc.cpp include/helpFunc.h include/lists.h)
endif (UNIX)
target_link_libraries(term_paper ${CMAKE_SOURCE_DIR}/libs/gmp-6.2.1/.libs/libgmpxx.a)
target_link_libraries(term_paper ${CMAKE_SOURCE_DIR}/libs/gmp-6.2.1/.libs/libgmp.a)