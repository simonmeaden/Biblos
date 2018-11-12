#include "mainwindow.h"
#include <QApplication>

#include "epubcommon.h"
#include "ebookeditor.h"
#include "qebookdocument.h"
#include "qepubdocument.h"
#include "qmobidocument.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<EBookEditor>();
    qRegisterMetaType<EBookData>();
    qRegisterMetaType<EPubDocument>();
    qRegisterMetaType<QMobiDocument>();

    MainWindow w;
    w.show();

    return a.exec();
}
