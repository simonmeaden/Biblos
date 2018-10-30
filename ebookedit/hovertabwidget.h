#ifndef HOVERTABWIDGET_H
#define HOVERTABWIDGET_H

#include <QtWidgets>

class HoverTabBar;

class HoverTabWidget : public QTabWidget {
  Q_OBJECT

public:
  HoverTabWidget(QWidget *parent = Q_NULLPTR);
  ~HoverTabWidget();

  int addTab(QWidget *widget, const QString &text);
  int addTab(QWidget *widget, const QString &text, QVariant data);

signals:
//  void tabEntered(int);
  //! \brief signal sent when the mouse enters the tab, with the tab index and optionally any tab data.
  void tabEntered(int, QPoint, QVariant);
  //! \brief signal sent when the mouse leaves the tab, with the tab index.
  void tabExited(int);

protected:
  QDialog *m_dlg;

  void tabWasEntered(int, QPoint);
};

class HoverTabBar : public QTabBar {
  Q_OBJECT
public:
  HoverTabBar(QWidget *parent = Q_NULLPTR);
  ~HoverTabBar();

//  void setDlg(QDialog *dlg);

signals:
  //! \brief signal sent when the mouse enters the tab, with the tab index.
  void tabEntered(int, QPoint);
  //! \brief signal sent when the mouse leaves the tab, with the tab index.
  void tabExited(int);

protected:
  void hoverEnter(QHoverEvent * event);
  void hoverLeave(QHoverEvent *);
  void hoverMove(QHoverEvent * event);
  bool event(QEvent * e);

  int m_hoverindex;
  QPoint m_hoverposition;

};


#endif // HOVERTABWIDGET_H
