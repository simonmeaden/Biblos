#ifndef IEBOOKINTERFACE_H
#define IEBOOKINTERFACE_H

#include <QtPlugin>

#include "interface_global.h"

class EBookDocument;

/*!
 * \brief The interface for all EBookEdit plugins.
 */
class INTERFACESHARED_EXPORT IEBookInterface
{
public:
    virtual ~IEBookInterface() {}

    virtual EBookDocument *createDocument(QString filename) = 0;
    virtual void saveDocument(QString filename, EBookDocument *document) = 0;

};
#define IEBookInterface_iid "uk.org.smelecomp.IEBookInterface/1.0"
Q_DECLARE_INTERFACE(IEBookInterface, IEBookInterface_iid)

#endif // IEBOOKINTERFACE_H
