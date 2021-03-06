// 添加low‘s sift的算法.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat src3, src4;
	Mat src1 = imread("E:/Visual studio 2017/Codes/sift/13.jpg", 1);
	Mat src2 = imread("E:/Visual studio 2017/Codes/sift/12.jpg", 1);
	if ((src1.data == NULL) || (src2.data == NULL))
	{
		cout << "No exist" << endl;
		return -1;
	}


	resize(src1, src3, Size(src1.cols / 6, src1.rows / 6), (0, 0), (0, 0), 3);
	resize(src2, src4, Size(src2.cols / 6, src2.rows / 6), (0, 0), (0, 0), 3);

	Ptr<Feature2D> sift = xfeatures2d::SIFT::create(400);//括号参数越大越精确，自定
	vector<KeyPoint>  keypoints_1, keypoints_2;


	sift->detect(src3, keypoints_1);
	sift->detect(src4, keypoints_2);


	Mat descriptors_1, descriptors_2;
	sift->detectAndCompute(src3, noArray(), keypoints_1, descriptors_1);//得到特征点和特征点描述
	sift->detectAndCompute(src4, noArray(), keypoints_2, descriptors_2);

	//drawKeypoints(src3, keypoints_1, descriptors_1);//画出特征点
	//drawKeypoints(src4, keypoints_2, descriptors_2);

	//接下来就是显示了

	/*namedWindow("pic1");
	resizeWindow("pic1", 500, 500);
	imshow("pic1", descriptors_1);
	namedWindow("pic2");
	resizeWindow("pic2", 500, 500);
	imshow("pic2", descriptors_2);
	waitKey(0);*/
	//return 0;

	FlannBasedMatcher matcher;
	vector<vector<DMatch> > matchePoints;
	vector<DMatch> GoodMatchePoints;

	vector<Mat> train_desc(1, descriptors_1);
	matcher.add(train_desc);
	matcher.train();

	matcher.knnMatch(descriptors_2, matchePoints, 2);
	cout << "total match points: " << matchePoints.size() << endl;

	// Lowe's algorithm,获取优秀匹配点
	for (int i = 0; i < matchePoints.size(); i++)
	{
		if (matchePoints[i][0].distance < 0.6 * matchePoints[i][1].distance)
		{
			GoodMatchePoints.push_back(matchePoints[i][0]);
		}
	}

	Mat first_match;
	drawMatches(src4, keypoints_2, src3, keypoints_1, GoodMatchePoints, first_match);
	namedWindow("first_match");
	resizeWindow("first_match", 500, 500);
	imshow("first_match ", first_match);
	//imwrite("first_match.jpg", first_match);
	waitKey();
	return 0;
}
