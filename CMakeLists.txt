cmake_minimum_required(VERSION 3.23)
project(PCS C)

set(CMAKE_C_STANDARD 99)

add_executable(PCS main.c pcs_struct_hash.c pcs_struct_hash.h pcs_storage.h
        pcs_storage.c pcs.h pcs.c random_functions.h random_functions.c
        needle_pcs.c needle_pcs.h needle_rho.c needle_rho.h pcg_basic.c pcg_basic.h collision_storage.c collision_storage.h)

link_directories("/opt/homebrew/lib")
include_directories("/opt/homebrew/include")

find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
    target_link_libraries(PCS PUBLIC ${MATH_LIBRARY})
endif()

find_path(GMP_INCLUDE_DIRS NAMES gmp.h PATHS $ENV{GMPDIR} ${INCLUDE_INSTALL_DIR})
find_library(GMP_LIBRARIES gmp PATHS $ENV{GMPDIR} ${LIB_INSTALL_DIR})
if(GMP_INCLUDE_DIRS AND GMP_LIBRARIES)
    target_link_libraries(PCS PUBLIC ${GMP_LIBRARIES})
endif()

target_include_directories(PCS PUBLIC ~/Downloads/pcg-c-master/include)
