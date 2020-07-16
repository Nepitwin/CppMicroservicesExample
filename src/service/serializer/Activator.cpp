#include "ISerializerService.h"

#include <cppmicroservices/BundleActivator.h>

#include <memory>
#include <set>

using namespace cppmicroservices;

namespace {

class US_ABI_LOCAL Activator : public BundleActivator
{

private:
  class SerializerImpl : public ISerializerService
  {

  public:
    SerializerImpl()
    {
    }
  };

public:

  void Start(BundleContext context)
  {
      // TODO
  }

  void Stop(BundleContext context)
  {
    // NOTE: The service is automatically unregistered
  }
};
}

CPPMICROSERVICES_EXPORT_BUNDLE_ACTIVATOR(Activator)
