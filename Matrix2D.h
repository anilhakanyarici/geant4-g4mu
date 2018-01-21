#ifndef MATRIX2D_H
#define MATRIX2D_H

#include <iostream>

template<typename T>
class Matrix2D
{
private:
    T *_matrixData = nullptr;
    int _rowLen;
    int _colLen;
public:

    inline int RowLength() const { return this->_rowLen; }
    inline int ColumnLength() const { return this->_colLen; }

    Matrix2D(int rowLength, int columnLength)
    {
        this->_rowLen = rowLength;
        this->_colLen = columnLength;
        long longLen = rowLength * columnLength;
        this->_matrixData = new T[longLen];
        for(long i = 0; i < longLen; i++){
            this->_matrixData[i] = T();
        }
    }
    ~Matrix2D() { delete[] this->_matrixData; }

    void Clear(){
        long longLen = this->_colLen * this->_rowLen;
        for(long i = 0; i < longLen; i++){
            this->_matrixData[i] = T();
        }
    }

    T &Item(const int i, const int j) { return this->_matrixData[i * this->_colLen + j]; }
    bool SaveToFile(std::string fileName)
    {
        std::ofstream file(fileName);
        if(file.is_open()){
            for(int i = 0; i < this->_rowLen; i++){
                for(int j = 0; j < this->_colLen; j++){
                    T e = this->Item(i, j);
                    file << e.toString();
                    file << " ";
                }
                file << "\n";
            }
            file.close();
            return true;
        } else {
            return false;
        }
    }
};

#endif // MATRIX2D_H
