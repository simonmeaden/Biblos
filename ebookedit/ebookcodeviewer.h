#ifndef EBAOOKCODEVIEWER_H
#define EBAOOKCODEVIEWER_H

#include <QWebEngineView>

#include "options.h"

class EBookCodeViewer : public QWebEngineView
{
public:
  EBookCodeViewer(Options options, QWidget* parent = nullptr);
  ~EBookCodeViewer();

protected:
  Options m_options;
};

#endif // EBAOOKCODEVIEWER_H
