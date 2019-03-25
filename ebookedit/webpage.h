#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QWebEnginePage>
#include <QWebEngineSettings>

class WebPage : public QWebEnginePage
{
  Q_OBJECT
public:
  WebPage(QWebEngineProfile* profile,
          QString jquery,
          QObject* parent = nullptr);
  ~WebPage();

  void highlightLinks();
  void insertCss() {}

protected:
  int m_load_progress;
  QString m_jquery;
};

#endif // WEBPAGE_H
