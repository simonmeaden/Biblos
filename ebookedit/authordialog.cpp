#include "authordialog.h"

#include "dbmanager.h"

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

AuthorDialog::AuthorDialog(DbManager* database, QWidget* parent)
  : QDialog(parent)
  , m_database(database)
  , m_modified(false)
  , m_names_index(0)
{
  up_key = m_pix_cache.insert(QPixmap(":/icons/up"));
  down_key = m_pix_cache.insert(QPixmap(":/icons/down"));
  plus_key = m_pix_cache.insert(QPixmap(":/icons/add"));
  minus_key = m_pix_cache.insert(QPixmap(":/icons/remove"));

  initGui();
}

void
AuthorDialog::setPartialNames(QStringList names)
{
  if (names.isEmpty()) {
    m_author_data = m_database->author();
    foreach (SharedAuthor author, m_author_data) {
      QListWidgetItem* item = new QListWidgetItem(author->name());
      item->setData(Qt::UserRole, QVariant::fromValue<SharedAuthor>(author));
      m_author_list->addItem(item);
    }
  } else {
    QString surname, forename, midnames;
    foreach (QString name, names) {
      m_author_data = m_database->author(name);
      surname = name;
      foreach (SharedAuthor author, m_author_data) {
        QStringList reduced = author->compareAndDiscard(names);
        reduced.removeOne(name); // remove surname as we already have that.
        foreach (QString n, reduced) {
          if (author->compareForename(n)) {
            forename = author->forename();
          } else if (author->compareMiddlenames(n)) {
            midnames = author->middlenames();
          }
        }
        QListWidgetItem* item = new QListWidgetItem(author->name());
        item->setData(Qt::UserRole, QVariant::fromValue<SharedAuthor>(author));
        m_author_list->addItem(item);
        if (reduced.size() == 1) {
          // colours the item appropriately
          if (item) {
            Author::Comparison comp =
              author->compare(forename, midnames, surname);
            if (comp == Author::ALL_MATCH) {
              item->setForeground(QColor("green"));
            } else if (comp == Author::FORE_AND_SURNAME_MATCH) {
              item->setForeground(QColor("cyan"));
            } else if (comp == Author::PARTIAL_MATCH) {
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
  QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
  SharedAuthor author;
  int index = m_surnames.indexOf(edit);
  if (!m_current_authors.at(index)) {
    author = SharedAuthor(new Author());
    m_current_authors.insert(index, author);
  } else {
    author = m_current_authors.at(index);
  }

  author->setSurname(name);
}

void
AuthorDialog::changeForename(const QString& name)
{
  QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
  SharedAuthor author;
  int index = m_forenames.indexOf(edit);
  if (!m_current_authors.at(index)) {
    author = SharedAuthor(new Author());
    m_current_authors.insert(index, author);
  } else {
    author = m_current_authors.at(index);
  }

  author->setForename(name);
}

void
AuthorDialog::changeMidnames(const QString& name)
{
  QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
  SharedAuthor author;
  int index = m_midnames.indexOf(edit);
  if (!m_current_authors.at(index)) {
    author = SharedAuthor(new Author());
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
  SharedAuthorList author_data;
  foreach (SharedAuthor author, authors()) {
    // get compatible authors from database.
    QString name;
    // Initially check all three names.
    author_data = m_database->author(
      author->surname(), author->forename(), author->middlenames());
    if (author_data.isEmpty()) {
      // No compatible author. Drop mid names from possibilities.
      author_data = m_database->author(author->surname(), author->forename());
      if (author_data.isEmpty()) {
        // OK offer to save author data.
        bool ok;
        QString name = QInputDialog::getText(
          this,
          tr("Author not in database!"),
          tr("<html>"
             "The author you have selected is not one we recognise.<br>"
             "Do you wish to save it for later reuse?"
             "</html>"),
          QLineEdit::Normal,
          author->name(),
          &ok);
        if (!name.isEmpty() && ok) {
          // TODO save to database.
          m_database->saveAuthor(author);
          author_data.append(author);
        }
      } else {
      }
    }
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
  m_midnames.at(m_names_index)->clear();
  m_surnames.at(m_names_index)->clear();
}

void
AuthorDialog::authorDoubleClicked(QListWidgetItem* item)
{
  m_current_authors.insert(m_names_index,
                           item->data(Qt::UserRole).value<SharedAuthor>());
  if (m_current_authors.size() > m_names_index) {
    m_forenames.at(m_names_index)
      ->setText(m_current_authors.at(m_names_index)->forename());
    m_midnames.at(m_names_index)
      ->setText(m_current_authors.at(m_names_index)->middlenames());
    m_surnames.at(m_names_index)
      ->setText(m_current_authors.at(m_names_index)->surname());
    if (m_surnames.size() > m_names_index + 1)
      m_names_index++;
  }
}

int
AuthorDialog::exec(AuthorDialog::Type type, QString title, QStringList names)
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

SharedAuthorList
AuthorDialog::authors()
{
  SharedAuthorList authors;
  // only return authos that have values (actually only surnames are tested.)
  foreach (SharedAuthor author, m_current_authors) {
    if (author && !author->isEmpty()) {
      authors.append(author);
    }
  }
  return authors;
}

QFrame*
AuthorDialog::createAuthorFrame()
{
  QFrame* frame = new QFrame(this);
  QGridLayout* layout = new QGridLayout;
  frame->setLayout(layout);

  QLabel* lbl = new QLabel(tr("Forename :"), this);
  layout->addWidget(lbl, 0, 0);
  QLineEdit* m_forename_edit = new QLineEdit(this);
  m_forenames.append(m_forename_edit);
  connect(m_forename_edit,
          &QLineEdit::textChanged,
          this,
          &AuthorDialog::changeForename);
  layout->addWidget(m_forename_edit, 0, 1);

  lbl = new QLabel(tr("Middle name(s) :"), this);
  layout->addWidget(lbl, 0, 2);
  QLineEdit* m_midname_edit = new QLineEdit(this);
  m_midnames.append(m_midname_edit);
  connect(m_midname_edit,
          &QLineEdit::textChanged,
          this,
          &AuthorDialog::changeMidnames);
  layout->addWidget(m_midname_edit, 0, 3);

  lbl = new QLabel(tr("Surname :"), this);
  layout->addWidget(lbl, 0, 4);
  QLineEdit* m_surname_edit = new QLineEdit(this);
  m_surnames.append(m_surname_edit);
  connect(m_surname_edit,
          &QLineEdit::textChanged,
          this,
          &AuthorDialog::changeSurname);
  layout->addWidget(m_surname_edit, 0, 5);

  // create placeholder for author info.
  m_current_authors.append(SharedAuthor(Q_NULLPTR));

  QPixmap minuspix;
  m_pix_cache.find(minus_key, &minuspix);
  QPushButton* btn = new QPushButton(minuspix, "", this);
  m_buttons.append(btn);
  layout->addWidget(btn, 0, 6);
  connect(btn, &QPushButton::clicked, this, &AuthorDialog::removeAuthorFrame);

  //  resetBtnConditions(plusbtn);
  m_frames.append(frame);

  return frame;
}

// void
// AuthorDialog::resetBtnConditions(QPushButton* plusbtn)
//{
//  QPixmap pluspix, minuspix;
//  m_pix_cache.find(plus_key, &pluspix);
//  m_pix_cache.find(minus_key, &minuspix);
//  foreach (QPushButton* btn, m_buttons) {
//    if (btn != plusbtn) {
//      btn->setIcon(minuspix);
//      //      plusbtn->setProperty("isplus", false);
//      disconnect(
//        plusbtn, &QPushButton::clicked, this,
//        &AuthorDialog::addAnotherAuthor);
//      connect(
//        btn, &QPushButton::clicked, this, &AuthorDialog::removeAuthorFrame);
//    } else {
//      plusbtn->setIcon(pluspix);
//      //  plusbtn->setProperty("isplus", true);
//      plusbtn->disconnect();
//      disconnect(
//        plusbtn, &QPushButton::clicked, this,
//        &AuthorDialog::removeAuthorFrame);
//      connect(
//        plusbtn, &QPushButton::clicked, this,
//        &AuthorDialog::addAnotherAuthor);
//    }
//  }
//}

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

    w = m_midnames.at(index);
    layout->removeWidget(w);
    m_midnames.removeAt(index);
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
    authorLayout->removeWidget(w);
    m_frames.removeAt(index);
    w->deleteLater();

    m_current_authors.removeAt(index);
    //    resetBtnConditions(m_buttons.last());
  }
}

void
AuthorDialog::addAnotherAuthor()
{
  bool last_empty = false;
  if (!m_surnames.last()->text().isEmpty()) {
    last_empty = true;
  }
  QFrame* frm = createAuthorFrame();
  authorLayout->addWidget(frm);
  if (last_empty) {
    m_names_index = m_surnames.size() - 1;
  }
}

void
AuthorDialog::upPressed()
{}

void
AuthorDialog::dnPressed()
{}

void
AuthorDialog::initGui()
{
  setWindowTitle(tr("Choose Author"));
  QGridLayout* l = new QGridLayout;
  setLayout(l);

  QFrame* authorFrame = new QFrame(this);
  QGridLayout* authorBoxlayout = new QGridLayout;
  authorFrame->setLayout(authorBoxlayout);

  QFrame* authorSubFrame = new QFrame(this);
  authorLayout = new QVBoxLayout;
  authorSubFrame->setLayout(authorLayout);
  authorBoxlayout->addWidget(authorSubFrame, 0, 0);

  QFrame* frm = createAuthorFrame();
  authorLayout->addWidget(frm);

  QFrame* btnFrame = new QFrame(this);
  QVBoxLayout* btnLayout = new QVBoxLayout;
  btnFrame->setLayout(btnLayout);

  QPixmap upicon, downicon, pluspix;
  m_pix_cache.find(up_key, &upicon);
  m_pix_cache.find(down_key, &downicon);
  m_pix_cache.find(plus_key, &pluspix);

  QPushButton* upBtn = new QPushButton(upicon, "", this);
  btnLayout->addWidget(upBtn);
  connect(upBtn, &QPushButton::clicked, this, &AuthorDialog::upPressed);

  QPushButton* plusbtn = new QPushButton(pluspix, "", this);
  btnLayout->addWidget(plusbtn);
  connect(
    plusbtn, &QPushButton::clicked, this, &AuthorDialog::addAnotherAuthor);

  QPushButton* dnBtn = new QPushButton(downicon, "", this);
  btnLayout->addWidget(dnBtn);
  connect(dnBtn, &QPushButton::clicked, this, &AuthorDialog::dnPressed);

  authorBoxlayout->addWidget(btnFrame, 0, 1);
  l->addWidget(authorFrame);

  frm = new QFrame(this);
  QGridLayout* frmLayout = new QGridLayout;
  frm->setLayout(frmLayout);

  m_text_lbl = new QLabel(this);
  frmLayout->addWidget(m_text_lbl, 1, 0, 3, 1);
  m_author_list = new QListWidget(this);
  connect(m_author_list,
          &QListWidget::itemDoubleClicked,
          this,
          &AuthorDialog::authorDoubleClicked);
  frmLayout->addWidget(m_author_list, 1, 3, 3, 1);
  l->addWidget(frm, 1, 0);

  QDialogButtonBox* btnBox = new QDialogButtonBox(Qt::Horizontal, this);
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

  m_help_btn = btnBox->addButton(tr("Help"), QDialogButtonBox::HelpRole);
  m_help_btn->setToolTip(tr("Brings up a Help dialog."));
  connect(m_help_btn, &QPushButton::clicked, this, &AuthorDialog::help);

  l->addWidget(btnBox, 2, 0, 6, 1);
}
