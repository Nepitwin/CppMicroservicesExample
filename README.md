# CppMicroservice Framework Example

## Introduction

The C++ Micro Services project is a collection of components for building modular and dynamic service-oriented applications. It is based on OSGi, but tailored to support native cross-platform solutions.

* https://github.com/CppMicroServices/CppMicroServices
* http://cppmicroservices.org

## Example

This example illustrates several bundle implementations and how to easily combine them using the C++20 [JThread](https://www.modernescpp.com/index.php/a-new-thread-with-c-20-std-jthread).

## Dependencies

* Build the CPP-Microservice project binaries and add them to the path
* CMake
* C++20 Compiler

## Services

This project contains one driver executable to start, stop, uninstall the bundles and three bundle implementations.

| Module                          | Description                                     |
| -------------                   | -------------                                   |
| ClockService                    | Simple clock management service by chrono       |
| ClockConsumer                   | Consumer from the clock service                 |
| Driver (driver)                 | Driver to control the CppFramework              |
| Shared (shared)                 | Shared directory for all interfaces             |
| FileWatcherService              | Sample implementation from a [filewatcher](https://solarianprogrammer.com/2019/01/13/cpp-17-filesystem-write-file-watcher-monitor) in C++20/C++17 |
| FileWatcherConsumer             | Consumer from the file wachter                  |

## Driver

Start the driver.exe and get the help function by the 'h' command

```
> driver.exe
Start framework
h
```

```
start <id | name>    Start the bundle with id <id> or name <name>
stop <id | name>     Stop the bundle with id <id> or name <name>
status               Print status information
load <path>          Load mms dll by full path
uninstall            Uninstall a bundle with id <id> or name <name>
shutdown             Shut down the framework
```

* Compile any service dll and load it
* After the dll is loaded it is installed to the framework
* Start/Stop it by the id or name
* Lifecycle management is implemented by CppMicroservice Framework from the OSGi Standard.

```
> load mms_service.dll

> status
Id | Symbolic Name        | State
-----------------------------------
 0 | system_bundle        | ACTIVE
 1 | mms_service          | INSTALLED

> start 1
Id | Symbolic Name        | State
-----------------------------------
 0 | system_bundle        | ACTIVE
 1 | mms_service          | ACTIVE

> stop 1
Stop MMS Service
> status
Id | Symbolic Name        | State
-----------------------------------
 0 | system_bundle        | ACTIVE
 1 | mms_service          | RESOLVED
```

## Consumer/Service example

* Compile the ClockService and ClockConsumerService
* Load both dll's
* Start first clock_consumer_service
  * Service will print out that the current service is unreachable
* Start first clock_consumer
  * Services are started and consumer get now the clock data
* Ignore prints simple put command in the shell to stop the service

```
> load clock_consumer_service.dll
> load clock_service.dll

> status
Id | Symbolic Name        | State
-----------------------------------
 0 | system_bundle        | ACTIVE
 1 | mms_service          | RESOLVED
 2 | clock_consumer_service | INSTALLED
 3 | clock_service        | INSTALLED

> start 2
ServiceTime reference invalid
ServiceTime reference invalid
> start 3
Elapsed: 1627063565882ms
Elapsed: 1627063566887ms
Elapsed: 1627063567889ms

> stop 2
End consumer service

> status
Id | Symbolic Name        | State
-----------------------------------
 0 | system_bundle        | ACTIVE
 1 | mms_service          | RESOLVED
 2 | clock_consumer_service | RESOLVED
 3 | clock_service        | ACTIVE
```

## Bundles

All services are implemented as bundles from the CppMicroserviceFramework.
* Each bundle overwrites the start and stop methods.
* These get the BundleContext of the framework as parameter.
* CppMicroservice example : http://docs.cppmicroservices.org/en/stable/doc/src/getting_started.html

```
class Activator : public cppmicroservices::BundleActivator {

public:

    void Start(cppmicroservices::BundleContext context) override {
        // Your start code here from the microservice
        // The method must not block as this is installing a bundle. If this blocks the framework will hang here.
    }

    void Stop(cppmicroservices::BundleContext context) override {
        // Your stop code here to clear the all data.
    }
};
```
