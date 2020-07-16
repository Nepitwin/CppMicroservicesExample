# [proj-begin]
project(CppMicroService)
cmake_minimum_required(VERSION 3.16)

find_package(CppMicroServices REQUIRED)
# [proj-end]

include(GenerateExportHeader)

function(GenerateService _name _package)
    set(_srcs ${ARGN})

    usFunctionGetResourceSource(TARGET ${_name} OUT _srcs)
    usFunctionGenerateBundleInit(TARGET ${_name} OUT _srcs)

    add_library(${_name} SHARED ${_srcs})
    set_property(TARGET ${_name} APPEND PROPERTY COMPILE_DEFINITIONS US_BUNDLE_NAME=${_name})
    set_property(TARGET ${_name} PROPERTY DEBUG_POSTFIX "")

    if(${_name}_DEPENDS)
        foreach(_dep ${${_name}_DEPENDS})
            include_directories(${PROJECT_SOURCE_DIR}/${_dep})
            target_link_libraries(${_name} Tutorial-${_dep})
        endforeach()
    endif()
    target_link_libraries(${_name} ${CppMicroServices_LIBRARIES})
    set_target_properties(${_name} PROPERTIES
            LABELS Tutorial
            OUTPUT_NAME ${_name}
            )

    usFunctionAddResources(TARGET ${_name} BUNDLE_NAME ${_name} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/service/${_package}/resources FILES manifest.json)
    usFunctionEmbedResources(TARGET ${_name})

endfunction()

add_subdirectory(src)
