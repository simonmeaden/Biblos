#include "mainwindow.h"
#include <QApplication>

#include "ebookcommon.h"
#include "ebookdocument.h"
#include "ebookeditor.h"
#include "ebookcodeeditor.h"
//#include "epubdocument.h"
//#include "mobidocument.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //  QCoreApplication::setOrganizationName("SM Electronic Components");
    //  QCoreApplication::setOrganizationDomain("smelecomp.co.uk");
    QCoreApplication::setApplicationName("EBookEditor");

    qRegisterMetaType<EBookEditor>();
    qRegisterMetaType<EBookCodeEditor>();
    qRegisterMetaType<EBookData>();
    qRegisterMetaType<EBookDocument>();
    //  qRegisterMetaType<EPubDocument>();
    //  qRegisterMetaType<MobiDocument>();

    MainWindow w;
    w.show();

    return a.exec();
}
