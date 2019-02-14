#ifndef FOCUSLINEEDIT_H
#define FOCUSLINEEDIT_H

#include <QLineEdit>

class FocusLineEdit : public QLineEdit
{
  Q_OBJECT
public:
  FocusLineEdit(QWidget* parent = nullptr);

  void setFocussed(bool focussed);

signals:
  void focussed(bool hasFocus);

protected:
  virtual void focusInEvent(QFocusEvent *e);
  virtual void focusOutEvent(QFocusEvent *e);

  static const QString SELECTED_BORDER;
  static const QString UNSELECTED_BORDER;

};

#endif // FOCUSLINEEDIT_H
