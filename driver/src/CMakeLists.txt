set(_lib_name driver)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(NOT BUILD_SHARED_LIBS)
  set(_target_zip_archives  ${CppMicroServices_LIBRARIES})
  set(_srcs main.cpp)
  add_executable(${_lib_name} ${_srcs})
  target_link_libraries(${_lib_name} ${_target_zip_archives})

  usFunctionAddResources(TARGET ${_lib_name}
                         BUNDLE_NAME main
                         ZIP_ARCHIVES ${_target_zip_archives})

  usFunctionEmbedResources(TARGET ${_lib_name})
else()
  add_executable(${_lib_name} main.cpp)
  target_link_libraries(${_lib_name} ${CppMicroServices_LIBRARIES})
endif()
