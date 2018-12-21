#ifndef IEBOOKINTERFACE_H
#define IEBOOKINTERFACE_H

#include <QtPlugin>

#include "iebookdocument.h"
#include "interface_global.h"
#include "iplugininterface.h"

class EBookDocument;

/*!
 * \brief The interface for all EBookEdit plugins.
 */
class INTERFACESHARED_EXPORT IEBookInterface : public IPluginInterface
{
public:
  virtual ~IEBookInterface() {}

  virtual QString title() const = 0;
  virtual QString publisher() const = 0;

  virtual IEBookDocument* createDocument(QString filename) = 0;
  virtual IEBookDocument* createCodeDocument() = 0;
  virtual void saveDocument(IEBookDocument* document) = 0;

  virtual QString fileFilter() = 0;
};
#define IEBookInterface_iid "uk.org.smelecomp.IEBookInterface/0.1.0"
Q_DECLARE_INTERFACE(IEBookInterface, IEBookInterface_iid)

#endif // IEBOOKINTERFACE_H
