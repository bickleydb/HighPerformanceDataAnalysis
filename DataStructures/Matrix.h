#include <vector>
#include <algorithm>
#include <utility>
#include <functional>
#include <string>
#include <exception>
#include <assert.h>
#include <iostream>
#include <chrono>
#include <random>

#ifndef MATRIX_H_
#define MATRIX_H_

#define DEFAULT_SIZE  2

class Matrix {
private:

  std::vector<long double> backingData;
  unsigned int rows;
  unsigned int cols;

public:
  Matrix();
  Matrix(unsigned int size);
  Matrix(unsigned int rows, unsigned int cols);
  Matrix(const Matrix& other);
  Matrix(Matrix && other);
  Matrix& operator = (const Matrix& other);
  Matrix& operator = (Matrix&& other);
  Matrix& eye(unsigned int size);
  void resize(unsigned int rows, unsigned int cols);
  ~Matrix();

  static void randMatrix(Matrix& matrix);
  static void randMatrix(unsigned int size, Matrix& matrix);
  static void randMatrix(unsigned int row, unsigned int col, Matrix& matrix);

  static void eyeMatrix(Matrix& matrix);
  static void eyeMatrix(unsigned int size, Matrix& matrix);
  static void eyeMatrix(unsigned int row, unsigned int col, Matrix& matrix);
  
  static void addMatrix(const Matrix& LHS, const Matrix& RHS, Matrix& rtn);
  static void subtractMatrix(const Matrix& LHS, const Matrix& RHS, Matrix& rtn);
  static void scalarMultiply(const Matrix& LHS, Matrix& rtn, long double mult);
  static void matrixMultiply(const Matrix& LHS, const Matrix& RHS, Matrix& rtn);

  void swapRows(unsigned int row1, unsigned int row2);
  void addRows(unsigned int row1, unsigned int row2);
  void multRow(unsigned int row, long double mult);
  void gaussJordanElimination();
  void inverse(Matrix& rtn);


  void inline setValue(const unsigned int row, const unsigned int col, const long double val);
  long double inline getValue(const unsigned int r, const unsigned int c) const;

  std::string toString();
};

#endif
