#ifndef QNUSPELL_H
#define QNUSPELL_H

#include <QLocale>
#include <QMap>
#include <QObject>
#include <QString>

#include <string>
#include <vector>

#include <nuspell/dictionary.hxx>
#include <nuspell/finder.hxx>

#include "interface.h"

class QNuSpell : public QObject
{
  Q_OBJECT

public:
  explicit QNuSpell(QObject* parent = nullptr);
  ~QNuSpell();

  QStringList languageCodes(QString code=QString());
  CountryData* countryData(QString language_code);

signals:

protected:
  nuspell::Finder finder;
  CountryData* m_current_country;
  QMap<QString, CountryData*>* m_dicts;
  QMap<QString, QList<QString>> m_compatibility;

private:
};

#endif // QNUSPELL_H
