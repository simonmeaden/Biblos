#include "metadataeditor.h"
#include "authordialog.h"
#include "iebookdocument.h"

MetadataEditor::MetadataEditor(Options* options,
                               AuthorsDB* authors,
                               LibraryDB* library,
                               QWidget* parent)
  : QWidget(parent)
  , m_options(options)
  , m_author_db(authors)
  , m_library_db(library)
{
  initGui();
}

void
MetadataEditor::setDocument(ITextDocument* doc)
{
  m_document = doc;
  setTitle(doc->title());
  setFileInfo(doc->filename());

  Metadata metadata = m_document->metadata();
  // make a clone of the original calibre data and modify the clone
  m_calibre = Calibre(new EBookCalibre(*metadata->calibre().data()));
  if (!m_calibre->seriesName().isEmpty()) {
    m_series_edit->setText(m_calibre->seriesName());
    m_library_db->insertOrGetSeries(m_calibre->seriesName());
    if (!m_calibre->seriesIndex().isEmpty()) {
      m_series_spin->setValue(
        m_series_spin->valueFromText(m_calibre->seriesIndex()));
    } else {
      m_series_spin->setValue(m_series_spin->valueFromText("1.00"));
      m_calibre->setSeriesIndex("1.00");
    }
  } else {
    m_series_edit->setText(QString());
    m_series_spin->setValue(1.0);
    m_series_spin->setEnabled(false);
  }

  QCompleter* completer = new QCompleter(m_library_db->seriesList(), this);
  completer->setCaseSensitivity(Qt::CaseInsensitive);
  m_series_edit->setCompleter(completer);

  QStringList creators = m_document->creators();
  AuthorList author_list;
  foreach (QString creator, creators) {
    AuthorData author_data = m_author_db->author(creator);
    if (!author_data) {
      QStringList names = creator.split(" ");

      if (!author_data) {
        author_data = AuthorData(new EBookAuthorData());
        author_data->displayName() = creator;
        author_list.append(author_data);
        m_author_db->insertAuthor(author_data);
      }
    } else {
      author_list.append(author_data);
    }
  }
  addAuthors(author_list);

  BookData book_data = m_library_db->bookByFile(doc->filename());
  if (book_data.isNull()) {
    m_book_data = BookData(new EBookData());
    quint64 uid = m_book_data->nextUid();
    m_book_data->uid = uid;
    m_book_data->filename = doc->filename();
    m_book_data->title = doc->title();
    if (!m_calibre->seriesName().isEmpty()) {
      m_book_data->series =
        m_library_db->insertOrGetSeries(m_calibre->seriesName());
      m_book_data->series_index = m_calibre->seriesIndex();
    }
    m_book_data->current_spine_index = 0;
    m_book_data->current_spine_lineno = 0;
    m_book_data->modified = true;
  } else {
    // Make a clone of the book data and modify the clone.
    m_book_data = BookData(new EBookData(*book_data.data()));
  }
}

void
MetadataEditor::setTitle(QString title)
{
  m_title_edit->setText(title);
}

void
MetadataEditor::addAuthors(AuthorList author_list)
{
  AuthorData author;
  QLineEdit* author_edit;
  QCheckBox* author_cbox;
  m_author_list.clear();
  for (int i = 0; i < author_list.size(); i++) {
    author = author_list.at(i);
    if (i == 0 && m_author_edits.size() == 1 &&
        m_author_edits.at(0)->text().isEmpty()) {
      // this one is the empty one created at startup
      author_edit = m_author_edits.at(0);
      author_edit->setText(author->displayName());
      author_cbox = m_author_cboxs.at(0);
      author_cbox->setChecked(author->surnameLast());
    } else if (i >= m_author_edits.size()) {
      author_edit = new QLineEdit(this);
      m_author_edits.append(author_edit);
      m_main_layout->removeWidget(m_spacer);
      m_main_layout->addWidget(author_edit, m_next_author_row, 1);
      author_cbox = new QCheckBox(QStringLiteral("Surname last"), this);
      m_author_cboxs.append(author_cbox);
      m_main_layout->addWidget(author_cbox, m_next_author_row, 2);
      author_cbox->setChecked(author->surnameLast());
      connect(author_cbox,
              &QCheckBox::stateChanged,
              this,
              &MetadataEditor::editSurnameLast);
      QPushButton* edit_author_btn =
        new QPushButton(QStringLiteral("Edit"), this);
      m_author_btns.append(edit_author_btn);
      m_main_layout->addWidget(edit_author_btn, m_next_author_row++, 3);
      m_main_layout->addWidget(m_spacer, m_next_author_row, 0, 1, 4);
      connect(edit_author_btn,
              &QPushButton::clicked,
              this,
              &MetadataEditor::editAuthor);
      author_edit->setText(author->displayName());
    } else
      author_edit = m_author_edits.at(i);
  }
  m_author_list = author_list;
}

void
MetadataEditor::setFileInfo(QString filepath)
{
  m_file_info.setFile(filepath);
  m_filename = m_file_info.baseName();
  m_original_filename = m_filename;
  //  QString extension = file_info.suffix();
  // use this as file may have a complex suffix
  // ie '.original.epub'
  QString full_extension = m_file_info.completeSuffix();
  full_extension.prepend(".");

  m_filename_edit->setText(m_filename);
  m_fileext_lbl->setText(full_extension);
}

void
MetadataEditor::initGui()
{
  m_main_layout = new QGridLayout;
  setLayout(m_main_layout);

  int row = 0;
  QLabel* lbl = new QLabel(tr("Filename : "), this);
  m_main_layout->addWidget(lbl, row, 0);
  m_filename_edit = new QLineEdit(this);
  m_main_layout->addWidget(m_filename_edit, row, 1);
  m_fileext_lbl = new QLabel(this);
  m_main_layout->addWidget(m_fileext_lbl, row, 2);
  QPushButton* filename_edit_btn =
    new QPushButton(QStringLiteral("Edit"), this);
  m_main_layout->addWidget(filename_edit_btn, row, 3);
  connect(filename_edit_btn,
          &QPushButton::clicked,
          this,
          &MetadataEditor::filenameChanged);

  row++;

  lbl = new QLabel(tr("Title : "), this);
  m_main_layout->addWidget(lbl, row, 0);
  m_title_edit = new QLineEdit(this);
  m_title_edit->setReadOnly(true);
  m_main_layout->addWidget(m_title_edit, row, 1, 1, 2);
  QPushButton* edit_title_btn = new QPushButton(QStringLiteral("Edit"), this);
  m_main_layout->addWidget(edit_title_btn, row, 3);
  connect(
    edit_title_btn, &QPushButton::clicked, this, &MetadataEditor::editTitle);

  row++;

  lbl = new QLabel(tr("Series : "), this);
  m_main_layout->addWidget(lbl, row, 0);
  m_series_edit = new QLineEdit(this);
  m_series_edit->setReadOnly(true);
  connect(m_series_edit,
          &QLineEdit::textChanged,
          this,
          &MetadataEditor::seriesChanged);
  m_main_layout->addWidget(m_series_edit, row, 1, 1, 2);
  m_series_spin = new QDoubleSpinBox(this);
  m_series_spin->setDecimals(2);
  m_main_layout->addWidget(m_series_spin, row, 3);
  connect(m_series_spin,
          QOverload<const QString&>::of(&QDoubleSpinBox::valueChanged),
          this,
          &MetadataEditor::seriesIndexChanged);

  row++;

  lbl = new QLabel(tr("Authors(s) :"), this);
  m_main_layout->addWidget(lbl, row, 0);

  QLineEdit* author = new QLineEdit(this);
  m_main_layout->addWidget(author, row, 1);
  QCheckBox* author_cbox = new QCheckBox(QStringLiteral("Surname last"), this);
  m_author_cboxs.append(author_cbox);
  m_main_layout->addWidget(author_cbox, row, 2);
  connect(author_cbox,
          &QCheckBox::stateChanged,
          this,
          &MetadataEditor::editSurnameLast);
  QPushButton* author_btn = new QPushButton(QStringLiteral("Edit"), this);
  m_author_btns.append(author_btn);
  connect(author_btn, &QPushButton::clicked, this, &MetadataEditor::editAuthor);
  m_main_layout->addWidget(author_btn, row, 3);
  m_next_author_row = row;

  row++;

  m_spacer = new QFrame(this);
  m_spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_main_layout->addWidget(m_spacer, row, 0, 1, 4);

  row++;

  QFrame* btn_frame = new QFrame(this);
  btn_frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  QHBoxLayout* btn_layout = new QHBoxLayout;
  btn_frame->setLayout(btn_layout);

  QPushButton* cancel_btn = new QPushButton(tr("Cancel Changes"), this);
  btn_layout->addWidget(cancel_btn);
  connect(
    cancel_btn, &QPushButton::clicked, this, &MetadataEditor::rejectChanges);

  QPushButton* accept_btn = new QPushButton(tr("Accept Changes"), this);
  btn_layout->addWidget(accept_btn);
  connect(
    accept_btn, &QPushButton::clicked, this, &MetadataEditor::acceptChanges);
  m_main_layout->addWidget(btn_frame, row, 0, 1, 4);

  row++;

  m_author_edits.append(author);
}

void
MetadataEditor::editTitle()
{
  bool ok;
  QString title =
    QInputDialog::getText(this,
                          QStringLiteral("Edit Book Title"),
                          QStringLiteral("Edit title and press OK to accept, "
                                         "Cancel to cancel edit"),
                          QLineEdit::Normal,
                          m_document->title(),
                          &ok);
  if (ok && !title.isEmpty()) {
    m_document->setTitle(title);
    setTitle(title);
  }
}

void
MetadataEditor::editAuthor()
{
  QPushButton* btn = qobject_cast<QPushButton*>(sender());
  if (btn) {
    int index = m_author_btns.indexOf(btn);
    AuthorData data = m_author_list.at(index);
    AuthorDialog dlg(m_author_db, this);
    if (dlg.execute(data) == QDialog::Accepted) {
      AuthorList author_list = dlg.authors();
      foreach (AuthorData data, author_list) {
        m_author_db->addAuthor(data);
      }
    }
  }
}

void
MetadataEditor::filenameChanged()
{
  QString text = m_filename_edit->text();
  m_filename = text;
  m_document->setModified(true);
}

void
MetadataEditor::editSurnameLast(int checked)
{
  QCheckBox* box = qobject_cast<QCheckBox*>(sender());
  if (box) {
    int index = m_author_cboxs.indexOf(box);
    if (m_author_list.size() > index) {
      AuthorData data = m_author_list.at(index);
      if (checked == Qt::Checked) {
        data->setSurnameLast(true);
      } else {
        data->setSurnameLast(false);
      }
    }
  }
}

void
MetadataEditor::acceptChanges()
{
  if (m_filename != m_original_filename) {
    QString path = m_file_info.path();
    path += m_filename;
    path += m_fileext_lbl->text();
  }
  if (m_calibre->isModified()) {
    Calibre calibre = m_document->metadata()->calibre();
    m_calibre->setSeriesName(calibre->seriesName());
    m_calibre->setSeriesIndex(calibre->seriesIndex());
    m_calibre->setTitleSort(calibre->titleSort());
    m_calibre->setAuthorLinkMap(calibre->authorLinkMap());
    m_calibre->setTimestamp(calibre->timestamp());
    m_calibre->setRating(calibre->rating());
    m_calibre->setPublicationType(calibre->publicationType());
    m_calibre->setUserMetadata(calibre->userMetadata());
    m_calibre->setUserCategories(calibre->userMetadata());
    m_calibre->setCustomMetadata(calibre->customMetadata());
  }
  if (m_book_data->modified) {
    m_library_db->insertOrUpdateBook(m_book_data);
  }
}

void
MetadataEditor::rejectChanges()
{
  m_filename = m_original_filename;
  m_filename_edit->setText(m_filename);
}

void
MetadataEditor::seriesChanged(const QString& text)
{
  m_calibre->setSeriesName(text);
  m_book_data->series =
    m_library_db->insertOrGetSeries(m_calibre->seriesName());
  m_book_data->series_index =
    m_series_spin->textFromValue(m_series_spin->value());
  if (!text.isEmpty() && !m_series_spin->isEnabled())
    m_series_spin->setEnabled(true);
  m_calibre->setModified(true);
  m_book_data->modified = true;
}

void
MetadataEditor::seriesIndexChanged(const QString& value)
{
  m_calibre->setSeriesIndex(value);
  m_book_data->series_index = value;
  m_calibre->setModified(true);
  m_book_data->modified = true;
}
