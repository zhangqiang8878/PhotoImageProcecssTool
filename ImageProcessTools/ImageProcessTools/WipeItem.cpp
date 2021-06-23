
/******************************************************************************
   DiffImg: image difference viewer
   Copyright(C) 2011-2014  xbee@xbee.net

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *******************************************************************************/

// Qt
#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QtGui/QCursor>

#include "WipeItem.h"
#include "WipeMethod.h"


WipeItem::WipeItem(QGraphicsItem * parent) : QGraphicsItem(parent),
m_wipe(false),
m_wipeMethod(WipeMethod::WIPE_HORIZONTAL)
{
    //setFlag(QGraphicsItem::ItemIsSelectable,true);
    //setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton);
    setCursor(Qt::SplitHCursor);
}

WipeItem::~WipeItem()
{
}

QRectF WipeItem::boundingRect() const
{
    QRectF rect;

    if (!m_pix1.isNull() ||
         !m_pix2.isNull())
    {
        if (m_pix1.isNull()) 
            rect = QRectF(QPointF(0,0), m_pix2.size());
        else 
            rect = QRectF(QPointF(0,0), m_pix1.size());
    }

    //qDebug() << "boundingRect" << rect;
    return rect;
}

void WipeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                QWidget *widget)
{
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    switch (m_wipeMethod)
    {
    case WipeMethod::WIPE_VERTICAL:
        {
            painter->drawPixmap(QPoint(0,0), 
                m_pix2, 
                QRect(0,0,m_pix2.width(),m_wipePoint.y()));
            painter->drawPixmap(QPoint(0,m_wipePoint.y()), 
                m_pix1, 
                QRect(0,m_wipePoint.y(),m_pix1.width(),m_pix1.height()-m_wipePoint.y()));
            break;
        }
    default:
    case WipeMethod::WIPE_HORIZONTAL:
        {
            painter->drawPixmap(QPoint(0,0), 
                m_pix2, 
                QRect(0,0,m_wipePoint.x(),m_pix2.height()));
            painter->drawPixmap(QPoint(m_wipePoint.x(),0), 
                m_pix1, 
                QRect(m_wipePoint.x(),0,m_pix1.width()-m_wipePoint.x(),m_pix1.height()));
            break;
        }
    }

}

void WipeItem::setImage1(const QImage &img)
{
    m_pix1 = QPixmap::fromImage(img);
    m_wipePoint = QPointF(0,0);
    prepareGeometryChange ();
    update();
}

void WipeItem::setImage2(const QImage &img)
{
    m_pix2 = QPixmap::fromImage(img);
    m_wipePoint = QPointF(0,0);
    prepareGeometryChange ();
    update();
}

void WipeItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{

    if (m_wipe)
    {
        m_wipePoint = event->pos();
        update();
    }
    else
        event->ignore();
    QGraphicsItem::mouseMoveEvent(event);
}

void WipeItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    if (event->button() == Qt::RightButton)
    {
        m_wipe = true;
    }
    else
        QGraphicsItem::mousePressEvent(event);
}

void WipeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) 
{
    if (event->button() == Qt::RightButton)
    {
        m_wipe = false;
    }
   // QGraphicsItem::mouseReleaseEvent(event);
}

void WipeItem::setWipeMethod(int method)
{
    if (m_wipeMethod != method)
    {
        m_wipeMethod = method;
        m_wipePoint = QPointF(0,0);
        if (m_wipeMethod == WipeMethod::WIPE_HORIZONTAL)
            setCursor(Qt::SplitHCursor);
        else
            setCursor(Qt::SplitVCursor);
        update();
    }
}