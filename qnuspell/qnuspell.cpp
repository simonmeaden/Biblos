#include "qnuspell.h"

using namespace nuspell;

QNuSpell::QNuSpell(QObject* parent)
  : QObject(parent)
  , m_dicts(new QMap<QString, CountryData*>())
{
  finder = Finder::search_all_dirs_for_dicts();
  finder.add_libreoffice_dir_paths();
  finder.add_openoffice_dir_paths();
  finder.add_mozilla_dir_paths();
  finder.search_for_dictionaries();
  auto dir_list = finder.get_dictionaries();
  for (std::vector<std::pair<std::string, std::string>>::iterator it =
         dir_list.begin();
       it != dir_list.end();
       ++it) {
    std::pair<std::string, std::string> item = *it;

    QString key = QString::fromStdString((*it).first);
    QString value = QString::fromStdString((*it).second);
    QString lang_code = key.split("_").first();

    QStringList comp_countries =
      m_compatibility.value(lang_code, QStringList());
    comp_countries.append(key);
    m_compatibility[lang_code] = comp_countries;

    CountryData* data = new CountryData();
    QLocale loc = QLocale(key);
    data->loc_code = key;
    data->bcp47 = loc.bcp47Name();
    data->country_name = QLocale::countryToString(loc.country());
    data->language_name = QLocale::languageToString(loc.language());
    data->script = QLocale::scriptToString(loc.script());
    data->path = value;

    m_dicts->insert(key, data);
  }
}

QNuSpell::~QNuSpell() {}

/*!
 * \brief Return a list of codes that are compatible with the two character
 * language code.
 *
 * if a code of "de" (German) is supplied, then a list of the following
 * langauge codes will be returned, if and only if, there are existing
 * dictionaries available for these languages.
 * <ul>
 * <li>de - German (Standard)</li>
 * <li>de_at - German (Austria)</li>
 * <li>de_ch - German (Switzerland)</li>
 * <li>de_li - German (Liechtenstein)</li>
 * <li>de_lu - German (Luxembourg)</li>
 * </ul>
 * These can be utilised by the countryData(QString) method to return
 * individual CountryData* objects with more info about these dictionaries.
 *
 * \param code a two character language code.
 * \return a list of compatible codes.
 */
QStringList
QNuSpell::languageCodes(QString code)
{
  if (code.isEmpty()) {
    return m_dicts->keys();
  }
  return m_compatibility.value(code);
}

/*!
 * \brief gets CountryData* objects containig name/path information for
 *        available dictionaries, or NULL of not available;
 *
 * \param language_code the required language code.
 * \return a CountryData* object or NULL.
 */
CountryData*
QNuSpell::countryData(QString language_code)
{
  return m_dicts->value(language_code);
}
