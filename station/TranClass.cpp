#include"TranClass.h"
#include<string>

double cost(std::vector<OpDate> i, parameter pam)
{
	double l = 0;
	for (auto it : i)
	{
		double x = it.origin.x, y = it.origin.y;
		x = pam.xx * it.origin.x + pam.yx * it.origin.y + pam.dx;
		y = pam.xy * it.origin.x + pam.yy * it.origin.y + pam.dy;
		l += pow(it.fact.x - x, 2) + pow(it.fact.y - y, 2);
	}
	return l;
}

parameter::parameter(double* p)
{
	xx = p[0];
	xy = p[1];
	yx = p[2];
	yy = p[3];
	dx = p[4];
	dy = p[5];
}

parameter::parameter()
{
	xx = 0.9286;
	xy = 0;
	yx = 0;
	yy = 0.9379;
	dx = 51.643;
	dy = 176.24;
}

void parameter::SetValue(double* p)
{
	xx = p[0];
	xy = p[1];
	yx = p[2];
	yy = p[3];
	dx = p[4];
	dy = p[5];
}

double* parameter::GetValue()
{
	double p[6];
	p[0] = xx;
	p[1] = xy;
	p[2] = yx;
	p[3] = yy;
	p[4] = dx;
	p[5] = dy;
	return p;
}

double parameter::derivative(double foot, int dim, std::vector<OpDate> i)
{
	parameter t1 = *this, t2 = *this;

	switch (dim)
	{
	case XX:t1.xx -= foot / 2; t2.xx += foot / 2; break;
	case XY:t1.xy -= foot / 2; t2.xy += foot / 2; break;
	case YX:t1.yx -= foot / 2; t2.yx += foot / 2; break;
	case YY:t1.yy -= foot / 2; t2.yy += foot / 2; break;
	case DX:t1.dx -= foot / 2; t2.dx += foot / 2; break;
	case DY:t1.dy -= foot / 2; t2.dy += foot / 2; break;
	default:
		break;
	}

	double d = cost(i, t2) - cost(i, t1) / foot;

	return d;
}