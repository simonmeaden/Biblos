#include "mainwindow.h"

#include <qlogger.h>

#include "ebookdocument.h"

using namespace qlogger;

HoverDialog::HoverDialog(QVariant data, QWidget *parent) : QDialog(parent) {
  m_data = data.value<EBookData>();
  initGui();
  m_titlelbl->setText(m_data.title);
  m_authorlbl->setText(m_data.author_names);
  m_publisherlbl->setText(m_data.publisher);
  QDateTime date = m_data.date;
  QString datestring = date.toString("d MMMM yyyy");
  m_datelbl->setText(datestring);
  m_subjectlbl->setText(m_data.subject);
  QLocale locale(m_data.language);
  m_langlbl->setText(locale.bcp47Name());
}

HoverDialog::~HoverDialog() {}

void HoverDialog::initGui()
{
  QGridLayout *l = new QGridLayout;
  setLayout(l);

  QLabel *lbl = new QLabel(tr("Title :"), this);
  l->addWidget(lbl, 0, 0);
  lbl = new QLabel(tr("Author(s) :"), this);
  l->addWidget(lbl, 1, 0);
  lbl = new QLabel(tr("Publisher :"), this);
  l->addWidget(lbl, 2, 0);
  lbl = new QLabel(tr("Date :"), this);
  l->addWidget(lbl, 3, 0);
  lbl = new QLabel(tr("Subject :"), this);
  l->addWidget(lbl, 4, 0);
  lbl = new QLabel(tr("Language :"), this);
  l->addWidget(lbl, 5, 0);

  m_titlelbl = new QLabel(this);
  l->addWidget(m_titlelbl, 0, 1);
  m_authorlbl = new QLabel(this);
  l->addWidget(m_authorlbl, 1, 1);
  m_publisherlbl = new QLabel(this);
  l->addWidget(m_publisherlbl, 2, 1);
  m_datelbl = new QLabel(this);
  l->addWidget(m_datelbl, 3, 1);
  m_subjectlbl = new QLabel(this);
  l->addWidget(m_subjectlbl, 4, 1);
  m_langlbl = new QLabel(this);
  l->addWidget(m_langlbl, 5, 1);
}

HoverTabWidget::HoverTabWidget(QWidget *parent) : QTabWidget(parent) {
  HoverTabBar *tabbar = new HoverTabBar(this);
  tabbar->setMouseTracking(true);
  setTabBar(tabbar);
//  connect(tabbar, &HoverTabBar::hoverTabSelected, this,
//          &HoverTabWidget::hoverSelected);
//  connect(tabbar, &HoverTabBar::hoverTabClose, this,
//          &HoverTabWidget::hoverCompleted);
}

HoverTabWidget::~HoverTabWidget() {}

int HoverTabWidget::addTab(EBookEditor *editor, const QString &text)
{
  int index = QTabWidget::addTab(editor, text);
  tabBar()->setTabData(index, editor->data());
  return index;
}

void HoverTabWidget::hoverSelected(int index) {
  if (index >= 0 && index < tabBar()->count()) {
  }
}

void HoverTabWidget::hoverCompleted() {}

HoverTabBar::HoverTabBar(QWidget *parent) : QTabBar(parent), m_dlg(Q_NULLPTR) {
  setAttribute(Qt::WA_Hover);
}

HoverTabBar::~HoverTabBar() {}

void HoverTabBar::hoverEnter(QHoverEvent *event) {
  // TODO create hover dialog - get tab index
  m_hoverindex = tabAt(event->pos());
  m_dlg = new HoverDialog(tabData(m_hoverindex), this);
//  emit hoverTabSelected(m_hoverindex);
}
void HoverTabBar::hoverLeave(QHoverEvent *event) {
  if (m_dlg) {
    m_dlg->close();
    m_hoverindex = -1;
  }
}
void HoverTabBar::hoverMove(QHoverEvent *event) {
  int index = tabAt(event->pos());
  m_hoverindex = index;
  if (index == -1) {
//    emit hoverTabClose();
  }
  if (index != m_hoverindex) {
//    emit hoverTabSelected(m_hoverindex);
  }
}

bool HoverTabBar::event(QEvent *e) {
  switch (e->type()) {
  case QEvent::HoverEnter:
    hoverEnter(static_cast<QHoverEvent *>(e));
    return true;
  case QEvent::HoverLeave:
    hoverLeave(static_cast<QHoverEvent *>(e));
    return true;
  case QEvent::HoverMove:
    hoverMove(static_cast<QHoverEvent *>(e));
    return true;
  default:
    break;
  }
  return QWidget::event(e);
}

const QString MainWindow::READ_ONLY = MainWindow::tr("Read Only");
const QString MainWindow::READ_WRITE = MainWindow::tr("Read Write");
const QString MainWindow::NO_FILE = MainWindow::tr("No File");
const QString MainWindow::NOT_MODIFIED = MainWindow::tr("Not Modified");
const QString MainWindow::MODIFIED = MainWindow::tr("Modified");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_currentindex(0), m_bookcount(0) {

  QLogger::addLogger("root", q5TRACE, CONSOLE);

  initBuild();

  //  createEPubDocument(/*m_currentbook*/);

  //  m_editor->setDocument(m_document);
}

MainWindow::~MainWindow() {}

void MainWindow::resizeEvent(QResizeEvent *e) {
  QSize size = e->size();
  m_width = size.width();
  m_height = size.height();
  m_prefchanged = true;
  saveOptions();
}

void MainWindow::moveEvent(QMoveEvent *e) {
  QPoint pos = e->pos();
  m_x = pos.x();
  m_y = pos.y();
  m_prefchanged = true;
  saveOptions();
}

void MainWindow::initBuild() {
  initActions();
  initMenus();
  initStatusbar();
  initGui();
  initSetup();
}

void MainWindow::initGui() {
  QFrame *mainFrame = new QFrame(this);
  setCentralWidget(mainFrame);
  QGridLayout *l = new QGridLayout;
  mainFrame->setLayout(l);

  m_tabs = new HoverTabWidget();
  connect(m_tabs, &QTabWidget::currentChanged, this,
          &MainWindow::documentChanged);

  l->addWidget(m_tabs, 0, 0);
}

// bool MainWindow::eventFilter(QObject *object, QEvent *event) {
//  if (object == m_tabbar && event->type() == QEvent::MouseMove) {
//    // TODO create hover window
//    return true;
//  }
//  return false;
//}

void MainWindow::initMenus() {
  m_filemenu = menuBar()->addMenu(tr("&File"));
  m_filemenu->addAction(m_fileopen);
  m_filemenu->addAction(m_filesave);
  m_filemenu->addAction(m_filesaveas);
  m_filemenu->addAction(m_filesaveall);
  m_filemenu->addSeparator();
  m_filemenu->addAction(m_fileexit);

  m_editmenu = menuBar()->addMenu(tr("&Edit"));
  m_editmenu->addAction(m_editundo);
  m_editmenu->addAction(m_editredo);
  m_editmenu->addSeparator();
  m_editmenu->addAction(m_editcut);
  m_editmenu->addAction(m_editcopy);
  m_editmenu->addAction(m_editpaste);
  m_editmenu->addAction(m_editpastehistory);

  m_windowmenu = menuBar()->addMenu(tr("&Window"));
  m_windowmenu->addAction(m_winmaximise);
  m_windowmenu->addAction(m_winminimise);
  m_windowmenu->addAction(m_winfullscreen);
  m_windowmenu->addAction(m_wincentre);
}

void MainWindow::initFileActions() {
  m_fileopen = new QAction(tr("&Open"), this);
  //    m_fileopen->setCheckable(true);
  m_fileopen->setShortcut(QKeySequence::Open);
  m_fileopen->setStatusTip(tr("Open a new file."));
  connect(m_fileopen, &QAction::triggered, this, &MainWindow::fileOpen);

  m_filesave = new QAction(tr("&Save"), this);
  m_filesave->setShortcut(QKeySequence::Save);
  m_filesave->setStatusTip(tr("Save the current file."));
  connect(m_filesave, &QAction::triggered, this, &MainWindow::fileSave);

  m_filesaveas = new QAction(tr("Save &As"), this);
  m_filesaveas->setShortcut(QKeySequence::SaveAs);
  m_filesaveas->setStatusTip(tr("Save the current file with a new name."));
  connect(m_filesaveas, &QAction::triggered, this, &MainWindow::fileSaveAs);

  m_filesaveall = new QAction(tr("Save A&ll"), this);
  m_filesaveall->setShortcut(tr("Ctrl_Shift+l"));
  m_filesaveall->setStatusTip(tr("Save all open file(s)."));
  connect(m_filesaveall, &QAction::triggered, this, &MainWindow::fileSaveAs);

  m_fileexit = new QAction(tr("E&xit"), this);
  m_fileexit->setShortcut(QKeySequence::Quit);
  m_fileexit->setStatusTip(tr("Quit the application."));
  connect(m_fileexit, &QAction::triggered, this, &MainWindow::fileExit);
}

void MainWindow::initWindowActions() {
  m_screengrp = new QActionGroup(this);

  m_winmaximise = new QAction(tr("Maximise screen"), this);
  m_screengrp->addAction(m_winmaximise);
  m_winmaximise->setShortcut(tr("Alt+X"));
  m_winmaximise->setStatusTip(
      tr("Set screen size to full screen with title bar."));
  connect(m_winmaximise, &QAction::triggered, this, &MainWindow::winMaximise);

  m_winminimise = new QAction(tr("Minimise screen"), this);
  m_screengrp->addAction(m_winminimise);
  m_winminimise->setShortcut(tr("Alt+N"));
  m_winminimise->setStatusTip(
      tr("Set screen size to minimised screen with title bar."));
  connect(m_winminimise, &QAction::triggered, this, &MainWindow::winMinimise);
  m_winminimise->setChecked(true);

  m_winfullscreen = new QAction(tr("Full screen"), this);
  m_screengrp->addAction(m_winfullscreen);
  m_winfullscreen->setShortcut(QKeySequence::FullScreen);
  m_winfullscreen->setStatusTip(
      tr("Set screen size to full screen without title bar."));
  connect(m_winfullscreen, &QAction::triggered, this,
          &MainWindow::winFullscreen);

  m_wincentre = new QAction(tr("Centre screen"), this);
  m_wincentre->setShortcut(QKeySequence::FullScreen);
  m_wincentre->setStatusTip(tr("Adjust the screen so that it is centred, "
                               "only available for minimised screens."));
  connect(m_wincentre, &QAction::triggered, this, &MainWindow::winCentre);
}

void MainWindow::initEditActions() {
  m_editundo = new QAction(tr("&Undo"), this);
  m_editundo->setShortcut(QKeySequence::Undo);
  m_editundo->setStatusTip(tr("Undo last change."));
  connect(m_editundo, &QAction::triggered, this, &MainWindow::editUndo);

  m_editredo = new QAction(tr("&Redo"), this);
  m_editredo->setShortcut(QKeySequence::Redo);
  m_editredo->setStatusTip(tr("Redo last Undone change."));
  connect(m_editredo, &QAction::triggered, this, &MainWindow::editRedo);

  m_editcopy = new QAction(tr("&Copy"), this);
  m_editcopy->setShortcut(QKeySequence::Copy);
  m_editcopy->setStatusTip(tr("Copy selected text."));
  connect(m_editcopy, &QAction::triggered, this, &MainWindow::editCopy);

  m_editcut = new QAction(tr("Cu&t"), this);
  m_editcut->setShortcut(QKeySequence::Cut);
  m_editcut->setStatusTip(tr("Cut selected text."));
  connect(m_editcut, &QAction::triggered, this, &MainWindow::editCut);

  m_editpaste = new QAction(tr("&Paste"), this);
  m_editpaste->setShortcut(QKeySequence::Paste);
  m_editpaste->setStatusTip(tr("paste selected text."));
  connect(m_editpaste, &QAction::triggered, this, &MainWindow::editPaste);

  m_editpastehistory = new QAction(tr("Paste from clipboard history"), this);
  m_editpastehistory->setShortcut(tr("Ctrl+Shift+V"));
  m_editpastehistory->setStatusTip(
      tr("Paste from a previous clipboard selection."));
  connect(m_editpastehistory, &QAction::triggered, this, &MainWindow::editCopy);

  m_editoptions = new QAction(tr("Preferences"), this);
  m_editoptions->setShortcut(QKeySequence::Preferences);
  m_editoptions->setStatusTip(tr("Modify preferences."));
  connect(m_editoptions, &QAction::triggered, this, &MainWindow::editOptions);
}

void MainWindow::initActions() {
  initFileActions();
  initEditActions();
  initWindowActions();
}

void MainWindow::initStatusbar() {
  m_filelbl = new QLabel(NO_FILE, this);
  m_filelbl->setFrameStyle(QFrame::StyledPanel);
  m_readonlylbl = new QLabel(READ_ONLY, this);
  m_readonlylbl->setFrameStyle(QFrame::StyledPanel);
  m_modifiedlbl = new QLabel(NOT_MODIFIED, this);
  m_modifiedlbl->setFrameStyle(QFrame::StyledPanel);
  statusBar()->addPermanentWidget(m_filelbl);
  statusBar()->addPermanentWidget(m_modifiedlbl);
  statusBar()->addPermanentWidget(m_readonlylbl);
}

void MainWindow::saveOptions() {
  QFile file(QString("preferences.yaml"));
  if (m_prefchanged) {
    if (file.open((QFile::ReadWrite | QFile::Truncate))) {
      YAML::Emitter emitter;
      emitter << YAML::BeginMap;
      emitter << YAML::Key << "x";
      emitter << YAML::Value << m_x;
      emitter << YAML::Key << "y";
      emitter << YAML::Value << m_y;
      emitter << YAML::Key << "width";
      emitter << YAML::Value << m_width;
      emitter << YAML::Key << "height";
      emitter << YAML::Value << m_height;
      emitter << YAML::Key << "current book";
      emitter << YAML::Value << m_currentindex;
      emitter << YAML::Key << "book list";
      emitter << YAML::BeginSeq;
      foreach (QString book, m_currentfiles) { emitter << book; }
      emitter << YAML::EndSeq;
      emitter << YAML::EndMap;
      QTextStream out(&file);
      out << emitter.c_str();
    }
  }
  m_prefchanged = false;
}

void MainWindow::loadOptions() {
  QFile file(QString("preferences.yaml"));
  if (file.exists()) {
    m_preferences = YAML::LoadFile(QString("preferences.yaml"));
    if (m_preferences["x"]) {
      m_x = m_preferences["x"].as<int>();
    } else {
      m_x = DEF_X;
      m_prefchanged = true;
    }
    if (m_preferences["y"]) {
      m_y = m_preferences["y"].as<int>();
    } else {
      m_y = DEF_Y;
      m_prefchanged = true;
    }
    if (m_preferences["width"]) {
      m_width = m_preferences["width"].as<int>();
    } else {
      m_width = DEF_WIDTH;
      m_prefchanged = true;
    }
    if (m_preferences["height"]) {
      m_height = m_preferences["height"].as<int>();
    } else {
      m_height = DEF_HEIGHT;
      m_prefchanged = true;
    }

    if (m_preferences["current"]) {
      m_currentindex = m_preferences["current"].as<int>();
    } else {
      m_currentindex = 0;
      m_prefchanged = true;
    }
    if (m_preferences["count"]) {
      m_bookcount = m_preferences["count"].as<int>();
    } else {
      m_bookcount = 0;
      m_prefchanged = true;
    }
    YAML::Node books = m_preferences["book list"];
    if (books && books.IsSequence()) {
      m_currentfiles.clear(); // Empty list just in case.
      for (uint i = 0; i < books.size(); i++) {
        m_currentfiles.append(books[i].as<QString>());
      }
    }
    //    m_currentfiles = m_preferences["book list"].as<QList<QString>>();
  } else {
    m_x = DEF_X;
    m_y = DEF_Y;
    m_width = DEF_WIDTH;
    m_height = DEF_HEIGHT;
    m_prefchanged = true;
  }

  if (!m_currentfiles.empty()) {
    foreach (QString filename, m_currentfiles) { loadDocument(filename); }
  }
}

void MainWindow::documentChanged(int index) {
  EBookDocument *document = m_documents.at(index);
  if (document->isModified()) {
    setModified();
  } else {
    setNotModified();
  }
  if (document->readonly()) {
    setReadOnly();
  } else {
    setReadWrite();
  }
  setFilename(document->documentPath());
}

void MainWindow::initSetup() {
  m_prefchanged = false;
  loadOptions();
  setGeometry(m_x, m_y, m_width, m_height);
  saveOptions();
}

void MainWindow::loadDocument(QString filename) {
  if (!m_currentfiles.contains(filename)) {
    m_currentfiles.append(filename);
  }
  EBookDocument *document = createDocument(filename);
  EBookEditor *edit = new EBookEditor(this);
  edit->setDocument(document);
  m_documents.append(document);
  QString tabname = QString(
      tr("%1, (%2)").arg(document->title()).arg(document->authorNames()));
  m_tabs->addTab(edit, tabname);
}

void MainWindow::fileOpen() {
  QStringList filenames = QFileDialog::getOpenFileNames(
      this, tr("Load Book"), m_defbookpath, tr("EBook (*.mobi *.epub)"));
  m_currentfiles.clear();
  m_tabs->clear();
  foreach (QString filename, filenames) { loadDocument(filename); }
  m_bookcount = m_currentfiles.size();
  m_prefchanged = true;
  saveOptions();
}

void MainWindow::fileSave() {}

void MainWindow::fileSaveAs() {}

void MainWindow::fileExit() {
  if (m_prefchanged) {
    saveOptions();
  }

  // TODO Are you really really really sure?
  close();
}

void MainWindow::editUndo() {}

void MainWindow::editRedo() {}

void MainWindow::editCopy() {}

void MainWindow::editCut() {}

void MainWindow::editPaste() {}

void MainWindow::editPasteFromHistory() {}

void MainWindow::editOptions() {}

void MainWindow::winFullscreen() { m_wincentre->setEnabled(false); }

void MainWindow::winMaximise() { m_wincentre->setEnabled(false); }

void MainWindow::winMinimise() { m_wincentre->setEnabled(true); }

void MainWindow::winCentre() {}

void MainWindow::clearDocuments() {
  m_mobi_doc = Q_NULLPTR;
  m_epub_doc = Q_NULLPTR;
}

void MainWindow::setModified() { m_modifiedlbl->setText(MODIFIED); }

void MainWindow::setNotModified() { m_modifiedlbl->setText(NOT_MODIFIED); }

void MainWindow::setReadOnly() { m_readonlylbl->setText(READ_ONLY); }

void MainWindow::setReadWrite() { m_readonlylbl->setText(READ_WRITE); }

void MainWindow::setFilename(QString name) { m_filelbl->setText(name); }

EBookDocument *MainWindow::createDocument(QString path) {
  // TODO something more rigorous perhaps
  if (path.toLower().endsWith(".epub")) {
    return createEPubDocument(path);
  } else if (path.toLower().endsWith(".mobi")) {
    return createMobiDocument(path);
  }
  return Q_NULLPTR;
}

EBookDocument *MainWindow::createEPubDocument(QString path) {
  EPubDocument *document = new EPubDocument(this);
  clearDocuments();
  document->openDocument(path);
  return document;
}

EBookDocument *MainWindow::createMobiDocument(QString path) {
  MobiDocument *document = new MobiDocument(this);
  clearDocuments();
  document->openDocument(path);
  return document;
}

OptionsDialog::OptionsDialog(QWidget *parent) : QDialog(parent) {}

OptionsDialog::~OptionsDialog() {}

void OptionsDialog::initGui() {}
