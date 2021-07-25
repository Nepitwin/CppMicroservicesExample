# The library name for the bundle
set(_lib_name file_watcher_service)

# A list of source code files
set(_srcs
        FileWatcherActivator.cpp
        FileWatcher.h
        ../../shared/src/IServiceFileWatcher.h
)

include_directories("../../shared/src")

GenerateService(${_lib_name} mms ${_srcs})
