#pragma once

#include <chrono>
#include <cppmicroservices/ServiceInterface.h>

struct US_ABI_EXPORT IServiceTime {
    virtual std::chrono::milliseconds elapsed() const = 0;
};