cmake_minimum_required (VERSION 2.6)

project (map-q3-hl)

option(USE_GLIMAGE "Make use of GL Image" ON)
option(USE_GLM "Make use of GLM" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake-modules/")

set(GLSDK_PATH "${CMAKE_SOURCE_DIR}/../glsdk_0_5_2/")

set(GLM_ROOT "${GLSDK_PATH}/glm/")
set(GLIMG_ROOT "${GLSDK_PATH}/glimg/")

if (USE_GLM)
	find_package(GLM REQUIRED)
	include_directories(${GLM_INCLUDE_DIR})
endif (USE_GLM)

if (USE_GLIMAGE)
	find_package(GLIMG REQUIRED)
	include_directories(${GLIMG_INCLUDE_DIR})
endif (USE_GLIMAGE)

include_directories("${CMAKE_SOURCE_DIR}/../common/")

set(src_app
    ../common/log.cpp
    ../common/targa.cpp
    ../common/tokenizer.cpp
    main.cpp
    hammer2quake.cpp
    mapparser.cpp
    material.cpp
    wadfile.cpp
)

add_executable(map-q3-hl ${src_app})

target_link_libraries (map-q3-hl
        ${OPENGL_LIBRARIES}
        ${GLIMG_LIBRARY}
)
