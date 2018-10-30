#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QtGlobal>
#include <QtWidgets>

struct Options
{
  int x, y, width, height;
  int popuptimeout;
  int currentindex;
  int bookcount;
  QStringList currentfiles;
  bool enablepopup;
};

class OptionsDialog : public QDialog
{
  Q_OBJECT
public:
  explicit OptionsDialog(Options options, QWidget* parent = nullptr);
  ~OptionsDialog();

  Options options();
  bool modified() const;

signals:

public slots:

protected:
  Options m_options, m_oldoptions;
  bool m_modified;

  void initGui();
  void changePopupTimeout(int);
  void changeEnablePopup(bool);
};

#endif // OPTIONSDIALOG_H
