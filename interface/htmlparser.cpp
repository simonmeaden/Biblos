#include "htmlparser.h"

//#include <qlogger/qlogger.h>
// using namespace qlogger;
#include "logging.h"

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
HtmlParser::saveCharOrPunctuationTag(char c,
                                     QString& data_text,
                                     ItemList& item_list)
{
  writeWordDataIf(data_text, item_list);
  Char ch = Char(new EBChar(c));
  item_list.append(ch);
}

void
HtmlParser::writeWordDataIf(QString& data_text, ItemList& item_list)
{
  if (!data_text.isEmpty()) {
    Word word = Word(new EBWord(data_text));
    item_list.append(word);
    data_text.clear();
  }
}

void
HtmlParser::writeEndTag(QString& tag_text,
                        QString& data_text,
                        ItemList& item_list,
                        bool& tag_opener,
                        bool& tag_closed,
                        bool& in_style)
{
  EBTag::Type type = EBItem::fromString(tag_text);
  if (type != EBItem::NONE) {
    writeWordDataIf(data_text, item_list);
    EndTag end_tag = EBEndTag::fromtype(type);
    item_list.append(end_tag);
    tag_text.clear();
    tag_opener = false;
    tag_closed = false;
    in_style = false;
  }
}

// void
// HtmlParser::insertStyleSheet(const QString& name, const QString& source)
//{
//  QWebEngineScript script;
//  QString s = QString::fromLatin1("(function() {"
//                                  "    css = document.createElement('style');"
//                                  "    css.type = 'text/css';"
//                                  "    css.id = '%1';"
//                                  "    document.head.appendChild(css);"
//                                  "    css.innerText = '%2';"
//                                  "})()")
//                .arg(name)
//                .arg(source.simplified());
//  m_css_names.insert(name, s);
//}

/*!
 * \brief parsing method.
 *
 * Parse takes as a parameter a QString containing a stripped html page.
 * basically everything inside, but not including, the body tag of an html ebook
 * page.Most ebooks are split into multiple pages, normally at chapter
 * boundaries, but a single page book is also supported.
 *
 * \param name - the file name.
 * \param document_string - the html document as a QString.
 * \param css_strings - css files as strings.
 * \return
 */
bool
HtmlParser::parse(QString name, QString document_string, CSSMap css_map)
{

  // text is already pruned to inside <body> tag.
  bool tag_opener = false;
  bool tag_closed = false;
  bool in_squote = false;
  bool in_dquote = false;
  bool tag_name_finished = false;
  bool att_name_started = false;
  bool att_name_finished = false;
  bool att_value_started = false;
  bool att_equals_found = false;
  bool in_style = false;
  QString att_value, att_name;
  QString tag_text;
  QString data_text;
  QString style_text;
  QString html_doc;
  ItemList item_list;
  QStringList css_list;

  Tag tag = Tag(new EBTag(EBItem::NONE));
  // TODO handle non-proper xhtml tags.
  //  foreach (QChar qc, document_string) {
  for (int i = 0; i < document_string.length(); i++) {
    QChar qc = document_string.at(i);
    char c = qc.toLatin1();
    if (c == 0) {
      // Unicode characters.
      if (!tag_opener) { // outside of a tag
        if (qc.isSpace() || qc.isPunct()) {
          saveCharOrPunctuationTag(c, data_text, item_list);
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
          if (!in_dquote) {
            tag_closed = true;
          } else {
            if (att_value_started) {
              att_value += c;
            }
          }
          break;
        }
        case '<': {
          if (!tag.isNull()) {
            writeWordDataIf(data_text, item_list);
          }
          tag_opener = true;
          break;
        }
        case '>': {
          if (tag_opener) {
            if (tag.isNull()) {
              if (!tag_closed) {
                EBTag::Type type = EBItem::fromString(tag_text);
                if (type != EBItem::NONE) {
                  tag = fromTagType(type);
                  tag_text.clear();
                  tag_name_finished = true;
                  tag_closed = false;
                  if (type == EBItem::STYLE) {
                    in_style = true;
                  }
                }
                tag->setClosed(false);
                if (!tag->isClosableType()) {
                  // Non closable types have no end tags they either never have
                  // a closing '/', ie <br>, or always have one, ie <link/>. so
                  // we need to manually set the flags.
                  tag_opener = false;
                  tag_closed = false;
                }
                tag_opener = false;
                tag_name_finished = false;
                item_list.append(tag);
                tag = Tag(nullptr);
              } else {
                writeEndTag(tag_text,
                            data_text,
                            item_list,
                            tag_opener,
                            tag_closed,
                            in_style);
              }
            } else {
              // TODO debug should be handled elsewhere?
              if (!tag.isNull()) {
                tag_opener = false;
                tag_name_finished = false;
                item_list.append(tag);
                tag = Tag(nullptr);
              } else {
                // shouldn't ever actually happen here. Track down why?
                qCDebug(LOG_PARSER) << QStringLiteral("Empty Tag?");
              }
            }
          }
          //          tag_text.clear();
          break;
        } // end of '>' section
        case '\n':
          break;
        default: {
          if (!tag_opener) {
            // Outside a tag but still latin characters.
            if (!tag.isNull()) {
              if (tag->type() != EBTag::STYLE) {
                if (qc.isSpace() || qc.isPunct()) {
                  saveCharOrPunctuationTag(c, data_text, item_list);
                } else {
                  data_text += qc;
                }
              } else {
                if (in_style) {
                  // a style tag is treated as a special case
                  style_text += c;
                }
              }
            } else {
              if (qc.isSpace() || qc.isPunct()) {
                saveCharOrPunctuationTag(c, data_text, item_list);
              } else {
                data_text += qc;
              }
            }
          } else if (tag_closed && tag_name_finished) { // tag has closed so
                                                        // this should be text.
            if ((c >= 'a' && c <= 'z') || c == ' ' || c == '\\' || c == '=') {
              tag_text += c;
              if (tag.isNull()) {
                writeEndTag(tag_text,
                            data_text,
                            item_list,
                            tag_opener,
                            tag_closed,
                            in_style);
              }
            }
          } else if (tag_opener) {
            // inside a tag. tag name and attributes
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ' ||
                c == '=' || c == '_' || c == ':' || c == '/' || c == '.' ||
                c == '#' || (c >= '0' && c <= '9') || c == ';' || c == '-') {
              if (c == ' ') {
                if (!tag_name_finished) {
                  // first space create tag.
                  EBTag::Type type = EBItem::fromString(tag_text);
                  if (type != EBItem::NONE) {
                    tag = fromTagType(type);
                    tag_text.clear();
                    tag_name_finished = true;
                    tag_closed = false;
                    if (type == EBItem::STYLE) {
                      in_style = true;
                    }
                  }
                } else {
                  if (att_name_started) {
                    if (att_name_finished && !att_value_started) {
                      // spaces between att name and =
                      continue;
                    }
                    if (att_value_started) {
                      if (in_dquote) { // spaces not between = and first "
                        att_value += c;
                      }
                    }
                  }
                }
              } else {
                if (c == '=') { // && att_name_started && !att_name_finished) {
                  if (att_name_started) {
                    if (!att_name_finished) {
                      att_name_finished = true;
                      att_equals_found = true;
                    } else {
                      if (att_value_started) {
                        if (in_dquote || in_squote) {
                          att_value += c;
                        } else {
                          // TODO error ?
                          qCDebug(LOG_PARSER) << QStringLiteral(
                            "= att value started but not in quotes");
                        }
                      } else {
                        // TODO debug should be handled elsewhere?
                        qCDebug(LOG_PARSER) << QStringLiteral(
                          "= att value not started but att_name finished?");
                      }
                    }
                  } else {
                    // shouldn't have an = here
                    qCDebug(LOG_PARSER)
                      << QStringLiteral("= but att name not started");
                  }
                } else {
                  if (c == '"') {
                    if (att_value_started) {
                      att_value += c;
                    } else if (att_name_started) {
                      att_name_finished = true;
                      att_equals_found = true;
                    }
                  } else {

                    if (!tag_name_finished) {
                      tag_text += c;
                    } else if (att_name_started && !att_name_finished) {
                      att_name += c;
                    } else if (att_name_finished) {
                      if (in_dquote) {
                        att_value += c;
                      }
                    } else {
                      if (!att_name_started) {
                        // a non-space character between tag name and
                        // attribute.
                        att_name_started = true;
                        att_name += c;
                      }
                    }
                  }
                }
              }
            } else if (c == '\'') { // single quote inside tag.
              if (!in_dquote) {
                if (in_squote) {
                  in_squote = false;
                }
                in_squote = true;
              } else {
                // ignore double quote inside single quotes.
                tag_text += c;
              }
            } else if (c == '"') { // double quote inside tag.
              if (!in_squote) {
                if (!in_dquote) {
                  in_dquote = true;
                  if (att_equals_found) {
                    att_value_started = true;
                  }
                } else {
                  if (att_value_started) {
                    // prepare for another attribute.
                    att_name_started = false;
                    att_name_finished = false;
                    att_value_started = false;
                    att_equals_found = false;
                    if (tag.isNull()) {
                      qCWarning(LOG_PARSER)
                        << QString("Missing html attribute %1").arg(att_name);
                      continue;
                    }
                    tag->setAttribute(att_name, att_value);
                    att_value.clear();
                    att_name.clear();
                  }
                  in_dquote = false;
                }
              } else {
                // ignore single quote inside double quotes.
                tag_text += c;
              }
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
    //    m_word_list.append(word_list);
    html_doc = toHtml(item_list, css_map);
    m_html_document_by_id.insert(name, html_doc);
    return true;
  }
  return false;
}

void
HtmlParser::clearParsed()
{
  //  m_total_list.clear();
  //  m_word_list.clear();
}

// QString
// HtmlParser::htmlById(QString id)
//{
//  return m_html_document_by_id.value(id);
//}

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
HtmlParser::toHtml(ItemList list, CSSMap styles)
{
  QString html = QStringLiteral("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
  for (int i = 0; i < list.size(); i++) {
    Item item = list.at(i);
    if (!item.isNull()) {
      if (item->type() == EBItem::LINK) {
        LinkTag link_tag = item.dynamicCast<EBLinkTag>();
        if (link_tag->isStylesheet()) {
        }
      } else {
        QString item_html = item->toHtml(styles);
        html += item_html; // item->toHtml(styles);
      }
    }
  }
  //  html = cleanHtml(html);
  return html;
}

// QString
// HtmlParser::toHtml()
//{
//  QString html;
//  html += QStringLiteral("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
//  html +=
//    QStringLiteral("<html "
//                   "    xmlns=\"http://www.w3.org/1999/xhtml\"\n"
//                   "    xmlns:epub=\"http://www.idpf.org/2007/ops\"\n"
//                   "    epub:prefix=\"z3998: "
//                   "http://www.daisy.org/z3998/2012/vocab/structure/#\"\n"
//                   "
//                   xmlns:ssml=\"http://www.w3.org/2001/10/synthesis\">\n");
//  html += QStringLiteral("<head>\n");
//  html += QStringLiteral("</head>\n");
//  html += QStringLiteral("<body>\n");
//  foreach (ItemList list, m_lists) {
//    html += toHtml(list);
//  }
//  html += QStringLiteral("</body>\n");
//  return html;
//}

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

EBItem::Type
EBItem::fromString(QString type)
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
  else if (t == "style")
    return STYLE;
  else if (t == "link")
    return LINK;
  else if (t == "html")
    return HTML;
  else if (t == "head")
    return HEAD;
  else if (t == "body")
    return BODY;
  else if (t == "meta")
    return META;
  else if (t == "title")
    return TITLE;
  else
    return NONE;
  // WORD and CHAR are handled within the parser.
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

QString
EBTagBase::fromType()
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
    case I:
      return QStringLiteral("i");
    case BR:
      return QStringLiteral("br");
    case CENTER:
      return QStringLiteral("center");
    case HR:
      return QStringLiteral("hr");
    case SVG:
      return QStringLiteral("svg");
    case STYLE:
      return QStringLiteral("style");
    case LINK:
      return QStringLiteral("link");
    case HTML:
      return QStringLiteral("html");
    case BODY:
      return QStringLiteral("body");
    case HEAD:
      return QStringLiteral("head");
    case META:
      return QStringLiteral("meta");
    case TITLE:
      return QStringLiteral("title");
    case WORD:
    case CHAR:
    case NONE:
      return QString();
  }
}

/* EBTag
 * *************************************************************************/

EBTag::EBTag(EBItem::Type type)
  : EBTagBase(type)
  , m_closed(false)
{
  setIndentable(INDENT);
}

/*
 * m_closed is only used when writing the tag to html.
 * If the tag is either EBNonClosedTag or EBAlwaysClosedTag then
 * this is fixed as false.
 */
void
EBTag::setClosed(bool value)
{
  if (!isClosableType())
    m_closed = value;
}

void
EBTag::setAttribute(QString name, QString value)
{
  m_attributes.insert(name, value);
}

QString EBTag::toHtml(CSSMap)
{
  QString html;
  //  html += HtmlParser::_indent();
  if (m_type != NONE) {
    html += "<" + fromType();
    foreach (QString key, m_attributes.keys()) {
      QString value = m_attributes.value(key);
      html += QString(" %1=\"%2\"").arg(key).arg(value);
    }
    if (m_closed) {
      html += "/";
    }
    html += ">";
  }
  return html;
}

bool
EBTag::isClosableType()
{
  return true;
}

/* EBEndTag
 * **********************************************************************/

EBEndTag::EBEndTag(EBItem::Type type)
  : EBTagBase(type)
{}

QString EBEndTag::toHtml(CSSMap)
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

QString EBChar::toHtml(CSSMap)
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
  : EBItem(WORD)
  , m_original(word)
{}

QString
EBWord::string()
{
  return m_original;
}

QString EBWord::toHtml(CSSMap)
{
  QString word = string();
  return word;
}

void
EBWord::setReplacement(const QString& replacement)
{
  m_replacement = replacement;
}

/* EBWord
 * ************************************************************************/

EBStyleTag::EBStyleTag(EBItem::Type type)
  : EBTag(type)
{}

void
EBStyleTag::setStyle(QString style)
{
  style_string = style;
}

QString
EBStyleTag::style()
{
  return style_string;
}

/* EBNonClosedTag
 * ************************************************************************/

EBNonClosedTag::EBNonClosedTag(EBItem::Type type)
  : EBTag(type)
{
  m_closed = false;
}

/*
 * This flag is only true if it is NOT an EBNonClosed or EBAlwaysClosed type.
 * It cannot be set manually.
 */
bool
EBNonClosedTag::isClosableType()
{
  return false;
}

QString EBNonClosedTag::toHtml(CSSMap)
{
  QString html;
  //  html += HtmlParser::_indent();
  html += "<" + fromType();
  foreach (QString key, m_attributes.keys()) {
    QString value = m_attributes.value(key);
    html += QString(" %1=\"%2\"").arg(key).arg(value);
  }
  html += ">";
  return html;
}

/* EBAlwaysClosedTag
 * ************************************************************************/

EBAlwaysClosedTag::EBAlwaysClosedTag(EBItem::Type type)
  : EBNonClosedTag(type)
{
  m_closed = true;
}

QString EBAlwaysClosedTag::toHtml(CSSMap)
{
  QString html;
  //  html += HtmlParser::_indent();
  html += "<" + fromType();
  foreach (QString key, m_attributes.keys()) {
    QString value = m_attributes.value(key);
    html += QString(" %1=\"%2\"").arg(key).arg(value);
  }
  html += "/>";
  return html;
}

/* EBLinkTag
 * ************************************************************************/

EBLinkTag::EBLinkTag(EBItem::Type type)
  : EBAlwaysClosedTag(type)
  , m_is_stylesheet(false)
{}

bool
EBLinkTag::isStylesheet()
{
  return m_is_stylesheet;
}

void
EBLinkTag::setAttribute(QString name, QString value)
{
  QString l_name = name.toLower();
  if (l_name == QStringLiteral("rel") &&
      value.toLower() == QStringLiteral("stylesheet")) {
    m_is_stylesheet = true;
  } else if (l_name == QStringLiteral("href")) {
    m_name = value;
  }
  EBTag::setAttribute(name, value);
}

QString
EBLinkTag::toHtml(CSSMap styles)
{
  if (m_is_stylesheet) {
    QString html;
    QString href = m_attributes.value("href");
    QString stylesheet = styles->value(href);
    if (!stylesheet.isEmpty()) {
      html += QString("<style title=\"%1\">%2<\\style>")
                .arg(m_stylesheet)
                .arg(stylesheet);
    }
    return html;
  } else {
    return EBAlwaysClosedTag::toHtml(styles);
  }
}

QString
EBLinkTag::name() const
{
  return m_name;
}

QString
EBLinkTag::stylesheet() const
{
  return m_stylesheet;
}

void
EBLinkTag::setStylesheet(const QString& stylesheet)
{
  m_stylesheet = stylesheet;
}

// void EBLinkTag::setName(const QString &name)
//{
//  m_name = name;
//}

/*********************************************************************************/

Tag
fromTagType(EBItem::Type type)
{
  Tag tag;
  if (type == EBTag::STYLE)
    tag = Tag(new EBStyleTag(type));
  else if (type == EBTag::BR)
    tag = Tag(new EBNonClosedTag(type));
  else if (type == EBTag::LINK)
    tag = Tag(new EBLinkTag(type));
  else if (type == EBTag::META)
    tag = Tag(new EBAlwaysClosedTag(type));
  else
    tag = Tag(new EBTag(type));
  return tag;
}

/*********************************************************************************/
