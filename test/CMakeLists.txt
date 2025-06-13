################ BUILD DEFINITIONS ################

######## UNIT TESTS & COVERAGE ########
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Coverage")
    
    ############## COVERAGE COMPILER FLAGS ##############
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
      set(CMAKE_C_FLAGS_COVERAGE "${CMAKE_C_FLAGS_DEBUG} --coverage")
      set(CMAKE_EXE_LINKER_FLAGS_COVERAGE "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")
      set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} --coverage")
      set(CMAKE_MODULE_LINKER_FLAGS_COVERAGE "${CMAKE_MODULE_LINKER_FLAGS_DEBUG} --coverage")
    endif()

endif()



################ COMMON DEFINITIONS ################

######## FETCH GOOGLE TEST ########
if (("${CMAKE_BUILD_TYPE}" STREQUAL "Test") OR ("${CMAKE_BUILD_TYPE}" STREQUAL "Coverage"))
  include(FetchContent)
  FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
  )
  FetchContent_MakeAvailable(gtest)
  target_compile_options(gtest PRIVATE -Wno-psabi)  # -Wno-psabi skips gcc compiler warnings
endif()


######## ENABLE TESTING ########
include_directories(${GTEST_INCLUDE_DIRS})
include(CTest)
include(GoogleTest)
enable_testing()


######## COMMON FILES ########
include_directories(${CMAKE_SOURCE_DIR}/src)
file(COPY ${CMAKE_SOURCE_DIR}/config DESTINATION ${CMAKE_BINARY_DIR}/test)
file(GLOB additional_files "${CMAKE_SOURCE_DIR}/src/utils/*.cpp")


######## EXECUTABLES ########

#### TEST EXECUTABLE ####
file(GLOB tests "./*.cpp")

add_executable(unit_tests ${tests} ${additional_files})
target_link_libraries(unit_tests GTest::gtest_main)

gtest_discover_tests(unit_tests)
