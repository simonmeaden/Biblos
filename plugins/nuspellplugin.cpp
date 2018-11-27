#include "nuspellplugin.h"

#include "nuspell/nuspell"

class NuDict : public nuspell::Dict_Base {

  using Dict_Base::spell_priv;
  template <class CharT>
  auto spell_priv(QString str)
  {
    std::string s = str.toStdString();
    return Dict_Base::spell_priv(s);
  }

};

const QString NuSpellPlugin::DEF_LOCALE = "en_GB";

NuSpellPlugin::NuSpellPlugin(QObject* parent)
  : BaseSpellClass(parent)
  , m_nuspell(new QNuSpell(parent))
{
  BaseSpellClass::NAME = "NuSpell";

  spellchecker = new NuSpellChecker(this);
  //  connect(spellchecker, &NuSpellChecker::wordCorrect, this,
  //  &NuSpellPlugin::wordCorrect); connect(spellchecker,
  //  &NuSpellChecker::wordUnknown, this, &NuSpellPlugin::wordUnknown);

  spellchecker->start();
}

void
NuSpellPlugin::setOptions(Options* options)
{
  m_options = options;
}

// NuSpellPlugin::NuSpellPlugin(Options* options, QObject* parent)
//  : BaseSpellClass(options, parent)
//  , m_nuspell(new QNuSpell(parent))
//{}

/*!
 * \brief Gets a list of available language codes.
 *
 * \param language_code
 * \return
 */
CountryData*
NuSpellPlugin::dictionary(QString language_code)
{
  return m_nuspell->countryData(language_code);
}

QStringList
NuSpellPlugin::languageCodes(QString language_code)
{
  return m_nuspell->languageCodes(language_code);
}

QStringList
NuSpellPlugin::compatibleLanguageCodes(QString language_code)
{
  return m_nuspell->languageCodes(language_code);
}

QString
NuSpellPlugin::language()
{
  return QString();
}

QString
NuSpellPlugin::country()
{
  return QString();
}

QString
NuSpellPlugin::path()
{
  return QString();
}

void
NuSpellPlugin::checkWord(QString word)
{}

QString
NuSpellPlugin::bcp47()
{
  return QString();
}

void
NuSpellPlugin::buildMenu()
{}

//= NuSpellChecker ============================================================
NuSpellChecker::NuSpellChecker(QObject* parent)
  : QThread(parent)
  , m_running(true)
{}

NuSpellChecker::~NuSpellChecker() {}

void NuSpellChecker::checkWord(QString /*word*/) {}

void
NuSpellChecker::run()
{
  auto d = new NuDict();
  d->set_encoding_and_language("UTF-8");

  while (m_running) {
    // TODO something
  }
}
