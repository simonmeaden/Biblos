#include "mainwindow.h"

#include <qlogger/qlogger.h>

#include "iebookinterface.h"
#include "iplugininterface.h"
#include "ispellinterface.h"

#include "iebookdocument.h"

#include "ebookcodeeditor.h"
#include "ebookeditor.h"

#include "ebooktoceditor.h"
#include "ebooktocwidget.h"
#include "ebookwordreader.h"
#include "ebookwrapper.h"
#include "libraryframe.h"

#include "aboutdialog.h"
#include "authordialog.h"
#include "libraryframe.h"
#include "optionsdialog.h"
#include "plugindialog.h"

using namespace qlogger;

const QString MainWindow::READ_ONLY = MainWindow::tr("Read Only");
const QString MainWindow::READ_WRITE = MainWindow::tr("Read Write");
const QString MainWindow::NO_FILE = MainWindow::tr("No File");
const QString MainWindow::NOT_MODIFIED = MainWindow::tr("Not Modified");
const QString MainWindow::MODIFIED = MainWindow::tr("Modified");

const QString MainWindow::PREF_FILE = "preferences.yaml";
const QString MainWindow::LIB_FILE = "library.yaml";
const QString MainWindow::AUTHOR_FILE = "authors.yaml";
const QString MainWindow::SERIES_FILE = "series.yaml";
// const QString MainWindow::DB_NAME = "library.sqlite";

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_initialising(true)
  , m_loading(false)
  , m_options(new Options(this))
  , m_bookcount(0)
  , m_popup(nullptr)
  , m_current_spell_checker(nullptr)
{
  QLogger::addLogger("root", q5TRACE, CONSOLE);

  setWindowTitle(QCoreApplication::applicationName());
  QPixmap library_icon;
  QPixmapCache::find(m_options->lib_key, &library_icon);
  setWindowIcon(QIcon(library_icon));

  m_authors_db = AuthorsDB(new EBookAuthorsDB());
  m_series_db = SeriesDB(new EBookSeriesDB());
  m_library_db = LibraryDB(new EBookLibraryDB(m_series_db));

  connect(
    m_options, &Options::loadLibraryFiles, this, &MainWindow::loadLibraryFiles);

  loadPlugins();
  initBuild();
  QDir dir;
  m_options->setHomeDirectiory(QStandardPaths::locate(
    QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory));
  m_options->setLibraryDirectory(
    QStandardPaths::writableLocation(QStandardPaths::DataLocation));
  dir.mkpath(m_options->libraryDirectory());
  m_options->setConfigDirectory(
    QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
  dir.mkpath(m_options->configDirectory());
  m_options->setConfigFile(m_options->configDirectory() + QDir::separator() +
                           PREF_FILE);
  m_options->setLibraryFile(m_options->configDirectory() + QDir::separator() +
                            LIB_FILE);
  m_options->setAuthorsFile(m_options->configDirectory() + QDir::separator() +
                            AUTHOR_FILE);
  m_options->setSeriesFile(m_options->configDirectory() + QDir::separator() +
                           SERIES_FILE);
  loadAuthors();
  loadSeries();
  loadLibrary();

  //  /* The database file will be created automatically by SqLite if it
  //   * does not exist. The tables are created if they do not already exist.
  //   */
  //  QString db_path = m_config_directory + QDir::separator() + DB_NAME;
  //  m_database = new DbManager(db_path, parent);

  initSetup();

  m_initialising = false;
}

MainWindow::~MainWindow() {}

void
MainWindow::resizeEvent(QResizeEvent* e)
{
  QSize size = e->size();
  m_options->rect().setSize(size);
  if (!m_initialising)
    saveOptions();
}

void
MainWindow::moveEvent(QMoveEvent* e)
{
  QPoint pos = e->pos();
  QRect rect = m_options->rect();
  rect.setTopLeft(pos);
  m_options->setRect(rect);
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
MainWindow::update(Options::TocPosition position)
{
  if (position == Options::LEFT) {
    m_splitter->insertWidget(0, m_toc_stack);
    m_splitter->insertWidget(1, m_doc_tabs);
    m_splitter->setStretchFactor(0, 1);
    m_splitter->setStretchFactor(1, 3);
  } else if (position == Options::RIGHT) {
    m_splitter->insertWidget(0, m_doc_tabs);
    m_splitter->insertWidget(1, m_toc_stack);
    m_splitter->setStretchFactor(0, 3);
    m_splitter->setStretchFactor(1, 1);
  }
  QWidget::update();
}

void
MainWindow::initGui()
{
  //  QFrame* mainFrame = new QFrame(this);
  //  setCentralWidget(mainFrame);

  m_doc_stack = new QStackedWidget(this);
  connect(m_doc_stack,
          &QStackedWidget::currentChanged,
          this,
          &MainWindow::setObjectVisibility);
  setCentralWidget(m_doc_stack);

  m_library_frame = new LibraryFrame(m_options, this);
  // These could not be created when the action was as l_library was still null.
  connect(m_library_shelf,
          &QAction::triggered,
          m_library_frame,
          &LibraryFrame::setToShelf);
  connect(m_library_shelf,
          &QAction::triggered,
          this,
          &MainWindow::setLibraryToolbarState);
  connect(m_library_tree,
          &QAction::triggered,
          m_library_frame,
          &LibraryFrame::setToTree);
  connect(m_library_tree,
          &QAction::triggered,
          this,
          &MainWindow::setLibraryToolbarState);
  m_stack_library = m_doc_stack->addWidget(m_library_frame);

  m_tabs_frame = new QFrame(this);
  QHBoxLayout* main_layout = new QHBoxLayout;
  m_tabs_frame->setLayout(main_layout);
  m_stack_editor = m_doc_stack->addWidget(m_tabs_frame);

  m_toc_position = m_options->tocPosition();

  m_splitter = new QSplitter(Qt::Horizontal, this);
  main_layout->addWidget(m_splitter);

  m_toc_stack = new QStackedWidget(this);

  m_doc_tabs = new QTabWidget(this);
  m_doc_tabs->setTabsClosable(true);
  connect(m_doc_tabs,
          &QTabWidget::currentChanged,
          this,
          &MainWindow::documentChanged);
  connect(
    m_doc_tabs, &QTabWidget::tabCloseRequested, this, &MainWindow::tabClosing);
  if (m_doc_tabs->count() == 0)
    m_doc_tabs->setEnabled(false);

  if (m_toc_position == Options::LEFT) {
    m_splitter->addWidget(m_toc_stack);
    m_splitter->addWidget(m_doc_tabs);
    m_splitter->setStretchFactor(0, 1);
    m_splitter->setStretchFactor(1, 3);
  } else {
    m_splitter->addWidget(m_doc_tabs);
    m_splitter->addWidget(m_toc_stack);
    m_splitter->setStretchFactor(0, 3);
    m_splitter->setStretchFactor(1, 1);
  }
  setLibraryToolbarState();
}

void
MainWindow::initFileMenu()
{
  m_filemenu = menuBar()->addMenu(tr("&File"));
  m_filemenu->addAction(m_file_new);
  m_filemenu->addAction(m_file_open);
  m_filemenu->addSeparator();
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
MainWindow::initViewMenu()
{
  m_view_menu = menuBar()->addMenu(tr("&View"));
  m_view_menu->addAction(m_view_maximise);
  m_view_menu->addAction(m_view_minimise);
  m_view_menu->addAction(m_view_fullscreen);
  m_view_menu->addAction(m_view_centre);
  m_view_menu->addSeparator();
  m_view_menu->addAction(m_show_library);
  m_view_menu->addAction(m_show_editor);
  m_view_menu->addAction(m_library_tree);
  m_view_menu->addAction(m_library_shelf);
  m_view_menu->addSeparator();
  m_view_menu->addAction(m_view_toc);
  m_view_menu->addAction(m_view_toc_position);
}

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
  initViewMenu();
  initHelpMenu();
}

void
MainWindow::initFileActions()
{
  QPixmap open_icon, save_icon;
  QPixmapCache::find(m_options->open_key, &open_icon);
  QPixmapCache::find(m_options->save_key, &save_icon);

  m_file_new = new QAction(open_icon, tr("&New"), this);
  //    m_fileopen->setCheckable(true);
  m_file_new->setShortcut(QKeySequence::New);
  m_file_new->setStatusTip(tr("Create a new empty file."));
  connect(m_file_new, &QAction::triggered, this, &MainWindow::fileNew);

  m_file_open = new QAction(open_icon, tr("&Open"), this);
  //    m_fileopen->setCheckable(true);
  m_file_open->setShortcut(QKeySequence::Open);
  m_file_open->setStatusTip(tr("Open a new file."));
  connect(m_file_open, &QAction::triggered, this, &MainWindow::fileOpen);

  m_file_save = new QAction(save_icon, tr("&Save"), this);
  m_file_save->setShortcut(QKeySequence::Save);
  m_file_save->setStatusTip(tr("Save the current file."));
  //  m_file_save->setEnabled(false);
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
MainWindow::initViewActions()
{
  m_screengrp = new QActionGroup(this);

  m_view_maximise = new QAction(tr("Maximise screen"), this);
  m_screengrp->addAction(m_view_maximise);
  m_view_maximise->setShortcut(tr("Alt+X"));
  m_view_maximise->setStatusTip(
    tr("Set screen size to full screen with title bar."));
  connect(
    m_view_maximise, &QAction::triggered, this, &MainWindow::viewMaximise);

  m_view_minimise = new QAction(tr("Minimise screen"), this);
  m_screengrp->addAction(m_view_minimise);
  m_view_minimise->setShortcut(tr("Alt+N"));
  m_view_minimise->setStatusTip(
    tr("Set screen size to minimised screen with title bar."));
  connect(
    m_view_minimise, &QAction::triggered, this, &MainWindow::viewMinimise);
  m_view_minimise->setChecked(true);

  m_view_fullscreen = new QAction(tr("Full screen"), this);
  m_screengrp->addAction(m_view_fullscreen);
  m_view_fullscreen->setShortcut(QKeySequence::FullScreen);
  m_view_fullscreen->setStatusTip(
    tr("Set screen size to full screen without title bar."));
  connect(
    m_view_fullscreen, &QAction::triggered, this, &MainWindow::viewFullscreen);

  m_view_centre = new QAction(tr("Centre screen"), this);
  m_view_centre->setShortcut(QKeySequence::FullScreen);
  m_view_centre->setStatusTip(tr("Adjust the screen so that it is centred, "
                                 "only available for minimised screens."));
  connect(m_view_centre, &QAction::triggered, this, &MainWindow::viewCentre);

  QString text;
  if (m_options->tocVisible()) {
    text = tr("Hide Table of Contents");
  } else {
    text = tr("Show Table of Contents");
  }
  m_view_toc = new QAction(text, this);
  m_view_toc->setShortcut(tr("Alt+T"));
  m_view_toc->setStatusTip(tr("Show/Hide the table of contents viewer."));
  connect(m_view_toc, &QAction::triggered, this, &MainWindow::viewToc);

  if (m_options->tocPosition() == Options::LEFT) {
    text = tr("Move Table of Contents to Right");
  } else {
    text = tr("Move Table of Contents to Left");
  }
  m_view_toc_position = new QAction(text, this);
  m_view_toc_position->setShortcut(tr("Alt+P"));
  m_view_toc_position->setStatusTip(
    tr("Set the Table of Contents viewer position."));
  connect(m_view_toc_position,
          &QAction::triggered,
          this,
          &MainWindow::viewTocPosition);
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
  QPixmap editor_icon, code_icon, meta_icon;

  QPixmapCache::find(m_options->editor_key, &editor_icon);
  QPixmapCache::find(m_options->code_key, &code_icon);
  QPixmapCache::find(m_options->meta_key, &meta_icon);

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
MainWindow::initLibActions()
{
  QPixmap library_icon, bookshelf_icon, tree_icon, editor_icon;

  QPixmapCache::find(m_options->editor_key, &editor_icon);
  QPixmapCache::find(m_options->lib_key, &library_icon);
  QPixmapCache::find(m_options->bookshelf_key, &bookshelf_icon);
  QPixmapCache::find(m_options->tree_key, &tree_icon);

  Options::ViewState state = m_options->viewState();

  m_show_library = new QAction(library_icon, tr("Show Library"), this);
  m_show_library->setStatusTip(tr("Shows the library page."));
  connect(
    m_show_library, &QAction::triggered, this, &MainWindow::viewShowLibrary);

  m_show_editor = new QAction(editor_icon, tr("Show Editor"), this);
  m_show_editor->setStatusTip(tr("Shows the editor page."));
  connect(
    m_show_editor, &QAction::triggered, this, &MainWindow::viewShowEditor);

  m_library_tree = new QAction(tree_icon, tr("Library Tree"), this);
  m_library_tree->setStatusTip(tr("Sets the library page to tree format."));
  //  connect(m_library_tree, &QAction::triggered, m_library,
  //  &LibraryFrame::setToTree);

  m_library_shelf = new QAction(bookshelf_icon, tr("Library Shelf"), this);
  m_library_shelf->setStatusTip(tr("Sets the library page to shelf format."));
  //  connect(m_library_shelf, &QAction::triggered, m_library,
  //  &LibraryFrame::setToShelf);

  setObjectVisibility(state);
}

void
MainWindow::initActions()
{
  initFileActions();
  initEditActions();
  initViewActions();
  initEditorActions();
  initHelpActions();
  initLibActions();
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
  m_lib_toolbar = addToolBar("library toolbar");
  m_lib_toolbar->addAction(m_show_library);
  m_lib_toolbar->addAction(m_show_editor);
  m_show_editor->setVisible(false);
  m_lib_toolbar->setIconSize(QSize(48, 48));

  m_lib_type_toolbar = addToolBar("library type toolbar");
  m_lib_type_toolbar->addAction(m_library_tree);
  m_lib_type_toolbar->addAction(m_library_shelf);
  m_lib_type_toolbar->setVisible(true);
  m_lib_type_toolbar->setIconSize(QSize(48, 48));

  m_file_toolbar = addToolBar("file toolbar");
  m_file_toolbar->addAction(m_file_open);
  m_file_toolbar->addAction(m_file_save);
  // TODO maybe reinstate this ?  m_file_save->setVisible(false);
  m_file_toolbar->setIconSize(QSize(48, 48));

  m_editor_toolbar = addToolBar("editor toolbar");
  m_editor_toolbar->addAction(m_open_editor);
  m_editor_toolbar->addAction(m_open_codeeditor);
  m_editor_toolbar->addAction(m_open_metadata);
  m_editor_toolbar->setIconSize(QSize(48, 48));
}

void
MainWindow::loadLibraryFiles(QStringList current_lib_files, int currentindex)
{
  if (!current_lib_files.empty()) {
    foreach (QString filename, current_lib_files) {
      //      filename = filename;
      QFile file(filename);
      if (file.exists()) {
        loadDocument(filename, true);
      }
      if (currentindex >= 0 && m_doc_tabs->count() > currentindex) {
        m_doc_tabs->setCurrentIndex(currentindex);
      } else {
        m_doc_tabs->setCurrentIndex(0);
      }
    }
  }
}

EBookDocumentType
MainWindow::checkMimetype(QString filename)
{
  QMimeDatabase db;
  QMimeType mime = db.mimeTypeForFile(filename, QMimeDatabase::MatchDefault);

  if (mime.name() == "application/epub+zip")
    return EPUB;
  // TODO add further mime types.
  return UNSUPPORTED_TYPE;
}

void
MainWindow::loadOptions()
{
  m_options->load(m_options->configFile());
  setObjectVisibility();
}

void
MainWindow::saveOptions()
{
  m_options->save(m_options->configFile());
}

void
MainWindow::loadLibrary()
{
  m_library_db->load(m_options->libraryFile());
}

void
MainWindow::saveLibrary()
{
  m_library_db->save();
}

void
MainWindow::loadAuthors()
{
  m_authors_db->load(m_options->authorsFile());
}

void
MainWindow::saveSeries()
{
  m_series_db->save();
}

void
MainWindow::loadSeries()
{
  m_series_db->load(m_options->seriesFile());
}

void
MainWindow::saveAuthors()
{
  m_authors_db->save();
}

void
MainWindow::initSetup()
{
  loadOptions();
  setGeometry(m_options->rect());
}

/*!
 * \brief Copies a book to the library.
 *
 * The file will be copied to the library store if it does not already exist.
 * The directory within the library will be the space and comma separated
 * concatenation of the author(s) names. If it does exist a dialog will pop up
 * offering several options. \list \li Overwriting the library file. \li Saving
 * the file with a different name. \li Opening the Library file instead.
 * \endlist
 * In all cases the result will be true if the file was copied or the library
 * file used otherwise false will be returned.
 * The new file name will be copied into the filename parameter.
 *
 * \param path - the original file name.
 * \param authors - the autor(s) of the book.
 * \return true if the copy was successful.
 */
// bool MainWindow::copyBookToStore(QString& path, QString authors)
//{
//  QString newpath = m_library_directory + QDir::separator();
//  QDir dir;
//  QString filename;
//  QFile in_file(path);
//  QFileInfo in_file_info(in_file);
//  QString ext = in_file_info.suffix();
//  bool result = false;

//  if (authors.isEmpty()) {
//    QString temp("temp");
//    dir.mkpath(newpath + temp);
//    filename = temp + QDir::separator() + in_file_info.fileName();
//  } else {
//    dir.mkpath(newpath + authors);
//    filename = authors + QDir::separator() + in_file_info.fileName();
//  }

//  QFile out_file(newpath + filename);
//  QFileInfo out_file_info(out_file);
//  if (in_file.exists()) {
//    if (!out_file.exists()) {
//      in_file.copy(out_file.fileName());
//    } else {
//      QMessageBox* box = new QMessageBox(
//        QMessageBox::Warning, tr("Overwriting existing file!"),
//        tr("You are attempting to overwrite an existing library file.\n"
//           "Please Note :\n"
//           "If you choose Overwrite then the Library file will be\n"
//           "removed and this cannot be undone!\n"
//           "Are you sure?"),
//        QMessageBox::Open | QMessageBox::Save | QMessageBox::Cancel |
//        QMessageBox::Ok, this);
//      box->setDefaultButton(QMessageBox::Cancel);
//      box->setButtonText(QMessageBox::Open, tr("Use Library Version"));
//      box->setButtonText(QMessageBox::Save, tr("Save As.."));
//      box->setButtonText(QMessageBox::Ok, tr("Overwrite Existing Library
//      Version")); int btn = box->exec(); if (btn == QMessageBox::Ok) { //
//      Overwrite.
//        bool success = in_file.copy(out_file.fileName());
//        if (!success) { // file exists
//          out_file.remove();
//          in_file.copy(out_file.fileName());
//        }
//        filename = out_file.fileName();
//        result = true;
//      } else if (btn == QMessageBox::Open) { // Use Library Version.
//        filename = out_file.fileName();
//        result = false;
//      } else if (btn == QMessageBox::Save) { // Save As.
//        bool ok;
//        QString text =
//          QInputDialog::getText(this, tr("Save file as.."), tr("Enter the new
//          file name : "),
//                                QLineEdit::Normal, out_file_info.fileName(),
//                                &ok);
//        if (ok && !text.isEmpty()) {
//          QString path = out_file_info.filePath();
//          QString name = path + "/" + text + "." + ext;
//          out_file.setFileName(name);
//          bool success = in_file.copy(out_file.fileName());
//          if (!success) { // file exists
//            out_file.remove();
//            in_file.copy(out_file.fileName());
//          }
//          filename = out_file.fileName();
//          result = true;
//        }
//      }
//    }
//  }

//  if (result) { // Only delete the old file if the copy has been made.
//    if (m_options->deleteOldBook()) {
//      if (m_options->alwaysConfirmDelete()) {
//        QCheckBox* neverAskBox = new QCheckBox(tr("Never ask this again"),
//        this); neverAskBox->setToolTip(tr("If this is checked then this dialog
//        will not be "
//                                   "shown again. This can be reset in the
//                                   Options->Editor " "dialog."));

//        QMessageBox* msgBox =
//          new QMessageBox(QMessageBox::Warning, tr("Delete Old Book"),
//                          tr("You are about to delete the old book, Are you
//                          sure?"), QMessageBox::Ok | QMessageBox::Cancel,
//                          this);
//        msgBox->setCheckBox(neverAskBox);

//        if (msgBox->exec() == QMessageBox::Accepted) {
//          in_file.remove();
//        }

//        if (neverAskBox->isChecked()) {
//          m_options->setNeverConfirmDelete(true);
//        }

//      } else {
//        in_file.remove();
//      }
//      return true;
//    }
//  }

//  return result;
//}

/*!
 * \brief Gets a list of author data.
 *
 * First it attempts to extract the name(s) from the filename as this is
 * often in the form of "title : author name or names.extension" or similar
 * construct.
 *
 * If this fails it will pop up an author dialog that allows the
 * user to either select an author from the database or input author
 * information manually.
 *
 * \param filename - the filename od the book.
 * \param title - the title of the book
 * \return a QStringList containing the author(s) names.
 */
AuthorList
MainWindow::selectAuthorNames(QString filename, QString title)
{
  AuthorList authors;
  AuthorDialog* author_dlg;
  // TODO no author in metadata. Create it?
  // First see if the author name is in the title.
  QStringList names = attemptToExtractAuthorFromFilename(filename, title);
  if (!names.isEmpty()) {
    author_dlg = new AuthorDialog(m_options, m_authors_db, this);
    if (author_dlg->execute(AuthorDialog::FromTitle, title, names) ==
        QDialog::Accepted) {
      authors = author_dlg->authors();
    }
  } else {
    author_dlg = new AuthorDialog(m_options, m_authors_db, this);
    if (author_dlg->execute(AuthorDialog::NoNames, title) ==
        QDialog::Accepted) {
      authors = author_dlg->authors();
    }
  }
  return authors;
}

void
MainWindow::setModifiedAuthors(IEBookDocument* doc, AuthorList authors)
{
  QStringList names;
  foreach (AuthorData author, authors) {
    names << author->displayName();
  }
  //  doc->setCreators(names);
  setStatusModified();
}

IEBookDocument*
MainWindow::copyToLibraryAndOpen(QString& filename,
                                 IEBookInterface* ebook_plugin)
{
  IEBookDocument* ebook_document;
  // load the supplied ebook.
  ebook_document = ebook_plugin->createDocument(filename);
  QStringList author_list = ebook_document->creators();
  QString concat_authors;
  QString newpath = m_options->libraryDirectory() + QDir::separator();
  QDir dir;
  QFile in_file(filename);
  QFileInfo in_file_info(in_file);
  QString ext = in_file_info.suffix();
  is_in_temp_store = false;

  // if there is no author information attempt to get it elsewhere.
  if (author_list.isEmpty()) {
    AuthorList authors = selectAuthorNames(filename, ebook_document->title());
    setModifiedAuthors(ebook_document, authors);

    foreach (AuthorData author, authors) {
      author_list << author->displayName();
    }
  }

  if (author_list.isEmpty()) {
    // temporary library store.
    newpath += "temp";
    dir.mkpath(newpath);
    is_in_temp_store = true;
  } else {
    // authors directory name.
    concat_authors = ebook_document->creatorNames(author_list);
    newpath = newpath + "library" + QDir::separator() + concat_authors;
    dir.mkpath(newpath);
  }

  // The book will be stored in two forms, an original form and a modified form.
  QFile out_file(newpath + QDir::separator() + in_file_info.baseName() + "." +
                 in_file_info.suffix());
  QFile out_file_original(newpath + QDir::separator() +
                          in_file_info.baseName() + "." + "original." +
                          in_file_info.suffix());
  QFileInfo out_file_info(out_file);

  if (in_file.exists()) {
    if (!out_file.exists()) {
      in_file.copy(out_file.fileName());
      in_file.copy(out_file_original.fileName());
      filename = out_file.fileName();
    } else {
      QMessageBox* box = new QMessageBox(
        QMessageBox::Warning,
        tr("Overwriting existing file!"),
        tr("You are attempting to overwrite an existing library file.\n"
           "Please Note :\n"
           "If you choose Overwrite then the Library file will be\n"
           "removed and this cannot be undone!\n"
           "Are you sure?"),
        QMessageBox::Open | QMessageBox::Save | QMessageBox::Cancel |
          QMessageBox::Ok,
        this);
      box->setDefaultButton(QMessageBox::Open);
      box->setButtonText(QMessageBox::Open, tr("Use Library Version"));
      box->setButtonText(QMessageBox::Save, tr("Save As.."));
      box->setButtonText(QMessageBox::Ok,
                         tr("Overwrite Existing Library Version"));
      int btn = box->exec();
      if (btn == QMessageBox::Ok) { // Overwrite.
        bool success = in_file.copy(out_file.fileName());
        if (!success) { // file exists
          out_file.remove();
          in_file.copy(out_file.fileName());
        }
        in_file.copy(out_file_original.fileName());
        if (!success) { // original file exists
          out_file_original.remove();
          in_file.copy(out_file_original.fileName());
        }
        filename = out_file.fileName();
      } else if (btn == QMessageBox::Open) { // Use Library Version.
        filename = out_file.fileName();
      } else if (btn == QMessageBox::Save) { // Save As.
        bool ok;
        QString text = QInputDialog::getText(this,
                                             tr("Save file as.."),
                                             tr("Enter the new file name : "),
                                             QLineEdit::Normal,
                                             out_file_info.fileName(),
                                             &ok);
        if (ok && !text.isEmpty()) {
          out_file.setFileName(out_file_info.filePath() + "/" + text + "." +
                               ext);
          bool success = in_file.copy(out_file.fileName());
          if (!success) { // file exists
            out_file.remove();
            in_file.copy(out_file.fileName());
          }

          out_file_original.setFileName(out_file_info.filePath() + "/" + text +
                                        "." + "original." + ext);
          in_file.copy(out_file_original.fileName());
          if (!success) { // original file exists
            out_file_original.remove();
            in_file.copy(out_file_original.fileName());
          }
          filename = out_file.fileName();
        }
      }
    }
  }
  return ebook_document;
}

void
MainWindow::loadDocument(QString file_name, bool from_library)
{
  m_loading = true;
  QString filename = file_name;
  EBookDocumentType ebook_type = checkMimetype(filename);

  // get the correct plugin.
  IEBookInterface* ebook_plugin = nullptr;
  foreach (IPluginInterface* plugin, m_plugins) {
    ebook_plugin = dynamic_cast<IEBookInterface*>(plugin);
    if (ebook_plugin) {
      if (ebook_type == ebook_plugin->type())
        break;
    }
  }

  IEBookDocument* ebook_document;
  if (ebook_plugin) {
    if (!from_library) {
      ebook_document = copyToLibraryAndOpen(filename, ebook_plugin);
    } // end of if (from_library)

    // reload the ebook document from the library version of the file.
    ebook_document = ebook_plugin->createDocument(filename);
    ITextDocument* itextdocument;
    EBookWrapper* wrapper;
    QString tabname;
    itextdocument = dynamic_cast<ITextDocument*>(ebook_document);
    //    htmldocument->setPlugin(ebook_plugin);
    //    IEBookDocument* codeDocument = ebook_plugin->createCodeDocument();
    wrapper = new EBookWrapper(
      m_options, m_authors_db, m_series_db, m_library_db, this);
    wrapper->editor()->setDocument(ebook_document);

    EBookTOCWidget* toc_widget = new EBookTOCWidget(this);
    toc_widget->setOpenLinks(false);
    //  m_toc->setTextInteractionFlags(Qt::NoTextInteraction);
    connect(toc_widget,
            &EBookTOCWidget::anchorClicked,
            this,
            &MainWindow::tocAnchorClicked);
    connect(toc_widget,
            &EBookTOCWidget::buildTocFromHtmlFiles,
            this,
            &MainWindow::buildTocFromData);
    connect(toc_widget,
            &EBookTOCWidget::buildManualToc,
            this,
            &MainWindow::builManualToc);
    connect(toc_widget,
            &EBookTOCWidget::addAnchorsToToc,
            this,
            &MainWindow::addTocAnchors);
    toc_widget->setDocumentString(ebook_document->tocAsString());
    m_toc_stack->addWidget(toc_widget);

    tabname =
      QString(tr("%1, (%2)")
                .arg(ebook_document->title())
                .arg(ebook_document->creatorNames(ebook_document->creators())));
    m_doc_tabs->addTab(wrapper, tabname);
    m_current_document = dynamic_cast<QTextDocument*>(ebook_document);
    connect(itextdocument,
            &ITextDocument::loadCompleted,
            wrapper,
            &EBookWrapper::update);

    wrapper->metaEditor()->setDocument(itextdocument);

    // set up the editor state correctly (editor visible, show_library btn
    // visible)
    m_doc_stack->setCurrentIndex(m_stack_editor);
    m_doc_tabs->setEnabled(true);
    m_show_library->setVisible(true);
    m_show_editor->setVisible(false);
    m_options->appendCurrentFile(filename);
    saveOptions();

  } // end of is(ebook_plugin)

  m_loading = false;
}

QString
MainWindow::concatenateAuthorNames(AuthorList authors)
{
  QStringList names;
  foreach (AuthorData author, authors) {
    QStringList f;
    f << author->forename().trimmed() << author->middlenames()
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
MainWindow::attemptToExtractAuthorFromFilename(QString path, QString title)
{
  QFile in_file(path);
  QFileInfo fileInfo(in_file);
  QString filename = fileInfo.fileName();
  filename = filename.left(filename.lastIndexOf("."));

  // This removes case worries.
  QString lower_filename(
    filename.toLower()); // the lowercase version of the filename.
  QString lower_title(title.toLower()); // the lowercase version of the title.
  QString remainder;

  // remove the title (if used) from filename.
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
MainWindow::saveDocument(IEBookDocument* document)
{
  //  EBookWrapper* wrapper =
  //  qobject_cast<EBookWrapper*>(m_tabs->currentWidget()); IEBookDocument*
  //  current_doc =
  //      dynamic_cast<IEBookDocument*>(wrapper->codeEditor()->document());
  if (document) {
    //    IEBookInterface* plugin = document->plugin();
    document->saveDocument();
  }
}

void
MainWindow::documentChanged(int index)
{
  if (!m_loading) {
    if (index >= 0) {
      m_options->setCurrentIndex(index);
      EBookWrapper* wrapper =
        qobject_cast<EBookWrapper*>(m_doc_tabs->widget(index));
      QTextDocument* textdocument =
        dynamic_cast<QTextDocument*>(wrapper->editor()->document());
      IEBookDocument* iebookdocument =
        dynamic_cast<IEBookDocument*>(wrapper->editor()->document());

      if (textdocument->isModified()) {
        setStatusModified();
      } else {
        setStatusNotModified();
      }
      //      if (document->readOnly()) {
      //        setStatusReadOnly();
      //      } else {
      //        setStatusReadWrite();
      //      }
      setStatusFilename(iebookdocument->filename());

      if (textdocument) {
        m_current_document = textdocument;

        if (iebookdocument) {
          QString language = iebookdocument->language();
          QLocale local(language);
        }
      } else {
        m_current_document = nullptr;
      }
    } else {
      // TODO - no documents selected.
    }
  }

  // toc matches document.
  m_toc_stack->setCurrentIndex(index);
}

void
MainWindow::tabClosing(int index)
{
  EBookWrapper* wrapper =
    qobject_cast<EBookWrapper*>(m_doc_tabs->widget(index));
  QTextDocument* textdocument =
    dynamic_cast<QTextDocument*>(wrapper->editor()->document());
  if (textdocument->isModified()) {
    // TODO - Check if the user wants to save the document.
    IEBookDocument* itextdocument =
      dynamic_cast<IEBookDocument*>(wrapper->editor()->document());
    if (itextdocument) {
      saveDocument(itextdocument);
    }
  }
  m_doc_tabs->removeTab(index);

  // load next document from m_tabs;
  wrapper = qobject_cast<EBookWrapper*>(m_doc_tabs->currentWidget());
  textdocument = dynamic_cast<QTextDocument*>(wrapper->editor()->document());
  if (wrapper) {
    if (textdocument) {
      m_current_document = textdocument;
    }
  } else {
    // no more tabs.
    m_current_document = nullptr;
  }
}

void
MainWindow::fileOpen()
{
  QString filters;
  QStringList individual_filters;
  foreach (IPluginInterface* plugin, m_plugins) {
    IEBookInterface* ebook = dynamic_cast<IEBookInterface*>(plugin);
    if (ebook) {
      QString composite = ebook->fileFilter();
      QString individual_filter =
        ebook->fileDescription() + " ( " + composite + " );;";
      individual_filters.append(individual_filter);
      if (filters.isEmpty())
        filters += composite;
      else
        filters += ' ' + composite;
    }
  }
  if (!filters.isEmpty()) {
    filters = tr("EBook ( %1 );;").arg(filters);
  }
  foreach (QString individual_filter, individual_filters) {
    filters += individual_filter;
  }
  filters += tr("All files (*.*)");

  QStringList filenames = QFileDialog::getOpenFileNames(
    this, tr("Load Book"), m_defbookpath, filters);
  foreach (QString filename, filenames) {
    loadDocument(filename);
  }
  m_bookcount = m_options->currentfiles().size();
  saveOptions();
}

void
MainWindow::fileSave()
{
  // TODO
  EBookWrapper* wrapper =
    qobject_cast<EBookWrapper*>(m_doc_tabs->currentWidget());
  if (wrapper) {
    QTextDocument* textdocument =
      qobject_cast<QTextDocument*>(wrapper->editor()->document());
    IEBookDocument* itextdocument = dynamic_cast<IEBookDocument*>(textdocument);
    if (textdocument->isModified()) {
      saveDocument(itextdocument);
    }
  }
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
  connect(dlg, &OptionsDialog::showToc, m_toc, &QTextEdit::setVisible);
  connect(dlg, &OptionsDialog::moveToc, this, &MainWindow::update);
  int result = dlg->exec();
  if (result == QDialog::Accepted) {
    if (dlg->modified()) {
      m_options = dlg->options();
      saveOptions();
    }
  }
}

void
MainWindow::viewFullscreen()
{
  m_view_centre->setEnabled(false);
}

void
MainWindow::viewMaximise()
{
  m_view_centre->setEnabled(false);
}

void
MainWindow::viewMinimise()
{
  m_view_centre->setEnabled(true);
}

void
MainWindow::viewCentre()
{
  QScreen* screen = QGuiApplication::primaryScreen();
  QRect screenGeometry = screen->geometry();
  move(screenGeometry.center() - rect().center());
  saveOptions();
}

void
MainWindow::viewToc()
{
  QString text;
  bool state = m_options->tocVisible();
  m_options->setTocVisible(!state);
  if (state) {
    text = tr("Show Table of Contents");
  } else {
    text = tr("Hide Table of Contents");
  }
  m_view_toc->setText(text);
  m_toc->setVisible(!state);
}

void
MainWindow::viewTocPosition()
{
  QString text;
  Options::TocPosition position = m_options->tocPosition();
  if (position == Options::LEFT) {
    m_options->setTocPosition(Options::RIGHT);
    text = tr("Move Table of Contents to Left");
    position = Options::RIGHT;
  } else {
    m_options->setTocPosition(Options::LEFT);
    text = tr("Move Table of Contents to Right");
    position = Options::LEFT;
  }
  m_view_toc_position->setText(text);
  update(position);
}

void
MainWindow::setLibraryToolbarState()
{
  if (m_library_frame->isTree()) {
    m_library_tree->setVisible(false);
    m_library_shelf->setVisible(true);
  } else {
    m_library_tree->setVisible(true);
    m_library_shelf->setVisible(false);
  }
}

void
MainWindow::viewShowLibrary()
{
  m_doc_stack->setCurrentIndex(m_stack_library);
  m_lib_type_toolbar->setEnabled(true);
  setLibraryToolbarState();
}

void
MainWindow::viewShowEditor()
{
  m_doc_stack->setCurrentIndex(m_stack_editor);
  m_lib_type_toolbar->setEnabled(false);
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
  AboutDialog* dlg = new AboutDialog(this);
  dlg->show();
}

void
MainWindow::helpAboutPlugins()
{
  PluginDialog* dlg = new PluginDialog(this);
  foreach (IPluginInterface* interface, m_plugins) {
    dlg->addPlugin(interface);
  }
  dlg->open();
}

void
MainWindow::helpCheckUpdates()
{
  // TODO
}

void
MainWindow::setStatusModified()
{
  m_modifiedlbl->setText(MODIFIED);
  m_file_save->setEnabled(true);
}

void
MainWindow::setStatusNotModified()
{
  m_modifiedlbl->setText(NOT_MODIFIED);
  //  m_file_save->setEnabled(false);
}

void
MainWindow::setStatusReadOnly()
{
  m_readonlylbl->setText(READ_ONLY);
}

void
MainWindow::setStatusReadWrite()
{
  m_readonlylbl->setText(READ_WRITE);
}

void
MainWindow::setStatusFilename(QString name)
{
  m_filelbl->setText(name);
}

void
MainWindow::openWindow()
{
  QObject* obj = sender();
  QAction* act = qobject_cast<QAction*>(obj);
  EBookWrapper* wrapper =
    qobject_cast<EBookWrapper*>(m_doc_tabs->currentWidget());
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

void
MainWindow::tocAnchorClicked(QUrl url)
{
  EBookWrapper* wrapper =
    qobject_cast<EBookWrapper*>(m_doc_tabs->currentWidget());
  EBookEditor* editor = wrapper->editor();
  QString fragment = url.fragment();
  if (fragment.isEmpty()) {
    editor->scrollToAnchor(url.path());
  } else {
    editor->scrollToAnchor(fragment);
  }
}

void
MainWindow::setObjectVisibility(int index)
{
  QObject* obj = sender();
  if (obj) {
    QStackedWidget* sw = dynamic_cast<QStackedWidget*>(obj);
    if (sw) {
      if (sw->currentWidget() == m_library_frame) {
        m_show_library->setVisible(false);
        m_show_editor->setVisible(true);
      } else if (sw->currentWidget() == m_tabs_frame) {
        m_show_library->setVisible(true);
        m_show_editor->setVisible(false);
      }
    } else {
      Options::ViewState state = Options::ViewState(index);
      switch (state) {
        case Options::VIEW_EDITOR:
          m_doc_stack->setCurrentIndex(m_stack_editor);
          break;
        case Options::VIEW_LIBRARY_TREE:
          m_doc_stack->setCurrentIndex(m_stack_library);
          m_library_frame->setToTree();
          setLibraryToolbarState();
          break;
        case Options::VIEW_LIBRARY_SHELF:
          m_doc_stack->setCurrentIndex(m_stack_library);
          m_library_frame->setToShelf();
          setLibraryToolbarState();
          break;
      }
    }
  }
}

void
MainWindow::buildTocFromData()
{
  EBookWrapper* wrapper =
    qobject_cast<EBookWrapper*>(m_doc_tabs->currentWidget());
  int index = m_doc_tabs->currentIndex();
  EBookEditor* editor = wrapper->editor();
  // get new toc data
  QString toc_string = editor->buildTocFromData();
  // create new data widget
  EBookTOCWidget* new_toc_widget = new EBookTOCWidget(this);
  new_toc_widget->setOpenLinks(false);
  connect(new_toc_widget,
          &EBookTOCWidget::anchorClicked,
          this,
          &MainWindow::tocAnchorClicked);
  connect(new_toc_widget,
          &EBookTOCWidget::buildManualToc,
          this,
          &MainWindow::builManualToc);
  new_toc_widget->setDocumentString(toc_string);
  // get current toc widget
  QWidget* toc_widget = m_toc_stack->currentWidget();
  // back it up
  m_toc_backup.insert(index, toc_widget);
  // replace it with the new toc data
  m_toc_stack->removeWidget(toc_widget);
  m_toc_stack->insertWidget(index, new_toc_widget);
  m_toc_stack->setCurrentIndex(index);

  new_toc_widget->enableHtmlMenuItem(false);
}

void
MainWindow::builManualToc()
{
  int index = m_doc_tabs->currentIndex();

  // create new data editor
  EBookTocEditor* toceditor = new EBookTocEditor(this);
  // get current toc widget
  EBookTOCWidget* toc_widget =
    qobject_cast<EBookTOCWidget*>(m_toc_stack->currentWidget());
  // back it up
  //  m_toc_backup.insert(index, toc_widget);
  //  // replace it with the new toc data
  //  m_toc_stack->removeWidget(toc_widget);
  toceditor->setDocument(toc_widget->document());
  //  m_toc_stack->insertWidget(index, toceditor);
  //  m_toc_stack->setCurrentIndex(index);

  toceditor->exec();
  QString toc = toceditor->documentString();
  toc_widget->setDocumentString(toc);
}

void
MainWindow::addTocAnchors()
{}

void
MainWindow::fileNew()
{
  // TODO create a new empty file
  // define type etc. Should be part of plugin??
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
//    m_popup = nullptr;
//  }
//}

// EBookDocument *MainWindow::createDocument(QString path)
//{
//    // TODO something more rigorous perhaps
//    if (path.toLower().endsWith(".epub")) {
//        return createEPubDocument(path);
//    } else if (path.toLower().endsWith(".mobi")) {
//        return createMobiDocument(path);
//    }
//    return nullptr;
//}

// EBookDocument *MainWindow::createDocument(EBookDocument *doc)
//{
//    if (doc) {
//        EBookContents *contents = doc->cloneData();
//        //        EPubDocument *document = new EBookDocument(doc->parent());
//        //        document->setDocumentLayout(new
//        //        QPlainTextDocumentLayout(document));
//        //        document->setClonedData(contents);
//        //        return document;
//    }
//    //  MobiDocument* mobi = dynamic_cast<MobiDocument*>(doc);
//    //  if (mobi) {
//    //    EPubDocument contents = doc->cloneData();
//    //    MobiDocument* document = new MobiDocument(this);
//    //    document->setClonedData(contents);
//    //    return document;
//    //  }
//    return nullptr;
//}

// EBookDocument *MainWindow::createEPubDocument(QString path)
//{
//    //    EPubDocument *document = new EPubDocument(this);
//    //    clearDocuments();
//    //    document->openDocument(path);
//    //    return document;
//}

// EBookDocument *MainWindow::createMobiDocument(QString path)
//{
//    //    MobiDocument *document = new MobiDocument(this);
//    //    clearDocuments();
//    //    document->openDocument(path);
//    //    return document;
//}

void
MainWindow::loadPlugins()
{
  QDir pluginsDir = QDir(qApp->applicationDirPath());
  pluginsDir.cd("plugins");

  foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
    if (fileName == "Makefile") // can remove this in installed versions.
      continue;
    QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
    QObject* plugin = loader.instance();

    if (plugin) {
      IPluginInterface* iebook = dynamic_cast<IPluginInterface*>(plugin);
      if (iebook) {
        iebook->buildMenu();
        // Add plugin to list of ALL plugins.
        m_plugins.append(iebook);
        iebook->setLoaded(true);

        // Add spellcheckers to list of spellcheckers;
        //                QString name = iebook->pluginName();
        //                ISpellInterface *spellinterface =
        //                dynamic_cast<ISpellInterface *>(iebook); if
        //                (spellinterface) {
        //                    m_spellchecker_plugins.insert(name,
        //                    spellinterface);
        //                }

        //      m_languages = m_current_spell_checker->languageCodes();
        //      foreach (QString lang, m_languages) {
        //        CountryData* data = m_current_spell_checker->dictionary(lang);
        //        m_dict_data.insert(lang, data);
        //        m_dict_paths.insert(lang, data->path);
        //      }
      }
    } else {
      QLOG_DEBUG(tr("Plugin error : %1").arg(loader.errorString()))
    }
  }
  // TODO handle more than one spellchecker plugin.
  m_current_spell_checker = m_spellchecker_plugins["HunSpell"];
  //  return false;
}
