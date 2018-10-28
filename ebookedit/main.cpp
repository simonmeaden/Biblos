#include "mainwindow.h"
#include <QApplication>

#include "ebookeditor.h"
#include "ebookdocument.h"
#include "epubdocument.h"
#include "mobidocument.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<EBookEditor>();
    qRegisterMetaType<EBookData>();
    qRegisterMetaType<EPubDocument>();
    qRegisterMetaType<MobiDocument>();

    MainWindow w;
    w.show();

    return a.exec();
}
