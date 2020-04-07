#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

enum parame { XX, XY, YX, YY, DX, DY };  //������ö��

struct OpDate {
	cv::Point2d origin;
	cv::Point3d fact;
};  //���ڵ������ݴ洢

class parameter {
public:
	double xx, xy, yx, yy, dx, dy;  //�Ż���ʹ�õĲ���
	parameter(double* p);  //ʹ�������ʼ������������˳��ͬ�ϳ�ʼ��˳��
	parameter();  //ʹ��Ĭ��ֵ��ʼ��������
	void SetValue(double* p);  //ʹ����������Բ�����ֵ
	double* GetValue();  //��������ת��Ϊ����
	double derivative(double foot, int dim, std::vector<OpDate> i);  //�Բ�������ĳ��������ƫ������ʧ������TranClass.cpp�е�cost()
};  //�Ż��ò�������

