# The library name for the bundle
set(_lib_name clock_service)

# A list of source code files
set(_srcs
        ServiceTime.cpp
        ../../shared/src/IServiceTime.h
        )

include_directories("../../shared/src")

GenerateService(${_lib_name} mms ${_srcs})
