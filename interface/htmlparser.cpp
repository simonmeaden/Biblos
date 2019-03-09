#include "htmlparser.h"

#include <qlogger/qlogger.h>
using namespace qlogger;

int HtmlParser::INDENT_STEP = 2;
int HtmlParser::INDENT = 0;

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

void
HtmlParser::setTagClosed(Tag& tag, bool& tag_closed)
{
  if (!tag.isNull()) {
    tag->setClosed(true);
    tag_closed = false;
  }
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
HtmlParser::parse(QString name, QString document_string)
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
  QString html_doc;
  ItemList item_list;
  WordList word_list;

  Tag tag;
  foreach (QChar qc, document_string) {
    char c = qc.toLatin1();
    if (c == 0) {
      // Unicode characters.
      if (!tag_opener) { // outside of a tag
        if (qc.isSpace() || qc.isPunct()) {
          if (!data_text.isEmpty()) {
            Word word = Word(new EBWord(data_text));
            item_list.append(word);
            word_list.append(word);
            data_text.clear();
          }
          Char ch = Char(new EBChar(c));
          item_list.append(ch);
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
            item_list.append(word);
            word_list.append(word);
            data_text.clear();
          }
          tag_opener = true;
          break;
        }
        case '>': {
          if (tag_opener) {
            setTagClosed(tag, tag_closed);
            tag_opener = false;
            item_list.append(tag);
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
                item_list.append(word);
                word_list.append(word);
                data_text.clear();
              }
              Char ch = Char(new EBChar(c));
              item_list.append(ch);
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
                  item_list.append(end_tag);
                  tag_text.clear();
                  tag_opener = false;
                  tag_closed = false;
                }
              }
            }
          } else if (tag_opener) {
            if ((c >= 'a' && c <= 'z') || c == ' ' || c == '=' || c == '_' ||
                c == '.' || c == '#' || (c >= '0' && c <= '9')) {
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
                  if (tag.isNull()) {
                    QLOG_ERROR(tr("Missing hml tag %1").arg(att_name));
                    continue;
                  }
                  tag->setAttribute(att_name, att_value);
                  att_value.clear();
                  att_name.clear();
                  tag_text.clear();
                }
                in_dquote = false;
                continue;
              }
              in_dquote = true;
            } /* else {
               //
             }*/
          }
        }
      } // end switch
    }   // end of latin-1 char secion.
  }     // end foreach character.
  if (!item_list.isEmpty()) {
    m_lists.append(item_list);
    m_itemlist_map.insert(name, item_list);
    m_word_list.append(word_list);
    html_doc = toHtml(item_list, 0);
    m_html_document_by_id.insert(name, html_doc);
    return true;
  }
  return false;
}

void
HtmlParser::clearParsed()
{
  //  m_total_list.clear();
  m_word_list.clear();
}

QString
HtmlParser::htmlById(QString id)
{
  return m_html_document_by_id.value(id);
}

bool
HtmlParser::insert(int index, ItemList list)
{
  if (index < 0 || index >= m_lists.size())
    return false;
  m_lists.insert(index, list);
  return true;
}

bool
HtmlParser::replace(int index, ItemList list)
{
  if (index < 0 || index >= m_lists.size())
    return false;
  m_lists.replace(index, list);
  return true;
}

bool
HtmlParser::removeAt(int index)
{
  if (index < 0 || index >= m_lists.size())
    return false;
  m_lists.removeAt(index);
  return true;
}

bool
HtmlParser::remove(ItemList list)
{
  if (m_lists.contains(list)) {
    bool result = m_lists.removeOne(list);
    return result;
  }
  return false;
}

int
HtmlParser::indexOf(ItemList list)
{
  return m_lists.indexOf(list);
}

QMap<QString, QString>
HtmlParser::htmlDocumentsById() const
{
  return m_html_document_by_id;
}

// QString
// HtmlParser::toHtml(int index)
//{
//  if (index < 0 || index >= m_lists.size())
//    return QString();
//  ItemList list = m_lists.at(index);
//  return toHtml(list, 1);
//}

QString
HtmlParser::toHtml(ItemList list, int indent)
{
  QString html;
  html += QStringLiteral("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  html +=
    QStringLiteral("<html "
                   "    xmlns=\"http://www.w3.org/1999/xhtml\"\n"
                   "    xmlns:epub=\"http://www.idpf.org/2007/ops\"\n"
                   "    epub:prefix=\"z3998: "
                   "http://www.daisy.org/z3998/2012/vocab/structure/#\"\n"
                   "    xmlns:ssml=\"http://www.w3.org/2001/10/synthesis\">\n");
  html += QStringLiteral("<head>\n");
  html += QStringLiteral("</head>\n");
  html += QStringLiteral("<body>\n");
  html += QStringLiteral("<div class=\"main\">"); // for one-page-js
  foreach (Item item, list) {
    if (!item.isNull()) {
      html += item->toHtml(indent + 1);
    }
  }
  html += QStringLiteral("</div>"); // for one-page-js
  html += QStringLiteral("</body>\n");
  return html;
}

QString
HtmlParser::toHtml()
{
  QString html;
  html += QStringLiteral("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  html +=
    QStringLiteral("<html "
                   "    xmlns=\"http://www.w3.org/1999/xhtml\"\n"
                   "    xmlns:epub=\"http://www.idpf.org/2007/ops\"\n"
                   "    epub:prefix=\"z3998: "
                   "http://www.daisy.org/z3998/2012/vocab/structure/#\"\n"
                   "    xmlns:ssml=\"http://www.w3.org/2001/10/synthesis\">\n");
  html += QStringLiteral("<head>\n");
  html += QStringLiteral("</head>\n");
  html += QStringLiteral("<body>\n");
  foreach (ItemList list, m_lists) {
    html += toHtml(list, 1);
  }
  html += QStringLiteral("</body>\n");
  return html;
}

/* EBItem
 * *************************************************************************/
EBItem::EBItem(EBItem::Type type)
{
  setType(type);
  m_indentable = UNCHANGED;
}

EBItem::~EBItem() {}

EBItem::Type
EBItem::type()
{
  return m_type;
}

void
EBItem::setIndentable(const Indentable& indentable)
{
  m_indentable = indentable;
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
  return QString();
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
{
  setIndentable(INDENT);
}

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
EBTag::toHtml(int indent)
{
  QString html;
  //  html += HtmlParser::_indent();
  html += "<" + fromType();
  foreach (QString key, m_attributes.keys()) {
    QString value = m_attributes.value(key);
    html += QString(" %1=\"%2\"\n").arg(key).arg(value);
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
EBEndTag::toHtml(int indent)
{
  QString html;
  html = "</" + fromType() + ">";
  return html;
}

/* EBChar
 * ************************************************************************/

EBChar::EBChar(char c)
  : QChar(c)
  , EBItem(Type::CHAR)
{
  setIndentable(UNCHANGED);
}

QChar
EBChar::qchar()
{
  return *this;
}

QString
EBChar::toHtml(int indent)
{
  QString html(*this);
  return html;
}

QString
EBChar::toString()
{
  QChar c(*this);
  return QString(c);
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
EBWord::toHtml(int indent)
{
  QString word = string();
  return word;
}

/*********************************************************************************/
