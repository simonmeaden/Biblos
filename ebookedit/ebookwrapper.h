#ifndef EBOOKWRAPPER_H
#define EBOOKWRAPPER_H

#include <QStackedWidget>
#include <QWebEngineProfile>

#include "ebookcodeeditor.h"
#include "ebookcommon.h"
//#include "ebookeditor.h"
#include "metadataeditor.h"
#include "webview.h"

class EBookWordReader;

class EBookWrapper : public QStackedWidget
{
  Q_OBJECT

public:
  EBookWrapper(Options options,
               AuthorsDB authors,
               SeriesDB series_db,
               LibraryDB library,
               QString jquery,
               QString one_page_js,
               QString one_page_css,
               QWidget* parent = nullptr);
  ~EBookWrapper();

  void setToEditor();
  void setToCode();
  void setToMetadata();
  EBookCodeEditor* codeEditor();
  WebView* editor();
  MetadataEditor* metaEditor();
  void optionsHaveChanged();

  void update();

protected:
  WebView* m_editor;
  QWebEngineProfile* m_profile;
  EBookCodeEditor* m_codeeditor;
  MetadataEditor* m_metaeditor;
  //  EBookWordReader* m_word_reader;
  int m_editorindex, m_codeindex, m_metaindex;
  Options m_options;
};

#endif // EBOOKWRAPPER_H
