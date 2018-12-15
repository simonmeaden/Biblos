#include "ipluginterfaceclass.h"

IPluginInterfaceClass::IPluginInterfaceClass() {}

IPluginInterfaceClass::~IPluginInterfaceClass() {}

QString
IPluginInterfaceClass::pluginGroup() const
{
    return m_plugin_group;
}

QString
IPluginInterfaceClass::pluginName() const
{
    return m_plugin_name;
}

//void IEBookInterfaceClass::setName(const QString &name) {
//    m_name = name;
//}

QString
IPluginInterfaceClass::vendor() const
{
    return m_vendor;
}

bool
IPluginInterfaceClass::loaded() const
{
    return m_loaded;
}

void IPluginInterfaceClass::setLoaded(bool loaded)
{
    m_loaded = loaded;
}

QString
IPluginInterfaceClass::version() const
{
    return m_version;
}

int
IPluginInterfaceClass::majorVersion() const
{
    return m_major_version;
}

int
IPluginInterfaceClass::minorVersion() const
{
    return m_minor_version;
}

int
IPluginInterfaceClass::buildVersion() const
{
    return m_build_version;
}

void
IPluginInterfaceClass::buildMenu()
{
    // Empty method - override for your own menus.
}


