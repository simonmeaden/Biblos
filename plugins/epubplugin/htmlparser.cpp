#include "htmlparser.h"

/*!
 * \class HtmlParser
 * \brief A simple html parser.
 *
 * This parser is not intended for general use; it is intended to parse ebook
 * html files for use in the Biblos  editor. As such it splits the html file
 * into useable segments such as html tags, punctuation characters and word.
 *
 * Please note that so far only space/punctuation separated words are supported.
 * Those languages that are do not use space supported words such as  Chinese
 * and Japanese among others are not as yet supported. Maybe later when I have
 * everything else  working.
 */

/*!
 * \brief A simple html parser class.
 * \param parent - The QObject parent of this class.
 */
HtmlParser::HtmlParser(QObject* parent)
  : QObject(parent)
{}

HtmlParser::~HtmlParser() {}

// void
// HtmlParser::setTagId(Tag& tag, QString& in_id)
//{
//  if (!in_id.isEmpty()) {
//    tag->setId(in_id);
//    in_id.clear();
//  }
//}

// void
// HtmlParser::setTagClass(Tag& tag, QString& in_class)
//{
//  if (!in_class.isEmpty()) {
//    tag->setClass(in_class);
//    in_class.clear();
//  }
//}

// void
// HtmlParser::setTagHref(Tag& tag, QString& in_href)
//{
//  if (!in_href.isEmpty()) {
//    tag->setHref(in_href);
//    in_href.clear();
//  }
//}

// void HtmlParser::setTagSrc(Tag &tag, QString &in_src)
//{
//  if (!in_src.isEmpty()) {
//    tag->setHref(in_src);
//    in_src.clear();
//  }
//}

// void HtmlParser::setTagWidth(Tag &tag, QString &in_width)
//{
//  if (!in_width.isEmpty()) {
//    tag->setWidth(in_width);
//    in_width.clear();
//  }
//}

// void HtmlParser::setTagHeight(Tag &tag, QString &in_height)
//{
//  if (!in_height.isEmpty()) {
//    tag->setHeight(in_height);
//    in_height.clear();
//  }
//}

// void HtmlParser::setTagAlt(Tag &tag, QString &in_alt)
//{
//  if (!in_alt.isEmpty()) {
//    tag->setAlt(in_alt);
//    in_alt.clear();
//  }
//}

void
HtmlParser::setTagClosed(Tag& tag, bool& tag_closed)
{
  tag->setClosed(true);
  tag_closed = false;
}

/*!
 * \brief parsing method.
 *
 * Parse takes as a parameter a QString containing a stripped html page.
 * basically everything inside, but not including, the body tag of an html ebook
 * page.Most ebooks are split into multiple pages, normally at chapter
 * boundaries, but a single page book is also supported.
 *
 * \param document_string
 * \return
 */

bool
HtmlParser::parse(QString document_string)
{
  // text is already pruned to inside <body> tag.
  bool tag_opener = false;
  bool tag_closed = false;
  bool in_squote = false;
  bool in_dquote = false;
  bool att_start = false;
  QString att_value, att_name;
  QString tag_text;
  QString data_text;

  Tag tag;
  foreach (QChar qc, document_string) {
    char c = qc.toLatin1();
    if (c == 0) {
      // Unicode characters.
      if (!tag_opener) { // outside of a tag
        if (qc.isSpace() || qc.isPunct()) {
          if (!data_text.isEmpty()) {
            Word word = Word(new EBWord(data_text));
            m_item_list.append(word);
            m_word_list.append(word);
            data_text.clear();
          }
          Char ch = Char(new EBChar(c));
          m_item_list.append(ch);
        } else {
          data_text += qc;
        }
      } else {                        // inside a tag.
        if (in_squote || in_dquote) { // non latin chars inside quotes allowed.
          tag_text += qc;
          continue;
        } else {
          // TODO throw an error
          // Actually there shouldn't be ANY unicode characters inside a tag
          // if not inside quotes so throw an error.
        }
      }
      // TODO - other special case unicode characters
    } else { // latin 1 section
      // html tags are always latin text chars except within quotation marks.
      switch (c) {
        case '/': {
          tag_closed = true;
          break;
        }
        case '<': {
          if (!data_text.isEmpty()) {
            Word word = Word(new EBWord(data_text));
            m_item_list.append(word);
            m_word_list.append(word);
            data_text.clear();
          }
          tag_opener = true;
          break;
        }
        case '>': {
          if (tag_opener) {
            setTagClosed(tag, tag_closed);
            tag_opener = false;
            m_item_list.append(tag);
            tag = Tag(nullptr);
          }
          tag_text.clear();
          break;
        } // end of '>' section
        default: {
          if (!tag_opener) { // Not a tag but still latin characters.
            if (qc.isSpace() || qc.isPunct()) {
              if (!data_text.isEmpty()) {
                Word word = Word(new EBWord(data_text));
                m_item_list.append(word);
                m_word_list.append(word);
                data_text.clear();
              }
              Char ch = Char(new EBChar(c));
              m_item_list.append(ch);
            } else {
              data_text += qc;
            }
          } else if (tag_closed) {
            if ((c >= 'a' && c <= 'z') || c == ' ' || c == '\\' || c == '=') {
              tag_text += c;
              if (tag.isNull()) {
                EBTag::Type type = EBItem::fromString(tag_text);
                if (type != EBItem::NONE) {
                  EndTag end_tag = EBEndTag::fromtype(type);
                  m_item_list.append(end_tag);
                  tag_text.clear();
                  tag_opener = false;
                  tag_closed = false;
                }
              }
            }
          } else if (tag_opener) {
            if ((c >= 'a' && c <= 'z') || c == ' ' || c == '=' ||
                (c >= '0' && c <= '9')) {
              if (c == ' ') // ignore spaces in tags.
                continue;
              tag_text += c;
              if (in_dquote) {
                if (att_start) {
                  att_value += c;
                }
              }
              if (tag.isNull()) {
                EBTag::Type type = EBItem::fromString(tag_text);
                if (type != EBItem::NONE) {
                  tag = EBTag::fromtype(type);
                  tag_text.clear();
                }
              }

              if (tag_text.contains("=")) {
                att_start = true;
                att_name = tag_text.left(tag_text.length() - 1);
                tag_text.clear();
              }
            } else if (c == '\'') { // single quote inside tag.
              if (in_dquote) {
                // ignore double quote inside single quotes.
                tag_text += c;
                continue;
              }
              if (in_squote) {
                in_squote = false;
              }
              in_squote = true;
            } else if (c == '"') { // double quote inside tag.
              if (in_squote) {
                // ignore single quote inside double quotes.
                tag_text += c;
                continue;
              }
              if (in_dquote) {
                if (att_start) {
                  att_start = false;
                  tag->setAttribute(att_name, att_value);
                  att_value.clear();
                  att_name.clear();
                  tag_text.clear();
                }
                in_dquote = false;
                continue;
              }
              in_dquote = true;
            } else {
            }
          }
        }
      } // end switch
    }   // end of latin-1 char secion.
  }     // end foreach character.
  if (!m_item_list.isEmpty())
    return true;
  return false;
}

ItemList
HtmlParser::getParsed()
{
  return m_item_list;
}

WordList
HtmlParser::getWordList()
{
  return m_word_list;
}

void
HtmlParser::clearParsed()
{
  m_item_list.clear();
}

ParsedItems::ParsedItems(QObject* parent)
  : QObject(parent)
{}

void
ParsedItems::append(ItemList list)
{
  m_total_list.append(list);
  m_lists.append(list);
  reorderItems();
}

void
ParsedItems::append(WordList list)
{
  m_word_list.append(list);
  reorderItems();
}

bool
ParsedItems::insert(int index, ItemList list)
{
  if (index < 0 || index >= m_lists.size())
    return false;
  m_lists.insert(index, list);
  reorderItems();
  return true;
}

bool
ParsedItems::replace(int index, ItemList list)
{
  if (index < 0 || index >= m_lists.size())
    return false;
  m_lists.replace(index, list);
  reorderItems();
  return true;
}

bool
ParsedItems::removeAt(int index)
{
  if (index < 0 || index >= m_lists.size())
    return false;
  m_lists.removeAt(index);
  reorderItems();
  return true;
}

bool
ParsedItems::remove(ItemList list)
{
  if (m_lists.contains(list)) {
    bool result = m_lists.removeOne(list);
    reorderItems();
    return result;
  }
  return false;
}

int
ParsedItems::indexOf(ItemList list)
{
  return m_lists.indexOf(list);
}

QString
ParsedItems::toHtml(int index)
{
  if (index < 0 || index >= m_lists.size())
    return QString();
  ItemList list = m_lists.at(index);
  return toHtml(list);
}

QString
ParsedItems::toHtml(ItemList list)
{
  QString html;
  foreach (Item item, list) {
    html += item->toHtml();
  }
  return html;
}

QString
ParsedItems::toHtml()
{
  QString html;
  foreach (ItemList list, m_lists) {
    html += toHtml(list);
  }
  return html;
}

void
ParsedItems::reorderItems()
{
  ItemList total_list;
  foreach (ItemList list, m_lists) {
    total_list.append(list);
  }
  m_total_list = total_list;
  emit orderChanged();
}

/* EBItem
 * *************************************************************************/
EBItem::EBItem(EBItem::Type type)
{
  setType(type);
}

EBItem::~EBItem() {}

EBItem::Type
EBItem::type()
{
  return m_type;
}

QChar
EBItem::qchar()
{
  return QChar();
}

QString
EBItem::string()
{
  return QString();
}

void
EBItem::setType(EBItem::Type type)
{
  m_type = type;
}

QString
EBItem::toString()
{
  switch (m_type) {
    case SPAN:
      return QStringLiteral("span");
    case DIV:
      return QStringLiteral("div");
    case P:
      return QStringLiteral("p");
    case H1:
      return "h1";
    case H2:
      return "h2";
    case H3:
      return "h3";
    case H4:
      return "h4";
    case H5:
      return "h5";
    case H6:
      return "h6";
    case A:
      return "a";
    case IMG:
      return "img";
    case CHAR:
      return QString(qchar());
    case WORD:
      return string();
      //      default : return QString();
  }
}

/* EBTagBase
 * *********************************************************************/

EBTagBase::EBTagBase(EBItem::Type type)
  : EBItem(type)
{}

/* EBTag
 * *************************************************************************/

EBTag::EBTag(EBItem::Type type)
  : EBTagBase(type)
  , m_closed(false)
{}

void
EBTag::setClosed(bool value)
{
  m_closed = value;
}

void
EBTag::setAttribute(QString name, QString value)
{
  m_attributes.insert(name, value);
}

QString
EBTag::toHtml()
{
  QString html = "<" + toString();
  foreach (QString key, m_attributes.keys()) {
    QString value = m_attributes.value(key);
    html += QString(" %1=\"%2\"").arg(key).arg(value);
  }
  if (m_closed) {
    html += "/";
  }
  html += ">";
  return html;
}

/* EBEndTag
 * **********************************************************************/

EBEndTag::EBEndTag(EBItem::Type type)
  : EBTagBase(type)
{}

QString
EBEndTag::toHtml()
{
  QString html = "</" + toString() + ">";
  return html;
}

/* EBChar
 * ************************************************************************/

EBChar::EBChar(char c)
  : QChar(c)
  , EBItem(Type::CHAR)
{}

QChar
EBChar::qchar()
{
  return *this;
}

QString
EBChar::toHtml()
{
  QString html = toString();
  return html;
}

/* EBWord
 * ************************************************************************/

EBWord::EBWord(QString word)
  : QString(word)
  , EBItem(WORD)
{}

QString
EBWord::string()
{
  return *this;
}

QString
EBWord::toHtml()
{
  QString html = string();
}

QTextCursor
EBWord::cursor() const
{
  return m_cursor;
}

void
EBWord::setCursor(const QTextCursor& cursor)
{
  m_cursor = cursor;
}

/*********************************************************************************/
