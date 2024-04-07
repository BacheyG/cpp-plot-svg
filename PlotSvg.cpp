// Created by Gábor Bácsi
// 4/5/2024

#include "PlotSvg.hpp"

#include "../Vector/Vector.hpp"
#include <fstream>
#include <limits>

static constexpr float k_MarginPercentage = 0.05f;

static Vector2D<float> convertVectorToSvgLocation(const Vector2D<float>& vector, const Vector2D<float>& offset, float scale, float height) {
	return Vector2D<float>((vector.X + offset.X) * scale, (height - vector.Y + offset.Y) * scale);
}

static void outputNormalizedVectorCoordinates(std::ofstream& fstream, const Vector2D<float>& vector, const Vector2D<float>& offset, float scale, float height) {
	Vector2D<float> convertedPosition = convertVectorToSvgLocation(vector, offset, scale, height);
	fstream << convertedPosition.X << ", " << convertedPosition.Y << " ";
}

void getDimensionAndOffset(const std::vector<Vector2D<float>> vertices, Vector2D<float>& dimension, Vector2D<float>& offset) {
	Vector2D<float> rectLowest = Vector2D<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vector2D<float> rectHighest = Vector2D<float>(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
	for (int i = 0; i < vertices.size(); ++i) {
		rectLowest.X = std::min(vertices[i].X, rectLowest.X);
		rectLowest.Y = std::min(vertices[i].Y, rectLowest.Y);
		rectHighest.X = std::max(vertices[i].X, rectHighest.X);
		rectHighest.Y = std::max(vertices[i].Y, rectHighest.Y);
	}
	dimension = Vector2D<float>(rectHighest.X - rectLowest.X, rectHighest.Y - rectLowest.Y);
	offset = rectLowest;
}

void PlotSvg::plotPoligonToSvg(const std::string& svgFilename, const std::vector<Vector2D<float>> vertices, const std::vector<int> indices, float width, float height) {
	std::ofstream svgFile(svgFilename);
	svgFile << "<svg height = \"" << height << "\" width = \"" << width << "\" xmlns = \"http://www.w3.org/2000/svg\">\n";
	float scale = 1;
	Vector2D<float> dimension;
	Vector2D<float> offset;
	getDimensionAndOffset(vertices, dimension, offset);
	scale = std::min(width / dimension.X, height / dimension.Y) * (1.f - 2.f * k_MarginPercentage);
	offset += dimension * k_MarginPercentage;
	for (int i = 0; i < indices.size(); i += 3) {
		svgFile << "<polygon points=\"";
		outputNormalizedVectorCoordinates(svgFile, vertices[indices[i]], offset, scale, dimension.Y);
		outputNormalizedVectorCoordinates(svgFile, vertices[indices[i + 1]], offset, scale, dimension.Y);
		outputNormalizedVectorCoordinates(svgFile, vertices[indices[i + 2]], offset, scale, dimension.Y);
		svgFile << "\" style=\"fill:lime;stroke:purple;stroke-width:2\"/>\n";
	}
	for (int i = 0; i < vertices.size(); ++i) {
		Vector2D<float> vertexPosition = convertVectorToSvgLocation(vertices[i], offset, scale, dimension.Y);
		constexpr static float k_vertexSize = 5;
		svgFile << "<circle r=\"" << k_vertexSize << "\" cx=\"" << vertexPosition.X << "\" cy=\"" << vertexPosition.Y << "\" fill=\"red\"/>\n";
	}
	svgFile << "</svg>\n";
	svgFile.close();
}
