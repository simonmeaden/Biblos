#ifndef EBOOKWRAPPER_H
#define EBOOKWRAPPER_H

#include <QStackedWidget>

#include "ebookcodeeditor.h"
#include "ebookcommon.h"
#include "ebookeditor.h"
#include "metadataeditor.h"

class EBookWordReader;

class EBookWrapper : public QStackedWidget
{
  Q_OBJECT

public:
  EBookWrapper(Options* options, AuthorsDB *authors, QWidget* parent = nullptr);
  ~EBookWrapper();

  void setToEditor();
  void setToCode();
  void setToMetadata();
  EBookCodeEditor* codeEditor();
  EBookEditor* editor();
  MetadataEditor* metaEditor();
  void optionsHaveChanged();
  void startWordReader();

  void update();

protected:
  EBookEditor* m_editor;
  EBookCodeEditor* m_codeeditor;
  MetadataEditor* m_metaeditor;
  //  EBookWordReader* m_word_reader;
  int m_editorindex, m_codeindex, m_metaindex;
  Options* m_options;
};

#endif // EBOOKWRAPPER_H
