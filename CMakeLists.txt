cmake_minimum_required(VERSION 2.8.11)

project(tmxbin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(COMMON_CONVERTER_SOURCES
    src/pugixml/pugiconfig.hpp
    src/pugixml/pugixml.cpp
    src/pugixml/pugixml.hpp
    src/util/fileutil.cpp
    src/util/fileutil.h
    src/util/xmlutil.cpp
    src/util/xmlutil.h
)

file(GLOB_RECURSE TMXBIN_SOURCES
    src/tmxbin/*.cpp
    src/tmxbin/*.h
)

add_library(tmxbin ${TMXBIN_SOURCES})

add_executable(tmx2tmb
    ${COMMON_CONVERTER_SOURCES}
    src/tmx2tmb/main.cpp
)

add_executable(tsx2tsb
    ${COMMON_CONVERTER_SOURCES}
    src/tsx2tsb/main.cpp
)

add_executable(tmxbin_loadertest src/test/loader.cpp)

target_link_libraries(tmx2tmb tmxbin)
target_link_libraries(tsx2tsb tmxbin)
target_link_libraries(tmxbin_loadertest tmxbin)

set(CUSTOM_LINKER_FLAGS "-g -lgcov --coverage -fprofile-arcs -static -static-libgcc -static-libstdc++")
set(CUSTOM_COMPILE_FLAGS "-g -std=c++11 -fprofile-arcs -ftest-coverage")

set_target_properties(tmx2tmb PROPERTIES
    COMPILE_FLAGS ${CUSTOM_LINKER_FLAGS}
    LINK_FLAGS ${CUSTOM_COMPILE_FLAGS}
)

set_target_properties(tsx2tsb PROPERTIES
    COMPILE_FLAGS ${CUSTOM_LINKER_FLAGS}
    LINK_FLAGS ${CUSTOM_COMPILE_FLAGS}
)

set_target_properties(tmxbin_loadertest PROPERTIES
    COMPILE_FLAGS ${CUSTOM_LINKER_FLAGS}
    LINK_FLAGS ${CUSTOM_COMPILE_FLAGS}
)

set_target_properties(tmxbin PROPERTIES
    COMPILE_FLAGS ${CUSTOM_LINKER_FLAGS}
    LINK_FLAGS ${CUSTOM_COMPILE_FLAGS}
)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")

