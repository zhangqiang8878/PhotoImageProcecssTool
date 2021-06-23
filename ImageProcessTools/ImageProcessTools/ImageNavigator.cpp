
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

#include <QtCore/QtDebug>
#include <QtGui/QResizeEvent>
#include <QtGui/QPainter>
#include <QGraphicsView>

#include "ImageNavigator.h"

const double factorMaxSize = 0.2; // max xx% of parent widget

// ImageNavigator --------------------------------------------------------------------

ImageNavigator::ImageNavigator(QGraphicsView* parent, Qt::WindowFlags flags) : QWidget(parent, flags),
    m_worldMatrix( new QTransform() ),
    m_imgMatrix( new QTransform() ),
    m_backgroundColor( QColor(0, 0, 0, 0) ),
    m_view(parent)
{
	setAcceptDrops(true);
}

void ImageNavigator::setImage(const QImage &img, std::vector<QRect> signBoxList)
{
    m_img = img;
	m_signBoxList = signBoxList;
    resizeImg();
}

void ImageNavigator::setTransforms(QTransform* worldMatrix, QTransform* imgMatrix)
{
    m_worldMatrix = worldMatrix;
    m_imgMatrix = imgMatrix;
}

void ImageNavigator::setViewPortRect(const QRectF &viewPortRect)
{
    m_viewPortRect = viewPortRect;
    resizeImg();
}

void ImageNavigator::paintEvent(QPaintEvent * /*event */)
{
    if (m_img.isNull() /*|| !m_imgMatrix || !m_worldMatrix*/)
        return;

    QPainter painter(this);

    //draw the image's location
    painter.setBrush(m_backgroundColor);
    painter.setPen( QColor(200, 200, 200) );

    painter.setOpacity(1.0f);//0.5f
    painter.drawImage( 0,0, m_imgT );
    painter.drawRect( m_imgT.rect().adjusted(0,0,0,0) );

    // compute view rect
    QRectF viewRect = m_view->mapToScene( m_view->rect() ).boundingRect();
    QRectF viewRectInImage = m_view->scene()->sceneRect().intersected(viewRect);

    QRectF viewRectInThumbnail = viewRectInImage.intersected( m_img.rect() );
    float reducFactor = m_imgT.height() / (float)m_img.height();
    viewRectInThumbnail = QRectF(viewRectInThumbnail.topLeft() * reducFactor,viewRectInThumbnail.size() * reducFactor);

    painter.setOpacity(1.0f);
	QPen pen(Qt::green, 2);//, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.setBrush( QColor(255, 0, 0, 5) );
    painter.drawRect(viewRectInThumbnail);
}

void ImageNavigator::mousePressEvent(QMouseEvent *event)
{
    m_enterPos = event->pos();
    m_lastPos = event->pos();
}

void ImageNavigator::mouseReleaseEvent(QMouseEvent *event)
{
    QPointF dxy = m_enterPos - QPointF( event->pos() );

    if (dxy.manhattanLength() < 4)
    {
        // compute view rect
        QRectF viewRect = m_view->mapToScene( m_view->rect() ).boundingRect();
        QRectF viewRectInImage = m_view->scene()->sceneRect().intersected(viewRect);

        QRectF viewRectInThumbnail = viewRectInImage.intersected( m_img.rect() );
        float reducFactor = m_imgT.height() / (float)m_img.height();
        viewRectInThumbnail = QRectF(viewRectInThumbnail.topLeft() * reducFactor,viewRectInThumbnail.size() * reducFactor);

		//更改为任意拖动，鼠标移出界面也可移动
		if (true)
        //if ( viewRectInThumbnail.contains( event->pos() ) )
        {
            QPointF newPt = event->pos() / reducFactor;
            emit moveView(newPt);
            event->accept();
        }
    }
}

void ImageNavigator::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() != Qt::LeftButton)
		return;
		

    // compute view rect
    QRectF viewRect = m_view->mapToScene( m_view->rect() ).boundingRect();
    QRectF viewRectInImage = m_view->scene()->sceneRect().intersected(viewRect);

    //QRectF viewRectInThumbnail = QRectF(viewRectInImage.topLeft()*factorMaxSize,viewRectInImage.size()*factorMaxSize);
    QRectF viewRectInThumbnail = viewRectInImage.intersected( m_img.rect() );
    float reducFactor = m_imgT.height() / (float)m_img.height();
    viewRectInThumbnail = QRectF(viewRectInThumbnail.topLeft() * reducFactor,viewRectInThumbnail.size() * reducFactor);

	//更改为任意拖动，鼠标移出界面也可移动
	if (true)
    //if ( viewRectInThumbnail.contains( event->pos() ) )
    {
        QPointF newPt = event->pos() / reducFactor;
        emit moveView(newPt);
        event->accept();
    }
}

void ImageNavigator::resizeEvent(QResizeEvent* event)
{
    if ( event->size() == size() )
        return;

    QWidget::resizeEvent(event);
}

QSize ImageNavigator::sizeHint() const
{
    return m_imgT.rect().adjusted(-1,-1,1,1).size();
}

void ImageNavigator::resizeImg()
{
    if ( m_img.isNull() )
        return;

    QSizeF maxSize = m_viewPortRect.size() * factorMaxSize;
    m_imgT = m_img.scaled(maxSize.toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    resize( sizeHint() );
	//highLightShow(m_signBoxList);
    return;

    QRectF imgRect = QRectF( QPoint(), m_img.size() );

    QTransform overviewImgMatrix = getScaledImageMatrix();          // matrix that always resizes the image to the current viewport
    QRectF overviewImgRect = overviewImgMatrix.mapRect(imgRect);
    overviewImgRect.setTop(overviewImgRect.top() + 1);
    overviewImgRect.setLeft(overviewImgRect.left() + 1);
    overviewImgRect.setWidth(overviewImgRect.width() - 1);            // we have a border... correct that...
    overviewImgRect.setHeight(overviewImgRect.height() - 1);

    // fast downscaling
    m_imgT = m_img.scaled(overviewImgRect.size().width(), overviewImgRect.size().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

QTransform ImageNavigator::getScaledImageMatrix()
{
    if ( m_img.isNull() )
        return QTransform();

    // the image resizes as we zoom
    QRectF imgRect = QRectF( QPoint(), m_img.size() );
    float ratioImg = imgRect.width() / imgRect.height();
    float ratioWin = (float)width() / (float)height();

    QTransform imgMatrix;
    float s;

    if (imgRect.width() == 0 || imgRect.height() == 0)
        s = 1.0f;
    else
        s = (ratioImg > ratioWin) ? (float)width() / imgRect.width() : (float)height() / imgRect.height();

    imgMatrix.scale(s, s);

    QRectF imgViewRect = imgMatrix.mapRect(imgRect);
    imgMatrix.translate( ( width() - imgViewRect.width() ) * 0.5f / s, ( height() - imgViewRect.height() ) * 0.5f / s );

    return imgMatrix;
}

//缺陷区域突出显示
//void ImageNavigator::highLightShow(std::vector<QRect> signBoxList)
//{
//	if (!signBoxList.size()){
//		m_backgroundColor = QColor(0, 0, 0, 100);
//		return;
//	}
//	m_backgroundColor = QColor(0, 0, 0, 0);
//
//	//图像转为opencv，绘制后返回
//	Mat cv_m_imgT, backgroundTrans;
//	cv_m_imgT = QImageToOpencvMat(m_imgT);
//	backgroundTrans = Mat(cv_m_imgT.size(), CV_8UC3, Scalar::all(100));
//	float zoomFactor = (float)m_imgT.width() / (float)m_img.width();
//	for (int i = 0; i < (int)signBoxList.size(); i++) {
//		Rect temp = Rect(signBoxList[i].x() * zoomFactor, signBoxList[i].y() * zoomFactor, signBoxList[i].width() * zoomFactor, signBoxList[i].height() * zoomFactor);
//		if (temp.width < 10) {
//			temp.x = temp.x - (10 - temp.width) / 2;
//			temp.width = 10;
//		}
//		if (temp.height < 10) {
//			temp.y = temp.y - (10 - temp.height) / 2;
//			temp.height = 10;
//		}
//		rectangle(backgroundTrans, temp, Scalar(0, 0, 255), -1);
//	}
//	addWeighted(cv_m_imgT, 0.2, backgroundTrans, 0.8, 0, cv_m_imgT);
//	//imwrite("cv_m_imgT.jpg", cv_m_imgT);
//	m_imgT = opencvMatToQImage(cv_m_imgT);
//}