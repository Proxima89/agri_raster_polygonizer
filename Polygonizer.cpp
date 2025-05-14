#include "Polygonizer.h"
#include <algorithm>
#include <cstddef>

Polygonizer::Polygonizer(const std::vector<std::vector<int>>& classifiedGrid)
  : grid_(classifiedGrid) {}

void Polygonizer::generatePolygons() {
  size_t rows = grid_.size(), cols = grid_[0].size();
  std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
  polygons_.clear();
  for (size_t i = 0; i < rows; ++i)
    for (size_t j = 0; j < cols; ++j)
      if (!visited[i][j]) {
        int classId = grid_[i][j];
        std::vector<std::pair<int, int>> region;
        dfs(i, j, classId, region, visited);
        if (!region.empty()) {
          auto boundary = extractBoundary(region);
          polygons_.push_back({boundary, classId, static_cast<double>(region.size())});
        }
      }
}

std::vector<std::pair<int, int>> Polygonizer::extractBoundary(const std::vector<std::pair<int, int>>& region) {
  if (region.empty()) return {};
  int minRow = region[0].first, maxRow = region[0].first, minCol = region[0].second, maxCol = region[0].second;
  for (const auto& pt : region) {
    minRow = std::min(minRow, pt.first); maxRow = std::max(maxRow, pt.first);
    minCol = std::min(minCol, pt.second); maxCol = std::max(maxCol, pt.second);
  }
  return {{minRow, minCol}, {minRow, maxCol}, {maxRow, maxCol}, {maxRow, minCol}, {minRow, minCol}};
}

void Polygonizer::dfs(int row, int col, int classId, std::vector<std::pair<int, int>>& region, std::vector<std::vector<bool>>& visited) {
  size_t rows = grid_.size(), cols = grid_[0].size();
  if (row < 0 || col < 0 || row >= static_cast<int>(rows) || col >= static_cast<int>(cols)) return;
  if (visited[row][col] || grid_[row][col] != classId) return;
  visited[row][col] = true;
  region.emplace_back(row, col);
  dfs(row-1, col, classId, region, visited);
  dfs(row+1, col, classId, region, visited);
  dfs(row, col-1, classId, region, visited);
  dfs(row, col+1, classId, region, visited);
}

void Polygonizer::mergeSmallPolygons(double minArea) {
  std::vector<Polygon> filtered;
  for (const auto& poly : polygons_)
    if (poly.area >= minArea)
      filtered.push_back(poly);
  polygons_ = filtered;
}

const std::vector<Polygon>& Polygonizer::getPolygons() const { return polygons_; }
