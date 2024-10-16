// Created by Gábor Bácsi
// 4/7/2024

#include "SvgShapes.hpp"

static VECTOR2D convertVectorToSvgLocation(const VECTOR2D& vector, const VECTOR2D& offset, float scale, float originalHeight) {
	return VECTOR2D((vector.X + offset.X) * scale, (originalHeight - vector.Y + offset.Y) * scale);
}

static void outputNormalizedVectorCoordinates(std::ofstream& fstream, const VECTOR2D& vector, const VECTOR2D& offset, float scale, float originalHeight) {
	VECTOR2D convertedPosition = convertVectorToSvgLocation(vector, offset, scale, originalHeight);
	fstream << convertedPosition.X << ", " << convertedPosition.Y << " ";
}

SvgLine::SvgLine(VECTOR2D a, VECTOR2D b, const std::string& color, float strokeWidth) : _A(a), _B(b), _color(color), _strokeWidth(strokeWidth) {}

void SvgLine::plot(std::ofstream& svgStream, VECTOR2D offset, float scale, float canvasHeight) {
	VECTOR2D A = convertVectorToSvgLocation(_A, offset, scale, canvasHeight);
	VECTOR2D B = convertVectorToSvgLocation(_B, offset, scale, canvasHeight);
	svgStream << "<line x1=\"" << A.X << "\" y1=\"" << A.Y << "\" x2=\"" << B.X << "\" y2=\"" << B.Y << "\" style=\"stroke:" << _color << "; stroke-width:" << _strokeWidth << "\"/>\n";
}

SvgCircle::SvgCircle(VECTOR2D center, float r, const std::string& color) : _center(center), _radius(r), _color(color) {}

void SvgCircle::plot(std::ofstream& svgStream, VECTOR2D offset, float scale, float canvasHeight) {
	VECTOR2D center = convertVectorToSvgLocation(_center, offset, scale, canvasHeight);
	svgStream << "<circle cx=\"" << center.X << "\" cy=\"" << center.Y << "\" r=\"" << _radius  << "\" fill=\"" << _color << "\"/>\n";
}

SvgPolygon::SvgPolygon(std::vector<VECTOR2D> vertices, std::vector<int> indices, const std::string& name, const std::string& fillColor, const std::string& strokeColor, float strokeWidth, bool animated) : _vertices(vertices), _indices(indices), _name(name), _fillColor(fillColor), _strokeColor(strokeColor), _strokeWidth(strokeWidth), _animated(animated) {}

static void addPropertyRepeatingAnimation(std::ofstream& svgStream, const std::string& name, const std::string& attributeName, const std::string& from, const std::string& to, int currentIndex, int endIndex, float startOffset, float animationDuration) {
	svgStream << "<animate id=\"anim_" << name << currentIndex << "\" attributeName = \"" << attributeName << "\" from=\"" << from << "\" to=\"" << to << "\" dur=\"" << animationDuration << "s\" ";
	if (currentIndex - 1 < 1) {
		svgStream << "begin=\"" << startOffset << "s;anim_" << name << endIndex << ".end + 2s\"";

	}
	else {
		svgStream << "begin=\"anim_" << name << currentIndex - 1 << ".end\"";
	}
	svgStream << " fill=\"freeze\" /> ";
	svgStream << "<set attributeName = \"" << attributeName << "\" to=\"" << from << "\" begin= \"anim_" << name << endIndex << ".end + 1s\" />";
}

void SvgPolygon::plot(std::ofstream& svgStream, VECTOR2D offset, float scale, float canvasHeight) {
	float animationStartOffset = 2;
	float animationDuration = 0.35f;
	int currentTriangleIndex = 1;
	int triangleCount = static_cast<int>(_indices.size() / 3);
	for (int i = 0; i < _indices.size(); i += 3) {
		svgStream << "<polygon points=\"";
		outputNormalizedVectorCoordinates(svgStream, _vertices[_indices[i]], offset, scale, canvasHeight);
		outputNormalizedVectorCoordinates(svgStream, _vertices[_indices[i + 1]], offset, scale, canvasHeight);
		outputNormalizedVectorCoordinates(svgStream, _vertices[_indices[i + 2]], offset, scale, canvasHeight);
		const std::string& startColor = _animated ? "#ffffff00" : _fillColor;
		float startStrokeWidth = _animated ? 0 : _strokeWidth;
		svgStream << "\" style=\"fill:" << startColor << ";stroke:" << _strokeColor << ";stroke-width:" << startStrokeWidth << "\">";
		if (_animated) {
			addPropertyRepeatingAnimation(svgStream, _name, "fill", "#ffffff00", _fillColor, currentTriangleIndex, triangleCount, animationStartOffset, animationDuration);
			addPropertyRepeatingAnimation(svgStream, _name, "stroke-width", "0", std::to_string(_strokeWidth), currentTriangleIndex, triangleCount, animationStartOffset, animationDuration);
			currentTriangleIndex += 1;
		}
		svgStream << "</polygon>\n";
	}
	for (int i = 0; i < _vertices.size(); ++i) {
		VECTOR2D vertexPosition = convertVectorToSvgLocation(_vertices[i], offset, scale, canvasHeight);
		constexpr static float k_vertexSize = 5;
		svgStream << "<circle r=\"" << k_vertexSize << "\" cx=\"" << vertexPosition.X << "\" cy=\"" << vertexPosition.Y << "\" fill=\"red\"/>\n";
	}
}

SvgText::SvgText(VECTOR2D position, const std::string& text, const std::string& color, int size) : _position(position), _text(text), _fillColor(color), _size(size) {}

void SvgText::plot(std::ofstream& svgStream, VECTOR2D offset, float scale, float canvasHeight) {
	VECTOR2D convertedVector = convertVectorToSvgLocation(_position, offset, scale, canvasHeight);
	svgStream << "<text x=\"" << convertedVector.X << "\" y=\"" << convertedVector.Y << "\" style=\"fill:" << _fillColor << "; font:bold " << _size << "px sans-serif;\">" << _text << "</text>";
}