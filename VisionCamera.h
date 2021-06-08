#pragma once

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class VisionCamera
{

private:
	VideoCapture cap;
	Mat img, imgHSV, mask;
	int hmin = 45, smin = 110, vmin = 75;
	int hmax = 110, smax = 240, vmax = 255;

	vector<vector<int>> myColors{
	{44, 52, 75, 66, 118, 255}, //green
	{0, 194, 75, 18, 246, 255} //red
	//{hmin, smin, vmin, hmax, smax, vmax} //blue - temp (delete after)
	};
	vector<Scalar> myColorValues{ {0, 255, 0} };

	bool openHand = true;
	bool handDetected = false;

public:
	Point currentPoint;
	bool appIsRunning = true;
	int currentCrosshair = 0;
	float videoHeight;
	float videoWidth;
	bool redDetected = false;

	VisionCamera(VideoCapture vidCap);

	~VisionCamera();

	void colorSettings();
	Point getContours();
	void findColor();
	void displayImage(); //delete maybe - not used
	void update();
	Point getCrossHairCoords();
};

