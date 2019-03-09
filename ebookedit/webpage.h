#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QWebEnginePage>

class WebPage : public QWebEnginePage
{
  Q_OBJECT
public:
  WebPage(QWebEngineProfile* profile,
          QString jquery,
          QObject* parent = nullptr);
  ~WebPage();

  void highlightLinks();

protected:
  int m_load_progress;
  QString m_jquery;
};

#endif // WEBPAGE_H
