// Created by Gábor Bácsi
// 4/5/2024

#include "PlotSvg.hpp"
#include "SvgShapes.hpp"

#include "../Vector/Vector.hpp"
#include <limits>

static constexpr float k_MarginPercentage = 0.05f;

PlotSvg::PlotSvg(const std::string& filename, int width, int height) : _width(width), _height(height) {
	_svgFile = std::ofstream(filename);
	_svgFile << "<svg height=\"" << height << "\" width=\"" << width << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
	_rectLowestPoint = Vector2D<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	_rectHighestPoint = Vector2D<float>(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
}

PlotSvg::~PlotSvg() {
	for (auto svgShape : _shapes) {
		delete svgShape;
	}
}

void PlotSvg::addLine(Vector2D<float> A, Vector2D<float> B, const std::string& color, float strokeWidth) {
	_rectLowestPoint.X = std::min(A.X, _rectLowestPoint.X);
	_rectLowestPoint.Y = std::min(A.Y, _rectLowestPoint.Y);
	_rectHighestPoint.X = std::max(A.X, _rectHighestPoint.X);
	_rectHighestPoint.Y = std::max(A.Y, _rectHighestPoint.Y);
	_rectLowestPoint.X = std::min(B.X, _rectLowestPoint.X);
	_rectLowestPoint.Y = std::min(B.Y, _rectLowestPoint.Y);
	_rectHighestPoint.X = std::max(B.X, _rectHighestPoint.X);
	_rectHighestPoint.Y = std::max(B.Y, _rectHighestPoint.Y);
	_shapes.push_back(new SvgLine(A, B, color, strokeWidth));
}

void PlotSvg::addPolygon(const std::vector<Vector2D<float>> vertices, const std::vector<int> indices) {
	for (int i = 0; i < vertices.size(); ++i) {
		_rectLowestPoint.X = std::min(vertices[i].X, _rectLowestPoint.X);
		_rectLowestPoint.Y = std::min(vertices[i].Y, _rectLowestPoint.Y);
		_rectHighestPoint.X = std::max(vertices[i].X, _rectHighestPoint.X);
		_rectHighestPoint.Y = std::max(vertices[i].Y, _rectHighestPoint.Y);
	}
	_shapes.push_back(new SvgWiredPolygon(vertices, indices));
}

void PlotSvg::finalize() {
	Vector2D<float> dimension = Vector2D<float>(_rectHighestPoint.X - _rectLowestPoint.X, _rectHighestPoint.Y - _rectLowestPoint.Y);
	float scale = scale = std::min(_width / dimension.X, _height / dimension.Y) * (1.f - 2.f * k_MarginPercentage);
	for (const auto svgShape : _shapes) {
		svgShape->plot(_svgFile, _rectLowestPoint + dimension * k_MarginPercentage, scale, dimension.Y);
	}
	_svgFile << "</svg>\n";
	_svgFile.close();
}
