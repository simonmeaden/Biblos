#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QColorDialog>
#include <QDialog>
#include <QtWidgets>

#include "ebookcommon.h"
#include "options.h"

class OptionsDialog : public QDialog
{
  Q_OBJECT
public:
  explicit OptionsDialog(Options options, QWidget* parent = nullptr);
  ~OptionsDialog();

  Options options();
  bool modified() const;

signals:
  void codeChanged();
  void showToc(bool);
  void moveToc(BiblosOptions::TocPosition);

public slots:

protected:
  Options m_options, m_oldoptions;
  bool m_modified;
  QPushButton* font_btn;
  QPushButton* normal_color_btn;
  QPushButton* normalBackBtn;
  QCheckBox* normalItalicBox;
  QComboBox* normalWeightBox;
  QPushButton* attColorBtn;
  QPushButton* attBackBtn;
  QCheckBox* attItalicBox;
  QComboBox* attWeightBox;
  QPushButton* tagColorBtn;
  QPushButton* tagBackBtn;
  QCheckBox* tagItalicBox;
  QComboBox* tagWeightBox;
  QPushButton* stringColorBtn;
  QPushButton* stringBackBtn;
  QCheckBox* stringItalicBox;
  QComboBox* stringWeightBox;
  QPushButton* errorColorBtn;
  QPushButton* errorBackBtn;
  QCheckBox* errorItalicBox;
  QComboBox* errorWeightBox;
  QPushButton* styleColorBtn;
  QPushButton* styleBackBtn;
  QCheckBox* styleItalicBox;
  QComboBox* styleWeightBox;
  QPushButton* scriptColorBtn;
  QPushButton* scriptBackBtn;
  QCheckBox* scriptItalicBox;
  QComboBox* scriptWeightBox;

  QCheckBox* toc_visible_box;
  QPushButton* toc_position_btn;

  void initGui();
  void resizeEvent(QResizeEvent* e);
  QFrame* initMiscTab();
  QFrame* initEditorTab();
  QFrame* initCodeEditorTab();
  void setCodeFont();
  void setNormalColor();
  void setNormalBackground();
  void setNormalItalic(bool);
  void setNormalWeight(int);
  void setAttributeColor();
  void setAttributeBackground();
  void setAttributeItalic(bool);
  void setAttributeWeight(int);
  void setTagColor();
  void setTagBackground();
  void setTagItalic(bool);
  void setTagWeight(int);
  void setStringColor();
  void setStringBackground();
  void setStringItalic(bool);
  void setStringWeight(int);
  void setErrorColor();
  void setErrorBackground();
  void setErrorItalic(bool);
  void setErrorWeight(int);
  void setStyleColor();
  void setStyleBackground();
  void setStyleItalic(bool);
  void setStyleWeight(int);
  void setScriptColor();
  void setScriptBackground();
  void setScriptItalic(bool);
  void setScriptWeight(int);

  void setShowTocEdit(bool);
  void setTocPosition();

  QColor enactColorDialog(QColor old_color);
  void buildCodeSection();
  QFrame* initCodeTab(BiblosOptions::CodeOptions options);
  void setColorConnection(BiblosOptions::CodeOptions options);
  void setBackgroundConnection(BiblosOptions::CodeOptions options);
  void setItalicConnection(BiblosOptions::CodeOptions options);
  void setWeightConnection(BiblosOptions::CodeOptions options);
  QPushButton* getColorBtn(BiblosOptions::CodeOptions options);
  QPushButton* getBackgroundBtn(BiblosOptions::CodeOptions options);
  QCheckBox* getItalicBox(BiblosOptions::CodeOptions options);
  QComboBox* getWeightBox(BiblosOptions::CodeOptions options);
  void setCodeColor(BiblosOptions::CodeOptions options);
  void setCodeBackground(BiblosOptions::CodeOptions options);
  QString convertColorToStyle(QColor color, QColor back);

  static const QString BTN_STYLE;
};

#endif // OPTIONSDIALOG_H
