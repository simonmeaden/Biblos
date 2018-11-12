#include "ebookwrapper.h"

EBookWrapper::EBookWrapper(Options *options, QWidget* parent)
  : QStackedWidget(parent)
  , m_editor(new EBookEditor(parent))
  , m_codeeditor(new EBookCodeEditor(options, parent))
  , m_options(options)
{
  m_editorindex = addWidget(m_editor);
  m_codeindex = addWidget(m_codeeditor);
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

void
EBookWrapper::optionsHaveChanged()
{
  m_codeeditor->rehighlight();
}
