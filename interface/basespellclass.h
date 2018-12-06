#ifndef INTERFACE_H
#define INTERFACE_H

#include "baseebookinterfaceclass.h"
#include "basespellinterfaceclass.h"

class Options;

class INTERFACESHARED_EXPORT BaseSpellClass
  : public QObject
  , public BaseSpellInterfaceClass
  , public BaseEBookInterfaceClass
{
  Q_OBJECT
  Q_INTERFACES(IEBookInterface)
  Q_INTERFACES(ISpellInterface)
public:
  explicit BaseSpellClass(Options* options, QObject* parent = Q_NULLPTR);

signals:
  void wordCorrect(QString word);
  void wordUnknown(QString word);
  void wordMatched(QString, QString);
  void wordSuggestions(QStringList);

};

#endif // INTERFACE_H
