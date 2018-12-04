#include "interface.h"

BaseEBookInterfaceClass::BaseEBookInterfaceClass(QObject* parent)
  : QObject(parent)
{}

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

BaseSpellClass::BaseSpellClass(QObject* parent)
  : BaseEBookInterfaceClass(parent)
{}

BaseSpellClass::BaseSpellClass(Options* options, QObject* parent)
  : BaseEBookInterfaceClass(parent)
  , m_options(options)
{}

void
BaseSpellClass::setLocale(QLocale locale)
{
  m_locale = locale;
}

QMap<QString, SpellInterface*>*
BaseSpellClass::spellcheckers()
{
  return m_spellcheckers;
}
