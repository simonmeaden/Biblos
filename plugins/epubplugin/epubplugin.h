#ifndef EPUBPLUGIN_H
#define EPUBPLUGIN_H

#include <QPlainTextDocumentLayout>
#include <QTextDocument>

#include "iebookdocument.h"
#include "iebookinterface.h"
#include "interface_global.h"
#include "options.h"
#include "library.h"
#include "authors.h"

class EPubDocument;

class INTERFACESHARED_EXPORT EPubPlugin : public QObject, public IEBookInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID IEBookInterface_iid FILE "ebookinterface.json")
  Q_INTERFACES(IPluginInterface)
  Q_INTERFACES(IEBookInterface)

public:
  EPubPlugin(QObject* parent = nullptr);

  IEBookDocument* createDocument(QString path) override;
  IEBookDocument* createCodeDocument() override;
  //  void saveDocument(IEBookDocument* m_document) override;

  // IPluginInterface interface
  QString pluginGroup() const override;
  QString pluginName() const override;
  QString vendor() const override;
  bool loaded() const override;
  void setLoaded(bool loaded) override;
  QString version() const override;
  int majorVersion() const override;
  int minorVersion() const override;
  int buildVersion() const override;
  void buildMenu() override;

  // IEBookInterface interface
  QString fileFilter() override;
  QString fileDescription() override;
  EBookDocumentType type() const override
  {
    return EPUB;
  }

protected:
  // static variables for IPluginInterface.
  static const QString m_plugin_group;
  static const QString m_plugin_name;
  static const QString m_vendor;
  static const QString m_version;
  static const int m_major_version;
  static const int m_minor_version;
  static const int m_build_version;
  static bool m_loaded;
  static const QString m_file_filter;
  static const QString m_file_description;

  // variables for IEBookInterface;
//  Options* m_options;
  IEBookDocument* m_document;
};

#endif // EPUBPLUGIN_H
