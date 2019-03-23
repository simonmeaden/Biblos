#include "focuslineedit.h"

const QString FocusLineEdit::SELECTED_BORDER =
  QString("FocusLineEdit {"
          "border: 2px solid green;"
          "border-radius: 4px;"
          "padding: 2px;"
          "}");
const QString FocusLineEdit::UNSELECTED_BORDER = QString("FocusLineEdit {"
                                                         "border: 0px;"
                                                         "padding: 4px;"
                                                         "}");

FocusLineEdit::FocusLineEdit(QWidget* parent)
  : QLineEdit(parent)
{
  setFocussed(false);
}

FocusLineEdit::~FocusLineEdit() {}

void
FocusLineEdit::setFocussed(bool focussed)
{
  if (focussed) {
    setStyleSheet(SELECTED_BORDER);
  } else {
    setStyleSheet(UNSELECTED_BORDER);
  }
}

void
FocusLineEdit::focusInEvent(QFocusEvent* e)
{
  QLineEdit::focusInEvent(e);
  emit(focussed(true));
}

void
FocusLineEdit::focusOutEvent(QFocusEvent* e)
{
  QLineEdit::focusOutEvent(e);
  emit(focussed(false));
}
