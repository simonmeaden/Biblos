#ifndef EBOOKWORDREADER_H
#define EBOOKWORDREADER_H

#include <QObject>

class EBookWordReader : public QObject
{
  Q_OBJECT
public:
  explicit EBookWordReader(QObject *parent = nullptr);

signals:

public slots:
};

#endif // EBOOKWORDREADER_H