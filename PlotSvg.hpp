// Created by Gábor Bácsi
// 4/5/2024

#pragma once

#include "type_defines.h"

#include <fstream>
#include <string>
#include <vector>

#include "../Math/Vector.hpp"

class SvgShape;

class PlotSvg {
public:
	PlotSvg(const std::string& filename, int width = 800, int height = 600);
	~PlotSvg();
	void addLine(VECTOR2D a, VECTOR2D b, const std::string& color = "#000000", float strokeWidth = 5);
	void addCircle(VECTOR2D center, float r = 1, const std::string& color = "#000000");
	void addPolygon(const std::vector<VECTOR2D> vertices, const std::vector<int> indices, const std::string& fillColor = "#00000000", const std::string& strokeColor = "#000000", float strokeWidth = 1, bool animated = false);
	void addText(VECTOR2D position, const std::string& text, const std::string& color, int size);
	void finalize();
private:
	void updateRect(const VECTOR2D& coordinate);
	std::ofstream _svgFile;
	std::string _svgFileName;
	VECTOR2D _rectLowestPoint, _rectHighestPoint;
	int _width, _height;
	std::vector<SvgShape*> _shapes;
};