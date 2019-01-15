#ifndef EBOOKTOCWIDGET_H
#define EBOOKTOCWIDGET_H

#include <QTextBrowser>
#include <QMenu>

class EBookTOCWidget : public QTextBrowser
{
  Q_OBJECT
public:
  EBookTOCWidget(QWidget* parent = Q_NULLPTR);

  void setDocumentString(QString document_string);

signals:
  void buildManualToc();
  void buildTocFromHtmlFiles();
  void addAnchorsToToc();


protected:
//  void contextMenuEvent(QContextMenuEvent* event);

  void showContextMenu(const QPoint& point);
  void manualToc();
  void htmlToc();
  void addAnchors();
};

#endif // EBOOKTOCWIDGET_H
