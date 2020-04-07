#include"Transform.h"

using namespace cv;
using namespace std;

Mat tp;  //����ִ��͸�ӱ任�ľ�����
int thresh_start = 90;  //�ǵ��ж���ʼ��ֵ
int thresh_stop = 10;  //�ǵ������ʼ��ֵ
bool op_ok = false;
vector<OpDate> opd;

parameter pa;

Mat GetCornerHarris(Mat frame)  //Ѱ��frame�е�harris�ǵ㲢����
{
	Mat g_image, d_image;
	Mat n_image;  //��һ�����ɵ�ͼ
	vector<Point2i> r;  //����ֵ

	cvtColor(frame, g_image, COLOR_BGR2GRAY);
	cornerHarris(g_image, d_image, 2, 3, 0.04, BORDER_DEFAULT);  //Ѱ�ҽǵ�
	normalize(d_image, n_image, 0, 255, NORM_MINMAX, CV_32FC1, Mat());  //��һ��

	return n_image;
}

vector<Rect2i> RangeInit()  //�˺���������Ѱ������������Χ����Ҫ�������ٴε���
{
	vector<Rect2i> range;
	range.push_back(Rect(550, 495, 20, 20));
	range.push_back(Rect(895, 495, 10, 10));
	range.push_back(Rect(675, 350, 10, 10));
	range.push_back(Rect(765, 350, 10, 10));
	return range;
}

float TransformInit(Mat FirstFrame)
{
	Mat may;  //�����������Ŷ�ͼ
	Point2f corner[4];  //�������ȷ����������
	Point2f tran[4];  //������ű任Ŀ����λ��
	vector<Rect2i> range;  //��������������ж���Χ
	Mat t;   //���ڴ��͸�Ӿ����Ա�����
	float thresh = 0;  //�洢��ʼ���������Ŷ�

	range = RangeInit();  //��ʼ����Χ����
	tran[0] = Point2f(501.25, 2298.7);  //��ʼ��Ŀ�������
	tran[1] = Point2f(998.75, 2298.7);
	tran[2] = Point2f(501.25, 443.1);
	tran[3] = Point2f(998.75, 443.1);
	may = GetCornerHarris(FirstFrame);  //��ȡ��������ͼ

	for(int i = 0; i < 4; i++)  //�ֱ��������4�����������Ŷ���ߵĵ�
	{
		Point2i temp;
		int x, y, max = 0;
		for (x = range[i].x; x < range[i].x + range[i].width; x++)
		{
			for (y = range[i].y; y < range[i].y + range[i].height; y++)
			{
				if ((int)may.at<float>(x, y) >= max)
				{
					temp.x = x;
					temp.y = y;
					max = (int)may.at<float>(x, y);
				}
			}
		}
		corner[i] = temp;
		thresh += max;
	}
	thresh /= 4;

	t = getPerspectiveTransform(tran, corner);  //��ȡ͸�ӱ任����
	invert(t, tp);  //�Ծ������棬��ȡ��任��
	return thresh;
}

vector<Point3d> camera2world(vector<Point2d> cp)
{
	vector<Point2d> c;
	vector<Point2d> wp;
	vector<Point3d> r;
	perspectiveTransform(cp, wp, tp);
	for (auto it : wp)  //�˴���ͬƽ�����껻�㹫ʽ��C4Dģ���ó�
	{
		it.x -= 200;
		Point3d temp;    //��ʼ����ʱ��ά��
		temp.x = it.x;
		temp.y = it.y;  //�ü�ȥ��ֵ����ִ��͸�ӱ任�������Ӱ������Ķ������أ������µ���

		//���´������ʵ��Ч�����Ѳ���ʹ�ã�������Ҫ��һ������
		//temp.y += 176.24 - 0.0621 * it.y;  //���ղ�ó����ĸ߶�21cm��������л���
		//temp.x += 51.643 - 0.0714 * it.x;

		//�Զ��Ż����֣�ֻ�е����ݼ������ݴ���4��ʱ�Ż������Զ��Ż�
		if (op_ok)
		{
			double x = temp.x, y = temp.y;
			x = pa.xx * temp.x + pa.yx * temp.y + pa.dx;
			y = pa.xy * temp.x + pa.yy * temp.y + pa.dy;
			temp.x = x;
			temp.y = y;
		}
		
		//���´���鳢���������Թ滮����ת�������ڵ�ƽ�棬Ӧ������ͷ������У׼
		if ((temp.y > 1700 && temp.x < 148 && temp.y < 2250) || (temp.y > 1550 && temp.x > 930 && temp.y < 2050))
			temp.z = 21;
		if ((temp.y > 2200 && temp.x < 140) || (temp.x >= 140 && temp.y < 2500 && temp.x < 400 && temp.y > 0.6222 * temp.x + 2153) && temp.y < 0.6667 * temp.x + 2300)
			temp.z = 61;
		if (temp.x > 724 && temp.y > 2500 && temp.y < 2250 && temp.y > 2907.6 - 0.7647 * temp.x)
			temp.z = 61;
		if ((temp.y > 2239.3 - 1.3333 * temp.x && temp.y < 1840.8 + 0.7 * temp.x && temp.y > 0.7 * temp.x + 1723 && temp.x < 432) ||
			(temp.x >= 432 && temp.x < 612 && temp.y > 2046 && temp.y < 2130))
			temp.z = 61;
		if ((temp.y < 1546 && temp.x > 708 && temp.y > 1015) || (temp.x > 868 && temp.y <= 1015))
			temp.z = 61;
		if (temp.x > 476 && temp.x < 709 && temp.y > 1239 && temp.y < 1442)
			temp.z = 61;
		if ((temp.x < 384 && temp.y > 1019 && temp.x > 152 && temp.y < 1300) || (temp.x < 144 && temp.y < 1562 && temp.y > 867))
			temp.z = 61;
		if ((temp.x > 952 && temp.y > 2006 && temp.y < 2202) || (temp.x < 112 && temp.y > 1024 && temp.y < 1147))
			temp.z = 85;

		if (temp.z == 21)  //21cm���������껻��
		{
			temp.y += 176.24 - 0.0621 * it.y;
			temp.x += 51.643 - 0.0714 * it.x;
		}
		else if (temp.z == 61)  //61cm���������껻��
		{
			temp.y += 626.99 - 0.2245 * it.y;
			temp.x += 163.73 - 0.2245 * it.x;
		}
		else if (temp.z == 85)  //85cm���������껻��
		{
			temp.y += 910.81 - 0.3220 * it.y;
			temp.x += 243.15 - 0.3342 * it.x;
		}
		//temp.z += 21;  //�ô���������汻ע�͵���ͬ������

		//Ϊ��ͼƬ�з�����֤�Ѿ��رյ�λת������Ҫ�������Ĳ���ȡ��ע��
		//temp.x *= 1.7;  //����ʱ��ά���x,y�ᵥλ������ת��Ϊcm
		//temp.y *= 1.7;

		r.push_back(temp);
	}
	return r;
}

Mat GetTranMatrix()
{
	return tp;
}

void optimize(OpDate n)
{
	opd.push_back(n);  //��������ĵ�������ݼ�
	if (opd.size() > 4)
	{
		double foot = 0.01, derivative[6];  //�˴��趨����ƫ��ʹ�õĲ������Ҿ��ô�Ż���Ҫ��һ�µ�
		op_ok = true;
		for (int i = 0; i < 6; i++)
			derivative[i] = pa.derivative(foot, i, opd);
		while (1)  //��ʼ�����ݶ��½���
		{
			double sum = 0;
			pa.xx += derivative[XX] * foot;  //��������ݶ��½��Ĺ�����ʹ���˲�����Ϊ�������粻����Ӧ��Ϊ����ֵ
			pa.xy += derivative[XY] * foot;
			pa.yx += derivative[YX] * foot;
			pa.yy += derivative[YY] * foot;
			pa.dx += derivative[DX] * foot;
			pa.dy += derivative[DY] * foot;
			for (int i = 0; i < 6; i++)
			{
				derivative[i] = pa.derivative(foot, i, opd);
				sum += derivative[i];
			}
			if (sum < 0.001)  //�˴��趨�������ݶ��½�ѭ��������
				break;
		}
	}
}