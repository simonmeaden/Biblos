#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtWidgets>
#include <QDialog>
#include <QStringList>

#include <qyaml.h>
#include <yaml.h>

#include "ebookdocument.h"
#include "epubdocument.h"
#include "mobidocument.h"
#include "ebookeditor.h"

class OptionsDialog : public QDialog {
  Q_OBJECT

public:
  OptionsDialog(QWidget *parent = Q_NULLPTR);
  ~OptionsDialog();

protected:
  void initGui();
};

class HoverDialog : public QDialog {
  Q_OBJECT

public:
  HoverDialog(QVariant data, QWidget *parent=Q_NULLPTR);
  ~HoverDialog();

protected:
  EBookData m_data;
  QLabel *m_titlelbl, *m_authorlbl, *m_publisherlbl;
  QLabel *m_langlbl, *m_datelbl, *m_subjectlbl;

  void initGui();

};

class HoverTabWidget : public QTabWidget {
  Q_OBJECT

public:
  HoverTabWidget(QWidget *parent = Q_NULLPTR);
  ~HoverTabWidget();

  int addTab(EBookEditor *widget, const QString &text);

protected:
  HoverDialog *m_dlg;

  void hoverSelected(int index);
  void hoverCompleted();
};

class HoverTabBar : public QTabBar {
  Q_OBJECT
public:
  HoverTabBar(QWidget *parent = Q_NULLPTR);
  ~HoverTabBar();

signals:
  void hoverTabSelected(int);
  void hoverTabClose();

protected:
  void hoverEnter(QHoverEvent * event);
  void hoverLeave(QHoverEvent * event);
  void hoverMove(QHoverEvent * event);
  bool event(QEvent * e);

  int m_hoverindex;
  HoverDialog *m_dlg;

};

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

protected:
  // main document pointer.
  EBookDocument *m_document = Q_NULLPTR;
  // pointer when cast to epub/mobi.
  EPubDocument *m_epub_doc = Q_NULLPTR;
  MobiDocument *m_mobi_doc = Q_NULLPTR;
  HoverTabWidget *m_tabs;
  QList<EBookDocument*> m_documents;

  void resizeEvent(QResizeEvent *e);
  void moveEvent(QMoveEvent *e);
  void saveOptions();
  void loadOptions();
  void documentChanged(int index);
//  bool eventFilter(QObject *object, QEvent *event);

  YAML::Node m_preferences;
  int m_x, m_y, m_width, m_height;
  bool m_prefchanged = false;
  QString m_defbookpath;
  QStringList m_currentfiles;
  int m_currentindex;
  int m_bookcount;

  void initBuild();
  void initGui();
  void initMenus();
  void initActions();
  void initStatusbar();
  void initSetup();
  void initFileActions();
  void initWindowActions();
  void initEditActions();
  EBookDocument *createDocument(QString path);
  EBookDocument *createEPubDocument(QString path);
  EBookDocument *createMobiDocument(QString path);
  void loadDocument(QString filename);
  void clearDocuments();

  void setModified();
  void setNotModified();
  void setReadOnly();
  void setReadWrite();
  void setFilename(QString name);

protected: // Menu/StatusBar stuff
  QMenuBar *m_menubar;
  QMenu *m_filemenu;
  QMenu *m_editmenu;
  QMenu *m_windowmenu;
  QMenu *m_helpmenu;

  QLabel *m_modifiedlbl;
  QLabel *m_readonlylbl;
  QLabel *m_filelbl;

  QAction *m_fileopen;
  QAction *m_filesave;
  QAction *m_filesaveas;
  QAction *m_filesaveall;
  QAction *m_fileexit;

  QAction *m_editundo;
  QAction *m_editredo;
  QAction *m_editcut;
  QAction *m_editcopy;
  QAction *m_editpaste;
  QAction *m_editpastehistory;
  QAction *m_editoptions;

  QActionGroup *m_screengrp;
  QAction *m_winfullscreen;
  QAction *m_winmaximise;
  QAction *m_winminimise;
  QAction *m_wincentre;

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

private:
  static const int DEF_WIDTH = 600;
  static const int DEF_HEIGHT = 1200;
  static const int DEF_X = 0;
  static const int DEF_Y = 0;
  static const QString READ_ONLY;
  static const QString READ_WRITE;
  static const QString NO_FILE;
  static const QString NOT_MODIFIED;
  static const QString MODIFIED;
};

#endif // MAINWINDOW_H
