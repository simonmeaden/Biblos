#ifndef REGEX_H
#define REGEX_H

#include <QString>
#include <QStringList>
#include <QRegularExpression>

#include "regex_global.h"

class REGEXSHARED_EXPORT Regex
{

public:
  Regex();

  QStringList detectHtmlTags(QString data);

};

#endif // REGEX_H
