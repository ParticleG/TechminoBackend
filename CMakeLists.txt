cmake_minimum_required(VERSION 3.5)
project(TechminoBackend CXX)

include(CheckIncludeFileCXX)

check_include_file_cxx(any HAS_ANY)
check_include_file_cxx(string_view HAS_STRING_VIEW)
check_include_file_cxx(ranges HAS_RANGES)
if (HAS_RANGES)
    set(CMAKE_CXX_STANDARD 20)
elseif (HAS_ANY AND HAS_STRING_VIEW)
    set(CMAKE_CXX_STANDARD 17)
else ()
    set(CMAKE_CXX_STANDARD 14)
endif ()

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} main.cc)

add_subdirectory(./lib_plugins)

aux_source_directory(controllers CTL_SRC)
aux_source_directory(filters FLT_SRC)
aux_source_directory(services SVC_SRC)

file(GLOB_RECURSE SCP_LIST ${CMAKE_CURRENT_SOURCE_DIR}/views/*.csp)
foreach (cspFile ${SCP_LIST})
    message(STATUS "cspFile:" ${cspFile})
    get_filename_component(classname ${cspFile} NAME_WE)
    message(STATUS "view classname:" ${classname})
    ADD_CUSTOM_COMMAND(OUTPUT ${classname}.h ${classname}.cc
            COMMAND drogon_ctl
            ARGS create view ${cspFile}
            DEPENDS ${cspFile}
            VERBATIM)
    set(VIEWSRC ${VIEWSRC} ${classname}.cc)
endforeach ()

target_link_libraries(${PROJECT_NAME} PRIVATE
        PLUGINS_LIB)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(${PROJECT_NAME} PRIVATE
        ${CTL_SRC}
        ${FLT_SRC}
        ${PLG_SRC}
        ${SRC_DIR}
        ${SVC_SRC}
        ${VIEWSRC})
################################
# uncomment the following line for dynamically loading views 
#set_property(TARGET ${PROJECT_NAME} PROPERTY ENABLE_EXPORTS ON)
