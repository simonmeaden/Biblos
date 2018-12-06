#include "googlespellplugin.h"

// GoogleSpellPlugin::GoogleSpellPlugin(Options* options, QObject* parent)
//  : BaseSpellClass(options, parent)
//{}

GoogleSpellPlugin::GoogleSpellPlugin(Options *options, QObject* parent)
  : BaseSpellClass(options, parent)
{
  m_name = "Google";
  m_plugin_group = "Spellchecker";
  m_vendor = "SM Electronic Components";
  m_major_version = GOOGLE_VERSION_MAJOR;
  m_minor_version = GOOGLE_VERSION_MINOR;
  m_build_version = GOOGLE_VERSION_BUILD;
  m_version = QString("%1.%2.%3")
                .arg(m_major_version)
                .arg(m_minor_version)
                .arg(m_build_version);
}

//CountryData*
//GoogleSpellPlugin::dictionary(QString language_code)
//{}

//QStringList
//GoogleSpellPlugin::languageCodes(QString language_code)
//{}

//QString
//GoogleSpellPlugin::language()
//{}

//QString
//GoogleSpellPlugin::country()
//{}

//QString
//GoogleSpellPlugin::path()
//{}

//QString
//GoogleSpellPlugin::bcp47()
//{}

//void
//GoogleSpellPlugin::buildMenu()
//{}

//QStringList GoogleSpellPlugin::compatibleLanguageCodes(QString language_code)
//{

//}

//void GoogleSpellPlugin::checkWord(QString word)
//{

//}

//void GoogleSpellPlugin::checkWords(QStringList words)
//{

//}

//void GoogleSpellPlugin::addWordToBookList(QString word)
//{

//}

//void GoogleSpellPlugin::addWordToAuthorList(QString word)
//{

//}

//void GoogleSpellPlugin::addWordMatch(QString word, QString match)
//{

//}

//QStringList GoogleSpellPlugin::suggestions(QString word)
//{

//}
