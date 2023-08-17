#include "GrView.h"
#include <QPainter>
#include <QResizeEvent>
#include <QDebug>
#include <QStaticText>


GrView::GrView(QWidget *parent) : QGraphicsView(parent)
{
	viewPort.setMinScale(800);
	viewPort.initMatrix();

	mat0 = cv::imread("D:\\Calib\\NX11b\\0.jpg");
	pix0 = new QPixmap(QPixmap::fromImage(Mat2QImage2(mat0)));
	
	setMouseTracking(true);
	setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

	scene = new QGraphicsScene();
	

	scene->addPixmap(*pix0);
	scene->addLine(0, 0, 1000, 1000);
	
	QGraphicsView::setScene(scene);

	//QGraphicsView::setMatrix(viewPort.matr);
	//QTransform matrix(viewPort.matr);
	//QGraphicsView::setTransform(matrix);

}

void GrView::drawMy(QPainter& p) {	
// 	p.drawPixmap(0, 0, *pix0);
// 
// 	p.setPen(QColor(255,0,0));
// 	p.drawLine(QPointF(0, 0), QPointF(500, 500));
// 
// 	p.setPen(QColor(0, 255, 0));
// 	p.drawPoint(QPointF(20, 20));	
// 	
// 	QStaticText text("hui1");
// 	text.setTextWidth(1000);
// 	text.prepare();
// 	p.drawStaticText(-10, -10, text);
}

void GrView::paintEvent(QPaintEvent *event)
{
// 	QPainter paint;
// 	paint.setRenderHint(QPainter::Antialiasing);
// 	paint.setRenderHint(QPainter::SmoothPixmapTransform);
// 
// 	paint.begin(this);
// 
// 	paint.setMatrix(viewPort.matr);
// 	drawMy(paint);
// 
// 	paint.end();
// 	qDebug() << "GrView::paintEvent"<< viewPort.matr;
}


void GrView::mousePressEvent(QMouseEvent *pme)
{
// 	Qt::MouseButton butt = pme->button();
// 	if (butt == Qt::MouseButton::MidButton) {
// 		this->setCursor(Qt::CursorShape::OpenHandCursor);
// 		QPointF pl = pme->localPos();
// 		viewPort.middleDown(pl);
// 		lastPos = pl;
// 	}	
// 	if (butt == Qt::MouseButton::RightButton) {
// 		OnFitWindow();		
// 	}
// 	if (butt == Qt::MouseButton::LeftButton) {
// 		QPen _pen0;
// 		_pen0.setColor(QColor(0, 100, 200));
// 		_pen0.setWidth(10);
// 		scene->addRect(0, 0, 200, 200);
// 	}
}

void GrView::OnFitWindow() {
// 	RectD jpgRect(0, 0, pix0->width(), pix0->height());
// 
// 	viewPort.OnFitWindow(jpgRect);
	//this->update();
}

void GrView::mouseReleaseEvent(QMouseEvent *event)
{
	//this->setCursor(Qt::CursorShape::ArrowCursor);
}

void GrView::mouseMoveEvent(QMouseEvent *pme)
{
// 	currPoint = pme->localPos();
// 	qDebug() << "mouseMoveEvent " << currPoint;
// 	if (pme->buttons().testFlag(Qt::MidButton)) {
// 		viewPort.middleMove(currPoint);
// 	}
// 
// 	viewPort.scrToReal2(currPoint);
// 
// 	this->update();
}

void GrView::wheelEvent(QWheelEvent *event)
{
// 	short zDelta = 120;
// 	if (event->angleDelta().y() > 0) zDelta = 120;
// 	else zDelta = -120;
// 
// 	currPoint = event->posF();
// 
// 	viewPort.scale(zDelta, currPoint);
// 	qDebug() << "wheelEvent " << currPoint;
// 	this->update();
// 
// 	int hh = 0;
}


void GrView::resizeEvent(QResizeEvent *event)
{
// 	QSize sz = event->size();
// 	viewPort.setWindowSize(sz.width(), sz.height());
// 	OnFitWindow();
}
