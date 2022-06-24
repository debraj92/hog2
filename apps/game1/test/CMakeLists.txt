cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(example_tests)

set(CMAKE_CXX_STANDARD 14)

set(This ExampleTests)

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

add_subdirectory(googletest)

enable_testing()

set(torch_DIR "/Users/debrajray/MyComputer/libtorch-latest-1.12/libtorch/share/cmake/Torch")

find_package(torch REQUIRED)
set(CMAKE_OSX_ARCHITECTURES "x86_64")

find_package(xsimd REQUIRED)
find_package(OpenMP REQUIRED)
find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)
find_package(xtensor REQUIRED PATHS /Users/debrajray/MyComputer/RTS/xtensor/build)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR} "./")

add_definitions(-DXTENSOR_ENABLE_XSIMD)
add_definitions(-DXTENSOR_USE_OPENMP)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_executable(${This} AStar_test.cpp GameMaps_test.cpp Observation_test.cpp ../gameEnv.h ../gameEnv.cpp ../fixedobstacles.h ../fixedobstacles.cpp ../gameConstants.h ../enemy/enemy.h ../enemy/fixedEnemy.cpp ../enemy/fixedEnemy.h ../gameSimulation.cpp ../gameSimulation.h ../findPath.cpp ../findPath.h ../player.cpp ../player.h ../state.h ../state.cpp ../rl_control.cpp ../rl_control.h ../observation.h ../coordinatesUtil.cpp ../coordinatesUtil.h
        ../plot/pbPlots.hpp ../plot/pbPlots.cpp ../plot/supportLib.hpp ../plot/supportLib.cpp ../objectLocator.cpp ../objectLocator.h ../observation.cpp ../AStar_.cpp ../AStar_.h ../AStarOpenList.cpp ../AStarOpenList.h ../node_.cpp ../node_.h Observation_test.cpp FullTrainingSingleAgent.cpp ../TestResult.h ../DQN/ReplayMemory.cpp ../DQN/ReplayMemory.h ../DQN/DQN_interface.h replayMemory_test.cpp Logging_test.cpp ../Logger.cpp ../Logger.h
        ../DQN/dueling-DQN/DQNNet.cpp ../DQN/dueling-DQN/DQNNet.h ../DQN/dueling-DQN/RLNN_Agent.cpp ../DQN/dueling-DQN/RLNN_Agent.h
        #../DQN/DDQN/DQNNet.cpp ../DQN/DDQN/DQNNet.h ../DQN/DDQN/RLNN_Agent.cpp ../DQN/DDQN/RLNN_Agent.h
        #../DQN/Vanilla-DQN/DQNNet.cpp ../DQN/Vanilla-DQN/DQNNet.h ../DQN/Vanilla-DQN/RLNN_Agent.cpp ../DQN/Vanilla-DQN/RLNN_Agent.h
        #../DQN/dueling-DQN-bounded/DQNNet.cpp ../DQN/dueling-DQN-bounded/DQNNet.h ../DQN/dueling-DQN-bounded/RLNN_Agent.cpp ../DQN/dueling-DQN-bounded/RLNN_Agent.h
        ../trainingMaps.cpp ../trainingMaps.h testing.h gameSimulation_test.cpp CNN_test.cpp ../FOV_CNN/CNN_controller.cpp ../FOV_CNN/CNN_controller.h)

target_include_directories(${This} PUBLIC ${GTEST_INCLUDE_DIR} ${xtensor_INCLUDE_DIRS})
target_link_libraries(${This} PUBLIC "${TORCH_LIBRARIES}" xtensor ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES} ${OPENGL_LIBRARY} OpenMP::OpenMP_CXX
        gtest
        gtest_main
        )

set_property(TARGET ${This} PROPERTY CXX_STANDARD 14)

add_test(
        NAME ${This}
        COMMAND ${This}
)

include(GoogleTest)
gtest_discover_tests(ExampleTests)