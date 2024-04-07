// Created by Gábor Bácsi
// 4/7/2024

#pragma once

#include "../Vector/Vector.hpp"
#include <fstream>
#include <string>
#include <vector>

class SvgShape {
public:
	virtual ~SvgShape() = default;
	virtual void plot(std::ofstream& svgStream, Vector2D<float> offset, float scale, float canvasHeight) = 0;
};

class SvgLine : public SvgShape {
public:
	SvgLine(Vector2D<float> a, Vector2D<float> b, const std::string& color = "#000000", float strokeWidth = 5);
	void plot(std::ofstream& svgStream, Vector2D<float> offset, float scale, float canvasHeight) override;
private:
	Vector2D<float> _A, _B;
	std::string _color;
	float _strokeWidth;
};

class SvgWiredPolygon : public SvgShape {
public:
	SvgWiredPolygon(std::vector<Vector2D<float>> vertices, std::vector<int> indices);
	void plot(std::ofstream& svgStream, Vector2D<float> offset, float scale, float canvasHeight) override;
private:
	std::vector<Vector2D<float>> _vertices;
	std::vector<int> _indices;
};
