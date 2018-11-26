#ifndef GOOGLESPELLPLUGIN_H
#define GOOGLESPELLPLUGIN_H

#include <QObject>

#include "interface.h"

class GoogleSpellPlugin
  : public BaseSpellClass
{
  Q_OBJECT
public:
  explicit GoogleSpellPlugin(QObject* parent = nullptr);

  // SpellInterface interface
  QString name();
  CountryData* dictionary(QString language_code = QString());
  QStringList languageCodes(QString language_code = QString()) = 0;
  QString language();
  QString country();
  QString path();
  QString bcp47();
  void buildMenu();

protected:



public:
};

#endif // GOOGLESPELLPLUGIN_H
