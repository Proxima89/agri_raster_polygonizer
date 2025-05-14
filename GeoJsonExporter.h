#pragma once
#include <vector>
#include <string>

struct MultiPolygonInfo {
  int classId;
  double averageValue;
  std::vector<std::vector<std::pair<int, int>>> polygons; // Each polygon is a vector of points
};

class GeoJsonExporter {
public:
  static std::string exportToGeoJson(const std::vector<MultiPolygonInfo>& multipolygons,
                                      double upperLeftX, double upperLeftY,
                                      double pixelWidth, double pixelHeight);
};
