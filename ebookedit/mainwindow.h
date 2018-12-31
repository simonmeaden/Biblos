#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QStringList>
#include <QtWidgets>

#include <qlogger/qlogger.h>
#include <qyaml-cpp/QYamlCpp>
#include <yaml-cpp/yaml.h>

#include "ebookcommon.h"
#include "iebookdocument.h"

// class Library;
class IEBookDocument;
class EPubDocument;
class MobiDocument;
class ISpellInterface;
class IPluginInterface;
class CountryData;
class DbManager;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  void closePopup();

  SharedAuthorList selectAuthorNames(QString filename, EBookData* data);

  void loadNonLibraryFiles();

signals:
  void codeChanged();

protected:
  // main document pointer.
  IEBookDocument* m_document = Q_NULLPTR;
  QSplitter* m_splitter;
  QTabWidget* m_tabs;
  QTextBrowser* m_toc;
  Options::TocPosition m_toc_position;
  QMap<QString, ISpellInterface*> m_spellchecker_plugins;
  QMap<QString, IPluginInterface*> m_ebookplugins;
  QList<IPluginInterface*> m_plugins;
  QStringList m_languages;
  QMap<QString, QString> m_dict_paths;
  QMap<QString, CountryData*> m_dict_data;
  QString m_home_directiory, m_data_directory, m_config_directory,
      m_config_file;
  //  QSharedPointer<Library> m_library;
  DbManager* m_database;

  void resizeEvent(QResizeEvent* e);
  void moveEvent(QMoveEvent* e);
  void saveOptions();
  void loadOptions();
  void documentChanged(int index);
  void tabClosing(int);
  //  bool eventFilter(QObject *object, QEvent *event);
  void update(Options::TocPosition position);

  YAML::Node m_preferences;
  bool m_initialising, m_loading;
  Options* m_options;
  bool m_prefchanged = false;
  QString m_defbookpath;
  int m_bookcount;
  //  HoverDialog *m_dlg;
  //  int m_hovertab;
  QFrame* m_popup;
  //  QTimer* m_popuptimer;
  int m_popupindex;
  IEBookDocument* m_current_document;
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
  void initFileMenu();
  void initEditMenu();
  void initViewMenu();
  //  void initSpellingMenu();
  void initHelpMenu();
  //    EBookDocument* createDocument(QString path);
  //    EBookDocument* createDocument(EBookDocument*);
  //    EBookDocument* createEPubDocument(QString path);
  //    EBookDocument* createMobiDocument(QString path);
  void loadPlugins();
  void loadDocument(QString filename);
  void saveDocument(IEBookDocument* document);
  //    void clearDocuments();
  QString copyBookToStore(QString filename, QString authors);
  QString concatenateAuthorNames(SharedAuthorList names);
  QString concatenateAuthorNames(QStringList names);
  QStringList removeEmpty(QStringList values);
  QStringList attemptToExtractAuthorFromFilename(QString, EBookData* data);
  QString cleanString(QString toClean);

  void setModified();
  void setNotModified();
  void setReadOnly();
  void setReadWrite();
  void setFilename(QString name);
  void tabEntered(int, QPoint pos, QVariant);
  void tabExited(int);
  void openWindow();
  void tocAnchorClicked(QUrl url);

protected: // Menu/StatusBar stuff
  QMenuBar* m_menubar;
  QMenu* m_filemenu;
  QMenu* m_editmenu;
  QMenu* m_view_menu;
  QMenu* m_spellingmenu;
  QMenu* m_helpmenu;

  QLabel* m_modifiedlbl;
  QLabel* m_readonlylbl;
  QLabel* m_filelbl;

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

  void helpContents();
  void helpIndex();
  void helpContext();
  void helpAboutEbookEditor();
  void helpAboutPlugins();
  void helpCheckUpdates();

  static const int DEF_WIDTH = 600;
  static const int DEF_HEIGHT = 1200;
  static const int DEF_X = 0;
  static const int DEF_Y = 0;
  static const int DEF_DLG_WIDTH = 300;
  static const int DEF_DLG_HEIGHT = 300;
  static const QString READ_ONLY;
  static const QString READ_WRITE;
  static const QString NO_FILE;
  static const QString NOT_MODIFIED;
  static const QString MODIFIED;
  static const int DEF_POPUP_TIMEOUT = 10000;

  static const QString DB_NAME;

  static const QString PREF_FILE;
  static QString POSITION;
  static QString DIALOG;
  static QString PREF_POPUP_TIMEOUT;
  static QString PREF_ENABLE_POPUP;
  static QString PREF_CURRENT_INDEX;
  static QString PREF_COUNT;
  static QString PREF_BOOKLIST;
  static QString PREF_LIBRARY;
  static QString CODE_OPTIONS;
  static QString CODE_NORMAL;
  static QString CODE_FONT;
  static QString CODE_ATTRIBUTE;
  static QString CODE_TAG;
  static QString CODE_ERROR;
  static QString CODE_STRING;
  static QString CODE_STYLE;
  static QString CODE_SCRIPT;
  static QString CODE_COLOR;
  static QString CODE_BACK;
  static QString CODE_WEIGHT;
  static QString CODE_ITALIC;
  static QString COPY_BOOKS_TO_STORE;
  static QString DELETE_OLD_BOOK;
  static QString SHOW_TOC;
  static QString TOC_POSITION;

  static const QString BTN_STYLE;
  void loadLibraryFiles();
  void setModifiedAuthors(IEBookDocument* doc, SharedAuthorList authors);
};

#endif // MAINWINDOW_H
