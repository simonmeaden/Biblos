#ifndef EPUBPLUGIN_H
#define EPUBPLUGIN_H

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

protected:
    Options *m_options;
};

#endif // EPUBPLUGIN_H
