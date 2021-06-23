#ifndef _IMAGENAVIGATOR_H_
#define _IMAGENAVIGATOR_H_

#include <QWidget>

class QGraphicsView;

class ImageNavigator :   public QWidget
{
    Q_OBJECT

public:

    explicit ImageNavigator(QGraphicsView * parent = 0, Qt::WindowFlags f = 0);
    virtual ~ImageNavigator() {}

    void setImage(const QImage &img, std::vector<QRect> signBoxList = std::vector<QRect>());

   void setTransforms(QTransform* worldMatrix, QTransform* imgMatrix);

    void setViewPortRect(const QRectF &viewPortRect);

    virtual QSize sizeHint() const;

	//void highLightShow(std::vector<QRect> signRoiList);

signals:

    void moveView(const QPointF &dxy);

protected:

    void resizeImg();
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent* event);
    QTransform getScaledImageMatrix();

private:

    QImage m_img;
    QImage m_imgT;
    QTransform* m_worldMatrix;
    QTransform* m_imgMatrix;
    QRectF m_viewPortRect;
    QPointF m_lastPos;
    QPointF m_enterPos;
    QColor m_backgroundColor;
    QGraphicsView *m_view;
	std::vector<QRect> m_signBoxList;
};

#endif // _IMAGENAVIGATOR_H_
