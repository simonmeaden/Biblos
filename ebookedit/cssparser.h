#ifndef CSSPARSER_H
#define CSSPARSER_H

#include <QObject>
#include <QRegularExpression>
#include <QString>
#include <QStringList>

#include <string>


class CssParser : public QObject
{

  struct Node {

  };
  typedef QSharedPointer<Node> node_t;
  typedef QList<node_t> NodeList;

  class Parser
  {};

  Q_OBJECT
public:
  explicit CssParser(QString css, QObject* parent = Q_NULLPTR);
  ~CssParser();

  int parse(const QString style);
  NodeList parseStyle(const QString style);



signals:

protected:
  QRegularExpression m_comment_regex;
  long pos;
  QString css;

//  bool whitespace() { return WHITESPACE.match(); }

//  long length();
//  QChar charAt(int i) const;
//  NodeList rules();
//  QRegularExpressionMatch search(const QRegularExpression& re) { return re.match(); }
//  bool search(const std::regex& re, std::smatch& m)
//  {
//    std::string::const_iterator first = css_.begin() + pos_, last = css_.end();
//    bool found = std::regex_search(first, last, m, re);
//    if (found) {
//      const std::string& s = m[0];
//      pos_ += s.length();
//    }

//    return found;
//  }

  static const QRegularExpression WHITESPACE;

};

#endif // CSSPARSER_H
