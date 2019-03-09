#include "webview.h"

#include <qlogger/qlogger.h>
using namespace qlogger;

/* WebView
 *****************************************************************************/
WebView::WebView(Options* options,
                 QString jquery,
                 QString one_page_js,
                 QString one_page_css,
                 QWebEngineProfile* profile,
                 QWidget* parent)
  : QWebEngineView(parent)
  , m_options(options)
  , m_jquery(jquery)
  , m_onepage_js(one_page_js)
  , m_onepage_css(one_page_css)
  , m_load_progress(100)
  , m_profile(profile)
{
  dialog_error_key = m_options->dialog_error_key;
  view_refresh_key = m_options->view_refresh_key;
  text_html_key = m_options->text_html_key;

  insertStyleSheet(QStringLiteral("one_page"), m_onepage_css);

  connect(this, &QWebEngineView::loadStarted, this, &WebView::setLoadStarted);
  connect(this, &QWebEngineView::loadProgress, this, &WebView::setLoadProgress);
  connect(this, &QWebEngineView::loadFinished, this, &WebView::loadFinished);
  connect(this, &QWebEngineView::iconChanged, this, &WebView::setChangedIcon);
  connect(this,
          &QWebEngineView::renderProcessTerminated,
          [this](QWebEnginePage::RenderProcessTerminationStatus termStatus,
                 int statusCode) {
            QString status;
            switch (termStatus) {
              case QWebEnginePage::NormalTerminationStatus:
                status = tr("Render process normal exit");
                break;
              case QWebEnginePage::AbnormalTerminationStatus:
                status = tr("Render process abnormal exit");
                break;
              case QWebEnginePage::CrashedTerminationStatus:
                status = tr("Render process crashed");
                break;
              case QWebEnginePage::KilledTerminationStatus:
                status = tr("Render process killed");
                break;
            }
            QMessageBox::StandardButton btn =
              QMessageBox::question(window(),
                                    status,
                                    tr("Render process exited with code: %1\n"
                                       "Do you want to reload the page ?")
                                      .arg(statusCode));
            if (btn == QMessageBox::Yes)
              QTimer::singleShot(0, [this] { reload(); });
          });
  setWebPage(new WebPage(m_profile, jquery, this));
}

QIcon
WebView::favIcon() const
{
  QIcon favIcon = icon();
  if (!favIcon.isNull())
    return favIcon;

  QPixmap dialog_error, view_refresh, text_html;
  QPixmapCache::find(dialog_error_key, &dialog_error);
  QPixmapCache::find(view_refresh_key, &view_refresh);
  QPixmapCache::find(text_html_key, &text_html);

  if (m_load_progress < 0) {
    QPixmap dialog_error;
    QPixmapCache::find(m_options->dialog_error_key, &dialog_error);
    static QIcon errorIcon(dialog_error);
    return errorIcon;
  } else if (m_load_progress < 100) {
    QPixmap view_refresh;
    QPixmapCache::find(m_options->view_refresh_key, &view_refresh);
    static QIcon loadingIcon(view_refresh);
    return loadingIcon;
  } else {
    QPixmap text_html;
    QPixmapCache::find(m_options->text_html_key, &text_html);
    static QIcon defaultIcon(text_html);
    return defaultIcon;
  }
}

int
WebView::loadProgress() const
{
  return m_load_progress;
}

void
WebView::setDocument(EBookDocument ebook_document)
{
  m_ebook_document = ebook_document;
  m_spine = ebook_document->spine();
  m_pages = ebook_document->pages();
  QString id = m_spine.first();
  setCurrentPage(id);
}

EBookDocument
WebView::document()
{
  return m_ebook_document;
}

void
WebView::setCurrentPage(QString page_id)
{
  QString html;
  if (page_id.isEmpty()) {
    if (m_pages.isEmpty()) {
      html = QString();
    } else {
      html = m_pages.first();
    }
  } else {
    html = m_pages.value(page_id, QString());
  }
  if (!html.isEmpty()) {
    page()->setHtml(html);
  }
}

QString
WebView::buildTocFromData()
{
  return m_ebook_document->buildTocFromData();
}

// bool
// WebView::event(QEvent* event)
//{
//  foreach (QObject* obj, this->children())
//    if (qobject_cast<QWidget*>(obj))
//      QLOG_WARN("Help");
//  //  return QWebEngineView::event(event);
//}

// void
// WebView::pageUp()
//{
//  page()->runJavaScript(QString("window.scrollTo(%1,
//  %2);").arg(scrollX).arg(scrollY));
//}

// void
// WebView::pageDown()
//{
//    page()->runJavaScript(QString("window.scrollTo(%1,
//    %2);").arg(scrollX).arg(scrollY));
//}

void
WebView::setWebPage(WebPage* page)
{
  createWebActionTrigger(page, QWebEnginePage::Forward);
  createWebActionTrigger(page, QWebEnginePage::Back);
  createWebActionTrigger(page, QWebEnginePage::Reload);
  createWebActionTrigger(page, QWebEnginePage::Stop);
  QWebEngineView::setPage(page);
}

void
WebView::setLoadStarted()
{
  m_load_progress = 0;
  emit favIconChanged(favIcon());
}

void
WebView::loadFinished(const bool& value)
{
  m_load_progress = value ? 100 : -1;
  emit favIconChanged(favIcon());
  WebPage* web_page = qobject_cast<WebPage*>(page());
  if (web_page) {
    web_page->runJavaScript(m_jquery);
    web_page->runJavaScript(m_onepage_js);
    web_page->highlightLinks();
  }
}
void
WebView::insertStyleSheet(const QString& name, const QString& source)
{
  QWebEngineScript script;
  QString s = QString::fromLatin1("(function() {"
                                  "    css = document.createElement('style');"
                                  "    css.type = 'text/css';"
                                  "    css.id = '%1';"
                                  "    document.head.appendChild(css);"
                                  "    css.innerText = '%2';"
                                  "})()")
                .arg(name)
                .arg(source.simplified());

  WebPage* web_page = qobject_cast<WebPage*>(page());
  if (web_page) {
    script.setName(name);
    script.setSourceCode(s);
    script.setInjectionPoint(QWebEngineScript::DocumentReady);
    script.setRunsOnSubFrames(true);
    script.setWorldId(QWebEngineScript::ApplicationWorld);
    web_page->scripts().insert(script);
  }
}

void
WebView::setLoadProgress(const int& value)
{
  m_load_progress = value;
}

void
WebView::setChangedIcon(const QIcon& icon)
{}

void
WebView::contextMenuEvent(QContextMenuEvent* event)
{
  QMenu* menu = page()->createStandardContextMenu();
  const QList<QAction*> actions = menu->actions();
  auto inspectElement =
    std::find(actions.cbegin(),
              actions.cend(),
              page()->action(QWebEnginePage::InspectElement));
  if (inspectElement == actions.cend()) {
    auto viewSource = std::find(actions.cbegin(),
                                actions.cend(),
                                page()->action(QWebEnginePage::ViewSource));
    if (viewSource == actions.cend())
      menu->addSeparator();

    //        QAction *action = new QAction(menu);
    //        action->setText("Open inspector in new window");
    //        connect(action, &QAction::triggered, [this]() { emit
    //        devToolsRequested(page()); });

    //        QAction *before(inspectElement == actions.cend() ? nullptr :
    //        *inspectElement); menu->insertAction(before, action);
  } else {
    (*inspectElement)->setText(tr("Inspect element"));
  }
  menu->popup(event->globalPos());
}

void
WebView::createWebActionTrigger(QWebEnginePage* page,
                                QWebEnginePage::WebAction web_action)
{
  QAction* action = page->action(web_action);
  connect(action, &QAction::changed, [this, action, web_action] {
    emit webActionEnabledChanged(web_action, action->isEnabled());
  });
}
