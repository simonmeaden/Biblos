#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <QList>
#include <QObject>
#include <QSharedPointer>
#include <QStack>
#include <QWebEngineScriptCollection>

#include <htmltidy.h>
#include <tidybuffio.h>

#include "ebookcommon.h"

class EBItem
{
public:
  enum Type
  {
    NONE,
    STYLE,
    LINK,
    HTML,
    BODY,
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

  virtual QString toHtml(CSSMap styles = CSSMap(nullptr)) = 0;
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

class EBLinkTag;
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

  QString toHtml(CSSMap = CSSMap(nullptr)) override;

  virtual bool isClosableType();

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

  QString toHtml(CSSMap = CSSMap(nullptr)) override;
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

  bool isClosableType() override;

  QString toHtml(CSSMap styles = CSSMap(nullptr)) override;

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

  QString toHtml(CSSMap styles = CSSMap(nullptr)) override;
};

class EBLinkTag : public EBAlwaysClosedTag
{
public:
  EBLinkTag(Type type);

  bool isStylesheet();
  void setAttribute(QString name, QString value) override;

  QString toHtml(CSSMap styles = CSSMap(nullptr)) override;

  QString name() const;
  //  void setName(const QString &name);

  QString stylesheet() const;
  void setStylesheet(const QString& stylesheet);

protected:
  bool m_is_stylesheet;
  QString m_stylesheet;
  QString m_name;
};
typedef QSharedPointer<EBLinkTag> LinkTag;

Tag
fromTagType(EBTag::Type type);

class EBChar
  : public QChar
  , public EBItem
{
public:
  EBChar(char c);
  QChar qchar() override;
  QString toHtml(CSSMap = CSSMap(nullptr)) override;
  QString toString();
};
typedef QSharedPointer<EBChar> Char;

class EBWord : public EBItem
{
public:
  EBWord(QString word);
  QString string() override;
  QString toHtml(CSSMap = CSSMap(nullptr)) override;

  void setReplacement(const QString& replacement);

protected:
  QString m_original;
  QString m_replacement;
};
typedef QSharedPointer<EBWord> Word;

class HtmlParser : public QObject
{
  Q_OBJECT
public:
  explicit HtmlParser(QObject* parent = nullptr);
  ~HtmlParser();

  bool parse(QString name, QString text, CSSMap css_map);
  void clearParsed();

  //  QString htmlById(QString id);

  //  QString toHtml(int index);
  QString toHtml(ItemList list, CSSMap styles = CSSMap(nullptr));
  //  QString toHtml();

  bool insert(int index, ItemList list);
  bool replace(int index, ItemList list);
  bool removeAt(int index);
  bool remove(ItemList list);
  int indexOf(ItemList list);

  static int INDENT_STEP;
  static int INDENT;

  QMap<QString, QString> htmlDocumentsById() const;

signals:
  void itemRemoved(int index, ItemList list);

protected:
  ItemList m_total_list; // all pages as a single page.
  //  QStringList m_word_list; // a list of non-tag words for the spellchecker.
  QList<ItemList> m_lists; // complete web pages
  ItemListMap m_itemlist_map;
  QMap<QString, QString> m_html_document_by_id;
  QMap<QString, QString> m_css_names;

  //  bool setTagClosed(Tag& tag, const bool tag_closed);
  //  QString cleanHtml(QString html);
  void saveCharOrPunctuationTag(char c,
                                QString& data_text,
                                ItemList& item_list);
  void writeWordDataIf(QString& data_text, ItemList& item_list);
  void writeEndTag(QString& tag_text,
                   QString& data_text,
                   ItemList& item_list,
                   bool& tag_opener,
                   bool& tag_closed,
                   bool& in_style);
  //  void insertStyleSheet(const QString& name, const QString& source);
};

#endif // HTMLPARSER_H
