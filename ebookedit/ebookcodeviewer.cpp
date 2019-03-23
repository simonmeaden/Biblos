#include "ebookcodeviewer.h"

EBookCodeViewer::EBookCodeViewer(Options options, QWidget* parent)
  : QWebEngineView(parent)
  , m_options(options)
{}

EBookCodeViewer::~EBookCodeViewer() {}
