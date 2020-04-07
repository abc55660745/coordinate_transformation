#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

enum parame { XX, XY, YX, YY, DX, DY };  //参数表枚举

struct OpDate {
	cv::Point2d origin;
	cv::Point3d fact;
};  //用于迭代数据存储

class parameter {
public:
	double xx, xy, yx, yy, dx, dy;  //优化所使用的参数
	parameter(double* p);  //使用数组初始化参数表，数组顺序同上初始化顺序
	parameter();  //使用默认值初始化参数表
	void SetValue(double* p);  //使用数组变量对参数表赋值
	double* GetValue();  //将参数表转换为数组
	double derivative(double foot, int dim, std::vector<OpDate> i);  //对参数表中某个参数求偏导，损失函数见TranClass.cpp中的cost()
};  //优化用参数类型

