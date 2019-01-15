#include "ebooktocwidget.h"

EBookTOCWidget::EBookTOCWidget(QWidget* parent)
  : QTextBrowser(parent)
{
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this,&QWidget::customContextMenuRequested,
          this,&EBookTOCWidget::showContextMenu);
}

void EBookTOCWidget::setDocumentString(QString document_string)
{
  document()->setHtml(document_string);
}

//void EBookTOCWidget::contextMenuEvent(QContextMenuEvent* event)
//{

//}

void EBookTOCWidget::showContextMenu(const QPoint &point)
{
  QAction *buildTocFromHtml = new QAction(tr("Build TOC from HTML files"), this);
  connect(buildTocFromHtml, &QAction::triggered, this, &EBookTOCWidget::htmlToc);

  QAction *buildTocFromScratch = new QAction(tr("Build TOC Manually"), this);
  connect(buildTocFromScratch, &QAction::triggered, this, &EBookTOCWidget::manualToc);

  QAction *addAnchorsToToc = new QAction(tr("Add Anchors to TOC"), this);
  connect(addAnchorsToToc, &QAction::triggered, this, &EBookTOCWidget::addAnchors);

  QMenu *contextMenu = createStandardContextMenu();
  contextMenu->addSeparator();
  contextMenu->addAction(buildTocFromHtml);
  contextMenu->addAction(buildTocFromScratch);
  contextMenu->addAction(addAnchorsToToc);
  contextMenu->exec(mapToGlobal(point));
}

void EBookTOCWidget::manualToc()
{
  emit buildManualToc();
}

void EBookTOCWidget::htmlToc()
{
  emit buildTocFromHtmlFiles();
}

void EBookTOCWidget::addAnchors()
{
  emit addAnchorsToToc();
}
