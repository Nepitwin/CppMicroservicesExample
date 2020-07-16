# The library name for the bundle
set(_lib_name SerializerService)

# A list of source code files
set(_srcs
        Activator.cpp
        ISerializerService.cpp
)

GenerateService(${_lib_name} serializer ${_srcs})
