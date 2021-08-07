cmake_minimum_required(VERSION 3.20)
project(FistOfRhalgr)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GUI_FILES RhalgrGui.cpp)
set(NETWORK_FILES RhalgrNetwork.cpp)
set(DATA_FILES SkillCodes.hpp StatusEffectCodes.hpp RhalgrMemory.cpp)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

enable_testing()

add_executable(${PROJECT_NAME} main.cpp ${GUI_FILES} ${NETWORK_FILES} ${DATA_FILES})

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

add_executable(rhalgr_network_test test/RhalgrNetworkTest.cpp ${NETWORK_FILES})

target_link_libraries(rhalgr_network_test ${CONAN_LIBS})
include(GoogleTest)
gtest_discover_tests(rhalgr_network_test)
