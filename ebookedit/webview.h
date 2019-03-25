#ifndef EBOOKVIEW_H
#define EBOOKVIEW_H

#include <QAction>
#include <QPixmapCache>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>
#include <QWebEngineView>
#include <QtWidgets>

//#include "ebookwrapper.h"
#include "iebookdocument.h"
#include "iebookinterface.h"
#include "options.h"
#include "webpage.h"

class WebView : public QWebEngineView
{
  Q_OBJECT
public:
  WebView(Options options,
          QString jquery,
          //          QString one_page_js,
          //          QString one_page_css,
          QWebEngineProfile* profile,
          QWidget* parent = nullptr);
  ~WebView();

  QIcon favIcon() const;
  int loadProgress() const;

  void setDocument(EBookDocument ebook_document);
  EBookDocument document();
  void setCurrentPage(QString page_id);
  QString buildTocFromData();

  int scrollWidth();
  int scrollHeight();

signals:
  void webActionEnabledChanged(QWebEnginePage::WebAction webAction,
                               bool enabled);
  void favIconChanged(const QIcon& icon);

protected:
  Options m_options;
  QString m_jquery /*, m_onepage_js, m_onepage_css*/;
  int m_load_progress;
  QWebEngineProfile* m_profile;
  QStringList m_spine;
  //  QMap<QString, QString> m_pages;
  ManifestItemMap m_pages;
  //  WebPage *m_page;
  EBookDocument m_ebook_document;
  QPixmapCache::Key dialog_error_key, view_refresh_key, text_html_key;

  //  QWebEngineView* createWindow(QWebEnginePage::WebWindowType) override;
  void setWebPage(WebPage* page);
  void setLoadStarted();
  void loadFinished(const bool& value);
  void setLoadProgress(const int& value);
  void setChangedIcon(const QIcon& icon);
  void contextMenuEvent(QContextMenuEvent* event) override;
  void createWebActionTrigger(QWebEnginePage* page,
                              QWebEnginePage::WebAction web_action);
  void insertStyleSheet(const QString& name, const QString& source);
  void handleViewSourceTriggered();
};

#endif // EBOOKVIEW_H
