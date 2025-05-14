#include "EnvLoader.h"
#include "DatabaseConnector.h"
#include "RasterGrid.h"
#include "Polygonizer.h"
#include "GeoJsonExporter.h"
#include <iostream>
#include <fstream>
#include <map>
#include <numeric>

int main() {
  auto env = loadEnv(".env");
  DatabaseConnector db(
    env["DB_HOST"],
    std::stoi(env["DB_PORT"]),
    env["DB_USER"],
    env["DB_PASSWORD"],
    env["DB_NAME"]
  );

  if (!db.connect()) {
    std::cerr << "Database connection failed!" << std::endl;
    return 1;
  }

  RasterInfo rasterInfo = db.fetchRasterWithGeotransform("grids");
  if (rasterInfo.grid.empty()) {
    std::cerr << "No raster data found!" << std::endl;
    return 1;
  }
  std::cout << "Raster size: " << rasterInfo.width << " x " << rasterInfo.height << std::endl;

  RasterGrid grid(rasterInfo.grid);
  grid.preClassificationSmoothing(3);
  grid.classify();
  grid.postClassificationSmoothing(2);

  Polygonizer poly(grid.getClassifiedGrid());
  poly.generatePolygons();
  poly.mergeSmallPolygons(5000.0);

  auto polygons = poly.getPolygons();
  std::map<int, MultiPolygonInfo> classToMultiPolygon;
  std::map<int, std::vector<int>> classToValues;
  const auto& classified = grid.getClassifiedGrid();
  const auto& original = grid.getOriginalGrid();

  for (const auto& poly : polygons)
    classToMultiPolygon[poly.classId].classId = poly.classId,
    classToMultiPolygon[poly.classId].polygons.push_back(poly.points);

  for (size_t i = 0; i < classified.size(); ++i)
    for (size_t j = 0; j < classified[0].size(); ++j)
      classToValues[classified[i][j]].push_back(original[i][j]);

  for (auto& kv : classToMultiPolygon) {
    const auto& values = classToValues[kv.first];
    kv.second.averageValue = values.empty() ? 0.0 : std::accumulate(values.begin(), values.end(), 0.0) / values.size();
  }

  std::vector<MultiPolygonInfo> multipolygons;
  for (const auto& kv : classToMultiPolygon)
    multipolygons.push_back(kv.second);

  std::string geojson = GeoJsonExporter::exportToGeoJson(multipolygons,
    rasterInfo.upperLeftX, rasterInfo.upperLeftY, rasterInfo.pixelWidth, rasterInfo.pixelHeight);

  std::ofstream out("output.geojson");
  out << geojson;
  out.close();

  std::cout << "GeoJSON exported to output.geojson\n";
  std::cout << "Number of polygons: " << polygons.size() << std::endl;
  return 0;
}
