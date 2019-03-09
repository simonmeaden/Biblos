#include "actiontabwidget.h"
#include "ebookwrapper.h"

ActionTabWidget::ActionTabWidget(QWidget* parent)
  : QTabWidget(parent)
{}

bool
ActionTabWidget::eventFilter(QObject* obj, QEvent* event)
{
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent* key = static_cast<QKeyEvent*>(event);
    EBookWrapper* wrapper = qobject_cast<EBookWrapper*>(currentWidget());
    if (wrapper) {
      WebView* web_view = qobject_cast<WebView*>(wrapper->editor());
      if (web_view) {
        WebPage* page = qobject_cast<WebPage*>(web_view->page());
        QSize contents_size = page->contentsSize().toSize();
        QPoint pos = page->scrollPosition().toPoint();
        int y = pos.y();
        int widget_h = web_view->height();
        int contents_h = contents_size.height();
        if (page) {
          if (key->key() == Qt::Key_PageUp) {
            y = y - widget_h < 0 ? 0 : y - widget_h;
          } else if (key->key() == Qt::Key_PageDown) {
            y = y + widget_h > contents_h ? contents_h : y + widget_h;
          } else if (key->key() == Qt::Key_Home) {
            y = 0;
          } else if (key->key() == Qt::Key_End) {
            y = contents_h - widget_h;
          }
          page->runJavaScript(QString("window.scrollTo(%1, %2);").arg(0).arg(y),
                              QWebEngineScript::ApplicationWorld);
        }
      }
    }
  }
  return QTabWidget::eventFilter(obj, event);
}
