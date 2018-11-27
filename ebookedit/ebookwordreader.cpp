#include "ebookwordreader.h"

#include <qlogger/qlogger.h>

#include "ebookeditor.h"

using namespace qlogger;

EBookWordReader::EBookWordReader(EBookEditor* editor, QObject* parent)
  : QThread(parent)
  , m_running(true)
  , m_document_loaded(false)
  , m_editor(editor)
  , m_document(Q_NULLPTR)
{}

EBookWordReader::~EBookWordReader()
{
//  m_editor = Q_NULLPTR;
//  m_document = Q_NULLPTR;
}

void
EBookWordReader::stopRunning()
{
  m_running = false;
}

void
EBookWordReader::documentIsLoaded()
{
  m_document = m_editor->document();
  m_document_loaded = true;
}

void
EBookWordReader::run()
{
  QString word;
//  QTextDocumentFragment fragment;
//  while (m_running) {
//    int end, start;

//    if (m_document_loaded) {
//      start = 0;
//      // we are checking the entire document so set end at end of document.
//      QTextCursor tmpCursor(m_editor->textCursor());
//      tmpCursor.movePosition(QTextCursor::End);
//      end = tmpCursor.position();

//      // block signals during check
//      m_editor->document()->blockSignals(true);

//      EBookTextCursor cursor(m_editor->textCursor());
//      cursor.beginEditBlock();
//      cursor.setPosition(start);
//      while (cursor.position() < end) {
//        cursor.moveToWordEnd(QTextCursor::KeepAnchor);
//      }

//      while (cursor.position() < end &&
//             !cursor.isWordChar(cursor.nextCharacter())) {
//        cursor.movePosition(QTextCursor::NextCharacter);
//      }
//      cursor.endEditBlock();
//      // clear signal block
//      m_editor->document()->blockSignals(false);

//      thread()->msleep(100);
//    }
//  }
}

EBookTextCursor::EBookTextCursor()
  : QTextCursor()
  , m_regex(QRegularExpression("^\\w$"))
{}

EBookTextCursor::EBookTextCursor(QTextDocument* document)
  : QTextCursor(document)
  , m_regex(QRegularExpression("^\\w$"))
{}

EBookTextCursor::EBookTextCursor(const QTextBlock& block)
  : QTextCursor(block)
  , m_regex(QRegularExpression("^\\w$"))
{}

EBookTextCursor::EBookTextCursor(const QTextCursor& cursor)
  : QTextCursor(cursor)
  , m_regex(QRegularExpression("^\\w$"))
{}

/*! /brief Retreive the num-th next character.
 *
 * /param index - the offset of the character to retreive.
 * /return A string containing the character, which might be empty.
 */
QString
EBookTextCursor::nextCharacter(int index) const
{
  EBookTextCursor cursor(*this);
  if (index > 1)
    cursor.movePosition(NextCharacter, MoveAnchor, index - 1);
  else
    cursor.setPosition(cursor.position());
  cursor.movePosition(NextCharacter, KeepAnchor);
  return cursor.selectedText();
}

/*! /brief Retreive the num-th previous character.
 *
 * /param index - the offset of the character to retreive.
 * /return A string containing the character, which might be empty.
 */
QString
EBookTextCursor::prevCharacter(int index) const
{
  EBookTextCursor cursor(*this);
  if (index > 1)
    cursor.movePosition(PreviousCharacter, MoveAnchor, index - 1);
  else
    cursor.setPosition(cursor.position());
  cursor.movePosition(PreviousCharacter, KeepAnchor);
  return cursor.selectedText();
}

/*! /brief Move the cursor to the start of the current word. Cursor must be
 *        inside a word. This method correctly honours apostrophes.
 *
 * /param moveMode The move mode, see QTextCursor::MoveMode.
 */
void
EBookTextCursor::moveToWordStart(QTextCursor::MoveMode moveMode)
{
  movePosition(StartOfWord, moveMode);
  QLOG_DEBUG(QString("Start : %1 : %2%3 | %4")
               .arg(position())
               .arg(prevCharacter(2))
               .arg(prevCharacter())
               .arg(nextCharacter()));

  // If we are in front of a quote...
  if (nextCharacter() == "'") {
    // If the previous char is alphanumeric, move left one word, otherwise
    // move right one char
    if (prevCharacter().contains(m_regex)) {
      movePosition(WordLeft, moveMode);
    } else {
      movePosition(NextCharacter, moveMode);
    }
  }
  // If the previous char is a quote, and the char before that is
  // alphanumeric, move left one word
  else if (prevCharacter() == "'" && prevCharacter(2).contains(m_regex)) {
    movePosition(
      WordLeft, moveMode, 2); // 2: because quote counts as a word boundary
  }
}

/*! /brief Move the cursor to the end of the current word. Cursor must be
 *        inside a word. This method correctly honours apostrophes.
 *
 * /param moveMode The move mode, see QTextCursor::MoveMode.
 */
void
EBookTextCursor::moveToWordEnd(QTextCursor::MoveMode moveMode)
{
  movePosition(EndOfWord, moveMode);
  QLOG_DEBUG(QString("End : %1 : %2 | %3%4")
               .arg(position())
               .arg(prevCharacter())
               .arg(prevCharacter())
               .arg(nextCharacter(2)));

  // If we are in behind of a quote...
  if (prevCharacter() == "'") {
    // If the next char is alphanumeric, move right one word, otherwise move
    // left one char
    if (nextCharacter().contains(m_regex)) {
      movePosition(WordRight, moveMode);
    } else {
      movePosition(PreviousCharacter, moveMode);
    }
  }
  // If the next char is a quote, and the char after that is alphanumeric,
  // move right one word
  else if (nextCharacter() == "'" && nextCharacter(2).contains(m_regex)) {
    movePosition(
      WordRight, moveMode, 2); // 2: because quote counts as a word boundary
  }
}

/*! /brief Returns whether the cursor is inside a word.
 *
 * /return Whether the cursor is inside a word.
 */
bool
EBookTextCursor::isInsideWord() const
{
  return nextCharacter().contains(m_regex) || prevCharacter().contains(m_regex);
}

/*! /brief Returns whether the specified character is a word character.
 *
 * /return Whether the specified character is a word character.
 */
bool
EBookTextCursor::isWordChar(const QString& character) const
{
  return character.contains(m_regex);
}
