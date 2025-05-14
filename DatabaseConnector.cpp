#include "DatabaseConnector.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

DatabaseConnector::DatabaseConnector(const std::string& host, int port, const std::string& user,
                                     const std::string& password, const std::string& dbname)
  : host_(host), port_(port), user_(user), password_(password), dbname_(dbname), conn_(nullptr) {}

bool DatabaseConnector::connect() {
  try {
    std::string conn_str = "host=" + host_ +
                           " port=" + std::to_string(port_) +
                           " user=" + user_ +
                           " password=" + password_ +
                           " dbname=" + dbname_;
    conn_ = std::make_unique<pqxx::connection>(conn_str);
    if (conn_->is_open()) {
      std::cout << "Connected to database successfully: " << conn_->dbname() << std::endl;
      return true;
    } else {
      std::cerr << "Failed to open database." << std::endl;
      return false;
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
}

RasterInfo DatabaseConnector::fetchRasterWithGeotransform(const std::string& table) {
  RasterInfo info;
  if (!conn_ || !conn_->is_open()) {
    std::cerr << "Database not connected!" << std::endl;
    return info;
  }
  try {
    pqxx::work txn(*conn_);
    std::string sql = "SELECT ST_Width(geom), ST_Height(geom), "
                      "ST_UpperLeftX(geom), ST_UpperLeftY(geom), "
                      "ST_PixelWidth(geom), ST_PixelHeight(geom), "
                      "ST_DumpValues(geom) FROM " + table + " LIMIT 1;";
    pqxx::result r = txn.exec(sql);

    if (r.empty()) {
      std::cerr << "Query returned no rows!" << std::endl;
      return info;
    }

    info.width = r[0][0].as<int>();
    info.height = r[0][1].as<int>();
    info.upperLeftX = r[0][2].as<double>();
    info.upperLeftY = r[0][3].as<double>();
    info.pixelWidth = r[0][4].as<double>();
    info.pixelHeight = r[0][5].as<double>();
    std::string arr = r[0][6].c_str();
    std::stringstream ss(arr);
    std::string rowStr;

    while (std::getline(ss, rowStr, '}')) {
      size_t brace = rowStr.find('{');
      if (brace == std::string::npos) continue;
      rowStr = rowStr.substr(brace + 1);
      std::vector<int> row;
      std::stringstream rowSS(rowStr);
      std::string val;
      while (std::getline(rowSS, val, ',')) {
        val.erase(0, val.find_first_not_of(" \t\n\r"));
        val.erase(val.find_last_not_of(" \t\n\r") + 1);
        if (!val.empty()) {
          try {
            row.push_back(std::stoi(val));
          } catch (const std::exception& e) {
            std::cerr << "Failed to convert value: '" << val << "' (" << e.what() << ")" << std::endl;
          }
        }
      }
      if (!row.empty())
        info.grid.push_back(row);
    }

    return info;
  } catch (const std::exception& e) {
    std::cerr << "Error fetching raster: " << e.what() << std::endl;
    return info;
  }
}
