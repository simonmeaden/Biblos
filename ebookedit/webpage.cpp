#include "webpage.h"

WebPage::WebPage(QWebEngineProfile* profile, QObject* parent)
  : QWebEnginePage(profile, parent)
  , m_load_progress(100)
{}
