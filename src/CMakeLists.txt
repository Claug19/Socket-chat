################ BUILD DEFINITIONS ################

######## DEBUG ########
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  # build specific implementation
endif()


######## RELEASE ########
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  # build specific implementation
endif()


######## RELEASE WITH DEBUG INFO ########
if ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
  # build specific implementation
endif()


######## MINIMAL SIZE RELEASE ########
if ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
  # build specific implementation
endif()


######## UNIT TESTS & COVERAGE ########
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Coverage")
    
    #### COVERAGE COMPILER FLAGS ####
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
      set(CMAKE_C_FLAGS_COVERAGE "${CMAKE_C_FLAGS_DEBUG} --coverage")
      set(CMAKE_EXE_LINKER_FLAGS_COVERAGE "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")
      set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} --coverage")
      set(CMAKE_MODULE_LINKER_FLAGS_COVERAGE "${CMAKE_MODULE_LINKER_FLAGS_DEBUG} --coverage")
    endif()

endif()



################ COMMON DEFINITIONS ################

######## MOVE EXECUTABLES IN BUILD ########
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


######## FIND PTHREAD ########
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


######## COMMON FILES ########
file(COPY ${CMAKE_SOURCE_DIR}/config DESTINATION ${CMAKE_BINARY_DIR})
file(GLOB additional_files "./utils/*.cpp")


######## EXECUTABLES ########

#### EXECUTABLE 1 ####
file(GLOB server_files "./chatServer.cpp")

add_executable(chatServer ${server_files} ${additional_files})
target_compile_options(chatServer PRIVATE -Wall -Wextra)
target_link_libraries(chatServer Threads::Threads)


#### EXECUTABLE 2 ####
file(GLOB client_files "./chatClient.cpp")

add_executable(chatClient ${client_files} ${additional_files})
target_compile_options(chatClient PRIVATE -Wall -Wextra)
target_link_libraries(chatClient Threads::Threads)
