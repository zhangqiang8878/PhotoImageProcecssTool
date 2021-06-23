
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

#ifndef _EDITORVIEW_H_
#define _EDITORVIEW_H_

// Qt
#include <QGraphicsView>
#include <QtCore/QPointF>
#include <QMenu>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QUrl>

class QGraphicsPixmapItem;
class ImageNavigator;
class WipeItem;

class ImageView : public QGraphicsView
{
	Q_OBJECT

public:

	ImageView(QWidget * map);
	~ImageView();

	//virtual void setImage(const QImage &newImg);

	void resetScale();
	void fitScale();
	qreal getScale() const;
	void setScale(qreal);

	void setShowOverview(bool);

	void setImage(const QImage &newImg, std::vector<QRect> signBoxList = std::vector<QRect>());

	void setMask(const QImage &newMask);

	void setSign(const QImage &newSign, QImage newSign2 = QImage(), QPointF signPos2 = QPointF());
	void setSignSecond(const QImage &newSign, QPointF signPos = QPointF());

	

	// functions for wipe effect
	void setWipeMode(bool);
	void setWipeMethod(int);
	void setWipeImage1(const QImage &img);
	void setWipeImage2(const QImage &img);

	// mask opacity (0.0->1.0)
	void setMaskOpacity(qreal);
	qreal maskOpacity() const;
	void setEnabledMask(bool);
	bool isMaskEnabled() const;
	void setEnabledMarker(bool);
	bool isMarkerEnabled() const;

	// Center
	const QPointF& getCenter() const
	{
		return m_centerPoint;
	}

	// mixShow option
	void setEnabledMixShow(bool, bool formatChecked = false, bool textChecked = false);
	bool isMixShowEnabled() const;

public slots:

	void updateSceneRect(const QRectF &);
	void setCenter(const QPointF& position);
	void setMarkerPosition(const QPointF &);
	void zoomOut();
	void zoomIn();
	void zoom(double factor);
	void translationQG(QKeyEvent *event);
	void deleteDrawOptionFunc();
	void addDrawOption();
	void showFormatCheckOptionFunc(bool state);
	void showTextCheckOptionFunc(bool state);

	// sign enable
	void setSignEnable(bool);
	void setSignEnableSecond(bool);

signals:

	void mouseMoved(const QPointF &pos);
	void scaleChanged(qreal);
	void somethingChanged(const QPointF &pt = QPointF()); // scale or center point change
	void mouseDragEvent();//QDragEnterEvent*event
	void deleteDrawSign(const QPointF &pos);
	void addDrawSign(const QPointF &pos);
	void showFormatSignClicked(bool);
	void showTextSignClicked(bool);

protected:

	//Take over the interaction
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent *mouseEvent);
	virtual void mouseReleaseEvent(QMouseEvent *mouseEvent);
	virtual void keyPressEvent(QKeyEvent *event);

	virtual void wheelEvent(QWheelEvent* event);
	virtual void resizeEvent(QResizeEvent* event);
	virtual void paintEvent(QPaintEvent* event);
	virtual bool event(QEvent *e);

	//拖动进入事件
	virtual void dragEnterEvent(QDragEnterEvent*event);
	virtual void dropEvent(QDropEvent *event);

	virtual void contextMenuEvent(QContextMenuEvent *event);
private:

	void initBackground();
	bool isImageInside();
	void drawMarker(QPainter &p);
	void drawLines(const QPointF &, QPainter &p);
	void createWipeItem();

	//Holds the current centerpoint for the view, used for panning and zooming
	QPointF m_centerPoint;
	QPixmap m_tileBg;

	ImageNavigator* m_navigator;

	QGraphicsPixmapItem *m_imageItem;
	QGraphicsPixmapItem *m_maskItem;
	QGraphicsPixmapItem *m_signItem;
	QGraphicsPixmapItem *m_signItemSecond;

	QPointF m_ImagePoint;

	// overview rendering
	float m_navigatorSize;
	float m_navigatorMargin;
	bool m_showNavigator;
	qreal m_maskOpacity;
	bool m_showMask;
	bool m_drag;
	bool m_showMarker;
	QPointF m_posMarker;
	bool m_wipeMode;
	int m_wipeMethod;
	WipeItem *m_wipeItem;

	// mask rightbutton option
	QMenu *contextmMenu = nullptr;
	QAction *deleteDrawOption;
	QAction *showFormatCheckOption;
	QAction *showTextCheckOption;
	QAction *showAnnotCheckOption;
	bool m_showFormatChecked;
	bool m_showTextChecked;
	bool m_MixShowEnabled;

	bool m_signEnabled;
	bool m_signEnabledSecond;
};

#endif // _EDITORVIEW_H_
