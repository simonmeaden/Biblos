#include "htmltidy.h"

Tidy::Tidy(QObject* parent)
  : QObject(parent)
{
  status = 0;
  error_msg = "";
  doc = tidyCreate();
}

Tidy::~Tidy()
{
  tidyRelease(doc);
}

/* prepare config tidy config file */
bool
Tidy::setUp(QString configfile)
{
  //  qDebug() << "### load config....  " << configfile;
  config_file_tidy = configfile;
  status = tidyLoadConfig(doc, qtchar(configfile));

  if (status != 0) {
    error_msg = "Not possibel to load Config File!";
    return false;
  } else {
    return true;
  }
}

/* QString clean file input string and put to output string */
bool
Tidy::cleanTidy(QString input_data, QString& out_data)
{
  //  qDebug() << "### load inputfile....  " << inputfile;
  //  qDebug() << "### load outfile....  " << outfile;
  if (!(config_file_tidy.size() > 0) && status != 0) {
    error_msg = "Set up a config file!";
    return false;
  }
  Bool ok;
  int rc = -1;
  /*TidyBuffer output;*/
  //  TidyBuffer errbuf;
  QString resultwork;
  QString inside = "";

  QByteArray ba = input_data.toLatin1();
  /*qDebug() << "### load input....  "  << ba.data();*/
  //  ok = tidyOptSetBool(doc, TidyXhtmlOut, yes);
  //  qDebug() << "### steep 1 ok  ";
  rc = tidyParseString(doc, ba.data());
  //  qDebug() << "### steep 1a ok  " << rc; /* rc become error message */
  if (rc > 0) {
    error_msg = "Success";
    //    qDebug() << "### steep 2 ok  ";
    //    rc = tidySaveFile(doc, qtchar(out_data));
    //    qDebug() << "### steep 3 ok  " << rc;
    //    ba.setRawData() tidyRelease(doc);
    //    out_data.setRawData(doc->);
    return true;
  }

  /*tidyBufFree( &errbuf );*/
  return false;
}
/* QString to const char* */
const char*
Tidy::qtchar(QString xml)
{
  QByteArray ba = xml.toLatin1();
  const char* hack = ba.data();
  return hack;
}
