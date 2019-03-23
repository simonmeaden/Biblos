#ifndef ACTIONTABWIDGET_H
#define ACTIONTABWIDGET_H

#include <QApplication>
#include <QEvent>
#include <QKeyEvent>
#include <QTabWidget>

class ActionTabWidget : public QTabWidget
{
  Q_OBJECT
public:
  explicit ActionTabWidget(QWidget* parent = nullptr);
  ~ActionTabWidget() override;

signals:

public slots:
  bool eventFilter(QObject* obj, QEvent* event) override;
};

#endif // ACTIONTABWIDGET_H
