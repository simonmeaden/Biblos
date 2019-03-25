#include "iebookdocument.h"

IEBookDocument::~IEBookDocument() {}

ITextDocumentPrivate::~ITextDocumentPrivate()
{
  QDir dir(m_resource_path);
  dir.removeRecursively();
}

QString
ITextDocumentPrivate::filename()
{
  return m_filename;
}

void
ITextDocumentPrivate::setFilename(QString filename)
{
  m_filename = filename;
  QFileInfo info(m_filename);
  m_resource_path = info.path() + QDir::separator() + "resources";
  QDir dir(m_resource_path);
  dir.mkpath(m_resource_path);
}

QString
ITextDocumentPrivate::creatorNames(const QStringList& names)
{
  QString result;
  for (int i = 0; i < names.size(); i++) {
    QString name = names.at(i);
    if (result.isEmpty()) {
      result = name;
    } else {
      result += ", ";
      result += name;
    }
  }
  return result;
}

QString
ITextDocumentPrivate::subject()
{
  return m_subject;
}

void
ITextDocumentPrivate::setSubject(QString subject)
{
  m_subject = subject;
}

QString
ITextDocumentPrivate::publisher()
{
  return m_publisher;
}

void
ITextDocumentPrivate::setPublisher(QString publisher)
{
  m_publisher = publisher;
}

QDate
ITextDocumentPrivate::published()
{
  return m_published;
}

void
ITextDocumentPrivate::setPublished(QDate date)
{
  m_published = date;
}

ITextDocument::ITextDocument(QObject* parent)
  : QObject(parent)
{}
