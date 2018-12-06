#include "baseebookinterfaceclass.h"

BaseEBookInterfaceClass::BaseEBookInterfaceClass() {}

BaseEBookInterfaceClass::~BaseEBookInterfaceClass() {}

QString
BaseEBookInterfaceClass::pluginGroup()
{
  return m_plugin_group;
}

QString
BaseEBookInterfaceClass::name()
{
  return m_name;
}

QString
BaseEBookInterfaceClass::vendor()
{
  return m_vendor;
}

bool
BaseEBookInterfaceClass::loaded()
{
  return m_loaded;
}

QString
BaseEBookInterfaceClass::version()
{
  return m_version;
}

int
BaseEBookInterfaceClass::majorVersion()
{
  return m_major_version;
}

int
BaseEBookInterfaceClass::minorVersion()
{
  return m_minor_version;
}

int
BaseEBookInterfaceClass::buildVersion()
{
  return m_build_version;
}

void
BaseEBookInterfaceClass::buildMenu()
{
  // Empty method -override for your own menus.
}
