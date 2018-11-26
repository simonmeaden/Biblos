#include "metadataeditor.h"

MetadataEditor::MetadataEditor(Options* options, QWidget* parent)
  : QWidget(parent)
  , m_options(options)
{
  initGui();
}

void
MetadataEditor::addAuthors(SharedAuthorList authorlist)
{
  SharedAuthor author;
  QLineEdit* author_edit;
  for (int i = 0; i < authorlist.size(); i++) {
    author = authorlist.at(i);
    if (i >= m_authors.size()) {
      author_edit = new QLineEdit(this);
      m_authors.append(author_edit);
      authorLayout->addWidget(author_edit);
    } else
      author_edit = m_authors.at(i);

    QString name = author->forename().trimmed();
    QString midname = author->middlenames().trimmed();
    QString surname = author->surname().trimmed();

    if (!midname.isEmpty()) {
      if (!name.isEmpty())
        name += " ";
      name += midname;
    }

    if (!surname.isEmpty()) {
      if (!name.isEmpty())
        name += " ";
      name += surname;
    }
    author_edit->setText(name);
  }
}

void
MetadataEditor::initGui()
{
  QGridLayout* mainLayout = new QGridLayout;
  setLayout(mainLayout);

  QLabel* lbl = new QLabel(tr("Title : "), this);
  mainLayout->addWidget(lbl, 0, 0);

  m_title = new QLineEdit(this);
  mainLayout->addWidget(m_title, 0, 1);

  lbl = new QLabel(tr("Authors(s) :"), this);
  mainLayout->addWidget(lbl, 1, 0);

  QFrame* authorFrame = new QFrame(this);
  authorLayout = new QVBoxLayout;
  authorFrame->setLayout(authorLayout);
  mainLayout->addWidget(authorFrame, 1, 1);

  QLineEdit* author = new QLineEdit(this);
  m_authors.append(author);
  authorLayout->addWidget(author);
}
