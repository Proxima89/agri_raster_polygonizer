#include "GeoJsonExporter.h"
#include <sstream>
#include <cmath> // For std::abs

std::string GeoJsonExporter::exportToGeoJson(const std::vector<MultiPolygonInfo>& multipolygons,
                                             double upperLeftX, double upperLeftY,
                                             double pixelWidth, double pixelHeight) {
  std::ostringstream oss;
  oss << "{\n  \"type\": \"FeatureCollection\",\n  \"features\": [\n";
  for (size_t i = 0; i < multipolygons.size(); ++i) {
    const auto& mp = multipolygons[i];
    oss << "    {\n      \"type\": \"Feature\",\n      \"properties\": {\n"
        << "        \"classId\": " << mp.classId << ",\n"
        << "        \"average\": " << mp.averageValue << "\n      },\n"
        << "      \"geometry\": {\n        \"type\": \"MultiPolygon\",\n"
        << "        \"coordinates\": [";
    for (size_t j = 0; j < mp.polygons.size(); ++j) {
      oss << "[[";
      const auto& poly = mp.polygons[j];
      for (size_t k = 0; k < poly.size(); ++k) {
        const auto& pt = poly[k];
        double x = upperLeftX + (pt.second + 0.5) * pixelWidth;
        double y = upperLeftY - (pt.first + 0.5) * std::abs(pixelHeight);
        oss << "[" << x << ", " << y << "]";
        if (k != poly.size() - 1) oss << ", ";
      }
      oss << "]]";
      if (j != mp.polygons.size() - 1) oss << ", ";
    }
    oss << "]\n      }\n    }";
    if (i != multipolygons.size() - 1) oss << ",";
    oss << "\n";
  }
  oss << "  ]\n}";
  return oss.str();
}
