#include "qnuspell.h"

using namespace nuspell;

const QString QNuSpell::NAME = "NuSpell";

QNuSpell::QNuSpell(QObject* parent)
  : QObject(parent)
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
    std::string f = item.first;
    std::string s = item.second;
    QString qf = QString::fromStdString(f);
    QString qs = QString::fromStdString(s);
  }
  //      stringvector_t::fromStdVector();
}

QString
QNuSpell::name()
{
  return NAME;
}

QStringList
QNuSpell::dictionaries()
{
  return QStringList();
}
