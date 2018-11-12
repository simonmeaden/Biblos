#ifndef SPELLINTERFACE_H
#define SPELLINTERFACE_H

#include <QObject>

class SpellInterface
{

public:
  virtual ~SpellInterface() {}

  virtual QString name();
  virtual QStringList dictionaries();

};

#endif // SPELLINTERFACE_H
