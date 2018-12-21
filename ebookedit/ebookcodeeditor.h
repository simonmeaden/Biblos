#ifndef EBOOKCODEEDITOR_H
#define EBOOKCODEEDITOR_H

#include <QPainter>
#include <QPlainTextEdit>

#include "iebookdocument.h"
#include "xhtmlhighlighter.h"

class EBookCodeEditor : public QPlainTextEdit {
 public:
  EBookCodeEditor(QWidget* parent = Q_NULLPTR);
  EBookCodeEditor(Options* options, QWidget* parent = Q_NULLPTR);
  EBookCodeEditor(const EBookCodeEditor& editor);
  ~EBookCodeEditor() override;

  void lineNumberAreaPaintEvent(QPaintEvent* event);
  int lineNumberAreaWidth();

  void setDocument(IEBookDocument* document);
  void rehighlight();

 protected:
  QWidget* lineNumberArea;
  XhtmlHighlighter* m_highlighter;
  Options* m_options;

  void resizeEvent(QResizeEvent* event) override;
  void updateLineNumberAreaWidth(int newBlockCount);
  void highlightCurrentLine();
  void updateLineNumberArea(const QRect&, int);
  void init();
};

class LineNumberArea : public QWidget {
 public:
  LineNumberArea(EBookCodeEditor* editor);

  QSize sizeHint() const override;

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  EBookCodeEditor* codeEditor;
};

Q_DECLARE_METATYPE(EBookCodeEditor)

#endif  // EBOOKCODEEDITOR_H
