#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class EPubDocument;
class EPubContainer;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    void scroll(int amount);
    void scrollPage(int amount);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QImage m_cover;
    EPubDocument *m_document;
    int m_currentChapter;
    int m_yOffset;
};

#endif // WIDGET_H
