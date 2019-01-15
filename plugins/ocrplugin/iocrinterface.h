#ifndef IOCRINTERFACE_H
#define IOCRINTERFACE_H

#include <QtPlugin>

#include "interface_global.h"
#include <iplugininterface.h>

class INTERFACESHARED_EXPORT IOcrInterface : public IPluginInterface
{
public:
  ~IOcrInterface()
  {
  }
};
#define IOcrPluginInterface_iid "uk.org.smelecomp.IOcrPluginInterface/0.1.0"
Q_DECLARE_INTERFACE(IOcrInterface, IOcrPluginInterface_iid)

#endif // IOCRINTERFACE_H
