#include "ebookview.h"

WebView::WebView(Options* options, QWidget* parent)
  : QWebEngineView(parent)
  , m_load_progress(100)
{
  connect(this, &QWebEngineView::loadStarted, this, &WebView::setLoadStarted);
  connect(this, &QWebEngineView::loadProgress, &WebView::setLoadProgress);
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
}

QIcon
WebView::favIcon() const
{
  QIcon favIcon = icon();
  if (!favIcon.isNull())
    return favIcon;

  QPixmap dialog_error, view_refresh, text_html;
  QPixmapCache::find(m_options->dialog_error_key, &dialog_error);
  QPixmapCache::find(m_options->view_refresh_key, &view_refresh);
  QPixmapCache::find(m_options->text_html_key, &text_html);

  if (m_load_progress < 0) {
    static QIcon errorIcon(dialog_error);
    return errorIcon;
  } else if (m_load_progress < 100) {
    static QIcon loadingIcon(view_refresh);
    return loadingIcon;
  } else {
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
WebView::setLoadStarted()
{
  m_load_progress = 0;
  emit favIconChanged(favIcon());
}

void
WebView::loadFinished(bool value)
{
  m_load_progress = value ? 100 : -1;
  emit favIconChanged(favIcon());
}

void
WebView::setLoadProgress(int value)
{
  m_load_progress = value;
}

void
WebView::setChangedIcon(const QIcon&)
{}
