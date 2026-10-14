cmake_minimum_required(VERSION 3.15...3.25)

project(FIXEngine
  VERSION 0.1.0
  DESCRIPTION "A FIX 4.2 engine in modern C++"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(FIXENGINE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

set(FIXENGINE_WARNINGS -Wall -Wextra)
if(FIXENGINE_WARNINGS_AS_ERRORS)
  list(APPEND FIXENGINE_WARNINGS -Werror)
endif()

# 1. Fetch Dependencies
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.8.1
)
FetchContent_Declare(
  pugixml
  GIT_REPOSITORY https://github.com/zeux/pugixml.git
  GIT_TAG v1.14
)
FetchContent_MakeAvailable(Catch2 pugixml)

# 2. Define Library
add_library(fixengine
  src/FIXMessage.cpp
  src/FIXMessage.h
  src/FIXDictionary.cpp
  src/FIXDictionary.h
  src/Heartbeat.cpp
  src/Heartbeat.h
  src/TypedMessage.h
  src/Logon.h
  src/Logon.cpp
  src/Logout.h
  src/Logout.cpp
  src/TestRequest.h
  src/TestRequest.cpp
  src/ResendRequest.h
  src/ResendRequest.cpp
  src/SequenceReset.h
  src/SequenceReset.cpp
  src/FIXSession.h
  src/FIXSession.cpp
)
target_include_directories(fixengine PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(fixengine PUBLIC pugixml::pugixml)
target_compile_options(fixengine PRIVATE ${FIXENGINE_WARNINGS} -Wpedantic)
target_compile_definitions(fixengine PRIVATE FIX_DICTIONARY_PATH="${CMAKE_SOURCE_DIR}/spec/FIX42.xml")

# 3. Demo Executable
add_executable(fixengine_demo examples/demo.cpp)
target_link_libraries(fixengine_demo PRIVATE fixengine)
target_compile_options(fixengine_demo PRIVATE ${FIXENGINE_WARNINGS} -Wpedantic)

# 4. Tests
enable_testing()
include(CTest)
include(${catch2_SOURCE_DIR}/extras/Catch.cmake)

function(fixengine_add_test name source)
  add_executable(${name} ${source})
  target_link_libraries(${name} PRIVATE fixengine Catch2::Catch2WithMain)
  target_compile_options(${name} PRIVATE ${FIXENGINE_WARNINGS})
  catch_discover_tests(${name})
endfunction()

fixengine_add_test(tests tests/test_fixmessage.cpp)
fixengine_add_test(dict_tests tests/test_fixdictionary.cpp)
fixengine_add_test(heartbeat_tests tests/test_heartbeat.cpp)
fixengine_add_test(session_tests tests/test_fixsession.cpp)
