#include <cppmicroservices/BundleActivator.h>
#include <cppmicroservices/GetBundleContext.h>
#include <iostream>
#include <thread>
#include <IServiceTime.h>

using namespace ::std::literals;

class ConsumerActivator : public cppmicroservices::BundleActivator {

public:

    void Start(cppmicroservices::BundleContext context) override {
        std::cout << "Start clockConsumer service" << '\n';
        ctx = context;

        consumer = std::jthread([this](std::stop_token stopToken) {
            while (!stopToken.stop_requested()) {
                std::this_thread::sleep_for(1s);
                auto ref = ctx.GetServiceReference<IServiceTime>();
                PrintTime(ref);
            }
        });
    }

    void Stop(cppmicroservices::BundleContext context) override {
        std::cout << "End clockConsumer service" << std::endl;
    }

    static void PrintTime(const cppmicroservices::ServiceReference<IServiceTime>& ref)
    {
        if (!ref) {
            std::cout << "ServiceTime reference invalid" << std::endl;
            return;
        }

        // We can also get the bundle context like this
        auto ctx = cppmicroservices::GetBundleContext();

        // Get the ServiceTime service
        auto svc_time = ctx.GetService(ref);
        if (!svc_time) {
            std::cout << "ServiceTime not available" << std::endl;
        } else {
            std::cout << "Elapsed: " << svc_time->elapsed().count() << "ms"
                      << std::endl;
        }
    }

private:
    [[maybe_unused]] std::jthread consumer;

    cppmicroservices::BundleContext ctx;
};

CPPMICROSERVICES_EXPORT_BUNDLE_ACTIVATOR(ConsumerActivator)
