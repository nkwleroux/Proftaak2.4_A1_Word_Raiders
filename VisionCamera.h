#pragma once

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "glm/glm.hpp"

// Using namespaces to make coding a bit easier
using namespace std;
using namespace cv;

class VisionCamera
{

private:
	// Creating variables
	// Variable for capturing videos
	VideoCapture videoCapture;
	// Image manipulation
	Mat img, imgHSV, mask;
	Point myPoint;
	//
	int hueMin = 45, saturationMin = 110, valueMin = 75;
	int hueMax = 110, saturationMax = 240, valueMax = 255;

	// Creating vector with colors
	vector<vector<int>> myColors{
	{0, 146, 165, 22, 236, 255}, //red
	{55, 52, 7, 93, 106, 172}, //green
	};
	vector<Scalar> myColorValues{ {0, 255, 0} };

	// Setting standard boolean variables
	bool openHand = true;
	bool handDetected = false;

public:
	// Variables we use
	// Point of VC
	Point currentPoint;
	// Crosshair we use
	int currentCrosshair = 0;
	// Store height and width of video
	float videoHeight = 0;
	float videoWidth = 0;
	// Boolean that stores if red is detected or not
	bool redDetected = false;

	// Constructor
	VisionCamera(VideoCapture vidCap);
	// Destructor
	~VisionCamera();

	// Method to set color settings
	void colorSettings();
	// Method to retreive contours
	Point getContours();
	// Method to find color
	void findColor();
	// Method to display an image on screen
	void displayImage();
	// Method to update the vision camera
	void update();
	// Method to retreive crosshair coordinates
	glm::vec2 getCrossHairCoords();
};

