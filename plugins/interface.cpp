#include "interface.h"

SpellInterface::~SpellInterface() {}

BaseSpellClass::BaseSpellClass(QObject* parent)
  : QObject(parent)
  , m_options(Q_NULLPTR)
{
  QMap<QString, SpellInterface*>* testList =
    dynamic_cast<QMap<QString, SpellInterface*>*>(m_spellcheckers);
  if (!testList) {
    m_spellcheckers = new QMap<QString, SpellInterface*>();
  }
  m_spellcheckers->insert(name(), this);
}

BaseSpellClass::BaseSpellClass(Options* options, QObject* parent)
  : QObject(parent)
  , m_options(options)
{}

BaseSpellClass::~BaseSpellClass() {}

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

QString
BaseSpellClass::name()
{
  return NAME;
}

EBookInterface::~EBookInterface() {}
