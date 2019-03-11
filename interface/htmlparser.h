#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <QList>
#include <QObject>
#include <QSharedPointer>
#include <QStack>

class EBItem
{
public:
  enum Type
  {
    NONE,
    STYLE,
    LINK,
    HTML,
    HEAD,
    META,
    TITLE,
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
    IMAGE,
    STRONG,
    EM,
    SMALL,
    B,
    BR,
    CENTER,
    I,
    SUB,
    UL,
    OL,
    LI,
    DD,
    DT,
    DL,
    TABLE,
    TD,
    TH,
    TR,
    THEAD,
    TFOOT,
    TBODY,
    CAPTION,
    COL,
    COLGROUP,
    SVG,
    HR,
    CHAR,
    WORD,
  };
  enum Indentable
  {
    INDENT,
    UNDENT,
    UNCHANGED,
  };

  EBItem(Type type);
  virtual ~EBItem();
  Type type();
  Indentable indentable() { return m_indentable; }

  int level() const;
  Indentable m_indentable;

  virtual QString toHtml(int indent) = 0;
  virtual QChar qchar();
  virtual QString string();

  static Type fromString(QString type);

protected:
  Type m_type;

  void setType(Type type);
  QString toString();
  void setIndentable(const Indentable& indentable);
};
typedef QSharedPointer<EBItem> Item;
typedef QList<Item> ItemList;
typedef QMap<QString, ItemList> ItemListMap;
typedef QStack<Item> ItemStack;

class EBTagBase : public EBItem
{
public:
  EBTagBase(Type type);

protected:
  QString fromType();
};

class EBTag;
typedef QSharedPointer<EBTag> Tag;

class EBTag : public EBTagBase
{
public:
  EBTag(Type type);

  void setClosed(bool value);
  virtual void setAttribute(QString name, QString value);

  QString toHtml(int) override;

  virtual bool isNonClosing() { return false; }

protected:
  bool m_closed;
  QMap<QString, QString> m_attributes;
};
typedef QList<Tag> TagList;
typedef QStack<Tag> TagStack;

class EBEndTag;
typedef QSharedPointer<EBEndTag> EndTag;

class EBEndTag : public EBTagBase
{
public:
  EBEndTag(Type type);

  QString toHtml(int) override;
  static EndTag fromtype(Type type)
  {
    EndTag tag = EndTag(new EBEndTag(type));
    return tag;
  }
};

class EBNonClosedTag : public EBTag
{
public:
  EBNonClosedTag(Type type);

  bool isNonClosing() override { return true; }

  QString toHtml(int) override;

protected:
};

class EBStyleTag : public EBTag
{
public:
  EBStyleTag(Type type);

  void setStyle(QString style);
  QString style();

protected:
  QString style_string;
};

class EBAlwaysClosedTag : public EBNonClosedTag
{
public:
  EBAlwaysClosedTag(Type type);

  QString toHtml(int) override;
};

class EBLinkTag : public EBAlwaysClosedTag
{
public:
  EBLinkTag(Type type);

  bool isStylesheet();
  void setAttribute(QString name, QString value) override;

protected:
  bool is_stylesheet;
};

Tag
fromTagType(EBTag::Type type);

class EBChar
  : public QChar
  , public EBItem
{
public:
  EBChar(char c);
  QChar qchar() override;
  QString toHtml(int indent) override;
  QString toString();
};
typedef QSharedPointer<EBChar> Char;

class EBWord
  : public QString
  , public EBItem
{
public:
  EBWord(QString word);
  QString string() override;
  QString toHtml(int) override;

protected:
};
typedef QSharedPointer<EBWord> Word;
typedef QList<Word> WordList;

class HtmlParser : public QObject
{
  Q_OBJECT
public:
  explicit HtmlParser(QObject* parent = nullptr);
  ~HtmlParser();

  bool parse(QString name, QString text, QMap<QString, QString> css_map);
  void clearParsed();

  QString htmlById(QString id);

  //  QString toHtml(int index);
  QString toHtml(ItemList list, QStringList styles = QStringList());
  //  QString toHtml();

  bool insert(int index, ItemList list);
  bool replace(int index, ItemList list);
  bool removeAt(int index);
  bool remove(ItemList list);
  int indexOf(ItemList list);

  static int INDENT_STEP;
  static int INDENT;

  //  static QString _indent(int index)
  //  {
  //    QString indent;
  //    for (int i = 0; i < (INDENT_STEP * INDENT); i++) {
  //      indent += " ";
  //    }
  //    return indent;
  //  }

  QMap<QString, QString> htmlDocumentsById() const;

signals:
  void itemRemoved(int index, ItemList list);

protected:
  ItemList m_total_list;
  WordList m_word_list;
  QList<ItemList> m_lists;
  ItemListMap m_itemlist_map;
  QMap<QString, QString> m_html_document_by_id;

  void setTagClosed(Tag& tag, bool& tag_closed);
};

#endif // HTMLPARSER_H
