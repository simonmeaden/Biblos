#ifndef EBOOKWRAPPER_H
#define EBOOKWRAPPER_H

#include <QStackedWidget>

#include "ebookeditor.h"
#include "ebookcodeeditor.h"
#include "epubcommon.h"

class EBookWrapper : public QStackedWidget
{
  Q_OBJECT

public:
  EBookWrapper(Options *options, QWidget* parent = Q_NULLPTR);
  ~EBookWrapper();

  void setToEditor();
  void setToCode();
  EBookCodeEditor* codeEditor();
  EBookEditor* editor();
  void optionsHaveChanged();

protected:
  EBookEditor* m_editor;
  EBookCodeEditor* m_codeeditor;
  int m_editorindex, m_codeindex;
  Options *m_options;

};


#endif // EBOOKWRAPPER_H
