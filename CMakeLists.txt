# [proj-begin]
project(CppMicroService)
cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 20)

find_package(CppMicroServices REQUIRED)
# [proj-end]

include(GenerateExportHeader)

function(GenerateService _name _args)
    set(_srcs ${ARGN})

    usFunctionGetResourceSource(TARGET ${_name} OUT _srcs)
    usFunctionGenerateBundleInit(TARGET ${_name} OUT _srcs)

    add_library(${_name} SHARED ${_srcs})
    set_property(TARGET ${_name} APPEND PROPERTY COMPILE_DEFINITIONS US_BUNDLE_NAME=${_name})
    set_property(TARGET ${_name} PROPERTY DEBUG_POSTFIX "")

    if(${_name}_DEPENDS)
        foreach(_dep ${${_name}_DEPENDS})
            include_directories(${PROJECT_SOURCE_DIR}/${_dep})
            target_link_libraries(${_name} ${_dep})
        endforeach()
    endif()
    target_link_libraries(${_name} ${CppMicroServices_LIBRARIES})
    set_target_properties(${_name} PROPERTIES
            OUTPUT_NAME ${_name}
            )

    usFunctionAddResources(TARGET ${_name} BUNDLE_NAME ${_name} WORKING_DIRECTORY ../resources FILES manifest.json)
    usFunctionEmbedResources(TARGET ${_name})

endfunction()

add_subdirectory(driver)
add_subdirectory(mms)
add_subdirectory(clock)
add_subdirectory(consumer)
