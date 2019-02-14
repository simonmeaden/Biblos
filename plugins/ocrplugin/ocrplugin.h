#ifndef OCRPLUGIN_H
#define OCRPLUGIN_H

#include <QObject>

#include "iocrinterface.h"
#include "ocrplugin_global.h"

class OCRPLUGINSHARED_EXPORT OcrPlugin : public QObject, public IOcrInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID IOcrPluginInterface_iid FILE "ocrplugin.json")
  Q_INTERFACES(IPluginInterface)
  Q_INTERFACES(IOcrInterface)

public:
  OcrPlugin(QObject* parent = nullptr);

  QString pluginGroup() const
  {
    return QString();
  }
  QString pluginName() const
  {
    return "OCRPlugin";
  }
  QString vendor() const
  {
    return "SM Electronic Components";
  }
  bool loaded() const
  {
    return false;
  }
  void setLoaded(bool)
  {
  }
  QString version() const
  {
    return QString();
  }
  int majorVersion() const
  {
    return 0;
  }
  int minorVersion() const
  {
    return 0;
  }
  int buildVersion() const
  {
    return 0;
  }
  void buildMenu()
  {
  }
};

#endif // OCRPLUGIN_H
