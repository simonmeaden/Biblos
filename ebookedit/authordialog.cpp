#include "authordialog.h"

//const QString AuthorDialog::SELECTED_BORDER = QString("QFrame {"
//    "border: 2px solid green;"
//    "border-radius: 4px;"
//    "padding: 2px;"
//    "}");
//const QString AuthorDialog::UNSELECTED_BORDER = QString("QFrame {"
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

AuthorDialog::AuthorDialog(AuthorsDB* authors, QWidget* parent)
  : QDialog(parent)
  , m_authors(authors)
  , m_modified(false)
  , m_names_index(0)
{
  up_key = QPixmapCache::insert(QPixmap(":/icons/up"));
  down_key = QPixmapCache::insert(QPixmap(":/icons/down"));
  plus_key = QPixmapCache::insert(QPixmap(":/icons/add"));
  minus_key = QPixmapCache::insert(QPixmap(":/icons/remove"));

  initGui();
}

void AuthorDialog::setPartialNames(QStringList names)
{
  if (names.isEmpty()) {
    m_authors_list = m_authors->authors();
    foreach (AuthorData author, m_authors_list) {
      QListWidgetItem* item = new QListWidgetItem(author->display_name);
      item->setData(Qt::UserRole, QVariant::fromValue<AuthorData>(author));
      m_author_list->addItem(item);
    }
  } else {
    QString surname, forename, middlenames;
    foreach (QString name, names) {
      m_authors_list = m_authors->authorsBySurname(surname);
      surname = name;
      foreach (AuthorData author, m_authors_list) {
        QStringList reduced = m_authors->compareAndDiscard(names);
        reduced.removeOne(name); // remove surname as we already have that.
        foreach (QString n, reduced) {
          if (author->surname.toLower() == n) {
            forename = author->forename;
            middlenames = author->middlenames;
          }
        }
        QListWidgetItem* item = new QListWidgetItem(author->display_name);
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

void AuthorDialog::changeSurname(const QString& name)
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

  author->surname = name;
}

void AuthorDialog::changeForename(const QString& name)
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

  author->forename = name;
}

void AuthorDialog::changeMidnames(const QString& name)
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

  author->middlenames = name;
}

void AuthorDialog::help()
{
}

void AuthorDialog::acceptAuthor()
{
  AuthorList author_data;
  foreach (AuthorData author, authors()) {
    // get compatible authors from database.
    //    QString name;
    //    // Initially check all three names.
    //    author_data = m_authors->author(author->surname(), author->forename(),
    //                                    author->middlenames());
    //    if (author_data.isEmpty()) {
    //      // No compatible author. Drop mid names from possibilities.
    //      author_data = m_authors->author(author->surname(),
    //      author->forename()); if (author_data.isEmpty()) {
    //        // OK offer to save author data.
    //        bool ok;
    //        QString name = QInputDialog::getText(
    //          this, tr("Author not in database!"),
    //          tr("<html>"
    //             "The author you have selected is not one we recognise.<br>"
    //             "Do you wish to save it for later reuse?"
    //             "</html>"),
    //          QLineEdit::Normal, author->name(), &ok);
    //        if (!name.isEmpty() && ok) {
    //          // TODO save to database.
    //          m_authors->saveAuthor(author);
    //          author_data.append(author);
    //        }
    //      } else {
    //      }
    //    }
  }
  accept();
}

void AuthorDialog::cancelChanges()
{
  reject();
}

void AuthorDialog::resetChanges()
{
  m_current_authors.clear();
  m_forenames.at(m_names_index)->clear();
  m_middlenames.at(m_names_index)->clear();
  m_surnames.at(m_names_index)->clear();
}

void AuthorDialog::authorDoubleClicked(QListWidgetItem* item)
{
  m_current_authors.insert(m_names_index,
                           item->data(Qt::UserRole).value<AuthorData>());
  if (m_current_authors.size() > m_names_index) {
    m_forenames.at(m_names_index)
    ->setText(m_current_authors.at(m_names_index)->forename);
    m_middlenames.at(m_names_index)
    ->setText(m_current_authors.at(m_names_index)->middlenames);
    m_surnames.at(m_names_index)
    ->setText(m_current_authors.at(m_names_index)->surname);
    if (m_surnames.size() > m_names_index + 1)
      m_names_index++;
  }
}

void AuthorDialog::authorRowClicked(bool focussed)
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

int AuthorDialog::execute(AuthorDialog::Type type, QString title,
                          QStringList names)
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

int AuthorDialog::execute(AuthorData author_data)
{
  setWindowTitle(QString("Modify %1").arg(author_data->display_name));
  QStringList names;
  names << author_data->surname << author_data->forename;
  setPartialNames(names);
  m_forenames.at(0)->setText(author_data->forename);
  m_middlenames.at(0)->setText(author_data->middlenames);
  m_surnames.at(0)->setText(author_data->surname);
  m_current_authors.clear(); // shouldn't be any here. mayn not be needed.
  m_current_authors.append(author_data);
  m_names_index = 0;
  return QDialog::exec();
}

int AuthorDialog::execute(AuthorList author_list)
{
  setWindowTitle(QString("Modify Authors"));
  QStringList names;
  foreach (AuthorData author_data, author_list) {
    names << author_data->surname << author_data->forename;
  }
  setPartialNames(names);
  m_current_authors.clear(); // shouldn't be any here. mayn not be needed.
  for (int i = 0; i < author_list.size(); i++) {
    AuthorData author_data = author_list.at(i);
    if (i > 0)
      addAnotherAuthor();
    m_forenames.at(i)->setText(author_data->forename);
    m_middlenames.at(i)->setText(author_data->middlenames);
    m_surnames.at(i)->setText(author_data->surname);
    m_current_authors.append(author_data);
    m_names_index = 0;
  }
  return QDialog::exec();
}

AuthorList AuthorDialog::authors()
{
  AuthorList authors;
  // only return authors that have values (actually only surnames are tested.)
  foreach (AuthorData author, m_current_authors) {
    if (author && !author->surname.isEmpty()) {
      authors.append(author);
    }
  }
  return authors;
}

QFrame* AuthorDialog::createAuthorFrame()
{
  QFrame* frame = new QFrame(this);
  frame->setContentsMargins(0, 0, 0, 0);
  QGridLayout* layout = new QGridLayout;
  frame->setLayout(layout);

  QLabel* lbl = new QLabel(tr("Forename :"), this);
  layout->addWidget(lbl, 0, 0);
  FocusLineEdit* m_forename_edit = new FocusLineEdit(this);
  m_forenames.append(m_forename_edit);
  connect(m_forename_edit, &FocusLineEdit::textChanged, this,
          &AuthorDialog::changeForename);
  connect(m_forename_edit, &FocusLineEdit::focussed, this,
          &AuthorDialog::authorRowClicked);
  layout->addWidget(m_forename_edit, 0, 1);

  lbl = new QLabel(tr("Middle name(s) :"), this);
  layout->addWidget(lbl, 0, 2);
  FocusLineEdit* m_midname_edit = new FocusLineEdit(this);
  m_middlenames.append(m_midname_edit);
  connect(m_midname_edit, &FocusLineEdit::textChanged, this,
          &AuthorDialog::changeMidnames);
  connect(m_midname_edit, &FocusLineEdit::focussed, this,
          &AuthorDialog::authorRowClicked);
  layout->addWidget(m_midname_edit, 0, 3);

  lbl = new QLabel(tr("Surname :"), this);
  layout->addWidget(lbl, 0, 4);
  FocusLineEdit* m_surname_edit = new FocusLineEdit(this);
  m_surnames.append(m_surname_edit);
  connect(m_surname_edit, &FocusLineEdit::textChanged, this,
          &AuthorDialog::changeSurname);
  connect(m_surname_edit, &FocusLineEdit::focussed, this,
          &AuthorDialog::authorRowClicked);
  layout->addWidget(m_surname_edit, 0, 5);

  m_empty_rows.append(true);
  m_current_authors.append(AuthorData(new EBookAuthorData()));

  QPixmap pixmap;
  QPixmapCache::find(minus_key, &pixmap);
  QPushButton* btn = new QPushButton(pixmap, "", this);
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

void AuthorDialog::removeAuthorFrame()
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
    authorLayout->removeWidget(w);
    m_frames.removeAt(index);
    w->deleteLater();

    m_current_authors.removeAt(index);
    //    resetBtnConditions(m_buttons.last());
  }
}

void AuthorDialog::addAnotherAuthor()
{
  checkEmptyRows();
  // Only allow one empty row.
  for (int i = 0; i < m_forenames.size(); i++) {
    if (isEmptyRow(i))
      return;
  }
  authorLayout->addWidget(createAuthorFrame());
}

void AuthorDialog::upPressed()
{
}

void AuthorDialog::dnPressed()
{
}

void AuthorDialog::initGui()
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
  QPixmapCache::find(up_key, &upicon);
  QPixmapCache::find(down_key, &downicon);
  QPixmapCache::find(plus_key, &pluspix);

  QPushButton* upBtn = new QPushButton(upicon, "", this);
  btnLayout->addWidget(upBtn);
  connect(upBtn, &QPushButton::clicked, this, &AuthorDialog::upPressed);

  QPushButton* plusbtn = new QPushButton(pluspix, "", this);
  btnLayout->addWidget(plusbtn);
  connect(plusbtn, &QPushButton::clicked, this,
          &AuthorDialog::addAnotherAuthor);

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
  connect(m_author_list, &QListWidget::itemDoubleClicked, this,
          &AuthorDialog::authorDoubleClicked);
  frmLayout->addWidget(m_author_list, 1, 3, 3, 1);
  l->addWidget(frm, 1, 0);

  QDialogButtonBox* btnBox = new QDialogButtonBox(Qt::Horizontal, this);
  m_accept_btn =
    btnBox->addButton(tr("Accept Author"), QDialogButtonBox::AcceptRole);
  m_accept_btn->setToolTip(tr("Click this to accept the authors name as is."));
  connect(m_accept_btn, &QPushButton::clicked, this,
          &AuthorDialog::acceptAuthor);

  m_cancel_btn =
    btnBox->addButton(tr("Cancel Changes"), QDialogButtonBox::ActionRole);
  m_cancel_btn->setToolTip(tr("Cancels all changes and closes dialog"));
  connect(m_cancel_btn, &QPushButton::clicked, this,
          &AuthorDialog::cancelChanges);

  m_reset_btn = btnBox->addButton(tr("Reset"), QDialogButtonBox::ActionRole);
  m_reset_btn->setToolTip(tr("Resets author to initial values."));
  connect(m_reset_btn, &QPushButton::clicked, this,
          &AuthorDialog::resetChanges);

  m_help_btn = btnBox->addButton(tr("Help"), QDialogButtonBox::HelpRole);
  m_help_btn->setToolTip(tr("Brings up a Help dialog."));
  connect(m_help_btn, &QPushButton::clicked, this, &AuthorDialog::help);

  l->addWidget(btnBox, 2, 0, 6, 1);
}
