# The library name for the bundle
set(_lib_name clock_service)

# A list of source code files
set(_srcs
        ServiceTime.cpp
        ServiceTime.h
)

GenerateService(${_lib_name} mms ${_srcs})
