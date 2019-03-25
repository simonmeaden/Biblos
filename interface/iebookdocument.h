#ifndef IEBOOKDOCUMENT_H
#define IEBOOKDOCUMENT_H

#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QTextDocument>

#include "ebookcommon.h"
#include "ebookmetadata.h"

class IEBookInterface;
// class EBookManifestItem;
// typedef QSharedPointer<EBookManifestItem> ManifestItem;

class IEBookDocument
{
public:
  virtual ~IEBookDocument();

  virtual QString filename() = 0;
  virtual void setFilename(const QString& filename) = 0;
  virtual void openDocument(const QString& path) = 0;
  virtual void saveDocument(const QString& path = QString()) = 0;
  virtual IEBookInterface* plugin() = 0;
  virtual void setPlugin(IEBookInterface* plugin) = 0;
  virtual EBookDocumentType type() const = 0;

  virtual bool isModified() = 0;
  //  virtual bool readOnly() = 0;
  //  virtual void setReadOnly(const bool readonly) = 0;

  virtual QString tocAsString() = 0;

  virtual QString title() = 0;
  virtual void setTitle(const QString& title) = 0;
  virtual QString subject() = 0;
  virtual void setSubject(const QString& subject) = 0;
  virtual QString language() = 0;
  virtual void setLanguage(const QString& language) = 0;
  virtual QDateTime date() = 0;
  virtual void setDate(const QDateTime& date) = 0;
  virtual QStringList creators() = 0;
  //  virtual void setCreators(const QStringList& creators) = 0;
  virtual QString creatorNames(const QStringList& names) = 0;
  virtual QString publisher() = 0;
  virtual void setPublisher(const QString& publisher) = 0;
  virtual QDate published() = 0;
  virtual void setPublished(const QDate& published) = 0;

  virtual QString buildTocFromData() = 0;

  virtual Metadata metadata() = 0;
  //  virtual QMap<QString, QString> pages() = 0;
  virtual ManifestItemMap pages() = 0;
  virtual QStringList spine() = 0;
  virtual QStringList cssKeys() = 0;
  virtual QString css(QString key) = 0;
  virtual CSSMap css() = 0;
  virtual QString javascript(QString key) = 0;
  virtual ManifestItem itemByHref(QString href) = 0;
};
typedef QSharedPointer<IEBookDocument> EBookDocument;

/*!
 * \brief Adds a signal for completion of load. Not possible in the interface.
 */
class ITextDocument
  : public QObject /*public QTextDocument*/
  , public IEBookDocument
{
  Q_OBJECT
public:
  ITextDocument(QObject* parent = nullptr);

signals:
  void loadCompleted();

protected:
};
typedef QSharedPointer<ITextDocument> EITextDocument;

/*!
 * \brief Variables common to all EBook types.
 */
class ITextDocumentPrivate
{
public:
  ~ITextDocumentPrivate();
  QString filename();
  void setFilename(QString filename);

  /*!
   * \brief Creates a & separated list of creators.
   * \return the list as a QString.
   */
  QString creatorNames(const QStringList& names);
  QString subject();
  void setSubject(QString subject);
  QString publisher();
  void setPublisher(QString publisher);
  QDate published();
  void setPublished(QDate date);

protected:
  IEBookInterface* m_plugin;
  QString m_filename, m_resource_path;
  QStringList m_creators;
  QString m_publisher;
  QDate m_published;
  QString m_subject;
  QString m_title;
  QMap<QString, QString> m_titles;
};

#endif // IEBOOKDOCUMENT_H
