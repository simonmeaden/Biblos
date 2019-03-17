#ifndef XHTMLHIGHLIGHTER_H
#define XHTMLHIGHLIGHTER_H

#include <QObject>
#include <QRegularExpression>
#include <QStack>
#include <QSyntaxHighlighter>

#include "ebookcommon.h"
#include "options.h"

class XhtmlHighlighter : public QSyntaxHighlighter
{
  Q_OBJECT

  typedef QSharedPointer<QTextCharFormat> format_t;
  //  struct Attribute
  //  {
  //    int start, end, value_start, value_end;
  //    format_t att_format;
  //    format_t value_format;
  //    format_t eq_format;
  //    QString name;
  //    QString value;
  //    int pos, eq_pos, value_pos;
  //    bool started, completed;
  //  };
  //  typedef QSharedPointer<Attribute> attribute_t;
  //  typedef QList<attribute_t> AttributeList;

  //  class TagNode;
  //  typedef QSharedPointer<TagNode> node_t;
  //  class TagNode
  //  {

  //  public:
  //    TagNode(node_t next, node_t prev)
  //      : started(false)
  //      , completed(false)
  //      , closed(false)
  //      , name_started(false)
  //      , name_completed(false)
  //      , prev_node(prev)
  //      , next_node(next)
  //    {}
  //    TagNode(TagNode& node)
  //    {
  //      started = node.started;
  //      completed = node.completed;
  //      closed = node.closed;
  //      name_started = node.name_started;
  //      name_completed = node.name_completed;
  //      tagname = node.tagname;
  //      format = node.format;
  //      attributes = node.attributes;
  //      next_node = node.next_node;
  //      prev_node = node.prev_node;
  //    }

  //    QString tagname;
  //    bool started, completed, closed, name_started, name_completed;
  //    format_t format;
  //    int open_pos, close_pos, closer_pos;
  //    int name_pos, current_att;

  //    node_t prev_node /* = node_t(nullptr)*/;
  //    node_t next_node /* = node_t(nullptr)*/;
  //    AttributeList attributes;
  //  };

public:
  XhtmlHighlighter(Options options, QTextDocument* parent = nullptr);
  ~XhtmlHighlighter() override;

  void resetFormattingOptions();
  void setNormalFormat(QColor fore = QColor(Qt::black),
                       QColor back = QColor(Qt::white),
                       QFont::Weight weight = QFont::Normal,
                       bool italic = false);
  void setErrorFormat(QColor fore = QColor(Qt::white),
                      QColor back = QColor(Qt::yellow),
                      QFont::Weight weight = QFont::Bold,
                      bool italic = false);
  void setAttributeFormat(QColor fore = QColor(Qt::cyan),
                          QColor back = QColor(Qt::white),
                          QFont::Weight weight = QFont::Normal,
                          bool italic = false);
  void setTagFormat(QColor fore = QColor(Qt::blue),
                    QColor back = QColor(Qt::white),
                    QFont::Weight weight = QFont::Normal,
                    bool italic = false);
  void setStringFormat(QColor fore = QColor(Qt::green),
                       QColor back = QColor(Qt::white),
                       QFont::Weight weight = QFont::Normal,
                       bool italic = false);
  void setStyleFormat(QColor fore = QColor(Qt::darkMagenta),
                      QColor back = QColor(Qt::white),
                      QFont::Weight weight = QFont::Normal,
                      bool italic = false);
  void setScriptFormat(QColor fore = QColor(Qt::darkGray),
                       QColor back = QColor(Qt::white),
                       QFont::Weight weight = QFont::Normal,
                       bool italic = false);

protected:
  bool m_tagstarted;  // triggered by the '<' start tag character.
  bool m_tagcomplete; // triggered by the '>' end tag character
  bool
    m_tagcloser; // triggered by '/' IF it is the second character in the tag.
  bool m_dquot;  // triggered by a start double quote '"'. A second one cancels.
  bool m_squot; // triggered by a start single quote '\''. A second one cancels.
  bool m_namestarted;  // triggered by the first non special character after the
                       // start tag.
  bool m_namecomplete; // trgiggerd by a space, a '/' or a '>' after
                       // m_namestarted is true.
  bool
    m_attstarted; // triggered by a non-special character after m_namecomplete.
  bool m_attcomplete; // triggered by a space, a '/', an '=' or a '>' or a
                      // non-special character.
  bool m_endtag;      // triggered by '>'
  bool m_eqstarted;   // triggered by an '=' character.
  bool m_error; // indicates an error has occured. Normally a special character
                // out of place.
  bool m_style_or_script; // a 'style' or a 'script' tag name has been detected
                          // without a tag closer.
  QString m_tagname;
  //  QStack<QString> m_namestack;
  int m_tagpos, m_tagstartpos;
  format_t m_current_format;
  Options m_options;
  //  node_t m_start_node, m_tagnode, m_current_node;

  void highlightBlock(const QString& text) override;
  //  void setError(QString errorstring);

  void resetTagFlags();

private:
  //  struct HighlightingRule
  //  {
  //    QRegularExpression pattern;
  //    QTextCharFormat format;
  //  };
  //  QVector<HighlightingRule> highlighting_rules;

  format_t m_normal_format;
  format_t m_tag_format;
  format_t m_string_format;
  format_t m_attribute_format;
  format_t m_style_format;
  format_t m_script_format;
  format_t m_error_format;
};

#endif // XHTMLHIGHLIGHTER_H
