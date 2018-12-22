#ifndef IEBOOKDOCUMENT_H
#define IEBOOKDOCUMENT_H

#include <QDateTime>
#include <QString>
#include <QTextDocument>

class IEBookInterface;

class IEBookDocument
{
public:
  virtual ~IEBookDocument() {}

  virtual QString filename() const = 0;
  virtual void setFilename(const QString& filename) = 0;
  virtual void openDocument(const QString& path) = 0;
  virtual IEBookInterface* plugin() const = 0;
  virtual void setPlugin(IEBookInterface* plugin) = 0;

  virtual bool isModified() const = 0;
  virtual bool readOnly() const = 0;
  virtual void setReadOnly(const bool readonly) = 0;

  virtual QString title() = 0;
  virtual void setTitle(const QString& title) = 0;
  virtual QString subject() = 0;
  virtual void setSubject(const QString& subject) = 0;
  virtual QString language() = 0;
  virtual void setLanguage(const QString& language) = 0;
  virtual QDateTime date() = 0;
  virtual void setDate(const QDateTime& date) = 0;
  virtual QStringList authors() = 0;
  virtual void setAuthors(const QStringList& authors) = 0;
  virtual QString authorNames() = 0;
  virtual QString publisher() = 0;
  virtual void setPublisher(const QString& publisher) = 0;
};

/*!
 * \brief Adds a signal for completion of load. Not possible in the interface.
 */
class ITextDocument : public QTextDocument, public IEBookDocument
{
  Q_OBJECT
public:
  ITextDocument(QObject* parent = Q_NULLPTR) : QTextDocument(parent) {}

signals:
  void loadCompleted();
};

#endif // IEBOOKDOCUMENT_H
