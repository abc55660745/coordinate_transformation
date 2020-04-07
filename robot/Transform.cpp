#include <cmath>
#include "Transform.h"

using namespace cv;
using namespace std;

double pi = atan(1) * 4;

Point3d GetAngel(Point3d me, Point3d traget)
{
	double x, y, z, l;
	Point3d r;
	x = traget.x - me.x;
	y = me.y - traget.y;
	z = traget.z - me.z;
	l = sqrt(x * x + y * y);
	r.x = atan2(y, x) * 180 / pi;
	if (r.x < 0)
		r.x += 360;
	r.y = atan2(z, l) * 180 / pi;
	r.z = sqrt(z * z + l * l);
	return r;
}

Point3d PrecTran(Point3d me, Point3d traget, Point2d face, Size2i frame, Size2d max_angel)
{
	Point3d angel = GetAngel(me, traget);
	Point3d r;

	//程序将优先处理水平面上超出视野的情况
	//再处理立体上超出视野的情况，而不是将其联合处理
	double max_l = face.x + max_angel.width / 2, max_r = face.x - max_angel.width / 2;
	double max_u = face.x + max_angel.height / 2, max_d = face.x - max_angel.height / 2;
	if (max_l > 360)
	{
		max_l -= 360;
	}
	else if (max_r < 0)
	{
		max_r += 360;
	}
	if (max_u > 360)
	{
		max_u -= 360;
	}
	else if (max_d< 0)
	{
		max_d += 360;
	}

	if (angel.x > max_l && angel.x < max_r)
	{
		r.z = 0;
		angel.x -= face.x;
		if (angel.x < 0)
			angel.x += 360;
		angel.x *= 180 / (360 - max_angel.width);
		angel.x += 180 - (180 * max_angel.width / (360 - max_angel.width));
		if (angel.x > 180)
			angel.x -= 360;
		angel.x = angel.x / 180 * pi;
		r.y = tan(angel.x) * frame.width / 2 * -1 + frame.height / 2;
		if (r.y > frame.height)
		{
			r.y = 0;
			r.x = frame.width - ((frame.height / 2) / tan(angel.x) + frame.width / 2);
		}
		else
			r.x = 0;
	}
	else if (angel.y > max_u)
	{
		r.z = 0;
		r.y = 0;
		angel.x -= face.x;
		angel.x *= max_angel.width / 360;
		angel.x = angel.x / 180 * pi;
		double l = (frame.width / 2) / tan(max_angel.width / 2);
		r.x = frame.width / 2 - l * tan(angel.x);
	}
	else if (angel.y < max_d)
	{
		r.z = 0;
		r.y = frame.height;
		angel.x -= face.x;
		angel.x *= max_angel.width / 360;
		angel.x = angel.x / 180 * pi;
		double l = (frame.width / 2) / tan(max_angel.width / 2);
		r.x = frame.width / 2 - l * tan(angel.x);
	}
	else
	{
		r.z = angel.z;
		angel.x -= face.x;
		if (angel.x > 180)
			angel.x -= 180;
		angel.x = angel.x / 180 * pi;
		double l = (frame.width / 2) / tan(max_angel.width / 2);
		r.x = frame.width / 2 - l * tan(angel.x);
		angel.y -= face.y;
		if (angel.y > 180)
			angel.y -= 180;
		angel.y = angel.y / 180 * pi;
		double l = (frame.height / 2) / tan(max_angel.height / 2);
		r.y = frame.height / 2 - l * tan(angel.y);
	}
	return r;
}