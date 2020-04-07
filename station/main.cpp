#include "Transform.h"

using namespace cv;
using namespace std;

int main()
{
	Mat frame = imread("Z:\\tran\\t\\1.png", 1);
	float ok = TransformInit(frame);
	Mat tp = GetTranMatrix(), tt;
	invert(tp, tt);
	Mat dst;
	warpPerspective(frame,
		dst,
		tt,
		cv::Size(1500, 2714),
		cv::INTER_LINEAR | cv::WARP_INVERSE_MAP);
	//imshow("dst", dst);
	imwrite("ddd.png", dst);
	//waitKey(0);
	cout << ok << endl;
	vector<Point2d> t;
	t.push_back(Point2d(590, 610));
	t.push_back(Point2d(875, 610));
	t.push_back(Point2d(734, 455));
	vector<Point3d> r = camera2world(t);
	cout << r;
	OpDate t2;
	t2.origin = Point2d(590, 610);
	t2.fact = Point3d(590, 610, 0);
	optimize(t2);
	optimize(t2);
	optimize(t2);
	optimize(t2);
	optimize(t2);
	optimize(t2);
	r = camera2world(t);
	cout << r;
}