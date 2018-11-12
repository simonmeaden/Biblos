#ifndef HOVERDIALOG_H
#define HOVERDIALOG_H

#include <QtWidgets>
#include <QFrame>
#include <QStringList>

#include "qebookdocument.h"

class HoverPopup : public QFrame {
  Q_OBJECT

public:
  HoverPopup(QWidget *parent=Q_NULLPTR);
  HoverPopup(QVariant data, QWidget *parent=Q_NULLPTR);
  ~HoverPopup();

  void setData(QVariant &data);

  void setLabels();

protected:
  EBookData m_data;
  QLabel *m_titlelbl, *m_authorlbl, *m_publisherlbl;
  QLabel *m_langlbl, *m_datelbl, *m_subjectlbl;

  void initGui();

  static const QString STYLESHEET;
};

#endif // HOVERDIALOG_H
