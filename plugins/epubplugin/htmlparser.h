#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <QList>
#include <QObject>
#include <QSharedPointer>
#include <QStack>

class EBookBase;
typedef QSharedPointer<EBookBase> Base;

class EBookBase
{
public:
  QString getData() const;
  void setData(const QString& value);

protected:
  QString data;
};

class EBookTag : public EBookBase
{
public:
};
typedef QSharedPointer<EBookTag> Tag;
typedef QList<Tag> TagList;
typedef QStack<Tag> TagStack;

class EBookEndTag : public EBookTag
{
public:
};
typedef QSharedPointer<EBookEndTag> EndTag;

class EBookSpan : public EBookTag
{
public:
};
typedef QSharedPointer<EBookSpan> Span;

class EBookEndSpan : public EBookEndTag
{
public:
};
typedef QSharedPointer<EBookEndSpan> EndSpan;

class EBookDiv : public EBookTag
{
public:
};
typedef QSharedPointer<EBookDiv> Div;

class EBookEndDiv : public EBookEndTag
{
public:
};
typedef QSharedPointer<EBookEndDiv> EndDiv;

class EBookPara : public EBookTag
{
public:
};
typedef QSharedPointer<EBookPara> Para;

class EBookEndPara : public EBookEndTag
{
public:
};
typedef QSharedPointer<EBookEndPara> EndPara;

class EBookHeader : public EBookTag
{
public:
  int headerType();
  void setHeaderType(int type);

protected:
  int m_type;
};
typedef QSharedPointer<EBookHeader> Header;

class EBookEndHeader : public EBookEndTag
{
public:
};
typedef QSharedPointer<EBookEndHeader> EndHeader;

class EBookText : public EBookBase
{
public:
  TagList getTags() const;
  void setTags(const TagList& value);
  void appendTag(Tag tag);

protected:
  TagList tags;
};
typedef QSharedPointer<EBookText> Text;
typedef QList<Text> TextList;

class HtmlParser : public QObject
{
  Q_OBJECT
public:
  enum State
  {

  };
  explicit HtmlParser(QObject* parent = nullptr);
  ~HtmlParser();

  bool parse(QString text);
  TextList getParsed();
  void clearParsed();

protected:
  TextList m_text_list;
};

#endif // HTMLPARSER_H
