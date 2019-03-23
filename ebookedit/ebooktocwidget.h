#ifndef EBOOKTOCWIDGET_H
#define EBOOKTOCWIDGET_H

#include <QMenu>
#include <QTextBrowser>

#include <ebookcommon.h>
#include <tocdisplaydocument.h>

class EBookTOCWidget : public QTextBrowser
{
  Q_OBJECT
public:
  EBookTOCWidget(QWidget* parent = nullptr);
  ~EBookTOCWidget();

  void setDocumentString(QString document_string);
  void enableHtmlMenuItem(bool enable);
  TocDisplayDocument* document();
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
