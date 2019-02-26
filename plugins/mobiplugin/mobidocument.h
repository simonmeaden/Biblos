#ifndef MOBIDOCUMENT_H
#define MOBIDOCUMENT_H

#include <QDateTime>
#include <QDir>
#include <QDomDocument>
#include <QElapsedTimer>
#include <QIODevice>
#include <QImage>
#include <QObject>
#include <QPainter>
#include <QSvgRenderer>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>

#include <csvsplitter/csvsplitter.h>

#include "ebookcommon.h"
#include "iebookdocument.h"
#include "interface_global.h"

class MobiDocumentPrivate;

class INTERFACESHARED_EXPORT MobiDocument : public ITextDocument {
  Q_OBJECT
  Q_DECLARE_PRIVATE(MobiDocument)

public:
  MobiDocument(QObject *parent = nullptr);
  MobiDocument(MobiDocumentPrivate *doc, QObject *parent = nullptr);
  MobiDocument(const MobiDocument &doc);
  virtual ~MobiDocument() override;

  // IEBookDocument interface
  QString filename() override;
  void setFilename(const QString &filename) override;
  void openDocument(const QString &path) override;
  void saveDocument() override;
  IEBookInterface *plugin() override;
  void setPlugin(IEBookInterface *plugin) override;
  //  bool isModified() override;
  //  bool readOnly() override;
  //  void setReadOnly(const bool readonly) override;
  QString title() override;
  void setTitle(const QString &title) override;
  QString subject() override;
  void setSubject(const QString &subject) override;
  QString language() override;
  void setLanguage(const QString &language) override;
  QDateTime date() override;
  void setDate(const QDateTime &date) override;
  QStringList creators();
  //  void setCreators(const QStringList& creators);
  QString creatorNames(const QStringList &);
  QString publisher() override;
  void setPublisher(const QString &publisher) override;
  QDate published() override;
  void setPublished(const QDate &published) override;
  EBookDocumentType type() const override { return MOBI; }

  QString tocAsString() override {}

  // IEBookDocument interface
  bool isModified() override {}

  QString buildTocFromData() override;

  Metadata metadata();

protected:
  MobiDocumentPrivate *d_ptr;
  MobiDocument(MobiDocumentPrivate &d);

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
};

Q_DECLARE_METATYPE(MobiDocument);

#endif // MOBIDOCUMENT_H
