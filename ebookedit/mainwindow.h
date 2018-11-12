#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStringList>
#include <QtWidgets>
#include <QDir>

#include <QYamlCpp>
#include <yaml.h>

#include "ebookcodeeditor.h"
#include "ebookeditor.h"
#include "ebookwrapper.h"
//#include "hoverpopup.h"
//#include "hovertabwidget.h"
#include "optionsdialog.h"
#include "qebookdocument.h"
#include "qepubdocument.h"
#include "qmobidocument.h"

#include "spellinterface.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  void closePopup();

signals:
  void codeChanged();

protected:
  // main document pointer.
  QEBookDocument* m_document = Q_NULLPTR;
  // pointer when cast to epub/mobi.
  EPubDocument* m_epub_doc = Q_NULLPTR;
  QMobiDocument* m_mobi_doc = Q_NULLPTR;
  //  HoverTabWidget* m_tabs;
  QTabWidget* m_tabs;
  QList<QEBookDocument*> m_documents;
  QMap<QString, SpellInterface*> m_spellcheckers;

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

  void initBuild();
  void initGui();
  void initMenus();
  void initActions();
  void initStatusbar();
  void initSetup();
  void initFileActions();
  void initWindowActions();
  void initEditActions();
  QEBookDocument* createDocument(QString path);
  QEBookDocument* createDocument(QEBookDocument*);
  QEBookDocument* createEPubDocument(QString path);
  QEBookDocument* createMobiDocument(QString path);
  void loadDocument(QString filename);
  void saveDocument(QEBookDocument* document);
  void clearDocuments();

  void setModified();
  void setNotModified();
  void setReadOnly();
  void setReadWrite();
  void setFilename(QString name);
  void tabEntered(int, QPoint pos, QVariant);
  void tabExited(int);

protected: // Menu/StatusBar stuff
  QMenuBar* m_menubar;
  QMenu* m_filemenu;
  QMenu* m_editmenu;
  QMenu* m_windowmenu;
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

  void winFullscreen();
  void winMaximise();
  void winMinimise();
  void winCentre();

  void loadPlugins();

public:
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
  void initFileMenu();
  void initEditMenu();
  void initWindowMenu();

  static const QString PREF_FILE;
  static QString POSITION;
  static QString DIALOG;
  static QString PREF_POPUP_TIMEOUT;
  static QString PREF_ENABLE_POPUP;
  static QString PREF_CURRENT_INDEX;
  static QString PREF_COUNT;
  static QString PREF_BOOKLIST;
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

  static const QString BTN_STYLE;
};

#endif // MAINWINDOW_H
