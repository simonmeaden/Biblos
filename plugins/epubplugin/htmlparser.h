#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <QList>
#include <QObject>
#include <QSharedPointer>
#include <QStack>
#include <QTextCursor>

class EBItem
{
public:
  enum Type
  {
    NONE,
    SPAN,
    DIV,
    P,
    H1,
    H2,
    H3,
    H4,
    H5,
    H6,
    A,
    IMG,
    CHAR,
    WORD,
  };
  EBItem(Type type);
  virtual ~EBItem();
  Type type();

  int level() const;

  virtual QString toHtml() = 0;
  virtual QChar qchar();
  virtual QString string();

  static Type fromString(QString type)
  {
    QString t = type.toLower();
    if (t == "span")
      return SPAN;
    else if (t == "div")
      return DIV;
    else if (t == "p")
      return P;
    else if (t == "a")
      return A;
    else if (t == "h1")
      return H1;
    else if (t == "h2")
      return H2;
    else if (t == "h3")
      return H3;
    else if (t == "h4")
      return H4;
    else if (t == "h5")
      return H5;
    else if (t == "h6")
      return H6;
    else if (t == "img")
      return IMG;
    else if (t == "word")
      return WORD;
    else if (t == "char")
      return CHAR;
    else
      return NONE;
  }

protected:
  Type m_type;

  void setType(Type type);
  QString toString();
};
typedef QSharedPointer<EBItem> Item;
typedef QList<Item> ItemList;
typedef QStack<Item> ItemStack;

class EBTagBase : public EBItem
{
public:
  EBTagBase(Type type);
};

class EBTag;
typedef QSharedPointer<EBTag> Tag;

class EBTag : public EBTagBase
{
public:
  EBTag(Type type);

  void setClosed(bool value);
  void setAttribute(QString name, QString value);

  QString toHtml() override;

  static Tag fromtype(Type type)
  {
    Tag tag = Tag(new EBTag(type));
    return tag;
  }

protected:
  QMap<QString, QString> m_attributes;
  bool m_closed;
};
typedef QList<Tag> TagList;
typedef QStack<Tag> TagStack;

class EBEndTag;
typedef QSharedPointer<EBEndTag> EndTag;

class EBEndTag : public EBTagBase
{
public:
  EBEndTag(Type type);

  QString toHtml() override;

  static EndTag fromtype(Type type)
  {
    EndTag tag = EndTag(new EBEndTag(type));
    return tag;
  }
};

class EBChar
  : public QChar
  , public EBItem
{
public:
  EBChar(char c);
  QChar qchar() override;
  QString toHtml() override;
};
typedef QSharedPointer<EBChar> Char;

class EBWord
  : public QString
  , public EBItem
{
public:
  EBWord(QString word);
  QString string() override;
  QString toHtml() override;

  QTextCursor cursor() const;
  void setCursor(const QTextCursor& cursor);

protected:
  QTextCursor m_cursor;
};
typedef QSharedPointer<EBWord> Word;
typedef QList<Word> WordList;

class HtmlParser : public QObject
{
  Q_OBJECT
public:
  explicit HtmlParser(QObject* parent = nullptr);
  ~HtmlParser();

  bool parse(QString text);
  ItemList getParsed();
  WordList getWordList();
  void clearParsed();

protected:
  ItemList m_item_list;
  WordList m_word_list;
  void setTagClosed(Tag& tag, bool& tag_closed);
};

class ParsedItems : public QObject
{
  Q_OBJECT
public:
  explicit ParsedItems(QObject* parent = nullptr);

  void append(ItemList list);
  bool insert(int index, ItemList list);
  bool replace(int index, ItemList list);
  bool removeAt(int index);
  bool remove(ItemList list);
  int indexOf(ItemList list);
  QString toHtml(int index);
  QString toHtml(ItemList list);
  QString toHtml();

  void append(WordList list);

signals:
  void orderChanged();
  void itemRemoved(int index, ItemList list);

protected:
  ItemList m_total_list;
  WordList m_word_list;
  QList<ItemList> m_lists;

  void reorderItems();
};

#endif // HTMLPARSER_H
