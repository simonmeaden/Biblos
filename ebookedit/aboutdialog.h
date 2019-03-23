#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QtWidgets>

class AboutDialog : public QDialog
{
public:
  AboutDialog(QWidget* parent = nullptr);
  ~AboutDialog() override;

protected:
  QTabWidget* m_tabs;
  QTableWidget* m_attribution;
};

#endif // ABOUTDIALOG_H
