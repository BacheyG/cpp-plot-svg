// Created by Gábor Bácsi
// 4/5/2024

#include <string>
#include <vector>

template <typename T>
struct Vector2D;

class PlotSvg {
public:
	static void plotPoligonToSvg(const std::string& filename, const std::vector<Vector2D<float>> vertices, const std::vector<int> indices, float width = 800, float height = 600);
};