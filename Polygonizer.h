#pragma once
#include <vector>
#include <utility>

struct Polygon {
  std::vector<std::pair<int, int>> points;
  int classId;
  double area;
};

class Polygonizer {
public:
  Polygonizer(const std::vector<std::vector<int>>& classifiedGrid);
  void generatePolygons();
  void mergeSmallPolygons(double minArea);
  const std::vector<Polygon>& getPolygons() const;

private:
  std::vector<std::vector<int>> grid_;
  std::vector<Polygon> polygons_;
  void dfs(int row, int col, int classId, std::vector<std::pair<int, int>>& region, std::vector<std::vector<bool>>& visited);
  std::vector<std::pair<int, int>> extractBoundary(const std::vector<std::pair<int, int>>& region);
};
