#ifndef GSWIDGET_H
#define GSWIDGET_H

#include <QOpenGLWidget>
#include <QPainter>

class GSWidget : public QOpenGLWidget
{
public:
    GSWidget(QWidget *centralwidget);

    void cleanPoints();
    void createLine();

    bool mousePressed;
    bool closeToPoint;
    bool lineDrawn;

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    int h, w;

    QPixmap m_mainPix;
    QPainter m_painter;
    unsigned int m_dragged;
    std::vector<QPointF> m_points;
    std::vector<QPointF> m_points1;
};

#endif // GSWIDGET_H
