#include "regex.h"


Regex::Regex()
{
}

QStringList Regex::detectHtmlTags(QString data)
{
  QString pattern = "</?\w+((\s+\w+(\s*=\s*(?:\".*?\"|'.*?'|[\^'\">\s]+))?)+\s*|\s*)/?>";
  QRegularExpression regex(pattern);

}
