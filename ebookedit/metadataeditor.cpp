#include "metadataeditor.h"

MetadataEditor::MetadataEditor(Options* options, QWidget* parent)
  : QWidget(parent)
  , m_options(options)
{
  initGui();
}

void MetadataEditor::addAuthors(SharedAuthorList authorlist)
{
  SharedAuthor author;
  QLineEdit* author_edit;
  for (int i = 0; i < authorlist.size(); i++) {
    author = authorlist.at(i);
    if (i >= m_authors.size()) {
      author_edit = new QLineEdit(this);
      m_authors.append(author_edit);
      author_layout->addWidget(author_edit);
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

void MetadataEditor::setFileInfo(QString filepath)
{
  QFileInfo file_info(filepath);
  QString filename = file_info.fileName();
  QString extension = file_info.suffix();
  QString full_extension = file_info.completeSuffix();

  m_filename_edit->setText(filename);
  m_fileext_lbl->setText(full_extension);
}

void MetadataEditor::initGui()
{
  QGridLayout* main_layout = new QGridLayout;
  setLayout(main_layout);

  int row = 0;
  QLabel* lbl = new QLabel(tr("Filename : "), this);
  main_layout->addWidget(lbl, row, 0);
  m_filename_edit = new QLineEdit(this);
  main_layout->addWidget(m_filename_edit, row, 1);
  m_fileext_lbl = new QLabel(this);
  main_layout->addWidget(m_fileext_lbl, row++, 2);

  lbl = new QLabel(tr("Title : "), this);
  main_layout->addWidget(lbl, row, 0);
  m_title_edit = new QLineEdit(this);
  main_layout->addWidget(m_title_edit, row++, 1, 1, 2);

  lbl = new QLabel(tr("Authors(s) :"), this);
  main_layout->addWidget(lbl, row, 0);

  QFrame* author_frame = new QFrame(this);
  author_layout = new QVBoxLayout;
  author_frame->setLayout(author_layout);
  main_layout->addWidget(author_frame, row++, 1, 1, 2);

  QLineEdit* author = new QLineEdit(this);
  m_authors.append(author);
  author_layout->addWidget(author);
}
