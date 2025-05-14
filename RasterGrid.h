#pragma once
#include <vector>

class RasterGrid {
public:
  RasterGrid(const std::vector<std::vector<int>>& data);

  void classify(int numClasses = 3);
  void preClassificationSmoothing(int windowSize = 3);
  void postClassificationSmoothing(int iterations = 1);

  const std::vector<std::vector<int>>& getClassifiedGrid() const;
  const std::vector<std::vector<int>>& getOriginalGrid() const;

private:
  std::vector<std::vector<int>> data_;
  std::vector<std::vector<int>> classified_;
};
