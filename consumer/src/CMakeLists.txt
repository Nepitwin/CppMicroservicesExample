# The library name for the bundle
set(_lib_name clock_consumer_service)

# A list of source code files
set(_srcs
        ConsumerActivator.cpp
        ServiceTime.h
)

GenerateService(${_lib_name} mms ${_srcs})
