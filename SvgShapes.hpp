// Created by Gábor Bácsi
// 4/7/2024

#pragma once

#include "../Math/Vector.hpp"
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
	SvgLine(Vector2D<float> a, Vector2D<float> b, const std::string& color, float strokeWidth);
	void plot(std::ofstream& svgStream, Vector2D<float> offset, float scale, float canvasHeight) override;
private:
	Vector2D<float> _A, _B;
	std::string _color;
	float _strokeWidth;
};

class SvgCircle : public SvgShape {
public:
	SvgCircle(Vector2D<float> center, float r, const std::string& color);
	void plot(std::ofstream& svgStream, Vector2D<float> offset, float scale, float canvasHeight) override;
private:
	Vector2D<float> _center;
	std::string _color;
	float _radius;
};

class SvgPolygon : public SvgShape {
public:
	SvgPolygon(std::vector<Vector2D<float>> vertices, std::vector<int> indices, const std::string& name, const std::string& fillColor, const std::string& strokeColor, float strokeWidth, bool animated);
	void plot(std::ofstream& svgStream, Vector2D<float> offset, float scale, float canvasHeight) override;
private:
	std::string _name;
	std::vector<Vector2D<float>> _vertices;
	std::vector<int> _indices;
	float _strokeWidth;
	std::string _fillColor, _strokeColor;
	bool _animated;
};
