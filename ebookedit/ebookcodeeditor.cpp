#include "ebookcodeeditor.h"

EBookCodeEditor::EBookCodeEditor(QWidget* parent) : QPlainTextEdit(parent) {
  init();
}

EBookCodeEditor::EBookCodeEditor(Options* options, QWidget* parent)
    : QPlainTextEdit(parent), m_options(options) {
  init();
}

EBookCodeEditor::EBookCodeEditor(const EBookCodeEditor& editor)
    : QPlainTextEdit(editor.parentWidget()) {}
EBookCodeEditor::~EBookCodeEditor() {}

void EBookCodeEditor::init() {
  QFont font;
  font.setFamily("Courier");
  font.setFixedPitch(true);
  font.setPointSize(10);
  setFont(font);

  lineNumberArea = new LineNumberArea(this);

  connect(this, &EBookCodeEditor::blockCountChanged, this,
          &EBookCodeEditor::updateLineNumberAreaWidth);
  connect(this, &EBookCodeEditor::updateRequest, this,
          &EBookCodeEditor::updateLineNumberArea);
  connect(this, &EBookCodeEditor::cursorPositionChanged, this,
          &EBookCodeEditor::highlightCurrentLine);

  updateLineNumberAreaWidth(0);
  highlightCurrentLine();
}

void EBookCodeEditor::lineNumberAreaPaintEvent(QPaintEvent* event) {
  QPainter painter(lineNumberArea);
  painter.fillRect(event->rect(), Qt::lightGray);

  QTextBlock block = firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top = int(blockBoundingGeometry(block).translated(contentOffset()).top());
  int bottom = top + int(blockBoundingRect(block).height());

  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      QString number = QString::number(blockNumber + 1);
      painter.setPen(Qt::black);
      painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                       Qt::AlignRight, number);
    }

    block = block.next();
    top = bottom;
    bottom = top + int(blockBoundingRect(block).height());
    ++blockNumber;
  }
}

int EBookCodeEditor::lineNumberAreaWidth() {
  int digits = 1;
  int max = qMax(1, blockCount());
  while (max >= 10) {
    max /= 10;
    ++digits;
  }

  int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

  return space;
}

void EBookCodeEditor::resizeEvent(QResizeEvent* e) {
  QPlainTextEdit::resizeEvent(e);

  QRect cr = contentsRect();
  lineNumberArea->setGeometry(
      QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void EBookCodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */) {
  setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void EBookCodeEditor::highlightCurrentLine() {
  QList<QTextEdit::ExtraSelection> extraSelections;

  if (!isReadOnly()) {
    QTextEdit::ExtraSelection selection;

    QColor lineColor = QColor(Qt::yellow).lighter(160);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);
  }

  setExtraSelections(extraSelections);
}

void EBookCodeEditor::updateLineNumberArea(const QRect& rect, int dy) {
  if (dy)
    lineNumberArea->scroll(0, dy);
  else
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

  if (rect.contains(viewport()->rect())) updateLineNumberAreaWidth(0);
}

void EBookCodeEditor::setDocument(IEBookDocument* document) {
  QTextDocument* doc = dynamic_cast<QTextDocument*>(document);
  m_highlighter = new XhtmlHighlighter(m_options, doc);
  QPlainTextEdit::setDocument(doc);
}

/*!
 * \brief Forces the internal text highlighter to rehighlight the code.
 *
 * Normally this is triggered when the user resets the options colours
 * or font weights or styles.
 */
void EBookCodeEditor::rehighlight() {
  setFont(m_options->codeFont);
  m_highlighter->resetFormattingOptions();
  m_highlighter->rehighlight();
}

LineNumberArea::LineNumberArea(EBookCodeEditor* editor) : QWidget(editor) {
  codeEditor = editor;
}

QSize LineNumberArea::sizeHint() const {
  return QSize(codeEditor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent* event) {
  codeEditor->lineNumberAreaPaintEvent(event);
}
