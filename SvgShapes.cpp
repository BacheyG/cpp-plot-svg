// Created by Gábor Bácsi
// 4/7/2024

#include "SvgShapes.hpp"

static Vector2D<float> convertVectorToSvgLocation(const Vector2D<float>& vector, const Vector2D<float>& offset, float scale, float originalHeight) {
	return Vector2D<float>((vector.X + offset.X) * scale, (originalHeight - vector.Y + offset.Y) * scale);
}

static void outputNormalizedVectorCoordinates(std::ofstream& fstream, const Vector2D<float>& vector, const Vector2D<float>& offset, float scale, float originalHeight) {
	Vector2D<float> convertedPosition = convertVectorToSvgLocation(vector, offset, scale, originalHeight);
	fstream << convertedPosition.X << ", " << convertedPosition.Y << " ";
}

SvgLine::SvgLine(Vector2D<float> a, Vector2D<float> b, const std::string& color, float strokeWidth) : _A(a), _B(b), _color(color), _strokeWidth(strokeWidth) {}

void SvgLine::plot(std::ofstream& svgStream, Vector2D<float> offset, float scale, float canvasHeight) {
	Vector2D<float> A = convertVectorToSvgLocation(_A, offset, scale, canvasHeight);
	Vector2D<float> B = convertVectorToSvgLocation(_B, offset, scale, canvasHeight);
	svgStream << "<line x1=\"" << A.X << "\" y1=\"" << A.Y << "\" x2=\"" << B.X << "\" y2=\"" << B.Y << "\" style=\"stroke:" << _color << "; stroke-width:" << _strokeWidth << "\"/>\n";
}

SvgCircle::SvgCircle(Vector2D<float> center, float r, const std::string& color) : _center(center), _radius(r), _color(color) {}

void SvgCircle::plot(std::ofstream& svgStream, Vector2D<float> offset, float scale, float canvasHeight) {
	Vector2D<float> center = convertVectorToSvgLocation(_center, offset, scale, canvasHeight);
	svgStream << "<circle cx=\"" << center.X << "\" cy=\"" << center.Y << "\" r=\"" << _radius  << "\" fill=\"" << _color << "\"/>\n";
}

SvgPolygon::SvgPolygon(std::vector<Vector2D<float>> vertices, std::vector<int> indices, const std::string& fillColor, const std::string& strokeColor, float strokeWidth) : _vertices(vertices), _indices(indices), _fillColor(fillColor), _strokeColor(strokeColor), _strokeWidth(strokeWidth) {}

void SvgPolygon::plot(std::ofstream& svgStream, Vector2D<float> offset, float scale, float canvasHeight) {
	for (int i = 0; i < _indices.size(); i += 3) {
		svgStream << "<polygon points=\"";
		outputNormalizedVectorCoordinates(svgStream, _vertices[_indices[i]], offset, scale, canvasHeight);
		outputNormalizedVectorCoordinates(svgStream, _vertices[_indices[i + 1]], offset, scale, canvasHeight);
		outputNormalizedVectorCoordinates(svgStream, _vertices[_indices[i + 2]], offset, scale, canvasHeight);
		svgStream << "\" style=\"fill:" << _fillColor << ";stroke:" << _strokeColor << ";stroke-width:" << _strokeWidth << "\"/>\n";
	}
	for (int i = 0; i < _vertices.size(); ++i) {
		Vector2D<float> vertexPosition = convertVectorToSvgLocation(_vertices[i], offset, scale, canvasHeight);
		constexpr static float k_vertexSize = 5;
		svgStream << "<circle r=\"" << k_vertexSize << "\" cx=\"" << vertexPosition.X << "\" cy=\"" << vertexPosition.Y << "\" fill=\"red\"/>\n";
	}
}