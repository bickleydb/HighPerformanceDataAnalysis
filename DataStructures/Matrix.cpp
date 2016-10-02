#include "Matrix.h"


Matrix::Matrix() {
  backingData.resize(DEFAULT_SIZE * DEFAULT_SIZE);
  rows = DEFAULT_SIZE;
  cols = DEFAULT_SIZE;
}

Matrix::Matrix(unsigned int size) {
  backingData.resize(size * size);
  rows = size;
  cols = size;
}

void Matrix::resize(unsigned int r, unsigned int c) {
  this->backingData.resize(r * c);
  this->rows = r;
  this->cols = c;

}

Matrix::Matrix(unsigned int r, unsigned int c) {
  backingData.resize(r * c);
  rows = r;
  cols = c;
}

void Matrix::randMatrix(Matrix& matrix) {
  randMatrix(DEFAULT_SIZE,DEFAULT_SIZE,matrix);
}

void Matrix::randMatrix(unsigned int size, Matrix& matrix) {
  randMatrix(size,size,matrix);
}

void Matrix::randMatrix(unsigned int row, unsigned int col, Matrix& matrix) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::normal_distribution<long double> dist(0,1);
  matrix.resize(row,col);
  for(std::vector<long double>::iterator it = matrix.backingData.begin(); it < matrix.backingData.end(); it++) {
    *it = dist(generator);
  }
}

void Matrix::eyeMatrix(Matrix& matrix) {
  eyeMatrix(DEFAULT_SIZE,DEFAULT_SIZE,matrix);
}

void Matrix::eyeMatrix(unsigned int size, Matrix& matrix) {
  eyeMatrix(size,size,matrix);
}

void Matrix::eyeMatrix(unsigned int row, unsigned int col, Matrix& matrix) {
  matrix.resize(row,col);
  for(unsigned int i = 0; i < std::min(row,col); i++) {
    matrix.setValue(i,i,1);
  }
}

Matrix::Matrix(const Matrix& other) {
  backingData.resize(other.backingData.size());
  rows = other.rows;
  cols = other.cols;
}

Matrix::Matrix(Matrix&& other) {
  std::move(other.backingData.begin(),other.backingData.end(),backingData.begin());
  rows = other.rows;
  cols = other.cols;
}

Matrix& Matrix::operator=(const Matrix& other) {
  if(&other == this) {
    return *this;
  }
  if(other.backingData.size() != backingData.size()) {
    backingData.resize(other.backingData.size());
  }

  std::copy(other.backingData.begin(),other.backingData.end(),backingData.begin());
  rows = other.rows;
  cols = other.cols;
  return *this;
}


Matrix& Matrix::operator=(Matrix&& other) {
  std::move(other.backingData.begin(),other.backingData.end(),backingData.begin());
  rows = other.rows;
  cols = other.cols;
  return *this;
}

Matrix::~Matrix() {}

void Matrix::addMatrix(const Matrix& LHS, const Matrix& RHS,Matrix& rtn) {
  if(LHS.rows != rtn.rows || LHS.cols != rtn.cols) {
    throw std::range_error("LHS Matrix is " + std::to_string(LHS.rows)+"x"+std::to_string(LHS.cols)+" does not have the same size as RHS Matrix " + std::to_string(rtn.rows) + "x"+ std::to_string(rtn.cols));
  }

  rtn = Matrix(LHS.rows,LHS.cols);
  std::copy(RHS.backingData.begin(),RHS.backingData.end(),rtn.backingData.begin());
  std::transform(rtn.backingData.begin(),rtn.backingData.end(),LHS.backingData.begin(),rtn.backingData.begin(),std::plus<long double>());
}


void Matrix::subtractMatrix(const Matrix& LHS, const Matrix& RHS,Matrix& rtn) {
  if(LHS.rows != rtn.rows || LHS.cols != rtn.cols) {
    throw std::range_error("LHS Matrix is " + std::to_string(LHS.rows)+"x"+std::to_string(LHS.cols)+" does not have the same size as RHS Matrix " + std::to_string(rtn.rows) + "x"+ std::to_string(rtn.cols));
  }
  rtn = Matrix(LHS.rows,LHS.cols);
  std::copy(RHS.backingData.begin(),RHS.backingData.end(),rtn.backingData.begin());

  for(std::vector<long double>::iterator it = rtn.backingData.begin(); it < rtn.backingData.end(); it++) {
    *it = LHS.backingData[it - rtn.backingData.begin()] - RHS.backingData[it - rtn.backingData.begin()];
  }
}

void Matrix::scalarMultiply(const Matrix& matrix, Matrix& rtn, long double mult) {
  rtn.resize(matrix.rows,matrix.cols);
  for(std::vector<long double>::iterator it = rtn.backingData.begin(); it < rtn.backingData.end(); it++) {
    *it = matrix.backingData[it - rtn.backingData.begin()];
  }

 }

void Matrix::setValue(const unsigned int r, const unsigned int c, const long double val) {
   if (r < 0 || r >= this->rows || c < 0 || c>=this->cols) {
    throw std::range_error("Invalid Range");
  }
  backingData[c + r * cols] = val;
}

long double Matrix::getValue(const unsigned int r, const unsigned int c) const {
  if (r < 0 || r >= this->rows || c < 0 || c>=this->cols) {
    throw std::range_error("Invalid Range at row " + std::to_string(r) + " and col " + std::to_string(c));
  }
  return backingData[c + r * cols];
}

std::string Matrix::toString() {
  std::string rtn = "[ ";
  for(unsigned int i = 0; i < backingData.size(); i++) {
    if(i % cols == 0 && i != 0) {
      rtn = rtn + "]\n[ ";
    }
    rtn = rtn + std::to_string(backingData[i]) + " ";
  }
  rtn = rtn +"]\n";
  return rtn;
}

void Matrix::matrixMultiply (const Matrix& LHS, const Matrix& RHS, Matrix& rtn) {
  if(LHS.cols != RHS.rows) {
    throw std::range_error("LHS Matrix is " + std::to_string(LHS.rows)+"x"+std::to_string(LHS.cols)+" cannot be multiplied by RHS Matrix " + std::to_string(rtn.rows) + "x"+ std::to_string(rtn.cols));
  }
  rtn.resize(LHS.rows,RHS.cols);
  for(unsigned int i = 0; i < rtn.rows; i++) {
    for(unsigned int t = 0; t < rtn.cols; t++) {
      for(unsigned int q = 0; q < RHS.rows; q++) {
        rtn.setValue(i,t,rtn.getValue(i,t) + LHS.getValue(i,q) * RHS.getValue(q,t));
      }
    }
  }
 }

void Matrix::swapRows(unsigned int row1, unsigned int row2) {
  if(row1 < 0 || row1 >= this->rows || row2 < 0 || row2 >= this->rows) {
    throw std::range_error("Rows are not valid");
  }
  std::vector<long double>::iterator startRow = this->backingData.begin() + row1*cols;
  std::vector<long double>::iterator endRow = this->backingData.begin() + row2 * cols;
  std::vector<long double> copy(this->cols);

  for(std::vector<long double>::iterator it = startRow; it < startRow+this->cols; it++) {
    copy[it - startRow] = *it;
  }
  for(std::vector<long double>::iterator it = endRow; it < endRow + this->cols; it++) {
    *(startRow+(it - endRow)) = *it;
  }
  for(std::vector<long double>::iterator it = endRow; it < endRow + this->cols; it++) {
    *it = copy[it - endRow];
  }
}

void Matrix::addRows(unsigned int row1, unsigned int row2) {
  if(row1 < 0 || row1 >= this->rows || row2 < 0 || row2 >= this->rows) {
    throw std::range_error("Rows are not valid");
  } 
  std::vector<long double>::iterator rowStart = this->backingData.begin() + row1 * cols;
  std::vector<long double>::iterator row2Start = this->backingData.begin() + row2*cols;

  for(std::vector<long double>::iterator it = row2Start; it < row2Start + cols; it++) {
    *it = *it + *(rowStart + (it - row2Start));
  }
}

void Matrix::multRow(unsigned int row, long double mult) {
  for(std::vector<long double>::iterator it = this->backingData.begin() + row * cols; it < this->backingData.begin() + row * cols + cols; it++) {
    *it = mult * *it;
  }
}

void Matrix::gaussJordanElimination() {
  for(unsigned int i = 0; i < this->rows; i++) {
    if(this->getValue(i,i) == 0) {
      bool swapped = false;
      for(unsigned int t = i+1; t < this->rows; t++) {
        if(this->getValue(t,i) != 0) {
          swapRows(i,t);
          swapped = true;
        }
        if(swapped) {
          break;
        }
      }
      if(!swapped) {
        continue;
      }
    }
    multRow(i, 1/this->getValue(i,i));
    for(unsigned int t = i+1; t < this->rows; t++) {
      if(this->getValue(t,i) != 0) {
        multRow(t,1/this->getValue(t,i));
         if(this->getValue(t,i) > 0) {
          multRow(t,-1);
        }
         addRows(i,t);
      }
    }
  }
  for(unsigned int i = this->rows-1; i < this->rows; i--) {
    long double curSpot = this->getValue(i,i);
    if(curSpot != 0) {
      if(curSpot < 0) {
        multRow(i,1/curSpot);
      } else {
        multRow(i,1/std::abs(curSpot));
      }
      for(unsigned int t = i - 1; t < i; t--) {
        long double rowVal = this->getValue(t,i);
        if(rowVal != 0) {
          if(rowVal > 0) {
            multRow(t,-1/rowVal);
              } else {
            multRow(t,1/std::abs(rowVal));
          }
          addRows(i,t);
        }
      }
    }
  }
}


void Matrix::inverse(Matrix& rtn) {
   rtn.resize(this->rows,this->cols);
  Matrix workingCopy(this->rows, this->cols * 2);
  for(unsigned int i = 0; i < this->rows; i++) {
    for(unsigned int t = 0; t < this->cols; t++) {
      workingCopy.setValue(i,t,this->getValue(i,t));
    }
  }
 
  for(unsigned int i = 0; i < rows; i++) {
    workingCopy.setValue(i,i+cols,1);
  }
   workingCopy.gaussJordanElimination();
  
   for(unsigned int i = 0; i < rtn.rows; i++) {
     for(unsigned int t = 0; t < rtn.cols; t++) {
       rtn.setValue(i,t,workingCopy.getValue(i,t+rtn.cols));
     }
   }
}


int main() {



}
