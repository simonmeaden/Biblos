#ifndef OPTIONS_H
#define OPTIONS_H

#include <QColor>
#include <QFont>
#include <QObject>
#include <QRect>
#include <QSize>
#include <QString>

#include <qyaml-cpp/QYamlCpp>
#include <yaml-cpp/yaml.h>

#include "ebookcommon.h"

class Options : public QObject
{
  Q_OBJECT

public:
  enum CodeOptions {
    NORMAL,
    TAG,
    STRING,
    ATTRIBUTE,
    ERROR,
    STYLE,
    SCRIPT,
  };
  enum TocPosition {
    LEFT,
    RIGHT,
  };
  enum ViewState {
    VIEW_LIBRARY_TREE = 1,
    VIEW_LIBRARY_SHELF,
    VIEW_EDITOR,
  };

  Options(QObject* parent = nullptr);
  ~Options();

  void save(const QString filename);
  void load(const QString filename);

  TocPosition tocPosition() const;
  void setTocPosition(const TocPosition position);
  bool tocVisible() const;
  void setTocVisible(const bool visible);

  QColor color(const CodeOptions options) const;
  void setColor(const CodeOptions options, const QColor color);

  QColor background(const CodeOptions options) const;
  void setBackground(const CodeOptions options, const QColor color);

  bool italic(const CodeOptions options) const;
  void setItalic(const CodeOptions options, const bool italic);

  QFont::Weight weight(const CodeOptions options) const;
  void setWeight(const CodeOptions options, const QFont::Weight weight);

  QRect rect() const;
  void setRect(const QRect& rect);

  QSize optionsDlgSize() const;
  void setOptionsDlgSize(const QSize& optionsDlgSize);

  int currentIndex() const;
  void setCurrentIndex(const int index);
  bool currentFilesContains(const QString filename);
  void appendCurrentFile(const QString filename);
//  void moveToLibFile(const QString filename);
  QStringList currentfiles() const;
  int bookCount() const;

  ViewState viewState() const;
  void setViewState(const ViewState& viewState);

  static QColor contrastingColor(const QColor color);
  static QString codeOptionToString(const CodeOptions options);
  static QString weightToString(const QFont::Weight weight);

  QColor normalColor() const;
  void setNormalColor(const QColor &normalColor);

  QColor normalBack() const;
  void setNormalBack(const QColor &normalBack);

  bool normalItalic() const;
  void setNormalItalic(bool normalItalic);

  QFont codeFont() const;
  void setCodeFont(const QFont &codeFont);

  QFont::Weight normalWeight() const;
  void setNormalWeight(const QFont::Weight &normalWeight);

  QColor attributeColor() const;
  void setAttributeColor(const QColor &attributeColor);

  QColor attributeBack() const;
  void setAttributeBack(const QColor &attributeBack);

  bool attributeItalic() const;
  void setAttribute_italic(bool attributeItalic);

  QFont::Weight attributeWeight() const;
  void setAttributeWeight(const QFont::Weight &attributeWeight);

  QColor tagColor() const;
  void setTagColor(const QColor &tagColor);

  QColor tagBack() const;
  void setTagBack(const QColor &tagBack);

  bool tagItalic() const;
  void setTagItalic(bool tagItalic);

  QFont::Weight tagWeight() const;
  void setTagWeight(const QFont::Weight &tagWeight);

  QColor stringColor() const;
  void setStringColor(const QColor &stringColor);

  QColor stringBack() const;
  void setStringBack(const QColor &stringBack);

  bool stringItalic() const;
  void setStringItalic(bool stringItalic);

  QFont::Weight stringWeight() const;
  void setStringWeight(const QFont::Weight &stringWeight);

  QColor errorColor() const;
  void setErrorColor(const QColor &errorColor);

  QColor errorBack() const;
  void setErrorBack(const QColor &errorBack);

  bool errorItalic() const;
  void setErrorItalic(bool errorItalic);

  QFont::Weight errorWeight() const;
  void setErrorWeight(const QFont::Weight &errorWeight);

  QColor scriptColor() const;
  void setScriptColor(const QColor &scriptColor);

  QColor scriptBack() const;
  void setScriptBack(const QColor &scriptBack);

  bool scriptItalic() const;
  void setScriptItalic(bool scriptItalic);

  QFont::Weight scriptWeight() const;
  void setScriptWeight(const QFont::Weight &scriptWeight);

  QColor styleColor() const;
  void setStyleColor(const QColor &styleColor);

  QColor styleBack() const;
  void setStyleBack(const QColor &styleBack);

  bool styleItalic() const;
  void setStyleItalic(bool styleItalic);

  QFont::Weight styleWeight() const;
  void setStyleWeight(const QFont::Weight &styleWeight);

signals:
  void loadLibraryFiles(QStringList, int);

protected:
  ViewState m_view_state = VIEW_LIBRARY_TREE;
  TocPosition m_toc_position = LEFT;
  bool m_toc_visible = true;

  YAML::Node m_preferences;
  bool m_pref_changed = false;

  QRect m_rect;
  QSize m_options_dlg_size;
  int m_currentindex;
  QList<int> m_current_document_indexes;
  QList<int> m_current_document_lines;
  QStringList m_current_files;


  // Code Editor options
  QFont m_code_font;
  QColor m_normal_color;
  QColor m_normal_back;
  bool m_normal_italic;
  QFont::Weight m_normal_weight;
  QColor m_attribute_color;
  QColor m_attribute_back;
  bool m_attribute_italic;
  QFont::Weight m_attribute_weight;
  QColor m_tag_color;
  QColor m_tag_back;
  bool m_tag_italic;
  QFont::Weight m_tag_weight;
  QColor m_string_color;
  QColor m_string_back;
  bool m_string_italic;
  QFont::Weight m_string_weight;
  QColor m_error_color;
  QColor m_error_back;
  bool m_error_italic;
  QFont::Weight m_error_weight;
  QColor m_script_color;
  QColor m_script_back;
  bool m_script_italic;
  QFont::Weight m_script_weight;
  QColor m_style_color;
  QColor m_style_back;
  bool m_style_italic;
  QFont::Weight m_style_weight;

  // static tag strings.
  static const int DEF_WIDTH = 600;
  static const int DEF_HEIGHT = 1200;
  static const int DEF_X = 0;
  static const int DEF_Y = 0;
  static const int DEF_DLG_WIDTH = 300;
  static const int DEF_DLG_HEIGHT = 300;

  static QString POSITION;
  static QString DIALOG;
  static QString PREF_CURRENT_INDEX;
  static QString PREF_CURRENT_ITEMS;
  static QString PREF_CURRENT_LINE_NOS;
  static QString PREF_COUNT;
  static QString PREF_BOOKLIST;
  static QString PREF_LIBRARY;
  static QString CODE_OPTIONS;
  static QString CODE_NORMAL;
  static QString CODE_FONT;
  static QString CODE_ATTRIBUTE;
  static QString CODE_TAG;
  static QString CODE_ERROR;
  static QString CODE_STRING;
  static QString CODE_STYLE;
  static QString CODE_SCRIPT;
  static QString CODE_COLOR;
  static QString CODE_BACK;
  static QString CODE_WEIGHT;
  static QString CODE_ITALIC;
  static QString COPY_BOOKS_TO_STORE;
  static QString DELETE_OLD_BOOK;
  static QString SHOW_TOC;
  static QString TOC_POSITION;
  static QString VIEW_STATE;
};

#endif // OPTIONS_H
