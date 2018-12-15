#ifndef EPUBPLUGIN_H
#define EPUBPLUGIN_H

#include <QTextDocument>
#include <QPlainTextDocumentLayout>

#include "iebookinterfaceclass.h"
#include "interface_global.h"

class INTERFACESHARED_EXPORT EpubPlugin : public IEBookInterfaceClass
{
    Q_OBJECT
    Q_INTERFACES(IPluginInterface)
    Q_INTERFACES(IEBookInterface)
public:
    EpubPlugin(Options *options,
               QObject* parent = Q_NULLPTR);

    EBookDocument *createDocument(QString path) override;
    EBookDocument *createCodeDocument(EBookDocument *doc) override;
    void saveDocument(QString, EBookDocument *) override;

protected:

};

#endif // EPUBPLUGIN_H
