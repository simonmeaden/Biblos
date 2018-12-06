#ifndef EBOOKWORDREADER_H
#define EBOOKWORDREADER_H

#include <QCoreApplication>
#include <QRegularExpression>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QTextEdit>
#include <QThread>

class EBookEditor;

class EBookTextCursor : public QTextCursor
{
public:
  EBookTextCursor();
  EBookTextCursor(QTextDocument* document);
  EBookTextCursor(const QTextBlock& block);
  EBookTextCursor(const QTextCursor& cursor);

  QString nextCharacter(int num = 1) const;
  QString prevCharacter(int num = 1) const;

  void moveToWordStart(MoveMode moveMode = MoveAnchor);
  void moveToWordEnd(MoveMode moveMode = MoveAnchor);

  bool isInsideWord() const;
  bool isWordChar(const QString& character) const;

protected:
  QRegularExpression m_regex;
};

class EBookWordReader : public QThread
{
  Q_OBJECT
public:
  explicit EBookWordReader(EBookEditor* editor, QObject* parent = Q_NULLPTR);
    ~EBookWordReader();

  void stopRunning();
  void documentIsLoaded();

signals:
  void finished();

protected:
  bool m_running, m_document_loaded;
  EBookEditor* m_editor;
  QTextDocument* m_document;

  void run();
  QString getNextWord(int *pos);
};

#endif // EBOOKWORDREADER_H
