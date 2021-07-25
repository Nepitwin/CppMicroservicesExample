#include <cppmicroservices/BundleActivator.h>
#include <cppmicroservices/GetBundleContext.h>
#include <iostream>
#include <IServiceFileWatcher.h>

using namespace ::std::literals;

class US_ABI_LOCAL FileWatcherActivator : public cppmicroservices::BundleActivator {

public:

    ~FileWatcherActivator() override {
        std::cout << "~FileWatcherActivator" << '\n';
    }

    void Start(cppmicroservices::BundleContext context) override {
        std::cout << "Start file watcher consumer" << '\n';

        if(const auto svc = GetService(context.GetServiceReference<IServiceFileWatcher>())) {
            std::cout << "Register file system watcher" << '\n';
            id = svc->watch(R"(F:\Data\Services\)", 1000);
        } else {
            std::cout << "Service bundle not active - Registration failed" << '\n';
        }
    }

    void Stop(cppmicroservices::BundleContext context) override {

        if(const auto svc = GetService(context.GetServiceReference<IServiceFileWatcher>())) {
            std::cout << "Unregister file system watcher" << '\n';
            if (id >= 0 && svc->stop_watch(id)) {
                std::cout << "Service stop to watch" << '\n';
            }
        } else {
            std::cout << "Service bundle not active" << '\n';
        }

        id = -1;
        std::cout << "End file watcher consumer" << std::endl;
    }

    static std::shared_ptr<IServiceFileWatcher> GetService(const cppmicroservices::ServiceReference<IServiceFileWatcher>& ref) {
        if (!ref) {
            std::cout << "Service reference invalid" << std::endl;
            return nullptr;
        }

        // We can also get the bundle context like this
        auto ctx = cppmicroservices::GetBundleContext();

        // Get the ServiceTime service
        if (auto svc = ctx.GetService<IServiceFileWatcher>(ref)) {
            return svc;
        }

        return nullptr;
    }

private:
    int id{-1};
};

CPPMICROSERVICES_EXPORT_BUNDLE_ACTIVATOR(FileWatcherActivator)
