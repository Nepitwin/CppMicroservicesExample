#include <cppmicroservices/Bundle.h>
#include <cppmicroservices/BundleContext.h>
#include <cppmicroservices/Framework.h>
#include <cppmicroservices/FrameworkEvent.h>
#include <cppmicroservices/FrameworkFactory.h>

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>
#include <stdexcept>

int main(int /*argc*/, char ** /*argv*/) {
    char cmd[256];

    std::cout << "Start framework" << std::endl;

    std::unordered_map<std::string, long> symbolicNameToId;

    cppmicroservices::FrameworkFactory factory;
    auto framework = factory.NewFramework();

    auto get_bundle = [&framework, &symbolicNameToId](const std::string &str) {
        std::stringstream ss(str);

        long int id = -1;
        ss >> id;
        if (!ss) {
            id = -1;
            auto it = symbolicNameToId.find(str);
            if (it != symbolicNameToId.end()) {
                id = it->second;
            }
        }

        return framework.GetBundleContext().GetBundle(id);
    };

    try {
        framework.Start();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::cout << "> ";
    while (std::cin.getline(cmd, sizeof(cmd))) {
        /*
         The user can stop the framework and we handle this like
         a regular shutdown command.
        */
        if (framework.GetState() != cppmicroservices::Bundle::STATE_ACTIVE) {
            break;
        }

        std::string strCmd(cmd);
        if (strCmd == "shutdown") {
            break;
        } else if (strCmd == "h") {
            std::cout << std::left << std::setw(20) << "h"
                      << " This help text\n"
                      << std::setw(20) << "start <id | name>"
                      << " Start the bundle with id <id> or name <name>\n"
                      << std::setw(20) << "stop <id | name>"
                      << " Stop the bundle with id <id> or name <name>\n"
                      << std::setw(20) << "status"
                      << " Print status information\n"
                      << std::setw(20) << "load <path>"
                      << " Load mms dll by full path\n"
                      << std::setw(20) << "uninstall"
                      << " Uninstall a bundle with id <id> or name <name>\n"
                      << std::setw(20) << "shutdown"
                      << " Shut down the framework\n"
                      << std::flush;
        } else if (strCmd.find("start ") != std::string::npos) {
            auto bundle = get_bundle(strCmd.substr(6));
            if (bundle) {
                try {
                    /* starting an already started bundle does nothing.
                       There is no harm in doing it. */
                    if (bundle.GetState() == cppmicroservices::Bundle::STATE_ACTIVE) {
                        std::cout << "Info: bundle already active" << std::endl;
                    }
                    bundle.Start();
                } catch (const std::exception &e) {
                    std::cerr << e.what() << std::endl;
                }
            } else {
                std::cerr << "Error: unknown id or symbolic name" << std::endl;
            }
        } else if (strCmd.find("stop ") != std::string::npos) {
            auto bundle = get_bundle(strCmd.substr(5));
            if (bundle) {
                try {
                    bundle.Stop();
                    if (bundle.GetBundleId() == 0) {
                        break;
                    }
                } catch (const std::exception &e) {
                    std::cerr << e.what() << std::endl;
                }
            } else {
                std::cerr << "Error: unknown id or symbolic name" << std::endl;
            }
        } else if (strCmd == "status") {
            std::map<long, cppmicroservices::Bundle> bundles;
            for (auto &b : framework.GetBundleContext().GetBundles()) {
                bundles.insert(std::make_pair(b.GetBundleId(), b));
            }

            std::cout << std::left;

            std::cout << "Id | " << std::setw(20) << "Symbolic Name"
                      << " | " << std::setw(9) << "State" << std::endl;
            std::cout << "-----------------------------------\n";

            for (auto &bundle : bundles) {
                std::cout << std::right << std::setw(2) << bundle.first << std::left
                          << " | ";
                std::cout << std::setw(20) << bundle.second.GetSymbolicName() << " | ";
                std::cout << std::setw(9) << (bundle.second.GetState());
                std::cout << std::endl;
            }
        } else if (strCmd.find("load ") != std::string::npos) {
            auto dll = strCmd.substr(5);
            try {
                std::cout << dll << std::endl;
                framework.GetBundleContext().InstallBundles(dll);
                for (auto b : framework.GetBundleContext().GetBundles()) {
                    symbolicNameToId[b.GetSymbolicName()] = b.GetBundleId();
                }
            } catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
            }

        } else if (strCmd.find("uninstall ") != std::string::npos) {
            auto bundle = get_bundle(strCmd.substr(10));
            if (bundle) {
                try {
                    bundle.Uninstall();
                } catch (const std::exception &e) {
                    std::cerr << e.what() << std::endl;
                }
            } else {
                std::cerr << "Error: unknown id or symbolic name" << std::endl;
            }
        } else {
            std::cout << "Unknown command: " << strCmd << " (type 'h' for help)"
                      << std::endl;
        }
        std::cout << "> ";
    }

    framework.Stop();
    framework.WaitForStop(std::chrono::seconds(2));

    return 0;
}

#ifndef US_BUILD_SHARED_LIBS
CPPMICROSERVICES_INITIALIZE_STATIC_BUNDLE(system_bundle)
CPPMICROSERVICES_IMPORT_BUNDLE(mms_service)
#endif
