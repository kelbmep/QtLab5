#include "gswidget.h"
#include <QOpenGLFunctions>
#include <QMouseEvent>

GSWidget::GSWidget(QWidget *centralwidget)
    : QOpenGLWidget(centralwidget)
{
    m_mainPix = QPixmap(2000, 2000);
    m_mainPix.fill(Qt::white);
    mousePressed = false;
    closeToPoint = false;
    lineDrawn = false;
    m_points.clear();
    m_dragged = -1;
}

unsigned fakt(int n)
{
   unsigned r;
   for (r=1;n>0;--n)
         r*=n;
   return r;
}
// расчет C(n,k)
unsigned bci(int n,int k)
{
   return fakt(n)/(fakt(k)*fakt(n-k));
}

void GSWidget::mousePressEvent(QMouseEvent *event)
{
    closeToPoint = false;
    QPainter tempPainter(&m_mainPix);
    if(m_points.size() > 3)
    {
        for(size_t i = 0; i < m_points.size(); i++)
        {
            if(fabs(event->pos().x() - m_points[i].x()) < 5 && fabs(event->pos().y() - m_points[i].y()) < 5)
            {
                if(i != 0)
                {
                    tempPainter.setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::FlatCap));
                    tempPainter.drawLine(m_points[i - 1], m_points[i]);
                    tempPainter.setPen(QPen(Qt::red, 7, Qt::SolidLine, Qt::FlatCap));
                    tempPainter.drawPoint(m_points[i - 1]);
                }
                if(i != m_points.size() - 1)
                {
                    tempPainter.setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::FlatCap));
                    tempPainter.drawLine(m_points[i], m_points[i + 1]);
                    tempPainter.setPen(QPen(Qt::red, 7, Qt::SolidLine, Qt::FlatCap));
                    tempPainter.drawPoint(m_points[i + 1]);
                }
                tempPainter.setPen(QPen(Qt::gray, 7, Qt::SolidLine, Qt::FlatCap));
                tempPainter.drawPoint(m_points[i]);
                m_dragged = i;
                mousePressed = true;
                closeToPoint = true;
                break;
            }
        }
    }
    if(!closeToPoint)
    {
        if(m_points.size() == 0)
        {
            tempPainter.setPen(QPen(Qt::red, 7, Qt::SolidLine, Qt::FlatCap));
            tempPainter.drawPoint(event->pos());
        }
        else
        {
            tempPainter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap));
            tempPainter.drawLine(m_points[m_points.size() - 1], event->pos());
            tempPainter.setPen(QPen(Qt::red, 7, Qt::SolidLine, Qt::FlatCap));
            tempPainter.drawPoint(m_points[m_points.size() - 1]);
            tempPainter.drawPoint(event->pos());
        }
        m_points.push_back(event->pos());
    }
    update();
}

void GSWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(mousePressed)
    {
        mousePressed = false;
        m_points[m_dragged] = event->pos();
        if(lineDrawn)
            createLine();
        else
        {
            QPainter tempPainter(&m_mainPix);
            tempPainter.fillRect(0, 0, 2000, 2000, Qt::white);
            tempPainter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap));
            for(size_t i = 0; i < m_points.size() - 1; i++)
                tempPainter.drawLine(m_points[i], m_points[i+1]);
            tempPainter.setPen(QPen(Qt::red, 7, Qt::SolidLine, Qt::FlatCap));
            for(size_t i = 0; i < m_points.size(); i++)
                tempPainter.drawPoint(m_points[i]);
        }
    }
    update();
}

void GSWidget::paintEvent(QPaintEvent */*event*/)
{
    m_painter.begin(this);
    m_painter.drawPixmap(0, 0, m_mainPix);
    m_painter.end();
}

void GSWidget::cleanPoints()
{
    auto g = geometry();
    h = g.height();
    w = g.width();

    m_mainPix = QPixmap(w, h);
    m_mainPix.fill(Qt::white);

    m_painter.begin(this);
    m_painter.drawPixmap(0, 0, m_mainPix);
    m_painter.end();

    mousePressed = false;
    closeToPoint = false;
    lineDrawn = false;

    m_points.clear();
    m_points1.clear();
}

QPointF findMiddle(QPointF a1, QPointF a2)
{
    if(a1 == a2)
        return a1;
    return QPointF((a1.x() + a2.x()) / 2, (a1.y() + a2.y()) / 2);
}

double controller(unsigned int a)
{
    if(a == 0)
        return 0;
    if(a % 4 == 0)
        return a / 2 + 0.5;
    else if(a % 4 == 1)
        return (a + 1) / 2;
    else if(a % 4 == 2)
        return a / 2 + 1;
    else
        return (a + 1) / 2 + 0.5;
}

void GSWidget::createLine()
{
    if(m_points.size() > 3)
    {
        QPainter tempPainter(&m_mainPix);
        if(lineDrawn)
        {
            tempPainter.fillRect(0, 0, 2000, 2000, Qt::white);
            tempPainter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap));
            for(size_t i = 0; i < m_points.size() - 1; i++)
                tempPainter.drawLine(m_points[i], m_points[i+1]);
            tempPainter.setPen(QPen(Qt::red, 7, Qt::SolidLine, Qt::FlatCap));
            for(size_t i = 0; i < m_points.size(); i++)
                tempPainter.drawPoint(m_points[i]);
        }
        tempPainter.setPen(QPen(Qt::blue, 3, Qt::SolidLine, Qt::FlatCap));
        m_points1.clear();
        double sumX;
        double sumY;
        unsigned int gl = 0;
        for(size_t k = 0; k < (2 * m_points.size() - 4) / 4 ; k++)
        {
            std::vector<QPointF> m_points2;
            m_points2.clear();
            for(size_t h = 0; h < 4; h++)
            {
                QPointF temp;
                temp = findMiddle(m_points[floor(controller(gl))], m_points[ceil(controller(gl))]);
                m_points2.push_back(temp);
                gl++;
                if(gl >= m_points.size() * 2 - 4)
                {
                    if(m_points2.size() > 0)
                        m_points2.pop_back();
                    m_points2.push_back(m_points[m_points.size() - 1]);
                    break;
                }
            }
            for(double t = 0.0; t <= 1.0; t += 0.01)
            {
                sumX = 0;
                sumY = 0;
                for(size_t i = 0; i < 4; i++)
                {
                    sumX += m_points2[i].x() * (bci(3, i) * pow(1 - t, 3 - i) * pow(t, i));
                    sumY += m_points2[i].y() * (bci(3, i) * pow(1 - t, 3 - i) * pow(t, i));
                }
                m_points1.push_back(QPointF(sumX, sumY));
            }
        }
        for(size_t i = 0; i < m_points1.size() - 1; i++)
            tempPainter.drawLine(m_points1[i], m_points1[i+1]);
        lineDrawn = true;
    }
    update();
}
