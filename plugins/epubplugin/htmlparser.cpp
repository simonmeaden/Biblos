#include "htmlparser.h"

HtmlParser::HtmlParser(QObject* parent)
  : QObject(parent)
{}

HtmlParser::~HtmlParser() {}

bool
HtmlParser::parse(QString document_string)
{
  // text is already pruned to inside <body> tag.
  bool tag_opener = false;
  //  bool tag_closer = false;
  bool tag_closed = false;
  bool span_tag = false;
  bool div_tag = false;
  bool p_tag = false;
  bool end_span_tag = false;
  bool end_div_tag = false;
  bool end_p_tag = false;
  bool h_tag;
  bool end_h_tag;
  int h_type;

  QString tag_text, in_p, in_div, in_span, data_text /*, id, class_*/;
  Text text;
  TagStack tag_stack;
  foreach (QChar c, document_string) {
    if (text.isNull())
      text = Text(new EBookText());

    switch (c.toLatin1()) {
      case '/':
        tag_closed = true;
        break;
      case '<':
        if (!data_text.isEmpty()) {
          text->setData(data_text);
          data_text.clear();
        }
        tag_opener = true;
        break;
      case '>':
        if (tag_closed) {
          tag_opener = false;
          tag_closed = false;
          if (span_tag) {
            span_tag = false;
            Span tag = Span(new EBookSpan());
            text->appendTag(tag);
            EndSpan end_tag = EndSpan(new EBookEndSpan());
            text->appendTag(end_tag);
          } else if (div_tag) {
            div_tag = false;
            Div tag = Div(new EBookDiv());
            text->appendTag(tag);
            EndDiv end_tag = EndDiv(new EBookEndDiv());
            text->appendTag(end_tag);
          } else if (p_tag) {
            p_tag = false;
            Para tag = Para(new EBookPara());
            text->appendTag(tag);
            EndPara end_tag = EndPara(new EBookEndPara());
            text->appendTag(end_tag);
          } else if (h_tag) {
            h_tag = false;
            Header tag = Header(new EBookHeader());
            text->appendTag(tag);
            EndHeader end_tag = EndHeader(new EBookEndHeader());
            text->appendTag(end_tag);
          } else if (end_span_tag) {
            end_span_tag = false;
            EndSpan end_tag = EndSpan(new EBookEndSpan());
            text->appendTag(end_tag);
            tag_stack.pop();
          } else if (end_div_tag) {
            end_div_tag = false;
            EndDiv end_tag = EndDiv(new EBookEndDiv());
            text->appendTag(end_tag);
            tag_stack.pop();
          } else if (end_p_tag) {
            end_p_tag = false;
            EndPara end_tag = EndPara(new EBookEndPara());
            text->appendTag(end_tag);
            tag_stack.pop();
          } else if (end_h_tag) {
            end_h_tag = false;
            EndHeader end_tag = EndHeader(new EBookEndHeader());
            text->appendTag(end_tag);
            tag_stack.pop();
          }
          if (tag_stack.isEmpty()) {
            m_text_list.append(text);
            text = Text(new EBookText());
          }
        } else if (tag_opener) {
          tag_opener = false;
          if (span_tag) {
            span_tag = false;
            Span tag = Span(new EBookSpan());
            text->appendTag(tag);
            tag_stack.push(tag);
          } else if (div_tag) {
            div_tag = false;
            Div tag = Div(new EBookDiv());
            text->appendTag(tag);
            tag_stack.push(tag);
          } else if (p_tag) {
            p_tag = false;
            Para tag = Para(new EBookPara());
            text->appendTag(tag);
            tag_stack.push(tag);
          } else if (h_tag) {
            h_tag = false;
            Header tag = Header(new EBookHeader());
            text->appendTag(tag);
            tag_stack.push(tag);
          }
        }
        // TODO parse tag?
        tag_text.clear();
        break;
      default:
        if (!tag_opener) {
          data_text += c;
        } else if (tag_closed) {
          if ((c >= 'a' && c <= "z") || c == " " || c == "\"" || c == "=") {
            tag_text += c;
            if (tag_text == "span") {
              end_span_tag = true;
            } else if (tag_text == "div") {
              end_div_tag = true;
            } else if (tag_text == "p") {
              end_p_tag = true;
            } else if (tag_text == "h1" || tag_text == "h2" ||
                       tag_text == "h3" || tag_text == "h4" ||
                       tag_text == "h5" || tag_text == "h6") {
              end_h_tag = true;
            }
            break;
          } else if (tag_opener) {
            if ((c >= 'a' && c <= "z") || c == " " || c == "\"" || c == "=") {
              tag_text += c;
              if (tag_text == "span") {
                span_tag = true;
              } else if (tag_text == "div") {
                div_tag = true;
              } else if (tag_text == "p") {
                p_tag = true;
              } else if (tag_text == "h1" || tag_text == "h2" ||
                         tag_text == "h3" || tag_text == "h4" ||
                         tag_text == "h5" || tag_text == "h6") {
                h_tag = true;
                if (tag_text == "h1") {
                  h_type = 1;
                } else if (tag_text == "h2") {
                  h_type = 2;
                } else if (tag_text == "h3") {
                  h_type = 3;
                } else if (tag_text == "h4") {
                  h_type = 4;
                } else if (tag_text == "h5") {
                  h_type = 5;
                } else if (tag_text == "h6") {
                  h_type = 6;
                }
              }
            }
          }
        }
    }
    if (tag_stack.isEmpty())
      return true;
    else
      // uneven number of tags.
      return false;
  }

  TextList HtmlParser::getParsed() { return m_text_list; }

  void HtmlParser::clearParsed() { m_text_list.clear(); }

  QString EBookBase::getData() const { return data; }

  void EBookBase::setData(const QString& value) { data = value; }

  TagList EBookText::getTags() const { return tags; }

  void EBookText::setTags(const TagList& value) { tags = value; }

  void EBookText::appendTag(Tag tag) { tags.append(tag); }

  int EBookHeader::headerType() { return m_type; }

  void EBookHeader::setHeaderType(int type) { m_type = type; }
