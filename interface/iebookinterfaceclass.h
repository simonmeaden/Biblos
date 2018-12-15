#ifndef IBOOKINTERFACECLASS_H
#define IBOOKINTERFACECLASS_H

#include <QObject>

#include "ebookdocument.h"
#include "iebookinterface.h"
#include "ipluginterfaceclass.h"

class Options;

class IEBookInterfaceClass : public QObject,
                             public IPluginInterfaceClass,
                             public IEBookInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID IEBookInterface_iid FILE "ebookinterface.json")
  Q_INTERFACES(IEBookInterface)
public:
  IEBookInterfaceClass(Options *options = Q_NULLPTR,
                       QObject *parent = Q_NULLPTR);

  // EBookInterface interface
  EBookDocument *createDocument(QString) override;
  EBookDocument *createCodeDocument(EBookDocument *) override;
  void saveDocument(QString, EBookDocument *) override;
  QString fileFilter() override;

protected:
  static const QString m_file_filter;

  Options *m_options;
  QString m_filename;
  EBookDocument *m_document;

  // IEBookInterface interface
public:
};

#endif // IBOOKINTERFACECLASS_H
