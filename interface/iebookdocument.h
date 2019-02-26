#ifndef IEBOOKDOCUMENT_H
#define IEBOOKDOCUMENT_H

#include <QDateTime>
#include <QString>
#include <QTextDocument>

#include "ebookcommon.h"
#include "ebookmetadata.h"

class IEBookInterface;

class IEBookDocument {
public:
  virtual ~IEBookDocument() {}

  virtual QString filename() = 0;
  virtual void setFilename(const QString &filename) = 0;
  virtual void openDocument(const QString &path) = 0;
  virtual void saveDocument() = 0;
  virtual IEBookInterface *plugin() = 0;
  virtual void setPlugin(IEBookInterface *plugin) = 0;
  virtual EBookDocumentType type() const = 0;

  virtual bool isModified() = 0;
  //  virtual bool readOnly() = 0;
  //  virtual void setReadOnly(const bool readonly) = 0;

  virtual QString tocAsString() = 0;

  virtual QString title() = 0;
  virtual void setTitle(const QString &title) = 0;
  virtual QString subject() = 0;
  virtual void setSubject(const QString &subject) = 0;
  virtual QString language() = 0;
  virtual void setLanguage(const QString &language) = 0;
  virtual QDateTime date() = 0;
  virtual void setDate(const QDateTime &date) = 0;
  virtual QStringList creators() = 0;
  //  virtual void setCreators(const QStringList& creators) = 0;
  virtual QString creatorNames(const QStringList &names) = 0;
  virtual QString publisher() = 0;
  virtual void setPublisher(const QString &publisher) = 0;
  virtual QDate published() = 0;
  virtual void setPublished(const QDate &published) = 0;

  virtual QString buildTocFromData() = 0;

  virtual Metadata metadata() = 0;
};

/*!
 * \brief Adds a signal for completion of load. Not possible in the interface.
 */
class ITextDocument : public QTextDocument, public IEBookDocument {
  Q_OBJECT
public:
  ITextDocument(QObject *parent = nullptr) : QTextDocument(parent) {}

signals:
  void loadCompleted();

protected:
};

/*!
 * \brief Variables common to all EBook types.
 */
class ITextDocumentPrivate {
public:
  QString filename() { return m_filename; }
  void setFilename(QString filename) { m_filename = filename; }
  //  bool isModified() { return m_modified; }
  //  void setModified(bool modified) { m_modified = modified; }
  //  bool readonly() { return m_readonly; }
  //  void setReadonly(bool readonly) { m_readonly = readonly; }
  // QStringList creators() { return m_creators; } QString creator(int i) {
  //  return m_creators.at(i); } void addCreator(QString creator) {
  //  m_creators.append(creator); } void addCreators(QStringList creators) {
  //  m_creators.append(creators); } void setCreators(QStringList creators) {
  //  m_creators = creators; } void removeCreator(QString creator) {
  //  m_creators.removeOne(creator); }
  /*!
   * \brief Creates a & separated list of creators.
   * \return the list as a QString.
   */
  QString creatorNames(const QStringList &names) {
    QString result;
    foreach (QString name, names) {
      if (result.isEmpty()) {
        result = name;
      } else {
        result += ", ";
        result += name;
      }
    }
    return result;
  }
  QString subject() { return m_subject; }
  void setSubject(QString subject) { m_subject = subject; }
  QString publisher() { return m_publisher; }
  void setPublisher(QString publisher) { m_publisher = publisher; }
  QDate published() { return m_published; }
  void setPublished(QDate date) { m_published = date; }

protected:
  IEBookInterface *m_plugin;
  QString m_filename;
  QStringList m_creators;
  QString m_publisher;
  QDate m_published;
  QString m_subject;
  QString m_title;
  QMap<QString, QString> m_titles;
};

#endif // IEBOOKDOCUMENT_H
