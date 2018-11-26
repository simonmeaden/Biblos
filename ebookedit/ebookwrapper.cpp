#include "ebookwrapper.h"

EBookWrapper::EBookWrapper(Options* options, QWidget* parent)
  : QStackedWidget(parent)
  , m_editor(new EBookEditor(parent))
  , m_codeeditor(new EBookCodeEditor(options, parent))
  , m_metaeditor(new MetadataEditor(options, parent))
  , m_editorindex(0)
  , m_codeindex(0)
  , m_metaindex(0)
  , m_options(options)
{
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

EBookEditor*
EBookWrapper::editor()
{
  return m_editor;
}

MetadataEditor *EBookWrapper::metaEditor() {
  return m_metaeditor;
}

void
EBookWrapper::optionsHaveChanged()
{
  m_codeeditor->rehighlight();
}
