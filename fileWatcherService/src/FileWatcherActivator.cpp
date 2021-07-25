#include "FileWatcher.h"

#include <cppmicroservices/BundleActivator.h>
#include <iostream>
#include <IServiceFileWatcher.h>
#include <vector>

using namespace ::std::literals;

class ServiceFileWatcher : public IServiceFileWatcher
{
public:

    ~ServiceFileWatcher() override {
        std::cout << "~ServiceFileWatcher" << '\n';
        stop();
    }

    void stop() override {
        _watchers.clear();
    };

    int watch(const std::string& path_to_watch, int delay_in_ms) override {
        auto container = std::make_unique<FileWatcherContainer>(path_to_watch, delay_in_ms);
        container->start();
        _watchers.emplace_back(std::move(container));
        return _watchers.size() - 1; // ID value is last element from watcher
    };

    bool stop_watch(unsigned int id) override {

        if(_watchers.size() > id) {
            _watchers.erase(_watchers.begin() + id);
            return true;
        }

        return false;
    };

private:
    std::vector<std::unique_ptr<FileWatcherContainer>> _watchers{};
};

class US_ABI_LOCAL MmsActivator : public cppmicroservices::BundleActivator {

public:

    void Start(cppmicroservices::BundleContext context) override {
        std::cout << "Start file watcher service" << '\n';
        auto service = std::make_shared<ServiceFileWatcher>();
        context.RegisterService<IServiceFileWatcher>(service);
    }

    void Stop(cppmicroservices::BundleContext context) override {
        std::cout << "Stop file watcher service" << std::endl;
    }
};

CPPMICROSERVICES_EXPORT_BUNDLE_ACTIVATOR(MmsActivator)
