#include "PicView.h"
#include <QPainter>
#include <QResizeEvent>
#include <QDebug>
#include <QStaticText>
#include <QTextDocument>

PicView::PicView(QWidget *parent) : QFrame(parent)
{
	viewPort.setMinScale(800);
	viewPort.initMatrix();

	mat0 = cv::imread("D:\\Calib\\NX11b\\0.jpg");
	pix0 = new QPixmap(QPixmap::fromImage(Mat2QImage(mat0)));
}

void PicView::drawMy(QPainter& p) {	
	p.drawPixmap(0, 0, *pix0);

	p.setPen(QColor(255,0,0));
	p.drawLine(QPointF(0, 0), QPointF(500, 500));

	p.setPen(QColor(0, 255, 0));
	p.drawPoint(QPointF(20, 20));	
	
// 	QStaticText text("hui1");
// 	text.setTextWidth(1000);
// 	text.prepare();
// 	p.drawStaticText(-10, -10, text);

	QString equation = "hui1";	
	p.setPen(Qt::red);
	p.setFont(QFont("Arial", 1000));
	
	p.drawText(QPoint(0,0), equation);

	QTextDocument td;
	td.setHtml("K<sub>max</sub>=K<sub>2</sub> &middot; 3");
	td.drawContents(&p);
}

void PicView::paintEvent(QPaintEvent *event)
{
	QPainter paint;
	paint.setRenderHint(QPainter::Antialiasing);
	paint.setRenderHint(QPainter::SmoothPixmapTransform);

	paint.setBackground(QBrush(QColor(Qt::blue)));

	paint.begin(this);

	paint.setBackground(QBrush(QColor(Qt::blue)));

	paint.setMatrix(viewPort.matr);
	drawMy(paint);

	paint.end();
	qDebug() << "PicView::paintEvent"<< viewPort.matr;
}


void PicView::mousePressEvent(QMouseEvent *pme)
{
	Qt::MouseButton butt = pme->button();
	if (butt == Qt::MouseButton::MidButton) {
		this->setCursor(Qt::CursorShape::OpenHandCursor);
		QPointF pl = pme->localPos();
		viewPort.middleDown(pl);
		lastPos = pl;
	}	
	if (butt == Qt::MouseButton::RightButton) {
		OnFitWindow();		
	}
}

void PicView::OnFitWindow() {
	RectD jpgRect(0, 0, pix0->width(), pix0->height());

	viewPort.OnFitWindow(jpgRect);
	this->update();
}

void PicView::mouseReleaseEvent(QMouseEvent *event)
{
	this->setCursor(Qt::CursorShape::ArrowCursor);
}

void PicView::mouseMoveEvent(QMouseEvent *pme)
{
	currPoint = pme->localPos();
	qDebug() << "mouseMoveEvent " << currPoint;
	if (pme->buttons().testFlag(Qt::MidButton)) {
		viewPort.middleMove(currPoint);
	}

	viewPort.scrToReal2(currPoint);

	this->update();
}

void PicView::wheelEvent(QWheelEvent *event)
{
	short zDelta = 120;
	if (event->angleDelta().y() > 0) zDelta = 120;
	else zDelta = -120;

	currPoint = event->posF();

	viewPort.scale(zDelta, currPoint);
	qDebug() << "wheelEvent " << currPoint;
	this->update();

	int hh = 0;
}


void PicView::resizeEvent(QResizeEvent *event)
{
	QSize sz = event->size();
	viewPort.setWindowSize(sz.width(), sz.height());
	OnFitWindow();
}
