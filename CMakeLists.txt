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

##########
# If you include the drogon source code locally in your project, use this method to add drogon
# add_subdirectory(drogon)
# target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
##########

#find_package(Drogon CONFIG REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

#if (CMAKE_CXX_STANDARD LESS 17)
#    #With C++14, use boost to support any and string_view
#    message(STATUS "use c++14")
#    find_package(Boost 1.61.0 REQUIRED)
#    target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
#elseif (CMAKE_CXX_STANDARD LESS 20)
#    message(STATUS "use c++17")
#else ()
#    message(STATUS "use c++20")
#endif ()

add_subdirectory(./lib_strategies)

aux_source_directory(controllers CTL_SRC)
aux_source_directory(filters FLT_SRC)
aux_source_directory(plugins PLG_SRC)
aux_source_directory(services SVC_SRC)
aux_source_directory(utils UTL_SRC)

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

find_package(OpenSSL REQUIRED)
find_package(cryptopp CONFIG REQUIRED)



target_link_libraries(${PROJECT_NAME} PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
        cryptopp-static
        MODEL_LIB)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
        )

target_sources(${PROJECT_NAME} PRIVATE
        ${CTL_SRC}
        ${FLT_SRC}
        ${PLG_SRC}
        ${SRC_DIR}
        ${SVC_SRC}
        ${UTL_SRC}
        ${VIEWSRC})
################################
# uncomment the following line for dynamically loading views 
#set_property(TARGET ${PROJECT_NAME} PROPERTY ENABLE_EXPORTS ON)
