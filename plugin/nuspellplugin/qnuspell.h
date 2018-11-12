#ifndef QNUSPELL_H
#define QNUSPELL_H

#include <QObject>
#include <QString>
#include <QVector>

#include <string>
#include <vector>

#include <nuspell/dictionary.hxx>
#include <nuspell/finder.hxx>

#include "spellinterface.h"

typedef QVector<QString> stringvector_t;

class QNuSpell : public QObject, public SpellInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "uk.org.smelecomp.SpellInterface" FILE "nuspell.json")
  Q_INTERFACES(SpellInterface)

public:
  explicit QNuSpell(QObject *parent = nullptr);

  QString name();
  QStringList dictionaries();

signals:

protected:
  stringvector_t m_dir_list;
  nuspell::Finder finder;

private:
  static const QString NAME;
};

#endif // QNUSPELL_H
