#include "cssparser.h"

const QRegularExpression CssParser::WHITESPACE = QRegularExpression("^\\s*");

CssParser::CssParser(QString css, QObject* parent)
  : QObject(parent)
  , m_comment_regex(QRegularExpression("(/\\*[^\\*]*\\*+([^/\\*][^\\*]*\\*+)*/)"))
  , pos(0)
  , css(css)
{}

CssParser::~CssParser() {}

int
CssParser::parse(const QString style)
{}

NodeList CssParser::parseStyle(const QString style)
{

}

long
CssParser::length()
{
  return css.length() > pos ? css.length() - pos : 0;
}

QChar
CssParser::charAt(int i) const
{
  return css.at(pos + 1);
}

NodeList CssParser::rules()
{
  NodeList rules;

}
