/*
#####################################
#								    #
#	V.1.2版本  迭代器尚未进行测试   #
#		 参数需要进一步优化         #
#								    #
#####################################
*/

#pragma once
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <string>
#include "TranClass.h"

float TransformInit(cv::Mat FirstFrame);
//输入一帧初始图像，对坐标转换进行初始化，返回变换矩阵置信度，该值应当高于60，若低于90应传入另一帧重新初始化
//注意初始化函数传入的图像必须先进行畸变矫正

void optimize(OpDate n);
//输入一组迭代数据，数据格式见TranClass.h头文件

std::vector<cv::Point3d> camera2world(std::vector<cv::Point2d> cp);
//输入需要变换的点的数组，返回该数组中的点在世界坐标中的位置，取地图在我方视角左上角为零点，单位为cm

cv::Mat GetTranMatrix();
//获取用于变换的矩阵，以便进行其他处理
