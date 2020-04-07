/*
#####################################
#								    #
#	V.1.2�汾  ��������δ���в���   #
#		 ������Ҫ��һ���Ż�         #
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
//����һ֡��ʼͼ�񣬶�����ת�����г�ʼ�������ر任�������Ŷȣ���ֵӦ������60��������90Ӧ������һ֡���³�ʼ��
//ע���ʼ�����������ͼ������Ƚ��л������

void optimize(OpDate n);
//����һ��������ݣ����ݸ�ʽ��TranClass.hͷ�ļ�

std::vector<cv::Point3d> camera2world(std::vector<cv::Point2d> cp);
//������Ҫ�任�ĵ�����飬���ظ������еĵ������������е�λ�ã�ȡ��ͼ���ҷ��ӽ����Ͻ�Ϊ��㣬��λΪcm

cv::Mat GetTranMatrix();
//��ȡ���ڱ任�ľ����Ա������������
