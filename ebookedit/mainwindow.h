#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QMainWindow>
#include <QStringList>
#include <QtWidgets>
#include <QSqlDatabase>

#include <qlogger/qlogger.h>
#include <qyaml-cpp/QYamlCpp>
#include <stringutil/stringutil.h>
#include <yaml-cpp/yaml.h>

#include "ebookcodeeditor.h"
#include "ebookcommon.h"
#include "ebookeditor.h"
#include "ebookwrapper.h"
#include "optionsdialog.h"
#include "ebookdocument.h"
#include "epubdocument.h"
#include "mobidocument.h"
#include "authordialog.h"
#include "dbmanager.h"

#include "interface.h"

class Library;
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  void closePopup();

  SharedAuthorList selectAuthorNames(QString filename, EBookData *data);

  void loadNonLibraryFiles();

signals:
  void codeChanged();

protected:
  // main document pointer.
  EBookDocument* m_document = Q_NULLPTR;
  // pointer when cast to epub/mobi.
  EPubDocument* m_epub_doc = Q_NULLPTR;
  MobiDocument* m_mobi_doc = Q_NULLPTR;
  //  HoverTabWidget* m_tabs;
  QTabWidget* m_tabs;
  QMap<QString, SpellInterface*> m_spellcheckers;
  QStringList m_languages;
  QMap<QString, QString> m_dict_paths;
  QMap<QString, CountryData*> m_dict_data;
  QString m_home_directiory, m_data_directory, m_config_directory,
    m_config_file;
  QSharedPointer<Library> m_library;
  DbManager *m_database;

  void resizeEvent(QResizeEvent* e);
  void moveEvent(QMoveEvent* e);
  void saveOptions();
  void loadOptions();
  void documentChanged(int index);
  void tabClosing(int);
  //  bool eventFilter(QObject *object, QEvent *event);

  YAML::Node m_preferences;
  bool m_initialising;
  Options* m_options;
  bool m_prefchanged = false;
  QString m_defbookpath;
  int m_bookcount;
  //  HoverDialog *m_dlg;
  //  int m_hovertab;
  QFrame* m_popup;
  QTimer* m_popuptimer;
  int m_popupindex;
  EBookDocument* m_current_document;
  SpellInterface* m_current_spell_checker;

  void initBuild();
  void initGui();
  void initMenus();
  void initActions();
  void initStatusbar();
  void initToolbar();
  void initSetup();
  void initFileActions();
  void initWindowActions();
  void initEditActions();
  void initEditorActions();
  void initFileMenu();
  void initEditMenu();
  void initWindowMenu();
  //  void initSpellingMenu();
  void initHelpMenu();
  EBookDocument *createDocument(QString path);
  EBookDocument* createDocument(EBookDocument*);
  EBookDocument* createEPubDocument(QString path);
  EBookDocument* createMobiDocument(QString path);
  void loadPlugins();
  void loadDocument(QString filename);
  void saveDocument(EBookDocument* document);
  void clearDocuments();
  void copyBookToStore(QString filename, QString authors);
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

protected: // Menu/StatusBar stuff
  QMenuBar* m_menubar;
  QMenu* m_filemenu;
  QMenu* m_editmenu;
  QMenu* m_windowmenu;
  QMenu* m_spellingmenu;
  QMenu* m_helpmenu;

  QLabel* m_modifiedlbl;
  QLabel* m_readonlylbl;
  QLabel* m_filelbl;

  QAction* m_fileopen;
  QAction* m_filesave;
  QAction* m_filesaveas;
  QAction* m_filesaveall;
  QAction* m_fileexit;

  QAction* m_editundo;
  QAction* m_editredo;
  QAction* m_editcut;
  QAction* m_editcopy;
  QAction* m_editpaste;
  QAction* m_editpastehistory;
  QAction* m_editoptions;
  QAction* m_editspellcheck;
  QAction* m_edithighlight_misspelled;
  QAction* m_editnextmisspelled;
  QAction* m_editsendtobooklist;
  QAction* m_editsendtoauthorlist;

  QAction *m_openeditor;
  QAction *m_opencodeeditor;
  QAction *m_openmetadata;

  QActionGroup* m_screengrp;
  QAction* m_winfullscreen;
  QAction* m_winmaximise;
  QAction* m_winminimise;
  QAction* m_wincentre;

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

  void winFullscreen();
  void winMaximise();
  void winMinimise();
  void winCentre();

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
  static const int DEF_POPUP_TIMEOUT = 10000; // 10 seconds

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

  static const QString BTN_STYLE;
  void loadLibraryFiles();
};

#endif // MAINWINDOW_H
