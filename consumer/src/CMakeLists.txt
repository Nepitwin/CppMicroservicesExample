# The library name for the bundle
set(_lib_name clock_consumer_service)

# A list of source code files
set(_srcs
        ConsumerActivator.cpp
        ../../shared/src/IServiceTime.h
)

include_directories("../../shared/src")

GenerateService(${_lib_name} mms ${_srcs})
