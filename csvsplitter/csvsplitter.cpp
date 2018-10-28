#include "csvsplitter.h"

const QChar CSVSplitter::SINGLE_QUOTE = QChar(0x27);
const QChar CSVSplitter::DOUBLE_QUOTE = QChar(0x22);
const QChar CSVSplitter::COMMA = QChar(0x2C);
const QChar CSVSplitter::GRAVE_ACCENT = QChar(0x60);
const QChar CSVSplitter::ACUTE_ACCENT = QChar(0xB4);
const QChar CSVSplitter::NEWLINE = QChar(0x0D);
const QChar CSVSplitter::COLON = QChar(0x3A);
const QChar CSVSplitter::SEMICOLON = QChar(0x3B);
const QChar CSVSplitter::SPACE = QChar(0x20);
const QChar CSVSplitter::SLASH = QChar(0x2F);
const QChar CSVSplitter::BACKSLASH = QChar(0x5C);


QStringList CSVSplitter::split(const QString data, const QChar delimiter) {
    bool quote_detected = false;
    bool quote_completed = false;
//    bool delim_detected = false;
    QStringList list;
    QString s;
    QChar c;

    for (int i = 0; i < data.length(); i++) {
        c = data[i];
//        int v = c.unicode();

        if (c == CSVSplitter::DOUBLE_QUOTE) {
            if (quote_detected) {
                quote_detected = false;
                quote_completed = true;

            } else {
                quote_detected = true;
            }

        } else if (c == CSVSplitter::SINGLE_QUOTE) {
            // A single quote character inside a double quoted section
            // needs to be escaped in CPP.
//            if (quote_detected && !quote_completed) {
                s += CSVSplitter::BACKSLASH;
                s += c;
//            }

        } else if (c == delimiter) {
//            delim_detected = true;

            if (quote_detected && !quote_completed) {
                // comma within quotes, just treat as a character.
                // just add char to s.
                s += c;

            } else if (!quote_detected && quote_completed) {
                list += s;
                s.clear();
                quote_detected = false;
                quote_completed = false;
//                delim_detected = false;

            } else {
                if (quote_completed) {
                    // ignore characters until delimiter
                    // TODO more work in here ???
                } else {
                    list += s;
                    s.clear();
//                    delim_detected = false;
                }
            }

        } else if (c == '\r' || c == '\n') {
            // Final section between last delimiter and end of line
            list += s;
            return list;

        } else {
            //            if (quote_completed)
            s += c;
        }
    }

    // Should only reach here if the last line has no newline characters at the end.
    if (quote_detected) {
        list += s;
    }

    return list;
}

QStringList CSVSplitter::splitOnComma(const QString data) {
    return split(data, CSVSplitter::COMMA);
}

QStringList CSVSplitter::splitOnColon(const QString data) {
    return split(data, CSVSplitter::COLON);
}

QStringList CSVSplitter::splitOnSpace(const QString data) {
    return split(data, CSVSplitter::SPACE);
}

QStringList CSVSplitter::splitOnSemicolon(const QString data) {
    return split(data, CSVSplitter::SEMICOLON);
}
