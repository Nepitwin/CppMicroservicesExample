#include <cppmicroservices/BundleActivator.h>
#include <iostream>
#include <set>
#include <IServiceTime.h>

class ServiceTimeSystemClock : public IServiceTime
{
    std::chrono::milliseconds elapsed() const override
    {
        auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch());
    }
};

class ServiceTimeActivator : public cppmicroservices::BundleActivator {

public:

    void Start(cppmicroservices::BundleContext ctx) override
    {
        std::cout << "Start Clock Service" << '\n';
        auto service = std::make_shared<ServiceTimeSystemClock>();
        ctx.RegisterService<IServiceTime>(service);
    }

    void Stop(cppmicroservices::BundleContext) override
    {
        std::cout << "Stop Clock Service" << '\n';
    }
};

CPPMICROSERVICES_EXPORT_BUNDLE_ACTIVATOR(ServiceTimeActivator)
