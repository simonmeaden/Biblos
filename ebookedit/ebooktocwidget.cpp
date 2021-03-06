#include "ebooktocwidget.h"

EBookTOCWidget::EBookTOCWidget(QWidget* parent)
  : QTextBrowser(parent)
{
  setDocument(new TocDisplayDocument(this));
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &QWidget::customContextMenuRequested, this, &EBookTOCWidget::showContextMenu);
}

void EBookTOCWidget::setDocumentString(QString document_string)
{
  document()->setHtml(document_string);
}

void EBookTOCWidget::enableHtmlMenuItem(bool enable)
{
  m_enable_html = enable;
}

TocDisplayDocument* EBookTOCWidget::document()
{
  return qobject_cast<TocDisplayDocument*>(QTextBrowser::document());
}

void EBookTOCWidget::setDocument(TocDisplayDocument* document)
{
  QTextBrowser::setDocument(document);
}

void EBookTOCWidget::showContextMenu(const QPoint& point)
{
  QAction* build_toc_from_html = new QAction(tr("Build TOC from HTML files"), this);
  connect(build_toc_from_html, &QAction::triggered, this, &EBookTOCWidget::htmlToc);
  build_toc_from_html->setEnabled(m_enable_html);

  QAction* build_toc_from_scratch = new QAction(tr("Edit TOC Manually"), this);
  connect(build_toc_from_scratch, &QAction::triggered, this, &EBookTOCWidget::manualToc);

  QAction* add_anchors_to_toc = new QAction(tr("Add Anchors to TOC"), this);
  connect(add_anchors_to_toc, &QAction::triggered, this, &EBookTOCWidget::addAnchors);

  QMenu* contextMenu = createStandardContextMenu();
  contextMenu->addSeparator();
  contextMenu->addAction(build_toc_from_html);
  contextMenu->addAction(build_toc_from_scratch);
  contextMenu->addAction(add_anchors_to_toc);
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

TocDisplayDocument::TocDisplayDocument(QObject* parent)
  : QTextDocument(parent)
{
}

TocDisplayDocument::~TocDisplayDocument()
{
}

void TocDisplayDocument::addLinePosition(int line_index, int start_position, int length)
{
  QPair<int, int> data = qMakePair<int,int>(start_position, length);
//  TocLinePosition line_position;
//  line_position.line_start = start_position;
//  line_position.filename = filename;
//  line_position.anchor = anchor;
//  line_position.contents = contents;
//  line_position.line_length = length;
  m_toc_positioning.insert(line_index, data);
}

QPair<int, int> TocDisplayDocument::linePosition(int index)
{
  return m_toc_positioning.value(index);
}
