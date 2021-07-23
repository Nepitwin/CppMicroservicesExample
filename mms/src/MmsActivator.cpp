#include "FileWatcher.h"

#include <cppmicroservices/BundleActivator.h>
#include <set>
#include <iostream>
#include <chrono>
#include <thread>

class MmsActivator : public cppmicroservices::BundleActivator {

public:

    void Start(cppmicroservices::BundleContext context) override {
        std::cout << "Start MMS Service" << '\n';

        threadWatcher = std::jthread([](std::stop_token stoken) {
            FileWatcher fw{R"(F:\Data\Services\)", std::chrono::milliseconds(1000), stoken};

            fw.start([](const std::string &file, FileStatus status) -> void {

                if (!std::filesystem::is_regular_file(std::filesystem::path(file)) &&
                    status != FileStatus::erased) {
                    return;
                }

                switch (status) {
                    case FileStatus::load:
                        std::cout << "File load: " << file << '\n';
                        break;
                    case FileStatus::updated:
                        std::cout << "File updated: " << file << '\n';
                        break;
                    case FileStatus::erased:
                        std::cout << "File erased: " << file << '\n';
                        break;
                    default:
                        std::cout << "Error! Unknown file status.\n";
                }
            });
        });
    }

    void Stop(cppmicroservices::BundleContext context) override {
        threadWatcher.request_stop();
        std::cout << "Stop MMS Service" << std::endl;
    }

private:
    std::jthread threadWatcher;
};

CPPMICROSERVICES_EXPORT_BUNDLE_ACTIVATOR(MmsActivator)
