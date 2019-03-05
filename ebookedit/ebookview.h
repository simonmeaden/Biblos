#ifndef EBOOKVIEW_H
#define EBOOKVIEW_H

#include <QPixmapCache>
#include <QWebEngineView>
#include <QtWidgets>

#include "options.h"

class WebView : public QWebEngineView
{
  Q_OBJECT
public:
  WebView(Options* options, QWidget* parent = nullptr);

  QIcon favIcon() const;
  int loadProgress() const;

signals:
  void webActionEnabledChanged(QWebEnginePage::WebAction webAction,
                               bool enabled);
  void favIconChanged(const QIcon& icon);

protected:
  Options* m_options;
  int m_load_progress;

  void setLoadStarted();
  void loadFinished(bool value);
  void setLoadProgress(int value);
  void setChangedIcon(const QIcon&);
  void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // EBOOKVIEW_H
