#pragma once
#include <string>
#include <sstream>
#include <typeinfo>
#include <math.h>

#include <QString>
#include <QPoint>
#include <QMetaType>
#include <QDataStream>

#include "opencv2/core.hpp"

using namespace std;

#define PI 3.14159265358979

template<class T>
string toStrStl(T t){
	ostringstream ost;
	ost<<t;
	return ost.str();
};

template<class T>
string toStrStl(T t, int i){
	ostringstream ost;
	ost.setf(ios::showpoint | ios::fixed);
	ost.precision(i);
	ost<<t;
	return ost.str();
};

template<class T>
wstring toWstringStl(T t){
	wostringstream ost;
	ost<<t;
	return ost.str();
};

template<class T>
wstring toWstringStl(T t, int i){
	wostringstream ost;
	ost.setf(ios::showpoint | ios::fixed);
	ost.precision(i);
	ost<<t;
	return ost.str();
};

double fromString(string str);
double fromString(QString str);

double Okr1(double value);
double Okr2(double value);
double Okr3(double value);

struct Point3 {
	Point3() {}
	Point3(double x1, double y1, double z1) : x(x1), y(y1), z(z1) {}

	Point3(const cv::Matx31d& mat) : x(mat(0, 0)), y(mat(1, 0)), z(mat(2, 0)) {}

	Point3(const QPointF& qp) : x(qp.x()), y(qp.y()), z(0.) {}

	Point3(const cv::Point2f& pf) : x(pf.x), y(pf.y), z(0.) {}

	void setPoints(double x1, double y1, double z1) {
		x = x1; y = y1; z = z1;
	}

	Point3 operator - (const Point3& pt)const {
		Point3 ret;
		ret.x = x - pt.x;
		ret.y = y - pt.y;
		ret.z = z - pt.z;
		return ret;
	}

	Point3 operator + (const Point3& pt)const {
		Point3 ret;
		ret.x = x + pt.x;
		ret.y = y + pt.y;
		ret.z = z + pt.z;
		return ret;
	}

	Point3 &operator += (const Point3 &pt){
		x += pt.x;
		y += pt.y;
		z += pt.z;
		return *this;
	}

	Point3 &operator /= (double delim) {
		x /= delim;
		y /= delim;
		z /= delim;
		return *this;
	}

	Point3 &operator *= (double mnoj) {
		x *= mnoj;
		y *= mnoj;
		z *= mnoj;
		return *this;
	}

	bool operator == (const Point3& pt)const {
		return pt.x == x && pt.y == y && pt.z == z;
	}
	bool operator != (const Point3& pt)const {
		return pt.x != x && pt.y != y && pt.z != z;
	}

	bool isEqual2(const Point3& pt)const {
		if (Okr2(pt.x) == Okr2(x) && Okr2(pt.y) == Okr2(y) && Okr2(pt.z) == Okr2(z))return true;
		else return false;
	}

	void move(Point3 pm) {
		*this = *this + pm;
	}

	void rotateXY(Point3 pm, double ang);

	operator cv::Point2d() { return cv::Point2d(x, y); }
	operator cv::Matx31d() { return cv::Matx31d(x, y, z); }

	operator QPointF() { return QPointF((qreal)x, (qreal)y); }
	
	double x;
	double y;
	double z;
};

inline void inv(Point3& p) {
	p.y *= -1.;
}

struct PointD {
	PointD() { x = 0; y = 0; }
	PointD(double x1, double y1) :x(x1), y(y1) {}

	PointD(const QPointF& cp) :x(cp.x()), y(cp.y()) {}

	//PointD(const PointD& pd):x(pd.x),y(-pd.y){}
	void set(double x1, double y1) { x = x1; y = y1; }
	PointD inv() {
		return PointD(x, -y);
	}
	PointD invXY() {
		return PointD(-x, -y);
	}
	bool operator == (const PointD& p)const {
		return (x == p.x) && (y == p.y);
	}
	bool operator != (const PointD& p) {
		return !((x == p.x) && (y == p.y));
	}
	PointD operator-(const PointD& pt)const {
		PointD ret;
		ret.x = x - pt.x;
		ret.y = y - pt.y;
		return ret;
	}
	PointD operator+(const PointD& pt)const {
		PointD ret;
		ret.x = x + pt.x;
		ret.y = y + pt.y;
		return ret;
	}

	//operator CPoint() { return CPoint(mathOkrI(x), mathOkrI(y)); }
	operator QPointF() { return QPointF((qreal)x, (qreal)y); }

	operator Point3() { return Point3(x, y, 0); }

	PointD operator  / (double del) { return PointD(x / del, y / del); }
	void invY() { y *= -1.; }
	PointD offset(double xo, double yo) {
		return PointD(x + xo, y + yo);
	}

	//void rotateXY(PointD pm, double ang);

	void moveXY(PointD mp) {
		*this = *this + mp;
	}
	void moveXY(Point3 p3) {
		x = x + p3.x;
		y = y + p3.y;
	}

	double x, y;
};

inline double distance3d(Point3 p1, Point3 p2) {
	return sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y) + (p2.z - p1.z)*(p2.z - p1.z));
}

inline double distance2dxy(Point3 p1, Point3 p2) {
	return sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
}

inline double distance2dyz(Point3 p1, Point3 p2) {
	return sqrt((p2.y - p1.y)*(p2.y - p1.y) + (p2.z - p1.z)*(p2.z - p1.z));
}

inline double distance2dxz(Point3 p1, Point3 p2) {
	return sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.z - p1.z)*(p2.z - p1.z));
}

inline double Distance(double x1, double y1, double x2, double y2) {
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}
inline double Distance(const PointD& p1, const PointD& p2) {
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}
inline double Distance(const Point3& p1, const Point3& p2) {
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y) + (p1.z - p2.z)*(p1.z - p2.z));
}

inline double radToDeg(double rad) { return (rad * 180.) / PI; }
inline double degToRad(double deg) { return deg * PI / 180.; }

// угол между прямыми по точкам
inline double linesAngleOnPoints(PointD p0, PointD p1, PointD p2) {
	double x0 = p0.x;
	double x1 = p1.x;
	double x2 = p2.x;
	double y0 = p0.y;
	double y1 = p1.y;
	double y2 = p2.y;
	double ret;

	ret = ((x1 - x0)*(x2 - x0) + (y1 - y0)*(y2 - y0)) / (sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0))*sqrt((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0)));

	return acos(ret);
}

inline double absLineAngle(PointD p0, PointD p1) {
	if (p0 == p1)return 0;
	double ret = linesAngleOnPoints(p0, PointD(p0.x + 1, p0.y), p1);
	if (p1.y > p0.y) return ret;
	else {
		ret = 2.*PI - ret;
		if (ret == 2.*PI) ret = 0;
		return ret;
	}
	return 0;
}

inline double absLineAngle(Point3 p0, Point3 p1) {
	if (p0 == p1)return 0;
	return absLineAngle(PointD(p0.x,p0.y), PointD(p1.x, p1.y));	
}

class Line3d {
public:
	Line3d() {}
	Line3d(Point3 p11, Point3 p22) : p1(p11), p2(p22) {
		X = p2.x - p1.x;
		Y = p2.y - p1.y;
		Z = p2.z - p1.z;
	}
	Line3d(Point3 p11, double X1, double Y1, double Z1) {
		p1 = p11;
		X = X1;
		Y = Y1;
		Z = Z1;
	}
	void setPoints(Point3 p11, Point3 p22) {
		p1 = p11;
		p2 = p22;
		X = p2.x - p1.x;
		Y = p2.y - p1.y;
		Z = p2.z - p1.z;
	}

	Point3 getXYonZ(double z) {
		Point3 ret;
		ret.z = z;
		ret.x = X / Z * (z - p1.z) + p1.x;
		ret.y = Y / Z * (z - p1.z) + p1.y;
		return ret;
	}

	double getLineAngleOnXYPlane() {
		Point3 p00 = getXYonZ(0);
		Point3 pend;
		if (p1.z >= p2.z) { pend = p1; }
		else pend = p2;

		Point3 pend0 = pend;
		pend0.z = 0;

		double a = pend.z;
		double b = distance3d(p00, pend0);

		return atan(a / b);
	}

	Point3 getP1()const { return p1; }
	Point3 getP2()const { return p2; }

	double getX()const { return X; }
	double getY()const { return Y; }
	double getZ()const { return Z; }

private:
	Point3 p1;
	Point3 p2;
	double X, Y, Z;
};

inline Point3 getMidPoint(Point3 pt1, Point3 pt2) {
	Point3 ret;
	ret.x = (pt1.x + pt2.x) / 2.;
	ret.y = (pt1.y + pt2.y) / 2.;
	ret.z = (pt1.z + pt2.z) / 2.;
	return ret;
}

inline QPointF getMidPoint(QPointF pt1, QPointF pt2) {
	return QPointF((pt1.x() + pt2.x()) / 2., (pt1.y() + pt2.y()) / 2.);
}

inline cv::Point2f getMidPoint(cv::Point2f pt1, cv::Point2f pt2) {
	return cv::Point2f((pt1.x + pt2.x) / 2., (pt1.y + pt2.y) / 2.);
}



inline PointD lineCenter(const PointD& p1, const PointD& p2) {
	PointD ret;
	ret.x = (p1.x + p2.x) / 2.;
	ret.y = (p1.y + p2.y) / 2.;
	return ret;
}

inline Point3 mid3(const Point3& p1, const Point3& p2) {
	Point3 ret;
	ret.x = (p1.x + p2.x) / 2.;
	ret.y = (p1.y + p2.y) / 2.;
	ret.z = (p1.z + p2.z) / 2.;
	return ret;
}

inline PointD mid2(const PointD& p1, const PointD& p2) {
	PointD ret;
	ret.x = (p1.x + p2.x) / 2.;
	ret.y = (p1.y + p2.y) / 2.;
	return ret;
}

// CRect
//4+--+3
//1+--+2
class RectD {
public:
	RectD() {
		xmin = 0.;
		xmax = 0.;
		ymin = 0.;
		ymax = 0.;
		p1 = PointD(0, 0);
		p2 = PointD(0, 0);
		p3 = PointD(0, 0);
		p4 = PointD(0, 0);
	}
	RectD(double xMin, double yMin, double xMax, double yMax) {
		//if((xMin>xMax)||(yMin>yMax)) throw Err("Incorrect max min values in RectD");			
		p1 = PointD(xMin, yMin);
		p2 = PointD(xMax, yMin);
		p3 = PointD(xMax, yMax);
		p4 = PointD(xMin, yMax);
		xmin = xMin;
		xmax = xMax;
		ymin = yMin;
		ymax = yMax;
	}
	RectD(PointD pt1, PointD pt2, PointD pt3, PointD pt4)
		: p1(pt1), p2(pt2), p3(pt3), p4(pt4) {
	}
	double getHeight() {
		double d = Distance(p1, p4);
		return d;
	}
	double getWeight() {
		double d = Distance(p1, p2);
		return d;
	}
	PointD getCenterPoint() {
	}

	PointD p1, p2, p3, p4;
	double xmin, xmax, ymin, ymax;
};

bool getCircleEquation(PointD PointD1, PointD PointD2, PointD PointD3, double *x0, double *y0, double *r);

// определяет четверть 1,2,3,4
int getQuarter(double ang);

// определяет угол между углами по окружности против часовой стрелки
double angBetweenAngles(double ang1, double ang2);

bool testOnCircleAntiClockWise(vector<QPointF> vv);

class Err {
public:
	Err() {}
	Err(string err) { str = err; }
	string what() { return str; }
private:
	string str;
};

struct Mtime {

	Mtime() { start = clock(); }
	void run() { start = clock(); }
	double fin() {
		finish = clock();
		ret = (double)(finish - start) / CLOCKS_PER_SEC;
		return ret;
	}
	clock_t start, finish;
	double ret;
};

inline int sign(double val) {
	if (val > 0)return 1;
	else return -1;
}



// y=k*x+b k,b по точкам
inline void lineKB(const double& x1, const double& y1,
	const double& x2, const double& y2,
	double& k, double& b) {
	k = (y2 - y1) / (x2 - x1);
	b = y1 - k * x1;
}
inline void lineKB(const PointD& p1, const PointD& p2, double& k, double& b) {
	k = (p2.y - p1.y) / (p2.x - p1.x);
	b = p1.y - k * p1.x;
}

inline void lineKB(const Point3& p1, const Point3& p2,
	double& k, double& b) {
	k = (p2.y - p1.y) / (p2.x - p1.x);
	b = p1.y - k * p1.x;
}

inline void lineKB(const cv::Point2f& p1, const cv::Point2f& p2,
	double& k, double& b) {
	k = (p2.y - p1.y) / (p2.x - p1.x);
	b = p1.y - k * p1.x;
}

inline cv::Point2f linesIntersec(double k1, double b1, double k2, double b2) {
	float xp = (b2 - b1) / (k1 - k2);
	float yp = k1 * xp + b1;
	return cv::Point2f(xp, yp);
}

inline void planeEquation(const Point3& p1, const Point3& p2, const Point3& p3, double& A, double& B, double& C, double& D) {
	cv::Matx13d m12(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
	cv::Matx31d m13(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);

	cv::Matx33d a(0, 0, 0,
		p2.x - p1.x, p2.y - p1.y, p2.z - p1.z,
		p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);

	double i = a(1, 1)*a(2, 2) - a(1, 2)*a(2, 1);
	double j = a(1, 2)*a(2, 0) - a(1, 0)*a(2, 2);
	double k = a(1, 0)*a(2, 1) - a(1, 1)*a(2, 0);

	A = i; B = j; C = k;
	D = -i * p1.x - j * p1.y - k * p1.z;
}


struct Plane {
	Plane(const Point3& p11, const Point3& p22, const Point3& p33) : p1(p11), p2(p22), p3(p33) {
		planeEquation(p1, p2, p3, A, B, C, D);
	}
	double A, B, C, D;
	Point3 p1, p2, p3;
};

Point3 pointLinesIntersect(Line3d ln1, Line3d ln2, double& delta);

vector<Point3> pointsOnDist(Line3d line, Point3 pt, double dist);

double relativeBaza(Point3 sn1, Point3 sn2, Point3 pd3);



