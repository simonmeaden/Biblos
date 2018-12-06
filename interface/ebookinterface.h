#ifndef EBOOKINTERFACE_H
#define EBOOKINTERFACE_H

#include <QObject>

#include "interface_global.h"

/*!
 * \brief The interface for all EBookEdit plugins.
 */
class INTERFACESHARED_EXPORT IEBookInterface
{
public:
  virtual ~IEBookInterface() {}

  virtual QString pluginGroup() = 0;
  virtual QString name() = 0;
  virtual void buildMenu() = 0;
  virtual QString vendor() = 0;
  virtual bool loaded() = 0;
  virtual QString version() = 0;
  virtual int majorVersion() = 0;
  virtual int minorVersion() = 0;
  virtual int buildVersion() = 0;
};
#define EBookInterface_iid "uk.org.smelecomp.EBookInterface/1.0"
Q_DECLARE_INTERFACE(IEBookInterface, EBookInterface_iid)

#endif // EBOOKINTERFACE_H
