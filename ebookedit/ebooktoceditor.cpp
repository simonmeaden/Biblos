#include "ebooktoceditor.h"

EBookTocEditor::EBookTocEditor(QWidget* parent)
  : QDialog(parent)
{
  initGui();
}

void EBookTocEditor::setDocument(QTextDocument* document)
{
  QString doc_string = document->toHtml();
  setDocumentString(doc_string);
}

void EBookTocEditor::setDocumentString(QString document_string)
{
  //  document()->setHtml(document_string);
  // TODO convert to table items.
  QDomDocument *doc = new QDomDocument;
  doc->setContent(document_string);

  QDomElement root = doc->documentElement();
  if (!root.isNull()) {
    QDomNode list = root.namedItem("ul");
    if (!list.isNull()) {
      // TODO unordered list
      return;
    }

    list = root.namedItem("ol");
    if (!list.isNull()) {
      // TODO ordered list
      return;
    }
  }
}

void EBookTocEditor::initGui()
{
  QGridLayout* layout = new QGridLayout;
  setLayout(layout);

  m_toc_editor = new QTableWidget(this);
  layout->addWidget(m_toc_editor, 0, 0, 1, 2);

  m_toc_widget = new EBookTOCWidget(this);
  layout->addWidget(m_toc_widget, 0, 1, 1, 1);

  QFrame* btn_frame = new QFrame(this);
  QHBoxLayout* btn_layout = new QHBoxLayout;
  btn_frame->setLayout(btn_layout);

  QPushButton* cancel_btn = new QPushButton(tr("Cancel"), this);
  btn_layout->addWidget(cancel_btn);

  QPushButton* accept_btn = new QPushButton(tr("Accept Canges"), this);
  btn_layout->addWidget(accept_btn);

  QPushButton* help_btn = new QPushButton(tr("Help"), this);
  btn_layout->addWidget(help_btn);
}
