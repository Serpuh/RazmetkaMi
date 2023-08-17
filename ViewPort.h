#pragma once
#include <QMatrix>
#include <QPainter>
#include <QDebug>

#include "func_new.h"
typedef QMatrix Matrix;

class ViewPort
{
public:
	// Координаты точки перемещающейся мыши
	PointD realP; // Реальные
	PointD realPinv; // Реальные c измененным Y
	PointD scrP;  // Экранные
	Matrix matr;
public:
	ViewPort();
//---------------------------------------------------------------------------------------------
	void initMatrix(){
		matr.reset();
	}

	void setPrintZoom(bool bSet){
		if(bSet){
			double r = 0.3;
			double ret = 1.;
			for(int i=0;i<4;i++) ret *=r;
			mnoj = 1. + ret;
		}
		else{
			double r = 0.68;
			double ret = 1.;
			for(int i=0;i<4;i++) ret *=r;
			mnoj = 1. + ret;
		}
	}

	void setZoom(double zoom){
		double r = zoom;
		double ret = 1.;
		for(int i=0;i<4;i++) ret *=r;
		mnoj = 1. + ret;
	}
	
	void setMinScale(double msc){minsc = msc;}

	void setPrintParam(bool bpr){
		bPrint = bpr;
		// временная заглушка
	}
	void setWindowSize(double x, double y){
		winX = x;
		winY = y;
		//matr.setMatrix(1.f,0.f,0.f,1.f,0.f,(float)winY);
		matr.setMatrix(1.f,0.f,0.f,1.f,0.f,0.f);
	}

//извлечение координат ---------------------------------------------------------------------------
	inline void scrToReal(PointD& inp) {
		QMatrix mt(matr);
		mt.inverted();

		//x' = m11*x + m21*y + dx
		//y' = m22*y + m12*x + dy

		double x = inp.x * mt.m11() + inp.y * mt.m21() + mt.dx();
		double y = inp.x * mt.m12() + inp.y * mt.m22() + mt.dy();

		inp.x = x;
		inp.y = -y;
	}

	inline void scrToReal2(const PointD& point){
		scrP = point;
		realP = point;
		scrToReal(realP);
		realPinv = realP;
		realPinv.y *= -1.;
	}

	inline PointD realToScr2(const PointD& inp) {
		double x = inp.x * matr.m11() + inp.y * matr.m21() + matr.dx();
		double y = inp.x * matr.m12() + inp.y * matr.m22() + matr.dy();
		return PointD(x, y);
	}

	inline void realToScr3(PointD& inp) {
		//REAL rr[6];
		//matr.GetElements(rr);
		double x = inp.x * matr.m11() + inp.y * matr.m21() + matr.dx();
		double y = inp.x * matr.m12() + inp.y * matr.m22() + matr.dy();
		inp.x = x;
		inp.y = -y; // test
	}

// Трансформация---------------------------------------------------------------------------
	virtual void applyTransform(QPainter& gr){
		//gr.resetMatrix();
		gr.setMatrix(matr);
		qDebug() << matr;
	}

	void applyTransform(QPainter& gr, const Matrix& matr2){
		gr.resetMatrix();

		QMatrix mt1(matr2);
		mt1 = mt1 * matr;
	}

	/*void applyAngleTransformPlan(Graphics& gr,PointF pos,double angle){
		gr.ResetTransform();
		Matrix mt1;
		mt1.Reset();
		mt1.RotateAt((float)-angle,pos,MatrixOrderAppend);
		mt1.Multiply(matr,MatrixOrderAppend);
		gr.SetTransform(&mt1);
	}

	void applyAngleTransform(Graphics& gr,PointF pos,double angle){
		gr.ResetTransform();
		Matrix mt1;
		mt1.Reset();
		mt1.RotateAt((float)-angle,pos,MatrixOrderAppend);
		//mt1.Multiply(matr,MatrixOrderAppend);
		gr.SetTransform(&mt1);
	}*/

// перемещение-----------------------------------------------------------------------------	
	void middleDown(const QPointF& point) {
		movePoint.set(point.x(), point.y());
	}

	void middleMove(const QPointF& point){
		double dx = point.x() - movePoint.x;
		double dy = point.y() - movePoint.y;
		movePoint = point;
		Matrix tmp(1.f,0.f,0.f,1.f,(float)dx,(float)dy);
		//REAL rr[6];
		//tmp.GetElements(rr);
		//matr->Multiply(&tmp,MatrixOrderAppend);
		matr = matr * tmp;
	}
	

	void scale(short zDelta, PointD pt){
		scrScalePoint = pt;
		if(zDelta>0) scaleFactor = mnoj;
		else scaleFactor = 1./mnoj;
		Matrix tmp((float)scaleFactor,0.f,0.f,(float)scaleFactor,(float)(-scrScalePoint.x*(scaleFactor-1.)),(float)(-scrScalePoint.y*(scaleFactor-1.)));
// 		REAL rr[6];
// 		tmp.GetElements(rr);
// 		matr->Multiply(&tmp,MatrixOrderAppend);
		matr = matr * tmp;

	}
	void OnFitWindow(RectD rc, int wdlg=0){			
		matr.setMatrix(1.f,0.f,0.f,1.f, (float)winX,(float)winY);
		PointD minp(rc.xmin,rc.ymin);
		PointD maxp(rc.xmax,rc.ymax);
		PointD cenp = mid2(minp,maxp); // центр объектов
		//cenp.y *= -1.;

		PointD minw(0,0);
		PointD maxw(winX,winY);
		PointD cenw = mid2(minw,maxw); // центр окна
		cenw.x += wdlg/2;

		qreal dx = cenw.x-cenp.x;
		qreal dy = cenw.y-cenp.y;

		matr.setMatrix(1,0,0,1,dx,dy); // установка объектов в центр окна

		double xmin = minp.x+dx;
		double xmax = maxp.x+dx;

		double ymin = -maxp.y+dy;
		double ymax = -minp.y+dy;

		PointD tmp1_max = maxp;
		PointD tmp1_min = minp;
		//tmp1_max.y *= -1.;
		//tmp1_min.y *= -1.;
		PointD tmp_max = realToScr2(tmp1_max);
		PointD tmp_min = realToScr2(tmp1_min);
		// объекты целиком меньше экрана
		if(xmin>0 && xmax< winX && ymin>0 && ymax<winY){
			while((tmp_max.x < winX)&&(tmp_max.y > 0)){
				scale(120,cenw);
				tmp_max = realToScr2(tmp1_max);
			}
			scale(-120,cenw);
			//scale(-120,cenw);
		}
		else{
			while((tmp_max.x > winX)||(tmp_max.y < 0)){
				scale(-120,cenw);
				tmp_max = realToScr2(tmp1_max);
			}
			scale(120, cenw);
		}
	}

	bool bTrans;
private:    
	double winX,winY;
	PointD movePoint;
	double currScale;
	int sc;
	double minsc;
	double msm; // величина смещения при нажатии на ->
	bool bPrint;
	double mnoj;
	double scaleFactor;
	PointD scrScalePoint;	
};
