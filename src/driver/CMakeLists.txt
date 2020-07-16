include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(NOT BUILD_SHARED_LIBS)
  set(_target_zip_archives  ${CppMicroServices_LIBRARIES})
  set(_srcs main.cpp)
  add_executable(ServiceDriver ${_srcs})
  target_link_libraries(ServiceDriver ${_target_zip_archives})

  usFunctionAddResources(TARGET ServiceDriver
                         BUNDLE_NAME main
                         ZIP_ARCHIVES ${_target_zip_archives})

  usFunctionEmbedResources(TARGET ServiceDriver)
else()
  add_executable(ServiceDriver main.cpp)
  target_link_libraries(ServiceDriver ${CppMicroServices_LIBRARIES})
endif()
