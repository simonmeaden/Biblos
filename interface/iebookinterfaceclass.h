#ifndef IBOOKINTERFACECLASS_H
#define IBOOKINTERFACECLASS_H

#include <QObject>

#include "iebookinterface.h"
#include "ipluginterfaceclass.h"
#include "ebookdocument.h"

class Options;

class IEBookInterfaceClass : public QObject,
    public IPluginInterfaceClass,
    public IEBookInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID IEBookInterface_iid FILE "ebookinterface.json")
    Q_INTERFACES(IEBookInterface)
public:
    IEBookInterfaceClass(Options *options = Q_NULLPTR,
                         QObject *parent = Q_NULLPTR);


protected:
    Options *m_options;
    QString m_filename;
    EBookDocument *m_document;


    // IEBookInterface interface
public:
    EBookDocument *createDocument(QString filename) override;
    void saveDocument(QString filename, EBookDocument *document) override;
};

#endif // IBOOKINTERFACECLASS_H
