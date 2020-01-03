project(day_and_night)
cmake_minimum_required(VERSION 3.6)


# global params
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE on)

file(GLOB_RECURSE NETWORK network/*.cpp network/*.hpp)
file(GLOB_RECURSE UTIL util/*.cpp util/*.hpp)

set(GENERAL_FILES ${NETWORK}
                  ${UTIL})


# main_day
add_executable(main_day ${GENERAL_FILES} main_day)

TARGET_LINK_LIBRARIES(main_day log4cpp)
TARGET_LINK_LIBRARIES(main_day pcap)
TARGET_LINK_LIBRARIES(main_day jsoncpp)
TARGET_LINK_LIBRARIES(main_day pthread)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config_day.json
        ${CMAKE_CURRENT_BINARY_DIR}/config_day.json COPYONLY)


# main_night
add_executable(main_night ${GENERAL_FILES} main_night)

TARGET_LINK_LIBRARIES(main_night log4cpp)
TARGET_LINK_LIBRARIES(main_night pcap)
TARGET_LINK_LIBRARIES(main_night jsoncpp)
TARGET_LINK_LIBRARIES(main_night pthread)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config_night.json
        ${CMAKE_CURRENT_BINARY_DIR}/config_night.json COPYONLY)


# clang-tidy
find_program(
  CLANG_TIDY_EXE
  NAMES "clang-tidy"
  DOC "Path to clang-tidy executable"
  )
if(NOT CLANG_TIDY_EXE)
  message(STATUS "clang-tidy not found.")
else()
  message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
  set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=*,-clang-analyzer-alpha.*,-cppcoreguidelines-pro-type-reinterpret-cast,-cppcoreguidelines-pro-bounds-pointer-arithmetic")
endif()

if(CLANG_TIDY_EXE)
  set_target_properties(
    main_day main_night PROPERTIES
    CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
  )
endif()
