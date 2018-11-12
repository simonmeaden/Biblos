#include "mainwindow.h"

#include <qlogger.h>

#include "qebookdocument.h"

using namespace qlogger;

const QString MainWindow::READ_ONLY = MainWindow::tr("Read Only");
const QString MainWindow::READ_WRITE = MainWindow::tr("Read Write");
const QString MainWindow::NO_FILE = MainWindow::tr("No File");
const QString MainWindow::NOT_MODIFIED = MainWindow::tr("Not Modified");
const QString MainWindow::MODIFIED = MainWindow::tr("Modified");

const QString MainWindow::PREF_FILE = "preferences.yaml";
QString MainWindow::POSITION = "window";
QString MainWindow::DIALOG = "options dialog";
QString MainWindow::PREF_POPUP_TIMEOUT = "popup timeout";
QString MainWindow::PREF_ENABLE_POPUP = "popup enable";
QString MainWindow::PREF_CURRENT_INDEX = "current book";
QString MainWindow::PREF_COUNT = "count";
QString MainWindow::PREF_BOOKLIST = "book list";
QString MainWindow::CODE_OPTIONS = "code editor";
QString MainWindow::CODE_FONT = "font";
QString MainWindow::CODE_NORMAL = "normal";
QString MainWindow::CODE_ATTRIBUTE = "attribute";
QString MainWindow::CODE_TAG = "tag";
QString MainWindow::CODE_ERROR = "error";
QString MainWindow::CODE_STRING = "string";
QString MainWindow::CODE_STYLE = "style";
QString MainWindow::CODE_SCRIPT = "script";
QString MainWindow::CODE_COLOR = "color";
QString MainWindow::CODE_BACK = "background";
QString MainWindow::CODE_WEIGHT = "weight";
QString MainWindow::CODE_ITALIC = "italic";

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_initialising(true)
  , m_options(new Options())
  , m_bookcount(0)
  , m_popup(Q_NULLPTR)
{
  QLogger::addLogger("root", q5TRACE, CONSOLE);

  loadPlugins();
  initBuild();

  m_initialising = false;
}

MainWindow::~MainWindow() {}

void
MainWindow::resizeEvent(QResizeEvent* e)
{
  QSize size = e->size();
  m_options->rect.setSize(size);
  m_prefchanged = true;
  if (!m_initialising)
    saveOptions();
}

void
MainWindow::moveEvent(QMoveEvent* e)
{
  QPoint pos = e->pos();
  m_options->rect.setTopLeft(pos);
  m_prefchanged = true;
  if (!m_initialising)
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

  m_tabs = new QTabWidget();
  m_tabs->setTabsClosable(true);
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
  QFile file(PREF_FILE);
  if (m_prefchanged) {
    if (file.open((QFile::ReadWrite | QFile::Truncate))) {
      YAML::Emitter emitter;
      {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << POSITION;
        emitter << YAML::Value << m_options->rect;
        emitter << YAML::Key << DIALOG;
        emitter << YAML::Value << m_options->options_dlg;
        emitter << YAML::Key << PREF_POPUP_TIMEOUT;
        emitter << YAML::Value << m_options->popuptimeout;
        emitter << YAML::Key << PREF_ENABLE_POPUP;
        emitter << YAML::Value << m_options->enablepopup;
        emitter << YAML::Key << PREF_CURRENT_INDEX;
        emitter << YAML::Value << m_options->currentindex;
        emitter << YAML::Key << PREF_BOOKLIST;
        { // Start of PREF_BOOKLIST
          emitter << YAML::BeginSeq;
          foreach (QString book, m_options->currentfiles) {
            emitter << book;
          }
          emitter << YAML::EndSeq;
        } // End of PREF_BOOKLIST
        emitter << YAML::Key << CODE_OPTIONS;
        { // Start of CODE_OPTIONS
          emitter << YAML::BeginMap;
          emitter << YAML::Key << CODE_FONT;
          emitter << YAML::Value << m_options->codeFont;
          emitter << YAML::Key << CODE_NORMAL;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_options->normalColor;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_options->normalBack;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << int(m_options->normalWeight);
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_options->normalItalic;
            emitter << YAML::EndMap;
          }
          emitter << YAML::Key << CODE_ATTRIBUTE;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_options->attributeColor;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_options->attributeBack;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << int(m_options->attributeWeight);
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_options->attributeItalic;
            emitter << YAML::EndMap;
          }
          emitter << YAML::Key << CODE_TAG;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_options->tagColor;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_options->tagBack;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << int(m_options->tagWeight);
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_options->tagItalic;
            emitter << YAML::EndMap;
          }
          emitter << YAML::Key << CODE_STRING;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_options->stringColor;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_options->stringBack;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << int(m_options->stringWeight);
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_options->stringItalic;
            emitter << YAML::EndMap;
          }
          emitter << YAML::Key << CODE_ERROR;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_options->errorColor;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_options->errorBack;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << m_options->errorWeight;
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_options->errorItalic;
            emitter << YAML::EndMap;
          }
          emitter << YAML::Key << CODE_STYLE;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_options->styleColor;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_options->styleBack;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << m_options->styleWeight;
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_options->styleItalic;
            emitter << YAML::EndMap;
          }
          emitter << YAML::Key << CODE_SCRIPT;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_options->scriptColor;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_options->scriptBack;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << m_options->scriptWeight;
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_options->scriptItalic;
            emitter << YAML::EndMap;
          }
        } // End of CODE_OPTIONS
        emitter << YAML::EndMap;
      }
      QTextStream out(&file);
      out << emitter.c_str();
    }
  }
  m_prefchanged = false;
}

void
MainWindow::loadOptions()
{
  QFile file(PREF_FILE);
  if (file.exists()) {
    m_preferences = YAML::LoadFile(QString("preferences.yaml"));
    // Last window position.
    if (m_preferences[POSITION]) {
      m_options->rect = m_preferences[POSITION].as<QRect>();
    } else {
      m_options->rect = QRect(0, 0, DEF_WIDTH, DEF_HEIGHT);
    }
    if (m_preferences[DIALOG]) {
      m_options->options_dlg = m_preferences[DIALOG].as<QSize>();
    } else {
      m_options->options_dlg = QSize(DEF_DLG_WIDTH, DEF_DLG_HEIGHT);
    }
    // Popup information enablement.
    if (m_preferences[PREF_ENABLE_POPUP]) {
      m_options->enablepopup = m_preferences[PREF_ENABLE_POPUP].as<bool>();
    } else {
      m_options->enablepopup = true;
    }
    // Timeout for popup information.
    if (m_preferences[PREF_POPUP_TIMEOUT]) {
      m_options->popuptimeout = m_preferences[PREF_POPUP_TIMEOUT].as<int>();
    } else {
      m_options->popuptimeout = DEF_POPUP_TIMEOUT;
    }
    // Current book being read/edited.
    if (m_preferences[PREF_CURRENT_INDEX]) {
      m_options->currentindex = m_preferences[PREF_CURRENT_INDEX].as<int>();
    } else {
      m_options->currentindex = 0;
    }
    // Last books loaded.
    YAML::Node books = m_preferences[PREF_BOOKLIST];
    if (books && books.IsSequence()) {
      m_options->currentfiles.clear(); // Empty list just in case.
      for (uint i = 0; i < books.size(); i++) {
        m_options->currentfiles.append(books[i].as<QString>());
      }
    }
    YAML::Node codeoptions = m_preferences[CODE_OPTIONS];
    if (codeoptions && codeoptions.IsMap()) {
      YAML::Node codefont = codeoptions[CODE_FONT];
      if (codefont) {
        m_options->codeFont = codefont.as<QFont>();
      }
      YAML::Node codenormal = codeoptions[CODE_NORMAL];
      if (codenormal && codenormal.IsMap()) {
        m_options->normalColor = codenormal[CODE_COLOR].as<QColor>();
        m_options->normalBack = codenormal[CODE_BACK].as<QColor>();
        m_options->normalWeight =
          QFont::Weight(codenormal[CODE_WEIGHT].as<int>());
        m_options->normalItalic = codenormal[CODE_ITALIC].as<bool>();
      }
      YAML::Node codeatt = codeoptions[CODE_ATTRIBUTE];
      if (codeatt && codeatt.IsMap()) {
        m_options->attributeColor = codeatt[CODE_COLOR].as<QColor>();
        m_options->attributeBack = codeatt[CODE_BACK].as<QColor>();
        m_options->attributeWeight =
          QFont::Weight(codeatt[CODE_WEIGHT].as<int>());
        m_options->attributeItalic = codeatt[CODE_ITALIC].as<bool>();
      }
      YAML::Node codetag = codeoptions[CODE_TAG];
      if (codetag && codetag.IsMap()) {
        m_options->tagColor = codetag[CODE_COLOR].as<QColor>();
        m_options->tagBack = codetag[CODE_BACK].as<QColor>();
        m_options->tagWeight = QFont::Weight(codetag[CODE_WEIGHT].as<int>());
        m_options->tagItalic = codeatt[CODE_ITALIC].as<bool>();
      }
      YAML::Node codeattstr = codeoptions[CODE_STRING];
      if (codeattstr && codeattstr.IsMap()) {
        m_options->stringColor = codeattstr[CODE_COLOR].as<QColor>();
        m_options->stringBack = codeattstr[CODE_BACK].as<QColor>();
        m_options->stringWeight =
          QFont::Weight(codeattstr[CODE_WEIGHT].as<int>());
        m_options->stringItalic = codeattstr[CODE_ITALIC].as<bool>();
      }
      YAML::Node codeerr = codeoptions[CODE_ERROR];
      if (codeerr && codeerr.IsMap()) {
        m_options->errorColor = codeerr[CODE_COLOR].as<QColor>();
        m_options->errorBack = codeerr[CODE_BACK].as<QColor>();
        m_options->errorWeight = QFont::Weight(codeerr[CODE_WEIGHT].as<int>());
        m_options->errorItalic = codeerr[CODE_ITALIC].as<bool>();
      }
      YAML::Node codestyle = codeoptions[CODE_STYLE];
      if (codestyle && codestyle.IsMap()) {
        m_options->styleColor = codestyle[CODE_COLOR].as<QColor>();
        m_options->styleBack = codestyle[CODE_BACK].as<QColor>();
        m_options->styleWeight =
          QFont::Weight(codestyle[CODE_WEIGHT].as<int>());
        m_options->styleItalic = codestyle[CODE_ITALIC].as<bool>();
      }
      YAML::Node codescript = codeoptions[CODE_SCRIPT];
      if (codescript && codescript.IsMap()) {
        m_options->scriptColor = codescript[CODE_COLOR].as<QColor>();
        m_options->scriptBack = codescript[CODE_BACK].as<QColor>();
        m_options->scriptWeight =
          QFont::Weight(codescript[CODE_WEIGHT].as<int>());
        m_options->scriptItalic = codescript[CODE_ITALIC].as<bool>();
      }
    }
    //    m_currentfiles = m_preferences["book list"].as<QList<QString>>();
  } else {
    m_options->rect = QRect(DEF_X, DEF_Y, DEF_WIDTH, DEF_HEIGHT);
    m_options->currentindex = -1;
    m_options->currentfiles = QStringList();
    m_options->bookcount = 0;
    m_prefchanged = true;
  }

  if (!m_options->currentfiles.empty()) {
    foreach (QString filename, m_options->currentfiles) {
      loadDocument(filename);
    }
  }
}

void
MainWindow::documentChanged(int index)
{
  QEBookDocument* document = m_documents.at(index);
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
  setGeometry(m_options->rect);
}

void
MainWindow::loadDocument(QString filename)
{
  if (!m_options->currentfiles.contains(filename)) {
    m_options->currentfiles.append(filename);
  }

  EBookWrapper* wrapper = new EBookWrapper(m_options, this);

  QEBookDocument* htmldocument = createDocument(filename);
  QEBookDocument* codeDocument = createDocument(htmldocument);
  wrapper->editor()->setDocument(htmldocument);
  wrapper->codeEditor()->setDocument(codeDocument);
  connect(
    this, &MainWindow::codeChanged, wrapper, &EBookWrapper::optionsHaveChanged);

  m_documents.append(htmldocument);
  QString tabname = QString(
    tr("%1, (%2)").arg(htmldocument->title()).arg(htmldocument->authorNames()));
  m_tabs->addTab(wrapper, tabname);
}

void
MainWindow::saveDocument(QEBookDocument* document)
{
  // TODO save the document
}

void
MainWindow::tabClosing(int tab)
{
  EBookWrapper* wrapper = qobject_cast<EBookWrapper*>(m_tabs->widget(tab));
  QEBookDocument* document =
    qobject_cast<QEBookDocument*>(wrapper->editor()->document());
  if (document->isModified()) {
    // TODO - Check if the user wants to save the document.
    saveDocument(document);
  }
}

void
MainWindow::fileOpen()
{
  QStringList filenames = QFileDialog::getOpenFileNames(
    this, tr("Load Book"), m_defbookpath, tr("EBook (*.mobi *.epub)"));
  foreach (QString filename, filenames) {
    loadDocument(filename);
  }
  m_bookcount = m_options->currentfiles.size();
  m_prefchanged = true;
  saveOptions();
}

void
MainWindow::fileSave()
{
  // TODO
}

void
MainWindow::fileSaveAs()
{
  // TODO
}

void
MainWindow::fileExit()
{
  saveOptions();

  // TODO Are you really really really sure?
  close();
}

void
MainWindow::editUndo()
{
  // TODO
}

void
MainWindow::editRedo()
{
  // TODO
}

void
MainWindow::editCopy()
{
  // TODO
}

void
MainWindow::editCut()
{
  // TODO
}

void
MainWindow::editPaste()
{
  // TODO
}

void
MainWindow::editPasteFromHistory()
{
  // TODO
}

void
MainWindow::editOptions()
{
  OptionsDialog* dlg = new OptionsDialog(m_options, this);
  connect(dlg, &OptionsDialog::codeChanged, this, &MainWindow::codeChanged);
  int result = dlg->exec();
  if (result == QDialog::Accepted) {
    if (dlg->modified()) {
      m_options = dlg->options();
      saveOptions();
      m_prefchanged = true;
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

// void
// MainWindow::tabEntered(int index, QPoint pos, QVariant data)
//{
//  // new tab entrance.
//  if (m_popupindex != -1) // this stops restarting popup.
//    return;

//  if (m_options->enablepopup) {
//    QPoint globalPos = mapToGlobal(pos);
//    m_popup = new HoverPopup(data, this);
//    m_popup->setFrameStyle(QFrame::NoFrame);
//    m_popup->setGeometry(
//      globalPos.x(), globalPos.y(), m_popup->width(), m_popup->height());
//    m_popupindex = index;
//    m_popuptimer = new QTimer(this);
//    connect(m_popuptimer, &QTimer::timeout, this, &MainWindow::closePopup);
//    m_popuptimer->start(m_options->popuptimeout);
//    m_popup->show();
//  }
//}

// void
// MainWindow::tabExited(int /*index*/)
//{
//  closePopup();
//  m_popupindex = -1;
//}

// void
// MainWindow::closePopup()
//{
//  if (m_popup) {
//    m_popup->close();
//    m_popup = Q_NULLPTR;
//  }
//}

QEBookDocument*
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

QEBookDocument*
MainWindow::createDocument(QEBookDocument* doc)
{
  EPubDocument* epub = dynamic_cast<EPubDocument*>(doc);
  if (epub) {
    EBookContents* contents = doc->cloneData();
    EPubDocument* document = new EPubDocument(doc->parent());
    document->setDocumentLayout(new QPlainTextDocumentLayout(document));
    document->setClonedData(contents);
    return document;
  }
  //  MobiDocument* mobi = dynamic_cast<MobiDocument*>(doc);
  //  if (mobi) {
  //    EPubDocument contents = doc->cloneData();
  //    MobiDocument* document = new MobiDocument(this);
  //    document->setClonedData(contents);
  //    return document;
  //  }
  return Q_NULLPTR;
}

QEBookDocument*
MainWindow::createEPubDocument(QString path)
{
  EPubDocument* document = new EPubDocument(this);
  clearDocuments();
  document->openDocument(path);
  return document;
}

QEBookDocument*
MainWindow::createMobiDocument(QString path)
{
  QMobiDocument* document = new QMobiDocument(this);
  clearDocuments();
  document->openDocument(path);
  return document;
}

void
MainWindow::loadPlugins()
{

  QDir pluginsDir = QDir(qApp->applicationDirPath());
  pluginsDir.cd("plugins");

  foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
    QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
    QObject* plugin = loader.instance();

    if (plugin) {

      SpellInterface* spellchecker = qobject_cast<SpellInterface*>(plugin);
      if (spellchecker)
        m_spellcheckers[spellchecker->name()] = spellchecker;
    }
  }
}
