#ifndef TIDY_H
#define TIDY_H

#include <QFile>
#include <QObject>

#include <tidy.h>
#include <tidybuffio.h>

class Tidy : public QObject
{
public:
  Tidy(QObject* parent);
  ~Tidy();
  bool setUp(QString configfile);
  bool cleanTidy(QString input_data, QString& out_data);

protected:
  int status;
  QString error_msg;
  QString config_file_tidy;
  TidyDoc doc;

  const char* qtchar(QString xml);
};

#endif // TIDY_H
