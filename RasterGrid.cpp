#include "RasterGrid.h"
#include <algorithm>

RasterGrid::RasterGrid(const std::vector<std::vector<int>>& data)
  : data_(data), classified_(data.size(), std::vector<int>(data[0].size(), 0)) {}

void RasterGrid::classify(int numClasses) {
  std::vector<int> flat;
  for (const auto& row : data_)
    flat.insert(flat.end(), row.begin(), row.end());

  int minVal = *std::min_element(flat.begin(), flat.end());
  int maxVal = *std::max_element(flat.begin(), flat.end());
  int range = maxVal - minVal + 1;
  int step = range / numClasses;

  for (size_t i = 0; i < data_.size(); ++i) {
    for (size_t j = 0; j < data_[0].size(); ++j) {
      int val = data_[i][j];
      int classId = std::min((val - minVal) / step, numClasses - 1);
      classified_[i][j] = classId;
    }
  }
}

const std::vector<std::vector<int>>& RasterGrid::getClassifiedGrid() const {
  return classified_;
}

const std::vector<std::vector<int>>& RasterGrid::getOriginalGrid() const {
  return data_;
}

void RasterGrid::preClassificationSmoothing(int windowSize) {
  int rows = data_.size();
  int cols = data_[0].size();
  std::vector<std::vector<int>> smoothed(rows, std::vector<int>(cols, 0));
  int half = windowSize / 2;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      int sum = 0, count = 0;
      for (int di = -half; di <= half; ++di) {
        for (int dj = -half; dj <= half; ++dj) {
          int ni = i + di, nj = j + dj;
          if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
            sum += data_[ni][nj];
            count++;
          }
        }
      }
      smoothed[i][j] = sum / count;
    }
  }
  data_ = smoothed;
}

void RasterGrid::postClassificationSmoothing(int iterations) {
  int rows = classified_.size();
  int cols = classified_[0].size();
  std::vector<std::vector<int>> temp = classified_;
  int d[8][2] = { {-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1} };

  for (int it = 0; it < iterations; ++it) {
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        std::vector<int> counts(3, 0);
        counts[classified_[i][j]]++; // include self
        for (int k = 0; k < 8; ++k) {
          int ni = i + d[k][0], nj = j + d[k][1];
          if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
              counts[classified_[ni][nj]]++;
          }
        }
        int maxClass = 0;
        for (int c = 1; c < 3; ++c) {
          if (counts[c] > counts[maxClass]) maxClass = c;
        }
        temp[i][j] = maxClass;
      }
    }
    classified_ = temp;
  }
}
