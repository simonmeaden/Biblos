#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QtWidgets>
//#include <QFrame>
//#include <QtGlobal>
#include <QColorDialog>

#include "epubcommon.h"

class OptionsDialog : public QDialog
{
  Q_OBJECT
public:
  explicit OptionsDialog(Options* options, QWidget* parent = nullptr);
  ~OptionsDialog();

  Options* options();
  bool modified() const;

signals:
  void codeChanged();

public slots:

protected:
  Options *m_options, *m_oldoptions;
  bool m_modified;
  QPushButton* fontBtn;
  QPushButton* normalColorBtn;
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

  void initGui();
  void resizeEvent(QResizeEvent* e);
  void changePopupTimeout(int);
  void changeEnablePopup(bool);
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
  QColor enactColorDialog(QColor old_color);
  void buildCodeSection();
  QFrame* initCodeTab(Options::CodeOptions options);
  void setColorConnection(Options::CodeOptions options);
  void setBackgroundConnection(Options::CodeOptions options);
  void setItalicConnection(Options::CodeOptions options);
  void setWeightConnection(Options::CodeOptions options);
  QPushButton* getColorBtn(Options::CodeOptions options);
  QPushButton* getBackgroundBtn(Options::CodeOptions options);
  QCheckBox* getItalicBox(Options::CodeOptions options);
  QComboBox* getWeightBox(Options::CodeOptions options);
  void setCodeColor(Options::CodeOptions options);
  void setCodeBackground(Options::CodeOptions options);
  QString convertColorToStyle(QColor color, QColor back);

  static const QString BTN_STYLE;
};

#endif // OPTIONSDIALOG_H
