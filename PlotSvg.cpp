// Created by Gábor Bácsi
// 4/5/2024

#include "PlotSvg.hpp"
#include "SvgShapes.hpp"

#include "../Math/Vector.hpp"
#include <limits>

static constexpr double k_MarginPercentage = 0.05f;

PlotSvg::PlotSvg(const std::string& filename, int width, int height) : _width(width), _height(height) {
	_svgFile = std::ofstream(filename);
	size_t dotPos = filename.find_last_of('.');
	_svgFileName = filename.substr(0, dotPos);

	_svgFile << "<svg height=\"" << height << "\" width=\"" << width << "\" xmlns=\"http://www.w3.org/2000/svg\" preserveAspectRatio=\"xMinYMin meet\" viewBox=\"0 0 " << width << " " << height << "\">\n";
	_rectLowestPoint = VECTOR2D(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	_rectHighestPoint = VECTOR2D(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
}

PlotSvg::~PlotSvg() {
	for (auto svgShape : _shapes) {
		delete svgShape;
	}
}

void PlotSvg::addLine(VECTOR2D A, VECTOR2D B, const std::string& color, float strokeWidth) {
	updateRect(A);
	updateRect(B);
	_shapes.push_back(new SvgLine(A, B, color, strokeWidth));
}

void PlotSvg::addCircle(VECTOR2D center, float r, const std::string& color) {
	updateRect(center);
	_shapes.push_back(new SvgCircle(center, r, color));
}

void PlotSvg::addPolygon(const std::vector<VECTOR2D> vertices, const std::vector<int> indices, bool triangulatedIndices, const std::string& fillColor, const std::string& strokeColor, float strokeWidth, bool animated) {
	for (int i = 0; i < vertices.size(); ++i) {
		updateRect(vertices[i]);
	}
	_shapes.push_back(new SvgPolygon(vertices, indices, _svgFileName, fillColor, strokeColor, strokeWidth, triangulatedIndices, animated));
}

void PlotSvg::addText(VECTOR2D position, const std::string& text, const std::string& color, int size) {
	_shapes.push_back(new SvgText(position, text, color, size));
}

void PlotSvg::finalize() {
	VECTOR2D dimension = VECTOR2D(_rectHighestPoint.X - _rectLowestPoint.X, _rectHighestPoint.Y - _rectLowestPoint.Y);
	float scale = std::min(_width / dimension.X, _height / dimension.Y);
	VECTOR2D scaleOffset = VECTOR2D((dimension.X - _width / scale) * 0.5f, (dimension.Y - _height / scale) * 0.5f);
	for (const auto svgShape : _shapes) {
		svgShape->plot(_svgFile, VECTOR2D(-_rectLowestPoint.X, _rectLowestPoint.Y) + dimension * k_MarginPercentage - scaleOffset, scale * (1.f - 2.f * k_MarginPercentage), dimension.Y);
	}
	_svgFile << "</svg>\n";
	_svgFile.close();
}

void PlotSvg::updateRect(const VECTOR2D& coordinate) {
	_rectLowestPoint.X = std::min(coordinate.X, _rectLowestPoint.X);
	_rectLowestPoint.Y = std::min(coordinate.Y, _rectLowestPoint.Y);
	_rectHighestPoint.X = std::max(coordinate.X, _rectHighestPoint.X);
	_rectHighestPoint.Y = std::max(coordinate.Y, _rectHighestPoint.Y);
}
