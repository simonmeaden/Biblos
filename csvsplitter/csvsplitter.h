#ifndef CSVSPLITTER_H
#define CSVSPLITTER_H

#include "csvsplitter_global.h"

#include <QString>
#include <QChar>
#include <QStringList>

class CSVSPLITTERSHARED_EXPORT CSVSplitter {

    static const QChar SINGLE_QUOTE;
    static const QChar DOUBLE_QUOTE;
    static const QChar COMMA;
    static const QChar GRAVE_ACCENT;
    static const QChar ACUTE_ACCENT;
    static const QChar NEWLINE;
    static const QChar COLON;
    static const QChar SEMICOLON;
    static const QChar SPACE;
    static const QChar SLASH;
    static const QChar BACKSLASH;

  public:
//    CSVSplitter(QObject *parent = nullptr);

    static QStringList split(const QString data, const QChar delimiter);
    static QStringList splitOnComma(const QString data);
    static QStringList splitOnColon(const QString data);
    static QStringList splitOnSpace(const QString data);
    static QStringList splitOnSemicolon(const QString data);

};

#endif // CSVSPLITTER_H
