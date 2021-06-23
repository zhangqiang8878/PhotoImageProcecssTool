#ifndef _WIPEITEM_H_
#define _WIPEITEM_H_

// Qt
#include <QGraphicsItem>

class QGraphicsSceneMouseEvent;

class WipeItem : public QGraphicsItem
{

public:

    WipeItem(QGraphicsItem * parent = 0);
    ~WipeItem();

    virtual QRectF	boundingRect () const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    void setImage1(const QImage &img);
    void setImage2(const QImage &img);
    void setWipeMethod(int method);

protected:

    virtual void	mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * event );
    virtual void	mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

private:

    QPixmap m_pix1;
    QPixmap m_pix2;
    bool m_wipe;
    QPointF m_wipePoint;
    int m_wipeMethod;

};

#endif // _WIPEITEM_H_
