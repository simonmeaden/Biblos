#include "mainwindow.h"

#include <qlogger/qlogger.h>

#include "authordialog.h"
#include "dbmanager.h"
#include "ebookcodeeditor.h"
#include "ebookdocument.h"
#include "ebookeditor.h"
#include "ebookwordreader.h"
#include "ebookwrapper.h"
#include "epubdocument.h"
#include "mobidocument.h"
//#include "library.h"
#include "basespellclass.h"

#include "optionsdialog.h"
#include "plugindialog.h"

using namespace qlogger;

const QString MainWindow::READ_ONLY = MainWindow::tr("Read Only");
const QString MainWindow::READ_WRITE = MainWindow::tr("Read Write");
const QString MainWindow::NO_FILE = MainWindow::tr("No File");
const QString MainWindow::NOT_MODIFIED = MainWindow::tr("Not Modified");
const QString MainWindow::MODIFIED = MainWindow::tr("Modified");

const QString MainWindow::PREF_FILE = "preferences.yaml";
const QString MainWindow::DB_NAME = "library.sqlite";

QString MainWindow::POSITION = "window";
QString MainWindow::DIALOG = "options dialog";
QString MainWindow::PREF_POPUP_TIMEOUT = "popup timeout";
QString MainWindow::PREF_ENABLE_POPUP = "popup enable";
QString MainWindow::PREF_CURRENT_INDEX = "current book";
QString MainWindow::PREF_COUNT = "count";
QString MainWindow::PREF_BOOKLIST = "book list";
QString MainWindow::PREF_LIBRARY = "library list";
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
QString MainWindow::COPY_BOOKS_TO_STORE = "copy books to store";
QString MainWindow::DELETE_OLD_BOOK = "delete old book";

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  //  , m_library(QSharedPointer<Library>(new Library()))
  , m_initialising(true)
  , m_options(new Options())
  , m_bookcount(0)
  , m_popup(Q_NULLPTR)
  , m_current_spell_checker(Q_NULLPTR)
{
  QLogger::addLogger("root", q5TRACE, CONSOLE);

  loadPlugins();
  initBuild();
  QDir dir;
  m_home_directiory = QStandardPaths::locate(
    QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory);
  m_data_directory =
    QStandardPaths::writableLocation(QStandardPaths::DataLocation);
  dir.mkpath(m_data_directory);
  m_config_directory =
    QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
  dir.mkpath(m_config_directory);
  m_config_file = m_config_directory + QDir::separator() + PREF_FILE;

  /* The database file will be created automatically by SqLite if it
   * does not exist. The tables are created if they do not already exist.
   */
  QString db_path = m_config_directory + QDir::separator() + DB_NAME;
  m_database = new DbManager(db_path, this);

  initSetup();

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
  initToolbar();
  initGui();
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
  connect(
    m_tabs, &QTabWidget::currentChanged, this, &MainWindow::documentChanged);
  connect(
    m_tabs, &QTabWidget::tabCloseRequested, this, &MainWindow::tabClosing);
  l->addWidget(m_tabs, 0, 0);
}

void
MainWindow::initFileMenu()
{
  m_filemenu = menuBar()->addMenu(tr("&File"));
  m_filemenu->addAction(m_file_open);
  m_filemenu->addAction(m_file_save);
  m_filemenu->addAction(m_file_save_as);
  m_filemenu->addAction(m_file_save_all);
  m_filemenu->addSeparator();
  m_filemenu->addAction(m_file_exit);
}

void
MainWindow::initEditMenu()
{
  m_editmenu = menuBar()->addMenu(tr("&Edit"));
  m_editmenu->addAction(m_edit_undo);
  m_editmenu->addAction(m_edit_redo);
  m_editmenu->addSeparator();
  m_editmenu->addAction(m_edit_cut);
  m_editmenu->addAction(m_edit_copy);
  m_editmenu->addAction(m_edit_paste);
  m_editmenu->addAction(m_edit_paste_history);
  m_editmenu->addSeparator();
  m_spellingmenu = m_editmenu->addMenu(tr("Spelling"));
  m_spellingmenu->addAction(m_edit_spellcheck);
  m_spellingmenu->addSeparator();
  m_spellingmenu->addAction(m_edit_highlight_misspelled);
  m_spellingmenu->addAction(m_edit_next_misspelled);
  m_spellingmenu->addSeparator();
  m_spellingmenu->addAction(m_edit_send_to_booklist);
  m_spellingmenu->addAction(m_edit_send_to_authorlist);
  m_editmenu->addSeparator();
  m_editmenu->addAction(m_edit_options);
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

// void
// MainWindow::initSpellingMenu()
//{
//  m_sp
//}

void
MainWindow::initHelpMenu()
{
  m_helpmenu = menuBar()->addMenu(tr("&Help"));
  m_helpmenu->addAction(m_help_contents);
  m_helpmenu->addAction(m_help_index);
  m_helpmenu->addAction(m_help_context);
  m_helpmenu->addSeparator();
  m_helpmenu->addAction(m_help_about_ebookeditor);
  m_helpmenu->addAction(m_help_about_plugins);
  m_helpmenu->addSeparator();
  m_helpmenu->addAction(m_help_check_updates);
}

void
MainWindow::initMenus()
{
  initFileMenu();
  initEditMenu();
  initWindowMenu();
  initHelpMenu();
}

void
MainWindow::initFileActions()
{
  m_file_open = new QAction(tr("&Open"), this);
  //    m_fileopen->setCheckable(true);
  m_file_open->setShortcut(QKeySequence::Open);
  m_file_open->setStatusTip(tr("Open a new file."));
  connect(m_file_open, &QAction::triggered, this, &MainWindow::fileOpen);

  m_file_save = new QAction(tr("&Save"), this);
  m_file_save->setShortcut(QKeySequence::Save);
  m_file_save->setStatusTip(tr("Save the current file."));
  connect(m_file_save, &QAction::triggered, this, &MainWindow::fileSave);

  m_file_save_as = new QAction(tr("Save &As"), this);
  m_file_save_as->setShortcut(QKeySequence::SaveAs);
  m_file_save_as->setStatusTip(tr("Save the current file with a new name."));
  connect(m_file_save_as, &QAction::triggered, this, &MainWindow::fileSaveAs);

  m_file_save_all = new QAction(tr("Save A&ll"), this);
  m_file_save_all->setShortcut(tr("Ctrl_Shift+l"));
  m_file_save_all->setStatusTip(tr("Save all open file(s)."));
  connect(m_file_save_all, &QAction::triggered, this, &MainWindow::fileSaveAs);

  m_file_exit = new QAction(tr("E&xit"), this);
  m_file_exit->setShortcut(QKeySequence::Quit);
  m_file_exit->setStatusTip(tr("Quit the application."));
  connect(m_file_exit, &QAction::triggered, this, &MainWindow::fileExit);
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
  m_edit_undo = new QAction(tr("&Undo"), this);
  m_edit_undo->setShortcut(QKeySequence::Undo);
  m_edit_undo->setStatusTip(tr("Undo last change."));
  connect(m_edit_undo, &QAction::triggered, this, &MainWindow::editUndo);

  m_edit_redo = new QAction(tr("&Redo"), this);
  m_edit_redo->setShortcut(QKeySequence::Redo);
  m_edit_redo->setStatusTip(tr("Redo last Undone change."));
  connect(m_edit_redo, &QAction::triggered, this, &MainWindow::editRedo);

  m_edit_copy = new QAction(tr("&Copy"), this);
  m_edit_copy->setShortcut(QKeySequence::Copy);
  m_edit_copy->setStatusTip(tr("Copy selected text."));
  connect(m_edit_copy, &QAction::triggered, this, &MainWindow::editCopy);

  m_edit_cut = new QAction(tr("Cu&t"), this);
  m_edit_cut->setShortcut(QKeySequence::Cut);
  m_edit_cut->setStatusTip(tr("Cut selected text."));
  connect(m_edit_cut, &QAction::triggered, this, &MainWindow::editCut);

  m_edit_paste = new QAction(tr("&Paste"), this);
  m_edit_paste->setShortcut(QKeySequence::Paste);
  m_edit_paste->setStatusTip(tr("paste selected text."));
  connect(m_edit_paste, &QAction::triggered, this, &MainWindow::editPaste);

  m_edit_paste_history = new QAction(tr("Paste from clipboard history"), this);
  m_edit_paste_history->setShortcut(tr("Ctrl+Shift+V"));
  m_edit_paste_history->setStatusTip(
    tr("Paste from a previous clipboard selection."));
  connect(
    m_edit_paste_history, &QAction::triggered, this, &MainWindow::editCopy);

  m_edit_options = new QAction(tr("Preferences"), this);
  m_edit_options->setShortcut(QKeySequence::Preferences);
  m_edit_options->setStatusTip(tr("Modify preferences."));
  connect(m_edit_options, &QAction::triggered, this, &MainWindow::editOptions);

  m_edit_spellcheck = new QAction(tr("Spellcheck"), this);
  //  m_editoptions->setShortcut(QKeySequence::SelectPrevious);
  m_edit_options->setStatusTip(tr("Spellcheck Current Document."));
  connect(
    m_edit_options, &QAction::triggered, this, &MainWindow::editSpellcheck);

  m_edit_highlight_misspelled =
    new QAction(tr("Highlight Misspelled Words"), this);
  //  m_editoptions->setShortcut(QKeySequence::Preferences);
  //  m_editoptions->setStatusTip(tr("Modify preferences."));
  connect(
    m_edit_options, &QAction::triggered, this, &MainWindow::editHighlightWords);

  m_edit_next_misspelled = new QAction(tr("Next Misspelled Word"), this);
  //  m_editoptions->setShortcut(QKeySequence::Preferences);
  m_edit_options->setStatusTip(tr("Modify preferences."));
  connect(m_edit_options, &QAction::triggered, this, &MainWindow::editNextWord);

  m_edit_send_to_booklist =
    new QAction(tr("Send Word to Book Dictionary"), this);
  //  m_editoptions->setShortcut(QKeySequence::Preferences);
  m_edit_options->setStatusTip(
    tr("This allows you to create a dictionary specifically for this book."));
  connect(
    m_edit_options, &QAction::triggered, this, &MainWindow::editSendToBookList);

  m_edit_send_to_authorlist =
    new QAction(tr("Send Word to Author Dictionary"), this);
  //  m_editoptions->setShortcut(QKeySequence::Preferences);
  m_edit_options->setStatusTip(
    tr("This allows you to create a dictionary specifically for this Author. "
       "Generally you would use this for an Authors Series where the same, "
       "possibly name or non-standard word might be used in muyltiple books."));
  connect(m_edit_options,
          &QAction::triggered,
          this,
          &MainWindow::editSendToAuthorList);
}

void
MainWindow::initEditorActions()
{
  QPixmap editor_icon(":/icons/editor");
  QPixmap code_icon(":/icons/code");
  QPixmap meta_icon(":/icons/metadata");

  m_open_editor = new QAction(editor_icon, tr("Open Editor"), this);
  m_open_editor->setStatusTip(tr("Switches to the Editor Window."));
  connect(m_open_editor, &QAction::triggered, this, &MainWindow::openWindow);

  m_open_codeeditor = new QAction(code_icon, tr("Open Code Editor"), this);
  m_open_codeeditor->setStatusTip(tr("Switches to the Code Editor Window."));
  connect(
    m_open_codeeditor, &QAction::triggered, this, &MainWindow::openWindow);

  m_open_metadata = new QAction(meta_icon, tr("Open Metadata editor"), this);
  m_open_metadata->setStatusTip(tr("Switches to the Metadata Editor Window."));
  connect(m_open_metadata, &QAction::triggered, this, &MainWindow::openWindow);
}

void
MainWindow::initHelpActions()
{
  m_help_contents = new QAction(tr("Contents"), this);
  m_help_contents->setStatusTip(tr("Access Help Contents."));
  connect(
    m_help_contents, &QAction::triggered, this, &MainWindow::helpContents);

  m_help_index = new QAction(tr("Index"), this);
  m_help_index->setStatusTip(tr("Access Help Index."));
  connect(m_help_index, &QAction::triggered, this, &MainWindow::helpIndex);

  m_help_context = new QAction(tr("Context Help"), this);
  m_help_context->setStatusTip(tr("Context Sensitive help."));
  connect(m_help_context, &QAction::triggered, this, &MainWindow::helpContext);

  m_help_about_ebookeditor = new QAction(tr("About EBookEditor"), this);
  m_help_about_ebookeditor->setStatusTip(tr("Information about EBookEditor."));
  connect(m_help_about_ebookeditor,
          &QAction::triggered,
          this,
          &MainWindow::helpAboutEbookEditor);

  m_help_about_plugins = new QAction(tr("About Plugins"), this);
  m_help_about_plugins->setStatusTip(
    tr("Information about available Plugins."));
  connect(m_help_about_plugins,
          &QAction::triggered,
          this,
          &MainWindow::helpAboutPlugins);

  m_help_check_updates = new QAction(tr("Check for Updates"), this);
  m_help_contents->setStatusTip(tr("Access Help Contents."));
  connect(
    m_help_contents, &QAction::triggered, this, &MainWindow::helpCheckUpdates);
}

void
MainWindow::initActions()
{
  initFileActions();
  initEditActions();
  initWindowActions();
  initEditorActions();
  initHelpActions();
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
MainWindow::initToolbar()
{
  QToolBar* toolbar = addToolBar("main toolbar");
  toolbar->addAction(m_open_editor);
  toolbar->addAction(m_open_codeeditor);
  toolbar->addAction(m_open_metadata);
  toolbar->setIconSize(QSize(64, 64));
}

void
MainWindow::saveOptions()
{
  QFile file(m_config_file);
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
        emitter << YAML::Key << COPY_BOOKS_TO_STORE;
        emitter << YAML::Value << m_options->copy_books_to_store;
        emitter << YAML::Key << DELETE_OLD_BOOK;
        emitter << YAML::Value << m_options->delete_old_book;
        emitter << YAML::Key << PREF_BOOKLIST;
        { // Start of PREF_BOOKLIST
          emitter << YAML::BeginSeq;
          foreach (QString book, m_options->current_files) {
            emitter << book;
          }
          emitter << YAML::EndSeq;
        } // End of PREF_BOOKLIST
        emitter << YAML::Key << PREF_LIBRARY;
        { // Start of PREF_LIBRARY
          emitter << YAML::BeginSeq;
          foreach (QString book, m_options->current_lib_files) {
            emitter << book;
          }
          emitter << YAML::EndSeq;
        } // End of PREF_LIBRARY
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
MainWindow::loadLibraryFiles()
{
  if (!m_options->current_lib_files.empty()) {
    foreach (QString filename, m_options->current_lib_files) {
      filename = m_data_directory + QDir::separator() + filename;
      QFile file(filename);
      if (file.exists()) {
        loadDocument(filename);
      }
      if (m_options->currentindex >= 0 &&
          m_tabs->count() > m_options->currentindex) {
        m_tabs->setCurrentIndex(m_options->currentindex);
      } else {
        m_tabs->setCurrentIndex(0);
      }
    }
  }
}

void
MainWindow::loadNonLibraryFiles()
{
  if (!m_options->current_files.empty()) {
    foreach (QString filename, m_options->current_files) {
      QFile file(filename);
      if (file.exists()) {
        loadDocument(filename);
      }
      if (m_options->currentindex >= 0 &&
          m_tabs->count() > m_options->currentindex) {
        m_tabs->setCurrentIndex(m_options->currentindex);
      } else {
        m_tabs->setCurrentIndex(0);
      }
    }
  }
}

void
MainWindow::loadOptions()
{
  QFile file(m_config_file);
  if (file.exists()) {
    m_preferences = YAML::LoadFile(file);
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
    // Copy edited books to a book store.
    if (m_preferences[COPY_BOOKS_TO_STORE]) {
      m_options->copy_books_to_store =
        m_preferences[COPY_BOOKS_TO_STORE].as<bool>();
    } else {
      m_options->copy_books_to_store = true;
    }
    // Copy edited books to a book store.
    if (m_preferences[DELETE_OLD_BOOK]) {
      m_options->delete_old_book = m_preferences[DELETE_OLD_BOOK].as<bool>();
    } else {
      m_options->delete_old_book = false;
    }
    // Last books loaded in library.
    YAML::Node library = m_preferences[PREF_LIBRARY];
    if (library && library.IsSequence()) {
      m_options->current_lib_files.clear(); // Empty list just in case.
      for (uint i = 0; i < library.size(); i++) {
        m_options->current_lib_files.append(library[i].as<QString>());
      }
    }
    loadLibraryFiles();

    // Last books loaded NOT in library.
    YAML::Node books = m_preferences[PREF_BOOKLIST];
    if (books && books.IsSequence()) {
      m_options->current_files.clear(); // Empty list just in case.
      for (uint i = 0; i < books.size(); i++) {
        m_options->current_files.append(books[i].as<QString>());
      }
    }
    loadNonLibraryFiles();

    //    m_options->currentfiles = m_preferences["book
    //    list"].as<QStringList>();
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
  } else {
    m_options->rect = QRect(DEF_X, DEF_Y, DEF_WIDTH, DEF_HEIGHT);
    m_options->currentindex = -1;
    m_options->current_files = QStringList();
    m_options->bookcount = 0;
    m_prefchanged = true;
  }
}

void
MainWindow::initSetup()
{
  m_prefchanged = false;
  loadOptions();
  setGeometry(m_options->rect);
}

void
MainWindow::copyBookToStore(QString path, QString authors)
{
  QString newpath = m_data_directory + QDir::separator();
  QDir dir;
  QString filename;
  QFile in_file(path);
  QFileInfo fileInfo(in_file);

  if (authors.isEmpty()) {
    QString temp("temp");
    dir.mkpath(newpath + temp);
    filename = temp + QDir::separator() + fileInfo.fileName();
  } else {
    dir.mkpath(newpath + authors);
    filename = authors + QDir::separator() + fileInfo.fileName();
  }

  QFile out_file(newpath + filename);
  if (in_file.exists() && !out_file.exists()) {
    in_file.copy(out_file.fileName());
    m_options->current_files.removeOne(path);
    m_options->current_lib_files.append(filename);
    //    if (m_options->delete_old_book) {
    //      if (!m_options->never_confirm_delete) {
    //        QCheckBox* neverAskBox =
    //          new QCheckBox(tr("Never ask this again"), this);
    //        neverAskBox->setToolTip(
    //          tr("If this is checked then this dialog will not be "
    //             "shown again. This can be reset in the Options->Editor "
    //             "dialog."));

    //        QMessageBox* msgBox = new QMessageBox(
    //          QMessageBox::Warning,
    //          tr("Delete Old Book"),
    //          tr("You are about to delete the old book, Are you sure?"),
    //          QMessageBox::Ok | QMessageBox::Cancel,
    //          this);
    //        msgBox->setCheckBox(neverAskBox);

    //        if (msgBox->exec() == QMessageBox::Accepted) {
    //          in_file.remove();
    //        }

    //        if (neverAskBox->isChecked()) {
    //          m_options->never_confirm_delete = true;
    //        }

    //      } else {
    //        in_file.remove();
    //      }
    //    }
  }
}

SharedAuthorList
MainWindow::selectAuthorNames(QString filename, EBookData* data)
{
  SharedAuthorList authors;
  AuthorDialog* authorDlg;
  // TODO no author in metadata. Create it?
  // First see if the author name is in the title.
  QStringList names = attemptToExtractAuthorFromFilename(filename, data);
  if (!names.isEmpty()) {
    authorDlg = new AuthorDialog(m_database, this);
    if (authorDlg->exec(AuthorDialog::FromTitle, data->title, names) ==
        QDialog::Accepted) {
      authors = authorDlg->authors();
    }
  } else {
    authorDlg = new AuthorDialog(m_database, this);
    if (authorDlg->exec(AuthorDialog::NoNames, data->title) ==
        QDialog::Accepted) {
      authors = authorDlg->authors();
    }
  }
  return authors;
}

void
MainWindow::loadDocument(QString filename)
{
  if (!m_options->current_files.contains(filename)) {
    m_options->current_files.append(filename);

    EBookWrapper* wrapper = new EBookWrapper(m_options, this);

    EBookDocument* htmldocument = createDocument(filename);
    EBookDocument* codeDocument = createDocument(htmldocument);
    wrapper->editor()->setDocument(htmldocument);
    wrapper->codeEditor()->setDocument(codeDocument);
    //  wrapper->startWordReader();
    connect(this,
            &MainWindow::codeChanged,
            wrapper,
            &EBookWrapper::optionsHaveChanged);
    EBookData* data = htmldocument->data();
    SharedAuthorList authors;
    QString authors_name;

    if (m_options->copy_books_to_store) {
      QStringList authorlist = data->authors;
      if (authorlist.isEmpty()) {
        authors = selectAuthorNames(filename, data);
        wrapper->metaEditor()->addAuthors(authors);
        authors_name = concatenateAuthorNames(authors);
        copyBookToStore(filename, authors_name);
      } else {
        authors_name = concatenateAuthorNames(authorlist);
        copyBookToStore(filename, authors_name);
      }
    }

    if (!htmldocument->title().isEmpty()) {
    }

    CountryData* country_data = m_dict_data[data->language];
    if (!country_data) {
      if (m_current_spell_checker) {
        QStringList codes =
          m_current_spell_checker->compatibleLanguageCodes(data->language);
        foreach (QString code, codes) {
          // TODO
        }
      }
    }
    //    country_data = m_dict_data;

    QString tabname = QString(tr("%1, (%2)")
                                .arg(htmldocument->title())
                                .arg(htmldocument->authorNames()));
    m_tabs->addTab(wrapper, tabname);
    m_current_document =
      qobject_cast<EBookDocument*>(wrapper->editor()->document());
  }
}

QString
MainWindow::concatenateAuthorNames(SharedAuthorList authors)
{
  QStringList names;
  foreach (SharedAuthor author, authors) {
    QStringList f;
    f << author->forename().trimmed() << author->middlenames().trimmed()
      << author->surname().trimmed();
    f = removeEmpty(f);
    QString n = f.join("_");
    names << n;
  }

  return names.join(", ");
}

QString
MainWindow::concatenateAuthorNames(QStringList authors)
{
  QStringList names;
  foreach (QString name, authors) {
    QStringList splits =
      name.split(QRegularExpression("[\\s.]"), QString::SkipEmptyParts);
    QStringList f;
    foreach (QString split, splits) {
      f << split;
    }
    f = removeEmpty(f);
    QString n = f.join("_");
    names << n;
  }
  return names.join(", ");
}

QStringList
MainWindow::removeEmpty(QStringList values)
{
  QStringList result;
  foreach (QString s, values) {
    if (s.isEmpty())
      continue;
    else
      result << s;
  }
  return result;
}

QStringList
MainWindow::attemptToExtractAuthorFromFilename(QString path, EBookData* data)
{
  QFile in_file(path);
  QFileInfo fileInfo(in_file);
  QString filename = fileInfo.fileName();
  filename = filename.left(filename.lastIndexOf("."));

  // This removes case worries.
  QString lower_filename(
    filename.toLower()); // the lowercase version of the filename.
  QString lower_title(
    data->title.toLower()); // the lowercase version of the title.
  QString remainder;

  int title_index;
  if (lower_filename.contains(lower_title)) {
    title_index = lower_filename.indexOf(lower_title);
    remainder = filename.left(title_index) +
                filename.right(lower_filename.length() -
                               (title_index + lower_title.length()));
  }
  remainder = cleanString(remainder);
  QStringList list =
    remainder.split(QRegularExpression("[\\s.]"), QString::SkipEmptyParts);
  return list;
}

QString
MainWindow::cleanString(QString unclean)
{
  QString clean = unclean;
  clean.replace('_', ' '); // replaces _ with spaces
  clean.replace('-', ' '); // replaces - with spaces
  clean.remove(
    QRegularExpression( // removes all sorts of other nonword characters
      QString::fromUtf8("[-`~!@#$%^&*()_—+=|:;<>«»,.?/{}\'\"\\[\\]\\\\]")));
  clean = clean.simplified(); // removes multiple spaces
  clean = clean.trimmed();    // removes pre and post spaces.
  return clean;
}
void
MainWindow::saveDocument(EBookDocument* document)
{
  // TODO save the document
}

void
MainWindow::documentChanged(int index)
{
  if (index >= 0) {
    m_options->currentindex = index;
    EBookWrapper* wrapper = qobject_cast<EBookWrapper*>(m_tabs->widget(index));
    EBookDocument* document =
      qobject_cast<EBookDocument*>(wrapper->editor()->document());
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

    if (document) {
      m_current_document = document;
      QString language = m_current_document->language();
      QLocale local(language);

    } else {
      m_current_document = Q_NULLPTR;
    }
  } else {
    // TODO - no documents selected.
  }
}

void
MainWindow::tabClosing(int index)
{
  EBookWrapper* wrapper = qobject_cast<EBookWrapper*>(m_tabs->widget(index));
  EBookDocument* document =
    qobject_cast<EBookDocument*>(wrapper->editor()->document());
  if (document->isModified()) {
    // TODO - Check if the user wants to save the document.
    saveDocument(document);
  }
  m_tabs->removeTab(index);

  wrapper = qobject_cast<EBookWrapper*>(m_tabs->currentWidget());
  document = qobject_cast<EBookDocument*>(wrapper->codeEditor()->document());
  if (document) {
    m_current_document = document;
  } else {
    m_current_document = Q_NULLPTR;
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
  m_bookcount = m_options->current_files.size();
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
MainWindow::editSpellcheck()
{
  // TDOD
}

void
MainWindow::editHighlightWords()
{
  // TODO
}

void
MainWindow::editNextWord()
{
  // TODO
}

void
MainWindow::editSendToBookList()
{
  // TODO
}

void
MainWindow::editSendToAuthorList()
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
MainWindow::helpContents()
{
  // TODO
}

void
MainWindow::helpIndex()
{
  // TODO
}

void
MainWindow::helpContext()
{
  // TODO
}

void
MainWindow::helpAboutEbookEditor()
{
  // TODO
}

void
MainWindow::helpAboutPlugins()
{
  PluginDialog* dlg = new PluginDialog(this);
  foreach (IEBookInterface* interface, m_plugins) {
    BaseEBookInterfaceClass* base =
      dynamic_cast<BaseEBookInterfaceClass*>(interface);
    dlg->addPlugin(base);
  }
  dlg->open();
}

void
MainWindow::helpCheckUpdates()
{
  // TODO
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
MainWindow::openWindow()
{
  QObject* obj = sender();
  QAction* act = qobject_cast<QAction*>(obj);
  EBookWrapper* wrapper = qobject_cast<EBookWrapper*>(m_tabs->currentWidget());
  if (wrapper) {
    if (act == m_open_editor) {
      wrapper->setToEditor();
    } else if (act == m_open_codeeditor) {
      wrapper->setToCode();
    } else if (act == m_open_metadata) {
      wrapper->setToMetadata();
    }
  }
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
MainWindow::createDocument(EBookDocument* doc)
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

void
MainWindow::loadPlugins()
{

  QDir pluginsDir = QDir(qApp->applicationDirPath());
  pluginsDir.cd("plugins");

  foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
    QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
    QObject* plugin = loader.instance();

    if (plugin) {
      IEBookInterface* interface = qobject_cast<IEBookInterface*>(plugin);
      if (interface) {
        interface->buildMenu();
        // Add plugin to list of ALL plugins.
        m_plugins.append(interface);

        ISpellInterface* spellchecker = qobject_cast<ISpellInterface*>(plugin);
        // Add spellcheckers to list of spellcheckers;
        m_spellchecker_plugins.insert(interface->name(), spellchecker);

        //      m_languages = m_current_spell_checker->languageCodes();
        //      foreach (QString lang, m_languages) {
        //        CountryData* data = m_current_spell_checker->dictionary(lang);
        //        m_dict_data.insert(lang, data);
        //        m_dict_paths.insert(lang, data->path);
        //      }
      }
    }
  }
  // TODO handle more than one spellchecker plugin.
  m_current_spell_checker = m_spellchecker_plugins["HunSpell"];
}
