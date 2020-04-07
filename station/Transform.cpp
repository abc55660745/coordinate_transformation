#include"Transform.h"

using namespace cv;
using namespace std;

Mat tp;  //用来执行透视变换的矩阵组
int thresh_start = 90;  //角点判定起始阈值
int thresh_stop = 10;  //角点结束起始阈值
bool op_ok = false;
vector<OpDate> opd;

parameter pa;

Mat GetCornerHarris(Mat frame)  //寻找frame中的harris角点并返回
{
	Mat g_image, d_image;
	Mat n_image;  //归一化生成的图
	vector<Point2i> r;  //返回值

	cvtColor(frame, g_image, COLOR_BGR2GRAY);
	cornerHarris(g_image, d_image, 2, 3, 0.04, BORDER_DEFAULT);  //寻找角点
	normalize(d_image, n_image, 0, 255, NORM_MINMAX, CV_32FC1, Mat());  //归一化

	return n_image;
}

vector<Rect2i> RangeInit()  //此函数定义了寻找特征点的最大范围，需要结合相机再次调整
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
	Mat may;  //用来接收置信度图
	Point2f corner[4];  //用来存放确定的特征点
	Point2f tran[4];  //用来存放变换目标点的位置
	vector<Rect2i> range;  //用来存放特征点判定范围
	Mat t;   //用于存放透视矩阵以便求逆
	float thresh = 0;  //存储初始化过程置信度

	range = RangeInit();  //初始化范围数组
	tran[0] = Point2f(501.25, 2298.7);  //初始化目标点数组
	tran[1] = Point2f(998.75, 2298.7);
	tran[2] = Point2f(501.25, 443.1);
	tran[3] = Point2f(998.75, 443.1);
	may = GetCornerHarris(FirstFrame);  //获取焦点置信图

	for(int i = 0; i < 4; i++)  //分别遍历查找4个矩形中置信度最高的点
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

	t = getPerspectiveTransform(tran, corner);  //获取透视变换矩阵
	invert(t, tp);  //对矩阵求逆，获取点变换阵
	return thresh;
}

vector<Point3d> camera2world(vector<Point2d> cp)
{
	vector<Point2d> c;
	vector<Point2d> wp;
	vector<Point3d> r;
	perspectiveTransform(cp, wp, tp);
	for (auto it : wp)  //此处不同平面坐标换算公式由C4D模拟后得出
	{
		it.x -= 200;
		Point3d temp;    //初始化临时三维点
		temp.x = it.x;
		temp.y = it.y;  //该减去的值是在执行透视变换中受相机影响产生的多余像素，需重新调节

		//以下代码块因实测效果不佳不予使用，可能需要进一步调参
		//temp.y += 176.24 - 0.0621 * it.y;  //按照测得车中心高度21cm对坐标进行换算
		//temp.x += 51.643 - 0.0714 * it.x;

		//自动优化部分，只有当数据集中数据大于4组时才会启动自动优化
		if (op_ok)
		{
			double x = temp.x, y = temp.y;
			x = pa.xx * temp.x + pa.yx * temp.y + pa.dx;
			y = pa.xy * temp.x + pa.yy * temp.y + pa.dy;
			temp.x = x;
			temp.y = y;
		}
		
		//以下代码块尝试利用线性规划估计转换点所在的平面，应在摄像头上重新校准
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

		if (temp.z == 21)  //21cm高区域坐标换算
		{
			temp.y += 176.24 - 0.0621 * it.y;
			temp.x += 51.643 - 0.0714 * it.x;
		}
		else if (temp.z == 61)  //61cm高区域坐标换算
		{
			temp.y += 626.99 - 0.2245 * it.y;
			temp.x += 163.73 - 0.2245 * it.x;
		}
		else if (temp.z == 85)  //85cm高区域坐标换算
		{
			temp.y += 910.81 - 0.3220 * it.y;
			temp.x += 243.15 - 0.3342 * it.x;
		}
		//temp.z += 21;  //该代码块与上面被注释的起同样作用

		//为在图片中方便验证已经关闭单位转换，需要请把下面的部分取消注释
		//temp.x *= 1.7;  //将临时三维点的x,y轴单位由像素转换为cm
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
	opd.push_back(n);  //将新输入的点加入数据集
	if (opd.size() > 4)
	{
		double foot = 0.01, derivative[6];  //此处设定了求偏导使用的步长，我觉得大概还是要调一下的
		op_ok = true;
		for (int i = 0; i < 6; i++)
			derivative[i] = pa.derivative(foot, i, opd);
		while (1)  //开始进行梯度下降法
		{
			double sum = 0;
			pa.xx += derivative[XX] * foot;  //下面进行梯度下降的过程中使用了步长作为比例，如不合适应改为其他值
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
			if (sum < 0.001)  //此处设定了跳出梯度下降循环的条件
				break;
		}
	}
}