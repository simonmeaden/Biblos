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
  //  virtual int indent(int indent) { return indent; }

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
    else if (t == "image")
      return IMAGE;
    else if (t == "sub")
      return SUB;
    else if (t == "ul")
      return UL;
    else if (t == "ol")
      return OL;
    else if (t == "li")
      return LI;
    else if (t == "dd")
      return DD;
    else if (t == "dt")
      return DT;
    else if (t == "dl")
      return DL;
    else if (t == "table")
      return TABLE;
    else if (t == "td")
      return TD;
    else if (t == "th")
      return TH;
    else if (t == "tr")
      return TR;
    else if (t == "thead")
      return THEAD;
    else if (t == "tfoot")
      return TFOOT;
    else if (t == "tbody")
      return TBODY;
    else if (t == "caption")
      return CAPTION;
    else if (t == "col")
      return COL;
    else if (t == "colgroup")
      return COLGROUP;
    else if (t == "strong")
      return STRONG;
    else if (t == "small")
      return SMALL;
    else if (t == "em")
      return EM;
    else if (t == "b")
      return B;
    else if (t == "br")
      return BR;
    else if (t == "center")
      return CENTER;
    else if (t == "hr")
      return HR;
    else if (t == "svg")
      return SVG;
    else
      return NONE;
    // WORD and CHAR are handled within the parser.
  }

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
  QString fromType()
  {
    switch (m_type) {
      case SPAN:
        return QStringLiteral("span");
      case DIV:
        return QStringLiteral("div");
      case P:
        return QStringLiteral("p");
      case A:
        return QStringLiteral("a");
      case H1:
        return QStringLiteral("h1");
      case H2:
        return QStringLiteral("h2");
      case H3:
        return QStringLiteral("h3");
      case H4:
        return QStringLiteral("h4");
      case H5:
        return QStringLiteral("h5");
      case H6:
        return QStringLiteral("h6");
      case IMG:
        return QStringLiteral("img");
      case IMAGE:
        return QStringLiteral("image");
      case SUB:
        return QStringLiteral("sub");
      case UL:
        return QStringLiteral("ul");
      case OL:
        return QStringLiteral("ol");
      case LI:
        return QStringLiteral("li");
      case DD:
        return QStringLiteral("dd");
      case DT:
        return QStringLiteral("dt");
      case DL:
        return QStringLiteral("dl");
      case TABLE:
        return QStringLiteral("table");
      case TD:
        return QStringLiteral("td");
      case TH:
        return QStringLiteral("th");
      case TR:
        return QStringLiteral("tr");
      case THEAD:
        return QStringLiteral("thead");
      case TFOOT:
        return QStringLiteral("tfoot");
      case TBODY:
        return QStringLiteral("tbody");
      case CAPTION:
        return QStringLiteral("caption");
      case COL:
        return QStringLiteral("col");
      case COLGROUP:
        return QStringLiteral("colgroup");
      case STRONG:
        return QStringLiteral("strong");
      case SMALL:
        return QStringLiteral("small");
      case EM:
        return QStringLiteral("em");
      case B:
        return QStringLiteral("b");
      case BR:
        return QStringLiteral("br");
      case CENTER:
        return QStringLiteral("center");
      case HR:
        return QStringLiteral("hr");
      case SVG:
        return QStringLiteral("svg");
      default:
        return QString();
    }
  }
};

class EBTag;
typedef QSharedPointer<EBTag> Tag;

class EBTag : public EBTagBase
{
public:
  EBTag(Type type);

  void setClosed(bool value);
  void setAttribute(QString name, QString value);

  QString toHtml(int indent) override;

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

  QString toHtml(int indent) override;
  //  int indent(int indent)
  //  {
  //    if (indent > 0)
  //      return indent - 1;
  //  }
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
  QString toHtml(int indent) override;

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

  bool parse(QString name, QString text);
  void clearParsed();

  QString htmlById(QString id);

  //  QString toHtml(int index);
  QString toHtml(ItemList list, int indent);
  QString toHtml();

  bool insert(int index, ItemList list);
  bool replace(int index, ItemList list);
  bool removeAt(int index);
  bool remove(ItemList list);
  int indexOf(ItemList list);

  static int INDENT_STEP;
  static int INDENT;

  static QString _indent(int index)
  {
    QString indent;
    for (int i = 0; i < (INDENT_STEP * INDENT); i++) {
      indent += " ";
    }
    return indent;
  }

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
