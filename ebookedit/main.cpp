#include "mainwindow.h"
#include <QApplication>

#include "ebookcodeeditor.h"
#include "ebookcommon.h"
#include "ebookeditor.h"
#include "iebookdocument.h"
//#include "epubdocument.h"
//#include "mobidocument.h"

int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  //  QCoreApplication::setOrganizationName("SM Electronic Components");
  //  QCoreApplication::setOrganizationDomain("smelecomp.co.uk");
  QCoreApplication::setApplicationName("Biblos");

  qRegisterMetaType<EBookEditor>();
  qRegisterMetaType<EBookCodeEditor>();
  //  qRegisterMetaType<EBookAuthorData>();
  //  qRegisterMetaType<EBookData>();
  //  qRegisterMetaType<EPubDocument>();
  //  qRegisterMetaType<EPubDocument>();
  //  qRegisterMetaType<MobiDocument>();

  MainWindow w;
  w.show();

  return a.exec();
}
