#pragma once

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "glm/glm.hpp"

using namespace std;
using namespace cv;

class VisionCamera
{

private:
	VideoCapture cap;
	Mat img, imgHSV, mask;
	int hmin = 45, smin = 110, vmin = 75;
	int hmax = 110, smax = 240, vmax = 255;

	
	vector<Scalar> myColorValues{ {0, 255, 0} };

	bool openHand = true;
	bool handDetected = false;

public:
	Point currentPoint;
	int currentCrosshair = 0;
	float videoHeight = 0;
	float videoWidth = 0;
	bool redDetected = false;

	VisionCamera(VideoCapture vidCap);

	~VisionCamera();

	void colorSettings(vector<int> *colorValues);
	Point getContours();
	void findColor();
	void displayImage(); //delete maybe - not used
	void update();
	glm::vec2 getCrossHairCoords();
	vector<vector<int>> myColors{
		//{16, 28, 7, 93, 106, 172}, //Yellow - temp (delete after)
		//{hmin, smin, vmin, hmax, smax, vmax} //blue - temp (delete after)
		{0, 146, 165, 22, 236, 255}, //red
		{55, 52, 7, 93, 106, 172}, //green
	};
};

