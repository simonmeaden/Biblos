#include "ebooktoceditor.h"

const QString EBookTocEditor::TOC_TITLE = "<h2>%1</h2>";
const QString EBookTocEditor::LIST_START = "<html><body><ul>";
const QString EBookTocEditor::LIST_END = "</ul></body></html>";
const QString EBookTocEditor::LIST_ITEM = "<li><a href=\"%1\">%2</li>";
const QString EBookTocEditor::LIST_BUILD_ITEM = "<li><a href=\"%1#%2\">%3</li>";
const QString EBookTocEditor::LIST_FILEPOS = "position%1";
const QString EBookTocEditor::LIST_SEPARATOR = " ";

EBookTocEditor::EBookTocEditor(QWidget* parent)
  : QDialog(parent)
  , m_loaded(false)
  , m_initialised(false)
{
  initGui();
  int x = parent->x() + (parent->width() / 2) - 450;
  int y = parent->y() + (parent->height() / 2) - 150;
  setGeometry(x, y, 900, 900);
  update();
}

void EBookTocEditor::setDocument(QTextDocument* document)
{
  QString doc_string = document->toHtml();
  setDocumentString(doc_string);
}

void EBookTocEditor::setDocumentString(QString document_string)
{
  m_original_string = document_string;
  m_modified = false;
  m_initialised = false;

  TocDisplayDocument* m_display_document = m_toc_display->document();
  QString toc_string = LIST_START;

  QDomDocument* doc = new QDomDocument;
  doc->setContent(document_string);
  m_toc_editor->clearContents();
  m_toc_editor->setRowCount(0);

  QDomElement root = doc->documentElement();
  if (!root.isNull()) {
    QDomNodeList html = doc->elementsByTagName("html");
    for (int i = 0; i < html.count(); i++) {
      QDomElement html_elem = html.at(i).toElement();
      QDomNodeList body = html_elem.elementsByTagName("body");
      for (int i = 0; i < body.count(); i++) {
        QDomElement body_elem = body.at(i).toElement();
        QDomNodeList ul = body_elem.elementsByTagName("ul");
        for (int i = 0; i < ul.count(); i++) {
          QDomElement ul_elem = ul.at(i).toElement();
          QDomNodeList li = ul_elem.elementsByTagName("li");
          for (int i = 0; i < li.count(); i++) {
            QDomElement li_elem = li.at(i).toElement();
            QDomNodeList a = li_elem.elementsByTagName("a");
            for (int i = 0; i < a.count(); i++) {
              QDomElement a_elem = a.at(i).toElement();
              QString href = a_elem.attribute("href");
              if (!href.isEmpty()) {
                QStringList splits = href.split("#");
                QString file, anchor, contents, doc_line;
                if (splits.size() > 0)
                  file = splits.at(0);
                if (splits.size() > 1)
                  anchor = splits.at(1);
                contents = a_elem.text();
                int row = m_toc_editor->rowCount();
                m_toc_editor->insertRow(row);

                QTableWidgetItem* enabled_item = new QTableWidgetItem();
                enabled_item->setCheckState(Qt::Checked);
                QTableWidgetItem* file_item = new QTableWidgetItem(file);
                QTableWidgetItem* anchor_item = new QTableWidgetItem(anchor);
                QTableWidgetItem* contents_item = new QTableWidgetItem(contents);
                m_toc_editor->setItem(row, 0, enabled_item);
                m_toc_editor->setItem(row, 1, file_item);
                m_toc_editor->setItem(row, 2, anchor_item);
                m_toc_editor->setItem(row, 3, contents_item);

                int start_position = toc_string.length();
                doc_line = LIST_BUILD_ITEM.arg(file).arg(anchor).arg(contents);
                toc_string.append(doc_line);
                m_display_document->addLinePosition(row, start_position, doc_line.length());
              }
            }
          }
        }
      }
    }
  }

  toc_string += LIST_END;
  m_display_document->setTocString(toc_string);
  m_initialised = true;
}

QString EBookTocEditor::documentString()
{
  return m_result_string;
}

void EBookTocEditor::initGui()
{
  QGridLayout* layout = new QGridLayout;
  setLayout(layout);

  QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
  QSizePolicy policy = splitter->sizePolicy();
  policy.setVerticalPolicy(QSizePolicy::Expanding);
  splitter->setSizePolicy(policy);
  layout->addWidget(splitter, 0, 0);

  m_toc_editor = new QTableWidget(1, 4, this);
  policy = m_toc_editor->sizePolicy();
  policy.setHorizontalStretch(2);
  m_toc_editor->setSizePolicy(policy);
  QStringList headers;
  headers << tr("Enabled") << tr("File") << tr("Anchor") << tr("Text");
  m_toc_editor->setHorizontalHeaderLabels(headers);
  m_toc_editor->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_toc_editor->horizontalHeader()->setStretchLastSection(true);
  m_toc_editor->verticalHeader()->setVisible(false);
  m_toc_editor->setSelectionMode(QTableWidget::SingleSelection);
  splitter->addWidget(m_toc_editor);
  connect(m_toc_editor, &QTableWidget::cellChanged, this, &EBookTocEditor::tocEditorCellChanged);

  m_toc_display = new EBookTOCWidget(this);
  policy = m_toc_display->sizePolicy();
  policy.setHorizontalStretch(1);
  m_toc_display->setSizePolicy(policy);
  m_toc_display->setReadOnly(true);
  splitter->addWidget(m_toc_display);

  QFrame* btn_frame = new QFrame(this);
  QHBoxLayout* btn_layout = new QHBoxLayout;
  btn_frame->setLayout(btn_layout);

  QPushButton* cancel_btn = new QPushButton(tr("Cancel"), this);
  connect(cancel_btn, &QPushButton::clicked, this, &EBookTocEditor::cancelClicked);
  btn_layout->addWidget(cancel_btn);

  QPushButton* accept_btn = new QPushButton(tr("Accept Canges"), this);
  connect(accept_btn, &QPushButton::clicked, this, &EBookTocEditor::acceptClicked);
  btn_layout->addWidget(accept_btn);

  QPushButton* help_btn = new QPushButton(tr("Help"), this);
  connect(help_btn, &QPushButton::clicked, this, &EBookTocEditor::helpClicked);
  btn_layout->addWidget(help_btn);
  layout->addWidget(btn_frame, 1, 0);
}

void EBookTocEditor::updateLine(int row, bool enabled)
{
  /* I initially used QTextCusrors to store line positions to allow modification of
   * the toc display document but this QTextCursors ignore all of the html tags
   * and only handles the text so these were all wrong. I now manage points in a
   * QString which is modified and passed to the document.
   */
  TocDisplayDocument* toc_document = m_toc_display->document();
  QPair<int, int> position = toc_document->linePosition(row);
  QString toc_string = toc_document->tocString();
  // remove the old line values.
  toc_string.remove(position.first, position.second);
  int old_line_length = position.second;

  QString doc_line;
  if (enabled) {
    QString file = m_toc_editor->item(row, 1)->text();
    QString anchor = m_toc_editor->item(row, 2)->text();
    QString contents = m_toc_editor->item(row, 3)->text();
    doc_line = LIST_BUILD_ITEM.arg(file).arg(anchor).arg(contents);
    position.second = doc_line.length();
  } else {
    doc_line = "";
    position.second = 0;
  }

  // update offsets for new line length.
  toc_document->updateLinePosition(row, position, position.second - old_line_length);
  // insert the modified line (possibly empty).
  toc_string.insert(position.first, doc_line);
  // reset the toc string.
  m_toc_display->document()->setTocString(toc_string);
}

void EBookTocEditor::tocEditorCellChanged(int row, int column)
{
  if (m_initialised) {
    switch (column) {
    case 0: {
      QTableWidgetItem* enabled_item = m_toc_editor->item(row, column);
      Qt::CheckState state = enabled_item->checkState();
      if (state == Qt::Checked) {
        updateLine(row, true);
      } else {
        updateLine(row, false);
      }
      m_modified = true;
      break;
    }
    case 1:
      updateLine(row, true);
      m_modified = true;
      break;
    case 2:
      updateLine(row, true);
      m_modified = true;
      break;
    case 3:
      updateLine(row, true);
      m_modified = true;
      break;
    default:
      m_modified = true;
      break;
    }
  }
  //  rebuildTocString();
}

void EBookTocEditor::tocLineEnabledClicked()
{
  //  rebuildTocString();
  //  if ()
}

// void EBookTocEditor::rebuildTocString(int row)
//{

//  //  QString modified_string = LIST_START;
//  //  QString file, anchor, contents;
//  //  for (int row = 0; row < m_toc_editor->rowCount(); row++) {
//  //    QCheckBox* enabledBox = qobject_cast<QCheckBox*>(m_toc_editor->cellWidget(row, 0));
//  //    if (enabledBox->isChecked()) {
//  //      file = m_toc_editor->item(row, 1)->data(Qt::DisplayRole).toString();
//  //      anchor = m_toc_editor->item(row, 2)->data(Qt::DisplayRole).toString();
//  //      contents = m_toc_editor->item(row, 3)->data(Qt::DisplayRole).toString();
//  //      modified_string += LIST_BUILD_ITEM.arg(file).arg(anchor).arg(contents);
//  //    } // otherwise ignore it.
//  //  }
//  //  modified_string += LIST_END;
//  //  m_toc_display->document()->setHtml(modified_string);
//}

void EBookTocEditor::cancelClicked()
{
  m_result_string = m_original_string;
  reject();
}

void EBookTocEditor::acceptClicked()
{
  if (m_modified) {
    // TODO build result from modified data.
  } else {
    m_result_string = m_original_string;
  }
  accept();
}

void EBookTocEditor::helpClicked()
{
  // TODO help dialog.
}

//============================================================================================
