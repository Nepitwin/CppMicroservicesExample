#pragma once

#include <cppmicroservices/ServiceInterface.h>

struct US_ABI_EXPORT IServiceFileWatcher {
    virtual ~IServiceFileWatcher() = default;
    virtual int watch(const std::string& path_to_watch, int delay_in_ms) = 0;
    virtual bool stop_watch(unsigned int id) = 0;
    virtual void stop() = 0;
};
