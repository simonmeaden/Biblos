#ifndef EBOOKTOCWIDGET_H
#define EBOOKTOCWIDGET_H

#include <QMenu>
#include <QTextBrowser>

struct TocPosition {
  QTextCursor start;
  QTextCursor end;

  bool isValid()
  {
    return (start.position() != end.position());
  }
};

class TocDisplayDocument : public QTextDocument
{
  Q_OBJECT
public:
  TocDisplayDocument(QObject* parent);
  ~TocDisplayDocument();

  void addPosition(int line_index, TocPosition& position);
  TocPosition position(int index);

protected:
  QMap<int, TocPosition> m_positioning;
};

class EBookTOCWidget : public QTextBrowser
{
  Q_OBJECT
public:
  EBookTOCWidget(QWidget* parent = Q_NULLPTR);

  void setDocumentString(QString document_string);
  void enableHtmlMenuItem(bool enable);
  TocDisplayDocument *document();
  void setDocument(TocDisplayDocument* document);

signals:
  void buildManualToc();
  void buildTocFromHtmlFiles();
  void addAnchorsToToc();

protected:
  //  void contextMenuEvent(QContextMenuEvent* event);
  bool m_enable_html = true;

  void showContextMenu(const QPoint& point);
  void manualToc();
  void htmlToc();
  void addAnchors();
};

#endif // EBOOKTOCWIDGET_H
