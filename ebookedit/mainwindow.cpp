#include "mainwindow.h"

#include <qlogger.h>

#include "ebookdocument.h"

using namespace qlogger;

const QString MainWindow::READ_ONLY = MainWindow::tr("Read Only");
const QString MainWindow::READ_WRITE = MainWindow::tr("Read Write");
const QString MainWindow::NO_FILE = MainWindow::tr("No File");
const QString MainWindow::NOT_MODIFIED = MainWindow::tr("Not Modified");
const QString MainWindow::MODIFIED = MainWindow::tr("Modified");

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_bookcount(0)
  , m_popup(Q_NULLPTR)
{
  QLogger::addLogger("root", q5TRACE, CONSOLE);

  initBuild();

  //  createEPubDocument(/*m_currentbook*/);
  //  m_editor->setDocument(m_document);
}

MainWindow::~MainWindow() {}

void
MainWindow::resizeEvent(QResizeEvent* e)
{
  QSize size = e->size();
  m_options.width = size.width();
  m_options.height = size.height();
  m_prefchanged = true;
  saveOptions();
}

void
MainWindow::moveEvent(QMoveEvent* e)
{
  QPoint pos = e->pos();
  m_options.x = pos.x();
  m_options.y = pos.y();
  m_prefchanged = true;
  saveOptions();
}

void
MainWindow::initBuild()
{
  initActions();
  initMenus();
  initStatusbar();
  initGui();
  initSetup();
}

void
MainWindow::initGui()
{
  QFrame* mainFrame = new QFrame(this);
  setCentralWidget(mainFrame);
  QGridLayout* l = new QGridLayout;
  mainFrame->setLayout(l);

  m_tabs = new HoverTabWidget();
  connect(
    m_tabs, &QTabWidget::currentChanged, this, &MainWindow::documentChanged);
  connect(m_tabs, &HoverTabWidget::tabEntered, this, &MainWindow::tabEntered);
  connect(m_tabs, &HoverTabWidget::tabExited, this, &MainWindow::tabExited);
  l->addWidget(m_tabs, 0, 0);
}

void
MainWindow::initFileMenu()
{
  m_filemenu = menuBar()->addMenu(tr("&File"));
  m_filemenu->addAction(m_fileopen);
  m_filemenu->addAction(m_filesave);
  m_filemenu->addAction(m_filesaveas);
  m_filemenu->addAction(m_filesaveall);
  m_filemenu->addSeparator();
  m_filemenu->addAction(m_fileexit);
}

void
MainWindow::initEditMenu()
{
  m_editmenu = menuBar()->addMenu(tr("&Edit"));
  m_editmenu->addAction(m_editundo);
  m_editmenu->addAction(m_editredo);
  m_editmenu->addSeparator();
  m_editmenu->addAction(m_editcut);
  m_editmenu->addAction(m_editcopy);
  m_editmenu->addAction(m_editpaste);
  m_editmenu->addAction(m_editpastehistory);
  m_editmenu->addSeparator();
  m_editmenu->addAction(m_editoptions);
}

void
MainWindow::initWindowMenu()
{
  m_windowmenu = menuBar()->addMenu(tr("&Window"));
  m_windowmenu->addAction(m_winmaximise);
  m_windowmenu->addAction(m_winminimise);
  m_windowmenu->addAction(m_winfullscreen);
  m_windowmenu->addAction(m_wincentre);
}

void
MainWindow::initMenus()
{
  initFileMenu();
  initEditMenu();
  initWindowMenu();
}

void
MainWindow::initFileActions()
{
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

void
MainWindow::initWindowActions()
{
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
  connect(
    m_winfullscreen, &QAction::triggered, this, &MainWindow::winFullscreen);

  m_wincentre = new QAction(tr("Centre screen"), this);
  m_wincentre->setShortcut(QKeySequence::FullScreen);
  m_wincentre->setStatusTip(tr("Adjust the screen so that it is centred, "
                               "only available for minimised screens."));
  connect(m_wincentre, &QAction::triggered, this, &MainWindow::winCentre);
}

void
MainWindow::initEditActions()
{
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

void
MainWindow::initActions()
{
  initFileActions();
  initEditActions();
  initWindowActions();
}

void
MainWindow::initStatusbar()
{
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

void
MainWindow::saveOptions()
{
  QFile file(QString("preferences.yaml"));
  if (m_prefchanged) {
    if (file.open((QFile::ReadWrite | QFile::Truncate))) {
      YAML::Emitter emitter;
      emitter << YAML::BeginMap;
      emitter << YAML::Key << "x";
      emitter << YAML::Value << m_options.x;
      emitter << YAML::Key << "y";
      emitter << YAML::Value << m_options.y;
      emitter << YAML::Key << "width";
      emitter << YAML::Value << m_options.width;
      emitter << YAML::Key << "height";
      emitter << YAML::Value << m_options.height;
      emitter << YAML::Key << "popup timeout";
      emitter << YAML::Value << m_options.popuptimeout;
      emitter << YAML::Key << "current book";
      emitter << YAML::Value << m_options.currentindex;
      emitter << YAML::Key << "book list";
      emitter << YAML::BeginSeq;
      foreach (QString book, m_options.currentfiles) {
        emitter << book;
      }
      emitter << YAML::EndSeq;
      emitter << YAML::EndMap;
      QTextStream out(&file);
      out << emitter.c_str();
    }
  }
  m_prefchanged = false;
}

void
MainWindow::loadOptions()
{
  QFile file(QString("preferences.yaml"));
  if (file.exists()) {
    m_preferences = YAML::LoadFile(QString("preferences.yaml"));
    // Last window position.
    if (m_preferences["x"]) {
      m_options.x = m_preferences["x"].as<int>();
    } else {
      m_options.x = DEF_X;
      m_prefchanged = true;
    }
    if (m_preferences["y"]) {
      m_options.y = m_preferences["y"].as<int>();
    } else {
      m_options.y = DEF_Y;
      m_prefchanged = true;
    }
    if (m_preferences["width"]) {
      m_options.width = m_preferences["width"].as<int>();
    } else {
      m_options.width = DEF_WIDTH;
      m_prefchanged = true;
    }
    if (m_preferences["height"]) {
      m_options.height = m_preferences["height"].as<int>();
    } else {
      m_options.height = DEF_HEIGHT;
      m_prefchanged = true;
    }
    // Timeout for popup information.
    if (m_preferences["popup timeout"]) {
      m_options.popuptimeout = m_preferences["popup timeout"].as<int>();
    } else {
      m_options.popuptimeout = DEF_POPUP_TIMEOUT;
      m_prefchanged = true;
    }
    // Current book being read/edited.
    if (m_preferences["current"]) {
      m_options.currentindex = m_preferences["current"].as<int>();
    } else {
      m_options.currentindex = 0;
      m_prefchanged = true;
    }
    // Last book count.
    if (m_preferences["count"]) {
      m_options.bookcount = m_preferences["count"].as<int>();
    } else {
      m_options.bookcount = 0;
      m_prefchanged = true;
    }
    // Last books loaded.
    YAML::Node books = m_preferences["book list"];
    if (books && books.IsSequence()) {
      m_options.currentfiles.clear(); // Empty list just in case.
      for (uint i = 0; i < books.size(); i++) {
        m_options.currentfiles.append(books[i].as<QString>());
      }
    }
    //    m_currentfiles = m_preferences["book list"].as<QList<QString>>();
  } else {
    m_options.x = DEF_X;
    m_options.y = DEF_Y;
    m_options.width = DEF_WIDTH;
    m_options.height = DEF_HEIGHT;
    m_options.currentindex = -1;
    m_options.currentfiles = QStringList();
    m_options.bookcount = 0;
    m_prefchanged = true;
  }

  if (!m_options.currentfiles.empty()) {
    foreach (QString filename, m_options.currentfiles) {
      loadDocument(filename);
    }
  }
}

void
MainWindow::documentChanged(int index)
{
  EBookDocument* document = m_documents.at(index);
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

void
MainWindow::initSetup()
{
  m_prefchanged = false;
  loadOptions();
  setGeometry(m_options.x, m_options.y, m_options.width, m_options.height);
  saveOptions();
}

void
MainWindow::loadDocument(QString filename)
{
  if (!m_options.currentfiles.contains(filename)) {
    m_options.currentfiles.append(filename);
  }
  EBookDocument* document = createDocument(filename);
  EBookEditor* edit = new EBookEditor(this);
  edit->setDocument(document);
  m_documents.append(document);
  QString tabname =
    QString(tr("%1, (%2)").arg(document->title()).arg(document->authorNames()));
  m_tabs->addTab(
    edit, tabname, QVariant::fromValue<EBookData>(document->data()));
  //  } else {
  //    int index = m_currentfiles.indexOf(filename);
  //    if (index != -1) {
  //      m_tabs->setCurrentIndex(index);
  //    }
  //  }
}

void
MainWindow::fileOpen()
{
  QStringList filenames = QFileDialog::getOpenFileNames(
    this, tr("Load Book"), m_defbookpath, tr("EBook (*.mobi *.epub)"));
  foreach (QString filename, filenames) {
    loadDocument(filename);
  }
  m_bookcount = m_options.currentfiles.size();
  m_prefchanged = true;
  saveOptions();
}

void
MainWindow::fileSave()
{}

void
MainWindow::fileSaveAs()
{}

void
MainWindow::fileExit()
{
  if (m_prefchanged) {
    saveOptions();
  }

  // TODO Are you really really really sure?
  close();
}

void
MainWindow::editUndo()
{}

void
MainWindow::editRedo()
{}

void
MainWindow::editCopy()
{}

void
MainWindow::editCut()
{}

void
MainWindow::editPaste()
{}

void
MainWindow::editPasteFromHistory()
{}

void
MainWindow::editOptions()
{
  OptionsDialog* dlg = new OptionsDialog(m_options, this);
  int result = dlg->exec();
  if (result == QDialog::Accepted) {
    if (dlg->modified()) {
      m_options = dlg->options();
      saveOptions();
    }
  }
}

void
MainWindow::winFullscreen()
{
  m_wincentre->setEnabled(false);
}

void
MainWindow::winMaximise()
{
  m_wincentre->setEnabled(false);
}

void
MainWindow::winMinimise()
{
  m_wincentre->setEnabled(true);
}

void
MainWindow::winCentre()
{
  QScreen* screen = QGuiApplication::primaryScreen();
  QRect screenGeometry = screen->geometry();
  move(screenGeometry.center() - rect().center());
  m_prefchanged = true;
  saveOptions();
}

void
MainWindow::clearDocuments()
{
  m_mobi_doc = Q_NULLPTR;
  m_epub_doc = Q_NULLPTR;
}

void
MainWindow::setModified()
{
  m_modifiedlbl->setText(MODIFIED);
}

void
MainWindow::setNotModified()
{
  m_modifiedlbl->setText(NOT_MODIFIED);
}

void
MainWindow::setReadOnly()
{
  m_readonlylbl->setText(READ_ONLY);
}

void
MainWindow::setReadWrite()
{
  m_readonlylbl->setText(READ_WRITE);
}

void
MainWindow::setFilename(QString name)
{
  m_filelbl->setText(name);
}

void
MainWindow::tabEntered(int index, QPoint pos, QVariant data)
{
  // new tab entrance.
  if (m_popupindex != -1) // this stops restarting popup.
    return;

  if (m_options.enablepopup) {
    m_popup = new HoverPopup(data, this);
    m_popup->setFrameStyle(QFrame::NoFrame);
    m_popupindex = index;
    m_popuptimer = new QTimer(this);
    connect(m_popuptimer, &QTimer::timeout, this, &MainWindow::closePopup);
    m_popuptimer->start(m_options.popuptimeout);
    m_popup->show();
  }
}

void
MainWindow::tabExited(int /*index*/)
{
  closePopup();
  m_popupindex = -1;
}

void
MainWindow::closePopup()
{
  if (m_popup) {
    m_popup->close();
    m_popup = Q_NULLPTR;
  }
}

EBookDocument*
MainWindow::createDocument(QString path)
{
  // TODO something more rigorous perhaps
  if (path.toLower().endsWith(".epub")) {
    return createEPubDocument(path);
  } else if (path.toLower().endsWith(".mobi")) {
    return createMobiDocument(path);
  }
  return Q_NULLPTR;
}

EBookDocument*
MainWindow::createEPubDocument(QString path)
{
  EPubDocument* document = new EPubDocument(this);
  clearDocuments();
  document->openDocument(path);
  return document;
}

EBookDocument*
MainWindow::createMobiDocument(QString path)
{
  MobiDocument* document = new MobiDocument(this);
  clearDocuments();
  document->openDocument(path);
  return document;
}
