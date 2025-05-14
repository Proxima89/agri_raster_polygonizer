#include <vector>
#include <string>
#include <memory>
#include <pqxx/pqxx>

struct RasterInfo {
  std::vector<std::vector<int>> grid;
  int width;
  int height;
  double upperLeftX;
  double upperLeftY;
  double pixelWidth;
  double pixelHeight;
};

class DatabaseConnector {
public:
  DatabaseConnector(const std::string& host, int port, const std::string& user,
                    const std::string& password, const std::string& dbname);
  bool connect();
  RasterInfo fetchRasterWithGeotransform(const std::string& table);
private:
  std::string host_;
  int port_;
  std::string user_;
  std::string password_;
  std::string dbname_;
  std::unique_ptr<pqxx::connection> conn_;
};
