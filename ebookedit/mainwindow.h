#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QMainWindow>
#include <QMimeDatabase>
#include <QSqlDatabase>
#include <QStringList>
#include <QtWidgets>

#include <qlogger/qlogger.h>
#include <qyaml-cpp/QYamlCpp>
//#include "qyaml-cpp.h"
//#include <yaml-cpp/yaml.h>

#include "actiontabwidget.h"
#include "ebookcommon.h"
#include "iebookdocument.h"

#include "authors.h"
#include "library.h"
#include "options.h"

class IEBookDocument;
class EPubDocument;
class MobiDocument;
class ISpellInterface;
class IPluginInterface;
class CountryData;
class LibraryFrame;
class EBookWrapper;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  void closePopup();

  AuthorList selectAuthorNames(QString filename, QString title);

signals:
  void codeChanged();

protected:
  // main document pointer.
  EBookDocument m_document;
  QStackedWidget *m_doc_stack, *m_toc_stack;
  int m_stack_library, m_stack_editor;
  QFrame* m_tabs_frame;
  QSplitter* m_splitter;
  ActionTabWidget* m_doc_tabs;
  QTextBrowser* m_toc;
  LibraryFrame* m_library_frame;
  QMap<int, QWidget*> m_toc_backup;
  QString m_jquery, m_onepage_js, m_onepage_css;

  Options::TocPosition m_toc_position;
  QMap<QString, ISpellInterface*> m_spellchecker_plugins;
  QMap<QString, IPluginInterface*> m_ebookplugins;
  QList<IPluginInterface*> m_plugins;
  QStringList m_languages;
  QMap<QString, QString> m_dict_paths;
  QMap<QString, CountryData*> m_dict_data;
  //  QString m_home_directiory;
  //  QString m_library_directory;
  //  QString m_config_directory;
  //  QString m_config_file;
  //  QString m_lib_file;
  //  QString m_authors_file;
  bool is_in_temp_store = false;
  //  DbManager* m_database;

  void resizeEvent(QResizeEvent* e);
  void moveEvent(QMoveEvent* e);
  void saveOptions();
  void loadOptions();
  void saveLibrary();
  void loadLibrary();
  void saveAuthors();
  void loadAuthors();
  void saveSeries();
  void loadSeries();
  void documentChanged(int index);
  void tabClosing(int);
  //  bool eventFilter(QObject *object, QEvent *event);
  void update(Options::TocPosition position);

  YAML::Node m_preferences;
  bool m_initialising, m_loading;
  Options* m_options;
  SeriesDB m_series_db;
  LibraryDB m_library_db;
  AuthorsDB m_authors_db;
  //  bool m_prefchanged = false;
  QString m_defbookpath;
  int m_bookcount;
  //  HoverDialog *m_dlg;
  //  int m_hovertab;
  QFrame* m_popup;
  //  QTimer* m_popuptimer;
  int m_popupindex;
  EBookDocument m_current_document;
  ISpellInterface* m_current_spell_checker;

  void initBuild();
  void initGui();
  void initMenus();
  void initActions();
  void initStatusbar();
  void initToolbar();
  void initSetup();
  void initFileActions();
  void initViewActions();
  void initEditActions();
  void initEditorActions();
  void initHelpActions();
  void initLibActions();
  void initFileMenu();
  void initEditMenu();
  void initViewMenu();
  //  void initSpellingMenu();
  void initHelpMenu();

  void loadPlugins();
  void loadDocument(QString file_name, bool from_library = false);
  void saveDocument(EBookDocument document);

  QString concatenateAuthorNames(AuthorList names);
  QString concatenateAuthorNames(QStringList names);
  QStringList removeEmpty(QStringList values);
  QStringList attemptToExtractAuthorFromFilename(QString, QString title);
  QString cleanString(QString toClean);

  void setStatusModified();
  void setStatusNotModified();
  void setStatusReadOnly();
  void setStatusReadWrite();
  void setStatusFilename(QString name);
  void tabEntered(int, QPoint pos, QVariant);
  void tabExited(int);
  void openWindow();
  void tocAnchorClicked(QUrl url);
  void setObjectVisibility(int index = 0);

  void buildTocFromData();
  void buildManualToc();
  void addTocAnchors();

protected: // Menu/StatusBar stuff
  QMenuBar* m_menubar;
  QMenu* m_filemenu;
  QMenu* m_editmenu;
  QMenu* m_view_menu;
  QMenu* m_spellingmenu;
  QMenu* m_helpmenu;

  QToolBar* m_lib_toolbar;
  QToolBar* m_lib_type_toolbar;
  QToolBar* m_file_toolbar;
  QToolBar* m_editor_toolbar;

  QLabel* m_modifiedlbl;
  QLabel* m_readonlylbl;
  QLabel* m_filelbl;

  QAction* m_show_library;
  QAction* m_show_editor;
  QAction* m_library_shelf;
  QAction* m_library_tree;

  QAction* m_file_new;
  QAction* m_file_open;
  QAction* m_file_save;
  QAction* m_file_save_as;
  QAction* m_file_save_all;
  QAction* m_file_exit;

  QAction* m_edit_undo;
  QAction* m_edit_redo;
  QAction* m_edit_cut;
  QAction* m_edit_copy;
  QAction* m_edit_paste;
  QAction* m_edit_paste_history;
  QAction* m_edit_options;
  QAction* m_edit_spellcheck;
  QAction* m_edit_highlight_misspelled;
  QAction* m_edit_next_misspelled;
  QAction* m_edit_send_to_booklist;
  QAction* m_edit_send_to_authorlist;

  QAction* m_open_editor;
  QAction* m_open_codeeditor;
  QAction* m_open_metadata;

  QAction* m_help_contents;
  QAction* m_help_index;
  QAction* m_help_context;
  QAction* m_help_about_ebookeditor;
  QAction* m_help_about_plugins;
  QAction* m_help_check_updates;

  QActionGroup* m_screengrp;
  QAction* m_view_fullscreen;
  QAction* m_view_maximise;
  QAction* m_view_minimise;
  QAction* m_view_centre;
  QAction* m_view_toc;
  QAction* m_view_toc_position;

  void fileNew();
  void fileOpen();
  void fileSave();
  void fileSaveAs();
  void fileSaveAll();
  void fileExit();

  void editUndo();
  void editRedo();
  void editCopy();
  void editCut();
  void editPaste();
  void editPasteFromHistory();
  void editOptions();
  void editSpellcheck();
  void editHighlightWords();
  void editNextWord();
  void editSendToBookList();
  void editSendToAuthorList();

  void viewFullscreen();
  void viewMaximise();
  void viewMinimise();
  void viewCentre();
  void viewToc();
  void viewTocPosition();
  void viewShowLibrary();
  void viewShowEditor();

  void helpContents();
  void helpIndex();
  void helpContext();
  void helpAboutEbookEditor();
  void helpAboutPlugins();
  void helpCheckUpdates();

  static const QString READ_ONLY;
  static const QString READ_WRITE;
  static const QString NO_FILE;
  static const QString NOT_MODIFIED;
  static const QString MODIFIED;

  static const QString DB_NAME;

  static const QString PREF_FILE;
  static const QString LIB_FILE;
  static const QString AUTHOR_FILE;
  static const QString SERIES_FILE;

  void loadLibraryFiles(QStringList current_lib_files, int currentindex);
  EBookDocumentType checkMimetype(QString filename);
  void setModifiedAuthors(EBookDocument doc, AuthorList authors);
  void setLibraryToolbarState();
  EBookDocument copyToLibraryAndOpen(QString& filename,
                                     IEBookInterface* ebook_plugin);
};

#endif // MAINWINDOW_H
