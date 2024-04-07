// Created by Gábor Bácsi
// 4/5/2024

#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "../Vector/Vector.hpp"

class SvgShape;

class PlotSvg {
public:
	PlotSvg(const std::string& filename, int width = 800, int height = 600);
	~PlotSvg();
	void addLine(Vector2D<float> a, Vector2D<float> b, const std::string& color = "#000000", float strokeWidth = 5);
	void addPolygon(const std::vector<Vector2D<float>> vertices, const std::vector<int> indices);
	void finalize();
private:
	std::ofstream _svgFile;
	Vector2D<float> _rectLowestPoint, _rectHighestPoint;
	int _width, _height;
	std::vector<SvgShape*> _shapes;
};