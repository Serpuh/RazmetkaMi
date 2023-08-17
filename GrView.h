#pragma once
#include <QFrame>
#include <QPixmap>
#include <QGraphicsView>

#include "ViewPort.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;


class GrView : public QGraphicsView
{
	Q_OBJECT
public:
	explicit GrView(QWidget *parent = nullptr);

	void OnFitWindow();

private:
	QPixmap *pix0{0};
	Mat mat0;
	ViewPort viewPort;
	QPointF lastPos;
	QPointF currPoint;

	void drawMy(QPainter& paint);

	QGraphicsScene *scene{ 0 };

signals:

public slots:

	// QWidget interface
protected:
	void paintEvent(QPaintEvent *event) override;

	// QWidget interface
protected:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

	// QWidget interface
protected:
	void resizeEvent(QResizeEvent *event) override;
};

inline QImage Mat2QImage2(cv::Mat const& src)
{
	cv::Mat temp; // make the same cv::Mat
	cv::cvtColor(src, temp, cv::COLOR_BGR2RGB); // cvtColor Makes a copt, that what i need
	QImage dest((const uchar *)temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
	dest.bits(); // enforce deep copy, see documentation
				 // of QImage::QImage ( const uchar * data, int width, int height, Format format )
	return dest;
}

