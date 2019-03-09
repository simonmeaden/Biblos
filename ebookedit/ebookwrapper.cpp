#include "ebookwrapper.h"

EBookWrapper::EBookWrapper(Options* options,
                           AuthorsDB authors,
                           SeriesDB series_db,
                           LibraryDB library,
                           QString jquery,
                           QString one_page_js,
                           QString one_page_css,
                           QWidget* parent)
  : QStackedWidget(parent)
  , m_codeeditor(new EBookCodeEditor(options, parent))
  , m_metaeditor(
      new MetadataEditor(options, authors, series_db, library, parent))
  , m_editorindex(0)
  , m_codeindex(0)
  , m_metaindex(0)
  , m_options(options)
{
  m_profile = QWebEngineProfile::defaultProfile();
  m_editor =
    new WebView(options, jquery, one_page_js, one_page_css, m_profile, parent);
  m_editorindex = addWidget(m_editor);
  m_codeindex = addWidget(m_codeeditor);
  m_metaindex = addWidget(m_metaeditor);
}

EBookWrapper::~EBookWrapper() {}

void
EBookWrapper::setToEditor()
{
  setCurrentIndex(m_editorindex);
}

void
EBookWrapper::setToCode()
{
  setCurrentIndex(m_codeindex);
}

void
EBookWrapper::setToMetadata()
{
  setCurrentIndex(m_metaindex);
}

EBookCodeEditor*
EBookWrapper::codeEditor()
{
  return m_codeeditor;
}

WebView*
EBookWrapper::editor()
{
  return m_editor;
}

MetadataEditor*
EBookWrapper::metaEditor()
{
  return m_metaeditor;
}

void
EBookWrapper::optionsHaveChanged()
{
  m_codeeditor->rehighlight();
}

void
EBookWrapper::update()
{
  m_editor->update();
  m_codeeditor->update();
}
