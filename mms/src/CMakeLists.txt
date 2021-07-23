# The library name for the bundle
set(_lib_name mms_service)

# A list of source code files
set(_srcs
        MmsActivator.cpp
        FileWatcher.h
)

GenerateService(${_lib_name} mms ${_srcs})
