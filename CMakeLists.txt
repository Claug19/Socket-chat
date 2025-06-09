cmake_minimum_required(VERSION 3.14)
project(socket_chat)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


################ Google test
include(FetchContent)
FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main
  # line bellow uses git archive url instead of git repository
  # URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(gtest)
target_compile_options(gtest PRIVATE -Wno-psabi) # -Wno-psabi skips gcc compiler warnings


################ Executable
include_directories(${CMAKE_SOURCE_DIR}/src)
file(GLOB tests "test/*.cpp")
add_executable(SChat ${tests})
target_compile_options(SChat PRIVATE -Wall -Wextra)
target_link_libraries(SChat GTest::gtest_main )


################ Testing
include(GoogleTest)
enable_testing()
gtest_discover_tests(SChat)
