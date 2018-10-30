#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStringList>
#include <QtWidgets>

#include <qyaml.h>
#include <yaml.h>

#include "optionsdialog.h"
#include "ebookdocument.h"
#include "ebookeditor.h"
#include "epubdocument.h"
#include "hoverpopup.h"
#include "hovertabwidget.h"
#include "mobidocument.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  void closePopup();

protected:
  // main document pointer.
  EBookDocument* m_document = Q_NULLPTR;
  // pointer when cast to epub/mobi.
  EPubDocument* m_epub_doc = Q_NULLPTR;
  MobiDocument* m_mobi_doc = Q_NULLPTR;
  HoverTabWidget* m_tabs;
  //  QTabWidget*m_tabs;
  QList<EBookDocument*> m_documents;

  void resizeEvent(QResizeEvent* e);
  void moveEvent(QMoveEvent* e);
  void saveOptions();
  void loadOptions();
  void documentChanged(int index);
  //  bool eventFilter(QObject *object, QEvent *event);

  YAML::Node m_preferences;
  Options m_options;
  bool m_prefchanged = false;
  QString m_defbookpath;

  int m_bookcount;
  //  HoverDialog *m_dlg;
  //  int m_hovertab;
  QFrame* m_popup;
  QTimer *m_popuptimer;
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
  EBookDocument* createDocument(QString path);
  EBookDocument* createEPubDocument(QString path);
  EBookDocument* createMobiDocument(QString path);
  void loadDocument(QString filename);
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

public:
  static const int DEF_WIDTH = 600;
  static const int DEF_HEIGHT = 1200;
  static const int DEF_X = 0;
  static const int DEF_Y = 0;
  static const QString READ_ONLY;
  static const QString READ_WRITE;
  static const QString NO_FILE;
  static const QString NOT_MODIFIED;
  static const QString MODIFIED;
  static const int DEF_POPUP_TIMEOUT = 10000; // 10 seconds
  void initFileMenu();
  void initEditMenu();
  void initWindowMenu();
};

#endif // MAINWINDOW_H
