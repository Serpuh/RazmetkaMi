#include "func_new.h"
#include <ctype.h>

double Okr1(double value) {
	wchar_t buffer[128];
	swprintf(buffer, L"%.1lf", value);
	return _wtof(buffer);
}

double Okr2(double value){
	wchar_t buffer[128];
	swprintf(buffer, L"%.2lf", value);
	return _wtof(buffer);
}

double Okr3(double value){
	wchar_t buffer[128];
	swprintf(buffer, L"%.3lf", value);
	return _wtof(buffer);
}

double fromString(string str){
	istringstream ist(str);
	double d;
	ist>>d;
	return d;
}

double fromString(QString str) {
	istringstream ist(str.toStdString());
	double d;
	ist >> d;
	return d;
}

void Point3::rotateXY(Point3 pm, double ang) {
	// формула Лист1
	Point3& pd3 = *this;
	if (pm == pd3) return;
	double R = ::Distance(pm.x, pm.y, pd3.x, pd3.y);
	double alf = -ang;
	double fi = absLineAngle(PointD(pm.x, pm.y), PointD(pd3.x, pd3.y));
	double y1 = R * sin(fi - alf);
	double x1 = R * cos(fi - alf);

	pd3.x = pm.x + x1;
	pd3.y = pm.y + y1;
}


bool getCircleEquation(PointD PointD1, PointD PointD2, PointD PointD3, double *x0, double *y0, double *r)
{
	double det = (PointD1.x * PointD2.y + PointD2.x * PointD3.y + PointD3.x * PointD1.y
		- PointD1.x * PointD3.y - PointD2.x * PointD1.y - PointD3.x * PointD2.y);

	double EPSILON = 10E-5;
	if (fabs(det) < EPSILON)
	{
		// Три точки на одной прямой, нельзя построить окружность
		return 0;
	}

	// Вычисляем ординату центра
	*y0 = (PointD2.x * PointD3.x * PointD3.x - PointD3.x * PointD2.x * PointD2.x +
		PointD1.x * PointD1.x * PointD3.x - PointD1.x * PointD3.x * PointD3.x +
		PointD1.x * PointD2.x * PointD2.x - PointD2.x * PointD1.x * PointD1.x +

		PointD2.x * PointD3.y * PointD3.y - PointD1.x * PointD3.y * PointD3.y +
		PointD1.x * PointD2.y * PointD2.y - PointD2.x * PointD1.y * PointD1.y +
		PointD3.x * PointD1.y * PointD1.y - PointD3.x * PointD2.y * PointD2.y) / (2 * det);

	// Вычисляем абциссу центра
	if (fabs(PointD1.x - PointD2.x) < EPSILON)
	{
		*x0 = (PointD3.x * PointD3.x + PointD3.y * PointD3.y
			- PointD2.x * PointD2.x - PointD2.y * PointD2.y
			+ 2 * *y0 * PointD2.y
			- 2 * *y0 * PointD3.y) / (2 * (PointD3.x - PointD2.x));
	}
	else {
		*x0 = (PointD2.x * PointD2.x + PointD2.y * PointD2.y
			- PointD1.x * PointD1.x - PointD1.y * PointD1.y
			+ 2 * *y0 * PointD1.y
			- 2 * *y0 * PointD2.y) / (2 * (PointD2.x - PointD1.x));
	}

	// Вычисляем радиус
	*r = sqrt(pow(PointD1.x - *x0, 2) + pow(PointD1.y - *y0, 2));

	// Тестирование
	double r2 = sqrt(pow(PointD2.x - *x0, 2) + pow(PointD2.y - *y0, 2));
	double r3 = sqrt(pow(PointD3.x - *x0, 2) + pow(PointD3.y - *y0, 2));

	if (fabs(*r - r2) > EPSILON || fabs(*r - r3) > EPSILON || fabs(r2 - r3) > EPSILON)
	{
		//printf("TEST FAIL: (%f,%f) (%f,%f) (%f,%f) => (%f %f) %f %f %f\n", PointD1.x, PointD1.y, PointD2.x, PointD2.y, PointD3.x, PointD3.y, *x0, *y0, *r, r2, r3);
		return false;
	}
	else return true;

	// Получили уравнение окружности
	return false;
}

// определяет четверть 1,2,3,4
int getQuarter(double ang) {
	if (ang >= 0 && ang < PI / 2.) return 1;
	else if (ang >= PI / 2. && ang < PI) return 2;
	else if (ang >= PI && ang < 1.5*PI) return 3;
	else return 4;
}

// определяет угол между углами по окружности против часовой стрелки
double angBetweenAngles(double ang1, double ang2) {
	int q1 = getQuarter(ang1);
	int q2 = getQuarter(ang2);
	if (q1 == q2) {
		if (ang2 > ang1)return ang2 - ang1;
		else return 2.*PI - (ang1 - ang2);
	}
	else if (q2 > q1) return ang2 - ang1;
	else return 2.*PI - ang1 + ang2;
}

bool testOnCircleAntiClockWise(vector<QPointF> vv) {
	// Проверка, что точки указаны против часовой стрелки
	double x0, y0, rad;
	bool bb = getCircleEquation(vv[0], vv[1], vv[2], &x0, &y0, &rad);
	if (!bb) return true;
	PointD cen(x0, y0);

	double angA = /*radToDeg*/(absLineAngle(cen, vv[0]));
	double angB = /*radToDeg*/(absLineAngle(cen, vv[1]));
	double angC = /*radToDeg*/(absLineAngle(cen, vv[2]));

	double ang1 = angBetweenAngles(angA, angB);
	double ang2 = angBetweenAngles(angA, angC);

	if (ang1 >= ang2) return false;
	else return true;

	return false;
}

Point3 pointLinesIntersect(Line3d ln1, Line3d ln2, double& delta) {
	double X1 = ln1.getX(), Y1 = ln1.getY(), Z1 = ln1.getZ();
	double X2 = ln2.getX(), Y2 = ln2.getY(), Z2 = ln2.getZ();

	double a11 = ln1.getP2().x - ln2.getP2().x;
	double a12 = ln1.getP2().y - ln2.getP2().y;
	double a13 = ln1.getP2().z - ln2.getP2().z;
	double a21 = X1, a22 = Y1, a23 = Z1;
	double a31 = X2, a32 = Y2, a33 = Z2;

	double det = (a11*a22*a33 + a12 * a23*a31 + a13 * a21*a32 - a13 * a22*a31 - a11 * a23*a32 - a12 * a21*a33);
	double det1 = Y1 * Z2 - Y2 * Z1;
	double det2 = Z1 * X2 - Z2 * X1;
	double det3 = X1 * Y2 - X2 * Y1;

	double ret = fabs(det) / sqrt(det1*det1 + det2 * det2 + det3 * det3);
	delta = det / sqrt(det1*det1 + det2 * det2 + det3 * det3);
	if (ret == 0.) {
		return Point3(0, 0, 0);
	}
	else {
		Point3 p1 = ln1.getP1(), p2 = ln2.getP1();
		double A1 = X1, B1 = Y1, C1 = Z1,
			A2 = X2, B2 = Y2, C2 = Z2;
		double a1 = p1.x, b1 = p1.y, c1 = p1.z,
			a2 = p2.x, b2 = p2.y, c2 = p2.z;

		//new2
		double f1, f2, f3, f4, f5, f6;
		f1 = (A1*A2 / C2 + B1 * B2 / C2 + C1);
		f2 = (A1*A1 / C1 + B1 * B1 / C1 + C1);
		f3 = A1 * A2*c2 / C2 - A1 * a2 - A1 * A1*c1 / C1 + A1 * a1 + B1 * B2*c2 / C2 - B1 * b2 - B1 * B1*c1 / C1 + B1 * b1;
		f4 = (A2*A2 / C2 + B2 * B2 / C2 + C2);
		f5 = (A2*A1 / C1 + B2 * B1 / C1 + C2);
		f6 = A2 * A2*c2 / C2 - A2 * a2 - A2 * A1*c1 / C1 + A2 * a1 + B2 * B2*c2 / C2 - B2 * b2 - B2 * B1*c1 / C1 + B2 * b1;

		double z1n = (f6*f1 / f4 - f3) / (f2 - f5 * f1 / f4);
		double x1n = X1 / Z1 * (z1n - p1.z) + p1.x;
		double y1n = Y1 / Z1 * (z1n - p1.z) + p1.y;

		double z2n = (f3 + z1n * f2) / f1;
		double x2n = X2 / Z2 * (z2n - p2.z) + p2.x;
		double y2n = Y2 / Z2 * (z2n - p2.z) + p2.y;

		Point3 pp1(x1n, y1n, z1n);
		Point3 pp2(x2n, y2n, z2n);
		Point3 pp = getMidPoint(pp1, pp2);

		return pp;
	}
}

vector<Point3> pointsOnDist(Line3d line, Point3 pt, double dist) {
	double X = line.getX(), Y = line.getY(), Z = line.getZ();
	Point3 ret1;
	ret1.z = pt.z - sqrt(dist*dist / (X*X / Z / Z + Y * Y / Z / Z + 1));
	ret1.x = X / Z * (ret1.z - pt.z) + pt.x;
	ret1.y = Y / Z * (ret1.z - pt.z) + pt.y;

	Point3 ret2;
	ret2.z = pt.z + sqrt(dist*dist / (X*X / Z / Z + Y * Y / Z / Z + 1));
	ret2.x = X / Z * (ret2.z - pt.z) + pt.x;
	ret2.y = Y / Z * (ret2.z - pt.z) + pt.y;

	vector<Point3> ret; ret.push_back(ret1); ret.push_back(ret2);
	return ret;
}

double relativeBaza(Point3 sn1, Point3 sn2, Point3 pd3) {
	double dist1 = ::Distance(sn1, pd3);
	double dist2 = ::Distance(sn2, pd3);

	Point3 pbl, pdal;
	double dist;
	if (dist1 <= dist2) {
		pbl = sn1;
		pdal = sn2;
		dist = dist1;
	}
	else {
		pbl = sn2;
		pdal = sn1;
		dist = dist2;
	}

	Line3d line1(pd3, pdal);

	//inline vector<Point3> pointsOnDist(Line3d line,Point3 pt,double dist);
	vector<Point3> v1 = pointsOnDist(line1, pd3, dist);

	double dista = Distance(pdal, v1[0]);
	double distb = Distance(pdal, v1[1]);

	double baza;
	if (dista < distb) {
		baza = Distance(v1[0], pbl);
	}
	else {
		baza = Distance(v1[1], pbl);
	}

	double ret = baza / dist;
	return ret;
}

