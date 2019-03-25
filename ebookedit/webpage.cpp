#include "webpage.h"

WebPage::WebPage(QWebEngineProfile* profile, QString jquery, QObject* parent)
  : QWebEnginePage(profile, parent)
  , m_load_progress(100)
  , m_jquery(jquery)
{
  //  connect(this, &WebPage::WebAction)
  QWebEngineSettings* settings = QWebEnginePage::settings();
  settings->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls,
                         true);
}

WebPage::~WebPage() {}

void
WebPage::highlightLinks()
{
  QString code = QStringLiteral("qt.jQuery('a').each( function () { "
                                "qt.jQuery(this).css('color', 'blue') } )");
  runJavaScript(code);
}
