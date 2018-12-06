#ifndef BASEEBOOKINTERFACECLASS_H
#define BASEEBOOKINTERFACECLASS_H

#include <QObject>
#include <QtPlugin>

#include "ebookinterface.h"

class INTERFACESHARED_EXPORT BaseEBookInterfaceClass : public IEBookInterface
{
  Q_INTERFACES(IEBookInterface)
  Q_PLUGIN_METADATA(IID EBookInterface_iid FILE "ebookinterface.json")
public:
  BaseEBookInterfaceClass();
  ~BaseEBookInterfaceClass();

  QString pluginGroup();
  QString name();
  QString vendor();
  bool loaded();
  QString version();
  int majorVersion();
  int minorVersion();
  int buildVersion();
  void testpluginbuildMenu();
  void buildMenu();

protected:
  QString m_name;
  QString m_plugin_group;
  QString m_vendor;
  QString m_version;
  int m_major_version;
  int m_minor_version;
  int m_build_version;
  bool m_loaded;
};

#endif // BASEEBOOKINTERFACECLASS_H
