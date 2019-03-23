#ifndef DELETEFILEDIALOG_H
#define DELETEFILEDIALOG_H

#include <QtWidgets>

class DeleteFileDialog : public QDialog
{
  Q_OBJECT
public:
  explicit DeleteFileDialog(QWidget* parent = nullptr);
  ~DeleteFileDialog();

signals:

public slots:

protected:
};

#endif // DELETEFILEDIALOG_H
