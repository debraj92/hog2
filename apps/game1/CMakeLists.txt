cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(example)
set(
        xtl_DIR
        /Users/debrajray/MyComputer/RTS/xtl/build
        CACHE STRING "" FORCE
)
set(
        xtensor_DIR
        /Users/debrajray/MyComputer/RTS/xtensor/build
        CACHE STRING "" FORCE
)
set(
        xsimd_DIR
        /Users/debrajray/MyComputer/RTS/xsimd/build
        CACHE STRING "" FORCE
)

find_package(torch REQUIRED)
set(CMAKE_OSX_ARCHITECTURES "x86_64")


find_package(xsimd REQUIRED)
find_package(OpenMP REQUIRED)
find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)
find_package(xtensor REQUIRED PATHS /Users/debrajray/MyComputer/RTS/xtensor/build)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

add_definitions(-DXTENSOR_ENABLE_XSIMD)
add_definitions(-DXTENSOR_USE_OPENMP)

add_executable(example example.cpp gameEnv.h gameEnv.cpp fixedobstacles.h fixedobstacles.cpp gameConstants.h enemy/enemy.h enemy/fixedEnemy.cpp enemy/fixedEnemy.h gameSimulation.cpp gameSimulation.h findPath.cpp findPath.h player.cpp player.h state.h state.cpp rl_control.cpp rl_control.h observation.h coordinatesUtil.cpp coordinatesUtil.h
        plot/pbPlots.hpp plot/pbPlots.cpp plot/supportLib.hpp plot/supportLib.cpp objectLocator.cpp objectLocator.h observation.cpp AStar_.cpp AStar_.h AStarOpenList.cpp AStarOpenList.h node_.cpp node_.h)

target_include_directories(example PUBLIC ${xtensor_INCLUDE_DIRS})
target_link_libraries(example "${TORCH_LIBRARIES}" xtensor ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES} ${OPENGL_LIBRARY} OpenMP::OpenMP_CXX)

set_property(TARGET example PROPERTY CXX_STANDARD 14)
