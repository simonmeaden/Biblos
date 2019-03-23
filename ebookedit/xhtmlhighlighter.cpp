#include "xhtmlhighlighter.h"

//#include <qlogger/qlogger.h>
// using namespace qlogger;

#include "logging.h"

// XhtmlHighlighter::TagNode::TagNode()
//  : prev_node(nullptr)
//  , next_node(nullptr)
//{}

XhtmlHighlighter::XhtmlHighlighter(Options options, QTextDocument* parent)
  : QSyntaxHighlighter(parent)
  , m_error(false)
  , m_options(options)
  //  , m_tagnode(nullptr)
  , m_normal_format(new QTextCharFormat())
  , m_tag_format(new QTextCharFormat())
  , m_string_format(new QTextCharFormat())
  , m_attribute_format(new QTextCharFormat())
  , m_style_format(new QTextCharFormat())
  , m_script_format(new QTextCharFormat())
  , m_error_format(new QTextCharFormat())
{
  setNormalFormat(m_options->normalColor(),
                  m_options->normalBack(),
                  m_options->normalWeight(),
                  m_options->normalItalic());
  setTagFormat(m_options->tagColor(),
               m_options->tagBack(),
               m_options->tagWeight(),
               m_options->tagItalic());
  setErrorFormat(m_options->errorColor(),
                 m_options->errorBack(),
                 m_options->errorWeight(),
                 m_options->errorItalic());
  setAttributeFormat(m_options->attributeColor(),
                     m_options->attributeBack(),
                     m_options->attributeWeight(),
                     m_options->attributeItalic());
  setStringFormat(m_options->stringColor(),
                  m_options->stringBack(),
                  m_options->stringWeight(),
                  m_options->stringItalic());
  setStyleFormat(m_options->styleColor(),
                 m_options->styleBack(),
                 m_options->styleWeight(),
                 m_options->styleItalic());
  setScriptFormat(m_options->scriptColor(),
                  m_options->scriptBack(),
                  m_options->scriptWeight(),
                  m_options->scriptItalic());
}

XhtmlHighlighter::~XhtmlHighlighter() {}

void
XhtmlHighlighter::highlightBlock(const QString& text)
{
  /* These all need to be stored outside the method in
  case the tag is split over two or more text blocks.*/
  if (!m_tagstarted) {
    // only initialise if we have already completed the last tag.
    resetTagFlags();
  } else {
    m_current_format = m_normal_format;
  }

  for (int i = 0; i < text.length(); i++) {
    m_tagpos = i;
    QChar qc = text.at(i);
    char c = qc.toLatin1(); // allows switch on character

    if (m_error) {
      setFormat(m_tagpos, 1, *(m_current_format.get()));
      continue;
    }

    if (c != 0) { // is a latin char. all html tag names are in ASCII basically
      // (NOT quoted text)
      if (m_tagstarted) {
        switch (c) {
          case '/': { // TAG CLOSED ? maybe if not in quotes.
            if (!(m_dquot || m_squot)) {
              if (m_tagstarted && !m_namestarted) {
                // Might be a tag closer. Should be the second character in the
                // tag.
                if (m_tagpos == m_tagstartpos + 1) {
                  m_tagcloser = true;
                }
              }
              if (!m_tagcloser && m_attstarted) {
                m_attstarted = false;
                m_attcomplete = true;
                m_current_format = m_tag_format;
              }

              //              m_tagclosed = true;
              m_current_format = m_tag_format;
            }
            setFormat(m_tagpos, 1, *(m_current_format.get()));
            break;
          }
          case ' ': {
            if (!(m_dquot || m_squot)) {
              if (!m_namestarted) {
                m_current_format = m_normal_format;
              } else if (m_attstarted) {
                m_attstarted = false;
                m_attcomplete = true;
                m_current_format = m_tag_format;
              } else if (m_namestarted && !m_attstarted) {
                m_namecomplete = true;
                m_current_format = m_normal_format;
              }
            }
            setFormat(m_tagpos, 1, *(m_current_format.get()));
            break;
          }
          case '=': {
            if (!(m_squot || m_dquot)) {
              if (m_attstarted) {
                m_attstarted = false;
                m_attcomplete = true;
                m_current_format = m_tag_format;
              }
            }
            setFormat(m_tagpos, 1, *(m_current_format.get()));
            break;
          }
          case '>': {
            if (!(m_squot || m_dquot)) {
              setFormat(m_tagpos, 1, *(m_current_format.get()));
              if (m_attstarted || m_attcomplete) {
                m_attstarted = false;
                m_attcomplete = false;
                m_current_format = m_normal_format;
              }
              if (m_namestarted) {
                // basically name is finished and no attributes.
                m_namecomplete = false;
                m_namestarted = false;
                m_tagcomplete = true;
              }
              m_tagstarted = false;
            }
            // Check if these are special tags
            if (!m_tagcloser) {
              if (m_tagname == "style") {
                m_style_or_script = true;
                m_current_format = m_style_format;
              } else if (m_tagname == "script") {
                m_current_format = m_script_format;
                m_style_or_script = true;
              }
            } else
              m_tagcloser = false;
            m_tagname.clear();
            break;
          }
          case '\'': {
            if (!m_dquot) {
              // ignore double quotes inside single quotes.
              if (m_squot) {
                // end of single quoted section
                m_squot = false;
                m_current_format = m_tag_format;
              } else {
                // start of single quoted section
                m_squot = true;
                m_current_format = m_string_format;
              }
            }
            setFormat(m_tagpos, 1, *(m_current_format.get()));
            break;
          }
          case '"': {
            if (!m_squot) {
              // ignore single quotes inside double quotes.
              if (m_dquot) {
                // end of double quoted section
                m_dquot = false;
                m_current_format = m_tag_format;
              } else if (m_style_or_script) {
                // TODO ???? not certain if I need anything here.
              } else {
                // start of double quoted section
                m_dquot = true;
                m_current_format = m_string_format;
              }
            }
            setFormat(m_tagpos, 1, *(m_current_format.get()));
            break;
          }
          case '<': {
            if (!(m_squot || m_dquot)) {
              m_current_format = m_error_format;
              m_error = true;
              qCDebug(LOG_HIGHLIGHTER)
                << tr(
                     "Error parsing xhtml, second open tag character (<) in %1")
                     .arg(text);
            }
            setFormat(m_tagpos, 1, *(m_current_format.get()));
            break;
          }
          default: {
            // any non-special character in text
            if (!(m_dquot || m_squot)) {
              if (!m_namestarted) {
                // first non-special character after start tag.
                m_namestarted = true;
                m_current_format = m_tag_format;
                m_tagname += qc;
              } else if (m_namecomplete) {
                // tag name is complete
                if (m_attcomplete) {
                  m_current_format = m_tag_format;
                } else if (!m_attstarted) {
                  m_attstarted = true;
                  m_attcomplete = false;
                  m_current_format = m_attribute_format;
                }
              } else {
                m_tagname += qc;
              }
            }
            setFormat(m_tagpos, 1, *(m_current_format.get()));
            break;
          }
        } // end of switch
      } else {
        // any pre-tag or first tag character code.
        if (c == '<') {
          m_tagstarted = true;
          m_tagstartpos = m_tagpos;
          m_current_format = m_tag_format;
          m_style_or_script = false;
          setFormat(m_tagpos, 1, *(m_current_format.get()));
        } else if (m_style_or_script) {
          setFormat(m_tagpos, 1, *(m_current_format.get()));
        }
      }
    } else { // is latin char check (c == 0)
      // Not a latin character so basically not an html tag wexcept possible
      // quoted string.
      setFormat(m_tagpos, 1, *(m_current_format.get()));
    }
  }
  setCurrentBlockState(0);
}

void
XhtmlHighlighter::resetFormattingOptions()
{
  {
    m_normal_format->setFontWeight(m_options->normalWeight());
    m_normal_format->setForeground(m_options->normalColor());
    m_normal_format->setBackground(m_options->normalBack());
    m_normal_format->setFontItalic(m_options->normalItalic());
  }
  {
    m_error_format->setFontWeight(m_options->errorWeight());
    m_error_format->setForeground(m_options->errorColor());
    m_error_format->setBackground(m_options->errorBack());
    m_error_format->setFontItalic(m_options->errorItalic());
  }
  {
    m_attribute_format->setFontWeight(m_options->attributeWeight());
    m_attribute_format->setForeground(m_options->attributeColor());
    m_attribute_format->setBackground(m_options->attributeBack());
    m_attribute_format->setFontItalic(m_options->attributeItalic());
  }
  {
    m_tag_format->setFontWeight(m_options->tagWeight());
    m_tag_format->setForeground(m_options->tagColor());
    m_tag_format->setBackground(m_options->tagBack());
    m_tag_format->setFontItalic(m_options->tagItalic());
  }
  {
    m_string_format->setFontWeight(m_options->stringWeight());
    m_string_format->setForeground(m_options->stringColor());
    m_string_format->setBackground(m_options->stringBack());
    m_string_format->setFontItalic(m_options->stringItalic());
  }
  {
    m_style_format->setFontWeight(m_options->styleWeight());
    m_style_format->setForeground(m_options->styleColor());
    m_style_format->setBackground(m_options->styleBack());
    m_style_format->setFontItalic(m_options->styleItalic());
  }
  {
    m_script_format->setFontWeight(m_options->scriptWeight());
    m_script_format->setForeground(m_options->scriptColor());
    m_script_format->setBackground(m_options->scriptBack());
    m_script_format->setFontItalic(m_options->scriptItalic());
  }
}

void
XhtmlHighlighter::setNormalFormat(QColor fore,
                                  QColor back,
                                  QFont::Weight weight,
                                  bool italic)
{
  m_normal_format->setFontWeight(weight);
  m_normal_format->setForeground(fore);
  m_normal_format->setBackground(back);
  m_normal_format->setFontItalic(italic);
}

void
XhtmlHighlighter::setErrorFormat(QColor fore,
                                 QColor back,
                                 QFont::Weight weight,
                                 bool italic)
{
  m_error_format->setFontWeight(weight);
  m_error_format->setForeground(fore);
  m_error_format->setBackground(back);
  m_error_format->setFontItalic(italic);
}

void
XhtmlHighlighter::setAttributeFormat(QColor fore,
                                     QColor back,
                                     QFont::Weight weight,
                                     bool italic)
{
  m_attribute_format->setFontWeight(weight);
  m_attribute_format->setForeground(fore);
  m_attribute_format->setBackground(back);
  m_attribute_format->setFontItalic(italic);
}

void
XhtmlHighlighter::setTagFormat(QColor fore,
                               QColor back,
                               QFont::Weight weight,
                               bool italic)
{
  m_tag_format->setFontWeight(weight);
  m_tag_format->setForeground(fore);
  m_tag_format->setBackground(back);
  m_tag_format->setFontItalic(italic);
}

void
XhtmlHighlighter::setStringFormat(QColor fore,
                                  QColor back,
                                  QFont::Weight weight,
                                  bool italic)
{
  m_string_format->setFontWeight(weight);
  m_string_format->setForeground(fore);
  m_string_format->setBackground(back);
  m_string_format->setFontItalic(italic);
}

void
XhtmlHighlighter::setStyleFormat(QColor fore,
                                 QColor back,
                                 QFont::Weight weight,
                                 bool italic)
{
  m_style_format->setFontWeight(weight);
  m_style_format->setForeground(fore);
  m_style_format->setBackground(back);
  m_style_format->setFontItalic(italic);
}

void
XhtmlHighlighter::setScriptFormat(QColor fore,
                                  QColor back,
                                  QFont::Weight weight,
                                  bool italic)
{
  m_script_format->setFontWeight(weight);
  m_script_format->setForeground(fore);
  m_script_format->setBackground(back);
  m_script_format->setFontItalic(italic);
}

// void
// XhtmlHighlighter::setError(QString errorstring)
//{
//  m_current_format = m_error_format;
//  m_error = true;
//  QLOG_ERROR(errorstring);
//}

void
XhtmlHighlighter::resetTagFlags()
{
  m_tagstarted = false;
  m_tagcomplete = false;
  m_tagcloser = false;
  //  m_tagclosed = false;
  m_namestarted = false;
  m_namecomplete = false;
  m_attstarted = false;
  m_attcomplete = false;
  m_squot = false;
  m_dquot = false;
  //  m_style_or_script = false; // don't clear this it is outside a tag.
  m_tagstartpos = 0;
}
