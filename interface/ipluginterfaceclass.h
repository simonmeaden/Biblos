#ifndef BASEEBOOKINTERFACECLASS_H
#define BASEEBOOKINTERFACECLASS_H

#include <QObject>
#include <QtPlugin>

#include "iplugininterface.h"

class INTERFACESHARED_EXPORT IPluginInterfaceClass : public IPluginInterface
{
    Q_INTERFACES(IPluginInterface)
    Q_PLUGIN_METADATA(IID IPluginInterface_iid FILE "plugininterface.json")
public:
    IPluginInterfaceClass();
    ~IPluginInterfaceClass();

    QString pluginGroup() const;
    QString pluginName() const;
    QString vendor() const;
    bool loaded() const;
    void setLoaded(bool loaded);
    QString version() const;
    int majorVersion() const;
    int minorVersion() const;
    int buildVersion() const;
    void buildMenu();


protected:
    static const QString m_plugin_group;
    static const QString m_plugin_name;
    static const QString m_vendor;
    static const QString m_version;
    static const int m_major_version;
    static const int m_minor_version;
    static const int m_build_version;
    static bool m_loaded;

    //  void setName(const QString &name);
};

#endif // BASEEBOOKINTERFACECLASS_H
