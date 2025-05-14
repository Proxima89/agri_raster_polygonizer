cmake_minimum_required(VERSION 3.10)
project(AgriRasterPolygonizer)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

include_directories(${PQXX_INCLUDE_DIRS})
link_directories(${PQXX_LIBRARY_DIRS})

add_executable(AgriRasterPolygonizer main.cpp DatabaseConnector.cpp RasterGrid.cpp Polygonizer.cpp GeoJsonExporter.cpp EnvLoader.cpp)

target_link_libraries(AgriRasterPolygonizer ${PQXX_LIBRARIES}) 
