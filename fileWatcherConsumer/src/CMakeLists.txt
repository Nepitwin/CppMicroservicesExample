# The library name for the bundle
set(_lib_name file_watcher_consumer)

# A list of source code files
set(_srcs
        FileWatcherConsumerActivator.cpp
        ../../shared/src/IServiceFileWatcher.h
)

include_directories("../../shared/src")

GenerateService(${_lib_name} mms ${_srcs})
