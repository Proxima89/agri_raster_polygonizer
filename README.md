# AgriRasterPolygonizer

## Overview
This project identifies connected or neighboring regions in a 512x512 meter agricultural field using raster data. The data has a resolution of 1x1 meter, with each cell containing a value between 1 and 255. This raster is stored in a PostGIS database. The result is three multipolygons in GeoJSON format, each representing a group of similar cells and including their average value. These can be visualized on platforms like geojson.io or any map-based web viewer.

## How it works
  **How To Run:**
  Add your database secrets in /build/.env

  And run the project from the same folder /build
    - From build folder run:
    ```
    cmake ..
    make
    ./AgriRasterPolygonizer
    ```
1. **Database Extraction:**
  Connects to the provided PostGIS database and fetches the raster grid and geotransform.
2. **Preprocessing:**
  Applies pre and post filters in order to smooth the clasification results.
3. **Classification:**
  Classifies each cell into 3 classes based on value, then applies a majority filter to ensure spatial coherence.
4. **Polygonization:**
  Detects connected regions of the same class and generates polygons, filtering out small polygons (<0.5 Ha).
5. **GeoJSON Export:**
  After succesfull execution the system generates output.geojson file inside the /build folder, which then can be imported in geojson.io
