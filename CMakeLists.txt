cmake_minimum_required (VERSION 2.6)

project (map-q3-hl)

include(win-cpp-deps.cmake/win-cpp-deps.cmake)
install_dep("https://bitbucket.org/wincppdeps/glm.git")

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIR})

### Find OpenGL
find_package(OpenGL REQUIRED)

set(src_app
    common/log.cpp
    common/targa.cpp
    common/tokenizer.cpp
    main.cpp
    hammer2quake.cpp
    mapparser.cpp
    material.cpp
    wadfile.cpp
)

add_executable(map-q3-hl ${src_app})

target_link_libraries (map-q3-hl
    ${OPENGL_LIBRARIES}
)
