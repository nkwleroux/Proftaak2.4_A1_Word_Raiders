#pragma once
#include "tigl.h"
using tigl::Vertex;

class BoundingBox {

	int x;

	std::vector<Vertex> verts;

	std::vector<Vertex> makeBoundingBox(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);

};
