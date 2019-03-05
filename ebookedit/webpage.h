#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QWebEnginePage>

class WebPage : public QWebEnginePage
{
public:
  WebPage(QWebEngineProfile* profile, QObject* parent = nullptr);

protected:
  int m_load_progress;
};

#endif // WEBPAGE_H
