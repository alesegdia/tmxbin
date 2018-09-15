cmake_minimum_required(VERSION 2.8.11)

project(tmxbin)

set(TMX2TMB_SOURCES
    src/tmx2tmb/main.cpp
)

set(TSX2TSB_SOURCES
    src/tsx2tsb/main.cpp
)

set(COMMON_SOURCES
    lib/pugixml/pugiconfig.hpp
    lib/pugixml/pugixml.cpp
    lib/pugixml/pugixml.hpp
    src/common/fileutil.cpp
    src/common/fileutil.h
    src/common/enums.h
)

add_executable(tmx2tmb
    ${TMX2TMB_SOURCES}
    ${COMMON_SOURCES}
)

add_executable(tsx2tsb
    ${TSX2TSB_SOURCES}
    ${COMMON_SOURCES}
)

set_target_properties(tmx2tmb PROPERTIES
    COMPILE_FLAGS "-g -std=c++11"
    LINK_FLAGS "-g -ftest-coverage -fprofile-arcs"
)
