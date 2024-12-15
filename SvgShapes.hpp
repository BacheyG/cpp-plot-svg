// Created by Gábor Bácsi
// 4/7/2024

#pragma once

#include "type_defines.h"

#include <fstream>
#include <string>
#include <vector>

class SvgShape {
public:
	virtual ~SvgShape() = default;
	virtual void plot(std::ofstream& svgStream, VECTOR2D offset, float scale, float canvasHeight) = 0;
};

class SvgLine : public SvgShape {
public:
	SvgLine(VECTOR2D a, VECTOR2D b, const std::string& color, float strokeWidth);
	void plot(std::ofstream& svgStream, VECTOR2D offset, float scale, float canvasHeight) override;
private:
	VECTOR2D _A, _B;
	std::string _color;
	float _strokeWidth;
};

class SvgCircle : public SvgShape {
public:
	SvgCircle(VECTOR2D center, float r, const std::string& color);
	void plot(std::ofstream& svgStream, VECTOR2D offset, float scale, float canvasHeight) override;
private:
	VECTOR2D _center;
	std::string _color;
	float _radius;
};

class SvgPolygon : public SvgShape {
public:
	SvgPolygon(std::vector<VECTOR2D> vertices, std::vector<int> indices, const std::string& name, const std::string& fillColor, const std::string& strokeColor, float strokeWidth, bool triangulatedIndices, bool animated);
	void plot(std::ofstream& svgStream, VECTOR2D offset, float scale, float canvasHeight) override;
private:
	std::string _name;
	std::vector<VECTOR2D> _vertices;
	std::vector<int> _indices;
	float _strokeWidth;
	std::string _fillColor, _strokeColor;
	bool _animated;
	bool _triangulated;
};

class SvgText : public SvgShape {
public:
	SvgText(VECTOR2D position, const std::string& text, const std::string& color, int size);
	void plot(std::ofstream& svgStream, VECTOR2D offset, float scale, float canvasHeight) override;
private:
	VECTOR2D _position;
	std::string _text;
	int _size;
	std::string _fillColor;
};
