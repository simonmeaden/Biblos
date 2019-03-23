#include "authordialog.h"

// const QString AuthorDialog::SELECTED_BORDER = QString("QFrame {"
//    "border: 2px solid green;"
//    "border-radius: 4px;"
//    "padding: 2px;"
//    "}");
// const QString AuthorDialog::UNSELECTED_BORDER = QString("QFrame {"
//    "border: 0px;"
//    "padding: 4px;"
//    "}");

const QString AuthorDialog::FULL_COLOR =
  "QListWidget::item { color: green; background-color: transparent; }";
const QString AuthorDialog::PARTIAL_COLOR =
  "QListWidget::item { color: blue; background-color: transparent; }";
const QString AuthorDialog::SURNAMEMATCH_COLOR =
  "QListWidget::item { color: cyan; background-color: transparent; }";

const QString AuthorDialog::NO_AUTHOR_DETAILS_FOUND =
  tr("No Author details found!");
const QString AuthorDialog::FROM_TITLE_TITLE = tr("Missing Author info %1 %2");
const QString AuthorDialog::FROM_TITLE =
  tr("<html>"
     "The Metadata does not have any Author information.<br>"
     "We have attempted to extract an Author name from<br>"
     "the file name and come up with the following, <br>"
     "<b>%1</b>.<br>"
     "Either select one or more authors from the list,<br>"
     "enter your own values or press <em>Cancel</em> to<br>"
     "leave the dialog without making changes."
     "</html>");
const QString AuthorDialog::NO_NAMES =
  tr("<html>"
     "The Metadata does not have any Author information.<br>"
     "We have attempted to extract an Author name from<br>"
     "the file name but have failed. Please enter author<br>"
     "information or select from author list."
     "</html");

AuthorDialog::AuthorDialog(Options options, AuthorsDB authors, QWidget* parent)
  : QDialog(parent)
  , m_authors(authors)
  , m_max_author_row(0)
  , m_btn_frame(new QFrame(this))
  , m_modified(false)
  , m_names_index(0)

{
  up_key = options->up_key;
  down_key = options->down_key;
  plus_key = options->plus_key;
  minus_key = options->minus_key;

  initGui();
}

AuthorDialog::~AuthorDialog() {}

void
AuthorDialog::setPartialNames(QStringList names)
{
  if (names.isEmpty()) {
    m_authors_list = m_authors->authors();
    for (int i = 0; i < m_authors_list.size(); i++) {
      AuthorData author = m_authors_list.at(i);
      QListWidgetItem* item = new QListWidgetItem(author->displayName());
      item->setData(Qt::UserRole, QVariant::fromValue<AuthorData>(author));
      m_author_list->addItem(item);
    }
  } else {
    QString surname, forename, middlenames;
    for (int i = 0; i < names.size(); i++) {
      QString name = names.at(i);
      m_authors_list = m_authors->authorsBySurname(surname);
      surname = name;
      for (int i = 0; i < m_authors_list.size(); i++) {
        AuthorData author = m_authors_list.at(i);
        QStringList reduced = m_authors->compareAndDiscard(names);
        reduced.removeOne(name); // remove surname as we already have that.
        for (int i = 0; i < reduced.size(); i++) {
          QString n = reduced.at(i);
          if (author->surname().toLower() == n) {
            forename = author->forename();
            middlenames = author->middlenames();
          }
        }
        QListWidgetItem* item = new QListWidgetItem(author->displayName());
        item->setData(Qt::UserRole, QVariant::fromValue<AuthorData>(author));
        m_author_list->addItem(item);
        if (reduced.size() == 1) {
          // colours the item appropriately
          if (item) {
            EBookAuthorData::Comparison comp =
              author->compare(forename, middlenames, surname);
            if (comp == EBookAuthorData::ALL_MATCH) {
              item->setForeground(QColor("green"));
            } else if (comp == EBookAuthorData::FORE_AND_SURNAME_MATCH) {
              item->setForeground(QColor("cyan"));
            } else if (comp == EBookAuthorData::PARTIAL_MATCH) {
              item->setForeground(QColor("blue"));
            }
          }
        }
      }
    }
  }
}

void
AuthorDialog::changeSurname(const QString& name)
{
  FocusLineEdit* edit = qobject_cast<FocusLineEdit*>(sender());
  int index = m_surnames.indexOf(edit);
  if (m_current_authors.isEmpty())
    return;
  AuthorData author = m_current_authors.at(index);
  if (author.isNull()) {
    author = AuthorData(new EBookAuthorData());
    m_current_authors.insert(index, author);
  } else {
    author = m_current_authors.at(index);
  }

  author->setSurname(name);
}

void
AuthorDialog::changeForename(const QString& name)
{
  FocusLineEdit* edit = qobject_cast<FocusLineEdit*>(sender());
  int index = m_forenames.indexOf(edit);
  if (m_current_authors.isEmpty())
    return;
  AuthorData author = m_current_authors.at(index);
  if (author.isNull()) {
    author = AuthorData(new EBookAuthorData());
    m_current_authors.insert(index, author);
  } else {
    author = m_current_authors.at(index);
  }

  author->setForename(name);
}

void
AuthorDialog::changeMidnames(const QString& name)
{
  FocusLineEdit* edit = qobject_cast<FocusLineEdit*>(sender());
  int index = m_middlenames.indexOf(edit);
  if (m_current_authors.isEmpty())
    return;
  AuthorData author = m_current_authors.at(index);
  if (author.isNull()) {
    author = AuthorData(new EBookAuthorData());
    m_current_authors.insert(index, author);
  } else {
    author = m_current_authors.at(index);
  }

  author->setMiddlenames(name);
}

void
AuthorDialog::help()
{}

void
AuthorDialog::acceptAuthor()
{
  AuthorData author_data = m_current_authors.at(m_current_index);
  if (m_image_lbl->modified()) {
    const QPixmap* pixmap = m_image_lbl->pixmap();
    if (!pixmap->isNull()) {
      author_data->setPixmap(*pixmap);
    }
  }
  QString site = m_web_edit->text();
  if (site != m_original_website) {
    author_data->setWebsite(site);
  }
  site = m_wiki_edit->text();
  if (site != m_original_wikipedia) {
    author_data->setWikipedia(site);
  }
  accept();
}

void
AuthorDialog::cancelChanges()
{
  reject();
}

void
AuthorDialog::resetChanges()
{
  m_current_authors.clear();
  m_forenames.at(m_names_index)->clear();
  m_middlenames.at(m_names_index)->clear();
  m_surnames.at(m_names_index)->clear();
}

void
AuthorDialog::authorDoubleClicked(QListWidgetItem* item)
{
  m_current_authors.insert(m_names_index,
                           item->data(Qt::UserRole).value<AuthorData>());
  if (m_current_authors.size() > m_names_index) {
    m_forenames.at(m_names_index)
      ->setText(m_current_authors.at(m_names_index)->forename());
    m_middlenames.at(m_names_index)
      ->setText(m_current_authors.at(m_names_index)->middlenames());
    m_surnames.at(m_names_index)
      ->setText(m_current_authors.at(m_names_index)->surname());
    if (m_surnames.size() > m_names_index + 1)
      m_names_index++;
  }
}

void
AuthorDialog::authorRowClicked(bool focussed)
{
  FocusLineEdit* edit = qobject_cast<FocusLineEdit*>(sender());
  int index = -1;
  if (edit) {
    if (m_surnames.contains(edit)) {
      index = m_surnames.indexOf(edit);
    } else if (m_forenames.contains(edit)) {
      index = m_forenames.indexOf(edit);
    } else if (m_middlenames.contains(edit)) {
      index = m_middlenames.indexOf(edit);
    } else {
      return;
    }
    if (index >= 0) {
      m_current_index = index;
      if (focussed) {
        m_surnames.at(index)->setFocussed(true);
        m_forenames.at(index)->setFocussed(true);
        m_middlenames.at(index)->setFocussed(true);
      } else {
        m_surnames.at(index)->setFocussed(false);
        m_forenames.at(index)->setFocussed(false);
        m_middlenames.at(index)->setFocussed(false);
      }
    }
  }
}

void
AuthorDialog::websiteChanged(const QString& text)
{}

void
AuthorDialog::wikipediaChanged(const QString& text)
{}

int
AuthorDialog::execute(AuthorDialog::Type type, QString title, QStringList names)
{
  switch (type) {
    case FromTitle:
      setWindowTitle(FROM_TITLE_TITLE.arg((title.length() > 0 ? ":" : ""))
                       .arg((title.length() > 0 ? title : "")));
      m_text_lbl->setText(FROM_TITLE.arg(names.join(" ")));
      setPartialNames(names);
      return QDialog::exec();

    case NoNames:
      setWindowTitle(FROM_TITLE_TITLE.arg((title.length() > 0 ? ":" : ""))
                       .arg((title.length() > 0 ? title : "")));
      m_text_lbl->setText(NO_NAMES);
      setPartialNames();
      return QDialog::exec();

    default:
      break;
  }
  return -1;
}

int
AuthorDialog::execute(AuthorData author_data)
{
  setWindowTitle(QString("Modify %1").arg(author_data->displayName()));
  QStringList names;
  names << author_data->surname() << author_data->forename();
  setPartialNames(names);
  m_forenames.at(0)->setText(author_data->forename());
  m_middlenames.at(0)->setText(author_data->middlenames());
  m_surnames.at(0)->setText(author_data->surname());
  m_current_authors.clear(); // shouldn't be any here. mayn not be needed.
  m_current_authors.append(author_data);
  m_web_edit->setText(author_data->website());
  m_original_website = author_data->website();
  m_wiki_edit->setText(author_data->wikipedia());
  m_original_wikipedia = author_data->wikipedia();
  m_names_index = 0;
  m_current_index = 0;
  QPixmap pixmap = author_data->pixmap();
  if (!pixmap.isNull())
    m_image_lbl->setPixmap(pixmap);
  return QDialog::exec();
}

int
AuthorDialog::execute(AuthorList author_list)
{
  setWindowTitle(QString("Modify Authors"));
  QStringList names;
  for (int i = 0; i < author_list.size(); i++) {
    AuthorData author_data = author_list.at(i);
    names << author_data->surname() << author_data->forename();
  }
  setPartialNames(names);
  m_current_authors.clear(); // shouldn't be any here. mayn not be needed.
  for (int i = 0; i < author_list.size(); i++) {
    AuthorData author_data = author_list.at(i);
    if (i > 0)
      addAnotherAuthor();
    m_forenames.at(i)->setText(author_data->forename());
    m_middlenames.at(i)->setText(author_data->middlenames());
    m_surnames.at(i)->setText(author_data->surname());
    m_current_authors.append(author_data);
    m_web_edit->setText(author_data->website());
    m_wiki_edit->setText(author_data->wikipedia());
    m_names_index = 0;
  }
  return QDialog::exec();
}

AuthorData
AuthorDialog::author()
{
  return m_current_authors.at(m_current_index);
}

AuthorList
AuthorDialog::authors()
{
  AuthorList authors;
  // only return authors that have values (actually only surnames are tested.)
  for (int i = 0; i < m_current_authors.size(); i++) {
    AuthorData author = m_current_authors.at(i);
    if (author && !author->surname().isEmpty()) {
      authors.append(author);
    }
  }
  return authors;
}

void
AuthorDialog::addAuthorRow()
{
  QLabel* lbl = new QLabel(tr("Forename :"), this);
  m_main_layout->addWidget(lbl, m_max_author_row, 0);

  FocusLineEdit* m_forename_edit = new FocusLineEdit(this);
  m_forenames.append(m_forename_edit);
  connect(m_forename_edit,
          &FocusLineEdit::textChanged,
          this,
          &AuthorDialog::changeForename);
  connect(m_forename_edit,
          &FocusLineEdit::focussed,
          this,
          &AuthorDialog::authorRowClicked);
  m_main_layout->addWidget(m_forename_edit, m_max_author_row, 1);

  lbl = new QLabel(tr("Middle name(s) :"), this);
  m_main_layout->addWidget(lbl, m_max_author_row, 2);

  FocusLineEdit* m_midname_edit = new FocusLineEdit(this);
  m_middlenames.append(m_midname_edit);
  connect(m_midname_edit,
          &FocusLineEdit::textChanged,
          this,
          &AuthorDialog::changeMidnames);
  connect(m_midname_edit,
          &FocusLineEdit::focussed,
          this,
          &AuthorDialog::authorRowClicked);
  m_main_layout->addWidget(m_midname_edit, m_max_author_row, 3);

  lbl = new QLabel(tr("Surname :"), this);
  m_main_layout->addWidget(lbl, m_max_author_row, 4);

  FocusLineEdit* m_surname_edit = new FocusLineEdit(this);
  m_surnames.append(m_surname_edit);
  connect(m_surname_edit,
          &FocusLineEdit::textChanged,
          this,
          &AuthorDialog::changeSurname);
  connect(m_surname_edit,
          &FocusLineEdit::focussed,
          this,
          &AuthorDialog::authorRowClicked);
  m_main_layout->addWidget(m_surname_edit, m_max_author_row, 5);

  m_empty_rows.append(true);
  m_current_authors.append(AuthorData(new EBookAuthorData()));

  QPixmap pixmap;
  QPixmapCache::find(minus_key, &pixmap);
  QPushButton* btn = new QPushButton(pixmap, QString(), this);
  m_buttons.append(btn);
  m_main_layout->addWidget(btn, m_max_author_row, 6);
  connect(btn, &QPushButton::clicked, this, &AuthorDialog::removeAuthorFrame);

  m_main_layout->removeWidget(m_btn_frame);
  m_main_layout->addWidget(m_btn_frame, 0, 7);

  m_max_author_row++;
}

void
AuthorDialog::removeAuthorFrame()
{
  QPushButton* btn = qobject_cast<QPushButton*>(sender());
  if (btn) {
    int index = m_buttons.indexOf(btn);
    QWidget* w;
    QLayout* layout = m_frames.at(index)->layout();
    w = m_forenames.at(index);
    layout->removeWidget(w);
    m_forenames.removeAt(index);
    w->deleteLater();

    w = m_middlenames.at(index);
    layout->removeWidget(w);
    m_middlenames.removeAt(index);
    w->deleteLater();

    w = m_surnames.at(index);
    layout->removeWidget(w);
    m_surnames.removeAt(index);
    w->deleteLater();

    w = m_buttons.at(index);
    layout->removeWidget(w);
    m_buttons.removeAt(index);
    w->deleteLater();

    w = m_frames.at(index);
    //    author_sub_layout->removeWidget(w);
    m_frames.removeAt(index);
    w->deleteLater();

    m_current_authors.removeAt(index);
    //    resetBtnConditions(m_buttons.last());
  }
}

void
AuthorDialog::addAnotherAuthor()
{
  checkEmptyRows();
  // Only allow one empty row.
  for (int i = 0; i < m_forenames.size(); i++) {
    if (isEmptyRow(i))
      return;
  }
  addAuthorRow();
}

void
AuthorDialog::upPressed()
{}

void
AuthorDialog::dnPressed()
{}

void
AuthorDialog::initPositionalButtons()
{
  QVBoxLayout* btn_layout = new QVBoxLayout;
  m_btn_frame->setLayout(btn_layout);

  QPixmap upicon, downicon, pluspix;
  QPixmapCache::find(up_key, &upicon);
  QPixmapCache::find(down_key, &downicon);
  QPixmapCache::find(plus_key, &pluspix);

  QPushButton* upBtn = new QPushButton(upicon, "", this);
  btn_layout->addWidget(upBtn);
  connect(upBtn, &QPushButton::clicked, this, &AuthorDialog::upPressed);

  QPushButton* plusbtn = new QPushButton(pluspix, "", this);
  btn_layout->addWidget(plusbtn);
  connect(
    plusbtn, &QPushButton::clicked, this, &AuthorDialog::addAnotherAuthor);

  QPushButton* dnBtn = new QPushButton(downicon, "", this);
  btn_layout->addWidget(dnBtn);
  connect(dnBtn, &QPushButton::clicked, this, &AuthorDialog::dnPressed);
}

QDialogButtonBox*
AuthorDialog::initMainButtonBox()
{
  QDialogButtonBox* btnBox = new QDialogButtonBox(Qt::Horizontal, this);

  m_help_btn = btnBox->addButton(tr("Help"), QDialogButtonBox::HelpRole);
  m_help_btn->setToolTip(tr("Brings up a Help dialog."));
  connect(m_help_btn, &QPushButton::clicked, this, &AuthorDialog::help);

  m_accept_btn =
    btnBox->addButton(tr("Accept Author"), QDialogButtonBox::AcceptRole);
  m_accept_btn->setToolTip(tr("Click this to accept the authors name as is."));
  connect(
    m_accept_btn, &QPushButton::clicked, this, &AuthorDialog::acceptAuthor);

  m_cancel_btn =
    btnBox->addButton(tr("Cancel Changes"), QDialogButtonBox::ActionRole);
  m_cancel_btn->setToolTip(tr("Cancels all changes and closes dialog"));
  connect(
    m_cancel_btn, &QPushButton::clicked, this, &AuthorDialog::cancelChanges);

  m_reset_btn = btnBox->addButton(tr("Reset"), QDialogButtonBox::ActionRole);
  m_reset_btn->setToolTip(tr("Resets author to initial values."));
  connect(
    m_reset_btn, &QPushButton::clicked, this, &AuthorDialog::resetChanges);

  btnBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  return btnBox;
}

void
AuthorDialog::initGui()
{
  setWindowTitle(tr("Choose Author"));
  m_main_layout = new QGridLayout;
  setLayout(m_main_layout);

  initPositionalButtons();
  addAuthorRow();

  m_text_lbl = new QLabel(this);
  m_main_layout->addWidget(m_text_lbl, m_max_author_row, 0, 1, 4);

  QLabel* lbl = new QLabel(tr("Website : "), this);
  m_main_layout->addWidget(lbl, m_max_author_row + 1, 0);
  m_web_edit = new QLineEdit(this);
  connect(
    m_web_edit, &QLineEdit::textChanged, this, &AuthorDialog::websiteChanged);
  m_main_layout->addWidget(m_web_edit, m_max_author_row + 1, 1, 1, 5);

  lbl = new QLabel(tr("Wikipedia : "), this);
  m_main_layout->addWidget(lbl, m_max_author_row + 2, 0);
  m_wiki_edit = new QLineEdit(this);
  connect(m_wiki_edit,
          &QLineEdit::textChanged,
          this,
          &AuthorDialog::wikipediaChanged);
  m_main_layout->addWidget(m_wiki_edit, m_max_author_row + 2, 1, 1, 5);

  m_author_list = new QListWidget(this);
  connect(m_author_list,
          &QListWidget::itemDoubleClicked,
          this,
          &AuthorDialog::authorDoubleClicked);
  m_main_layout->addWidget(m_author_list, m_max_author_row + 3, 0, 1, 4);

  m_image_lbl = new AuthorImage(this);
  m_image_lbl->setAlignment(Qt::AlignCenter);
  m_image_lbl->setFrameStyle(QFrame::StyledPanel);
  m_main_layout->addWidget(m_image_lbl, m_max_author_row + 3, 4, 1, 2);

  QDialogButtonBox* btn_box = initMainButtonBox();
  m_main_layout->addWidget(btn_box, m_max_author_row + 4, 0, 7, 1);
}

AuthorImage::AuthorImage(QWidget* parent)
  : QLabel(parent)
  , m_modified(false)
{}

bool
AuthorImage::modified() const
{
  return m_modified;
}

void
AuthorImage::contextMenuEvent(QContextMenuEvent* event)
{
  QMenu menu(this);
  QAction* paste_image = new QAction(tr("Paste Image"), this);
  connect(
    paste_image, &QAction::triggered, this, &AuthorImage::pasteAuthorImage);
  menu.addAction(paste_image);
  QAction* load_image = new QAction(tr("Load Image from file"), this);
  connect(load_image, &QAction::triggered, this, &AuthorImage::loadAuthorImage);
  menu.addAction(load_image);
  menu.exec(event->globalPos());
}

void
AuthorImage::pasteAuthorImage()
{
  QClipboard* clipboard = QGuiApplication::clipboard();
  const QMimeData* mimedata = clipboard->mimeData();
  if (mimedata->hasImage()) {
    //    setPixmap(qvariant_cast<QPixmap>(mimedata->imageData()));
    QImage image = clipboard->image();
    QPixmap pixmap = QPixmap::fromImage(image);
    setPixmap(pixmap);
    m_modified = true;
  }
}

void
AuthorImage::loadAuthorImage()
{
  QString fileName = QFileDialog::getOpenFileName(
    this, tr("Open Image"), "~", tr("Image Files (*.png *.jpg *.bmp)"));
}
