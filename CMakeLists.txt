################ CMAKE SETUP ################
cmake_minimum_required(VERSION 3.14)
project(socket_chat)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



################ COMMON SETUP ################
include_directories(include)



################ BUILD DEFINITIONS ################

######## DEFAULT ########
if ("${CMAKE_BUILD_TYPE}" STREQUAL "")

  #### ADD SUBDIRECTORIES ####
  add_subdirectory(src)
endif()


######## DEBUG ########
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")

  #### ADD SUBDIRECTORIES ####
  add_subdirectory(src)
  # build specific implementation
endif()


######## RELEASE ########
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")

  #### ADD SUBDIRECTORIES ####
  add_subdirectory(src)
  # build specific implementation
endif()


######## RELEASE WITH DEBUG INFO ########
if ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")

  #### ADD SUBDIRECTORIES ####
  add_subdirectory(src)
  # build specific implementation
endif()


######## MINIMAL SIZE RELEASE ########
if ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")

  #### ADD SUBDIRECTORIES ####
  add_subdirectory(src)
  # build specific implementation
endif()


######## UNIT TESTS ########
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Test")

  #### ADD SUBDIRECTORIES ####
  add_subdirectory(test)
endif()


######## UNIT TESTS & COVERAGE ########
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Coverage")

  #### FETCH GCOVR ####
  execute_process(
    # find_package(Python3 REQUIRED COMPONENTS Interpreter)
    COMMAND ${PYTHON_EXECUTABLE} -m pip install gcovr
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )

  #### RUN GCOVR ####
  add_custom_target(coverage
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
    COMMAND ${Python3_EXECUTABLE} -m gcovr -r ${CMAKE_SOURCE_DIR} --html --html-nested -o ${CMAKE_BINARY_DIR}/coverage/index.html
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )

  #### ADD SUBDIRECTORIES ####
  add_subdirectory(src)
  add_subdirectory(test)
endif()
