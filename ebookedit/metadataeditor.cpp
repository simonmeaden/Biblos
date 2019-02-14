#include "metadataeditor.h"
#include "authordialog.h"
#include "iebookdocument.h"

MetadataEditor::MetadataEditor(Options* options, AuthorsDB* authors,
                               QWidget* parent)
  : QWidget(parent)
  , m_options(options)
  , m_author_db(authors)
{
  initGui();
}

void MetadataEditor::setDocument(ITextDocument* doc)
{
  m_document = doc;
  setTitle(doc->title());

  setFileInfo(doc->filename());

  QStringList creators = m_document->creators();
  AuthorList author_list;
  foreach (QString creator, creators) {
    AuthorData author_data = m_author_db->author(creator);
    if (!author_data) {
      QStringList names = creator.split(" ");
      // surname is usually either last (western names) or first (primarily
      // asian names).
      if (names.size() > 1) {
        QString last = names.last();
        AuthorList author_list = m_author_db->authorsBySurname(last);
        if (author_list.size() == 1) {
          author_data = author_list.at(0);
        } else {
          // check all for most accurate.
        }
      }

      author_data = AuthorData(new EBookAuthorData());
      author_data->display_name = creator;
      author_data->uid = m_author_db->insertAuthor(author_data);
    } else {
      author_list.append(author_data);
    }
  }
  addAuthors(author_list);
}

void MetadataEditor::setTitle(QString title)
{
  m_title_edit->setText(title);
}

void MetadataEditor::addAuthors(AuthorList author_list)
{
  AuthorData author;
  QLineEdit* author_edit;
  m_author_list.clear();
  for (int i = 0; i < author_list.size(); i++) {
    author = author_list.at(i);
    if (i == 0 && m_authors.size() == 1 && m_authors.at(0)->text().isEmpty()) {
      // this one is the empty one created at startup
      author_edit = m_authors.at(0);
    } else if (i >= m_authors.size()) {
      author_edit = new QLineEdit(this);
      m_authors.append(author_edit);
      m_main_layout->removeWidget(m_spacer);
      m_main_layout->addWidget(author_edit, m_next_author_row, 1);
      QPushButton* edit_title_btn =
        new QPushButton(QStringLiteral("Edit"), this);
      m_main_layout->addWidget(edit_title_btn, m_next_author_row++, 2);
      m_main_layout->addWidget(m_spacer, m_next_author_row, 0, 1, 4);
      connect(edit_title_btn, &QPushButton::clicked, this,
              &MetadataEditor::editAuthor);
    } else
      author_edit = m_authors.at(i);

    QString name = author->forename.trimmed();
    QString surname = author->surname.trimmed();

    if (!surname.isEmpty()) {
      if (!name.isEmpty())
        name += " ";
      name += surname;
    }
    author_edit->setText(name);
  }
  m_author_list = author_list;
}

void MetadataEditor::setFileInfo(QString filepath)
{
  QFileInfo file_info(filepath);
  QString filename = file_info.baseName();
  //  QString extension = file_info.suffix();
  QString full_extension = file_info.completeSuffix();
  full_extension.prepend(".");

  m_filename_edit->setText(filename);
  m_fileext_lbl->setText(full_extension);
}

void MetadataEditor::initGui()
{
  m_main_layout = new QGridLayout;
  setLayout(m_main_layout);

  int row = 0;
  QLabel* lbl = new QLabel(tr("Filename : "), this);
  m_main_layout->addWidget(lbl, row, 0);
  m_filename_edit = new QLineEdit(this);
  m_filename_edit->setReadOnly(true);
  m_main_layout->addWidget(m_filename_edit, row, 1);
  m_fileext_lbl = new QLabel(this);
  m_main_layout->addWidget(m_fileext_lbl, row, 2);
  QPushButton* filename_edit_btn =
    new QPushButton(QStringLiteral("Edit"), this);
  m_main_layout->addWidget(filename_edit_btn, row++, 3);
  connect(filename_edit_btn, &QPushButton::clicked, this,
          &MetadataEditor::editFilename);

  lbl = new QLabel(tr("Title : "), this);
  m_main_layout->addWidget(lbl, row, 0);
  m_title_edit = new QLineEdit(this);
  m_title_edit->setReadOnly(true);
  m_main_layout->addWidget(m_title_edit, row, 1, 1, 2);
  QPushButton* edit_title_btn = new QPushButton(QStringLiteral("Edit"), this);
  m_main_layout->addWidget(edit_title_btn, row++, 3);
  connect(edit_title_btn, &QPushButton::clicked, this,
          &MetadataEditor::editTitle);

  lbl = new QLabel(tr("Authors(s) :"), this);
  m_main_layout->addWidget(lbl, row, 0);

  //  QFrame* author_frame = new QFrame(this);
  //  author_frame->setStyleSheet(QStringLiteral("QFrame { border: 0px; padding:
  //  0px; }")); author_frame->setContentsMargins(0, 0, 0, 0); m_author_layout =
  //  new QGridLayout; author_frame->setLayout(m_author_layout);
  //  main_layout->addWidget(author_frame, row++, 1, 1, 4);

  QLineEdit* author = new QLineEdit(this);
  m_main_layout->addWidget(author, row, 1, 1, 2);
  QPushButton* author_btn = new QPushButton(QStringLiteral("Edit"), this);
  m_buttons.append(author_btn);
  connect(author_btn, &QPushButton::clicked, this, &MetadataEditor::editAuthor);
  m_main_layout->addWidget(author_btn, row++, 3);
  m_next_author_row = row;

  m_spacer = new QFrame(this);
  m_spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_main_layout->addWidget(m_spacer, m_next_author_row, 0, 1, 4);

  m_authors.append(author);
}

void MetadataEditor::editTitle()
{
  bool ok;
  QString title =
    QInputDialog::getText(this, QStringLiteral("Edit Book Title"),
                          QStringLiteral("Edit title and press OK to accept, "
                                         "Cancel to cancel edit"),
                          QLineEdit::Normal, m_document->title(), &ok);
  if (ok && !title.isEmpty()) {
    m_document->setTitle(title);
    setTitle(title);
  }
}

void MetadataEditor::editAuthor()
{
  QPushButton* btn = qobject_cast<QPushButton*>(sender());
  if (btn) {
    int index = m_buttons.indexOf(btn);
    bool ok;
    QLineEdit* e = m_authors.at(index);
    AuthorData data = m_author_list.at(index);
    AuthorDialog dlg(m_author_db, this);
    if (dlg.execute(data) == QDialog::Accepted) {
    }

    //    QString author = QInputDialog::getText(
    //                       this, QStringLiteral("Edit Book Author"),
    //                       QStringLiteral("Edit author and press OK to accept,
    //                       "
    //                                      "Cancel to cancel edit"),
    //                       QLineEdit::Normal, e->text(), &ok);
    //    if (ok && !author.isEmpty()) {
    //      AuthorData new_data = m_author_db->author(author);
    //      if (new_data.isNull()) {
    //        new_data = AuthorData(new EBookAuthorData());
    //        e->setText(author);
    //        // TODO need to create a new author.
    //      } else {
    //        if (data == new_data) {
    //          // author hasn't changed so ignore.
    //        } else {
    //          // an existing author so replace
    //          data = new_data;
    //          m_author_list.replace(index, new_data);
    //        }
    //      }
    //    }
  }
}

void MetadataEditor::editFilename()
{
}
