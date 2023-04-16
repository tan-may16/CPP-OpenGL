#pragma once
#include <fstream>
#include <vector>
#include <thread>
#include <iostream>
#include <algorithm>
#include <iomanip>

template <class T, const int NR, const int NC>
class Matrix2D {
private:
	T* content; 

public:
	Matrix2D() 
	{
		content = new T[NR * NC]; 
	};

	Matrix2D(const Matrix2D<T, NR, NC>& original);

	~Matrix2D() 
	{
		delete[] content; 
	};

	void set(int row, int col, const T& incoming);

	const T& value(int row, int col) const;

	bool readFile(const std::string& fileName);

	void print(std::ostream& output = std::cout, int printLimit = -1) const;

	Matrix2D<T, NR, NC>& operator=(const Matrix2D<T, NR, NC>& original);

	void transpose();

	void scalarMultiply(const T& factor);

	void matrixMultiply(Matrix2D<T, NC, NR>& otherMatrix, Matrix2D <T, NR, NC>& resultMatrix,
		int startRow = 1, int endRow = NR, int startCol = 1, int endCol = NC);

	void matrixMultiplyParallel(Matrix2D<T, NC, NR>& otherMatrix,
		Matrix2D <T, NR, NR>& resultMatrix);

public: static void threadEntry(Matrix2D<T, NR, NC>* thisPtr);

	  Matrix2D<T, NR, NC>* rightHandMatrixPtr;
	  Matrix2D<T, NR, NC>* resultMatrixPtr;
	  int numbThreads = 4;
	  int currThread;

	  
protected:
	bool decomposeLUP();
	T* decomposed = nullptr;
	int* permutation = nullptr;
	bool needToRecalcDecomposed;  

public: const T& determinant();
public: void invert();

};

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::set(int row, int col, const T& incoming)
{
	if ((row>=1 && row<=NR) && (col>=1 && col <=NC))
		content[(row-1) * NC + (col-1)] = incoming; 
	else
		throw std::out_of_range("Bad row or column number on set value function");
}

template<class T, int NR, int NC>
inline const T& Matrix2D<T, NR, NC>::value(int row, int col) const
{
	if ((row >= 1 && row <= NR) && (col >= 1 && col <= NC))
		return content[(row - 1) * NC + (col - 1)]; 
	else
		throw std::out_of_range("Bad row or column number on get value function");
}

template<class T, int NR, int NC>
inline bool Matrix2D<T, NR, NC>::readFile(const std::string& fileName)
{
	using namespace std;

	ifstream inFile(fileName);
	if (inFile.is_open()) {
		for (int i = 0; i < NR; i++)
			for (int j = 0; j < NC; j++)
				inFile >> content[i * NC + j];
		inFile.close();
		return true;
	}
	else
		return false;
}

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::print(std::ostream& output, int printLimit) const
{
	using namespace std;

	int rowLimit = NR, colLimit = NC;
	if (printLimit > 0) {
		rowLimit = min(NR, printLimit);
		colLimit = min(NC, printLimit);
	}

	for (int i = 0; i < rowLimit; i++) {
		for (int j = 0; j < colLimit; j++) {
			output << setw(10) << setprecision(3) << content[i * NC + j] << " ";
		}
		output << endl;
	}
}

template<class T, int NR, int NC>
inline Matrix2D<T, NR, NC>::Matrix2D(const Matrix2D<T, NR, NC>& original)
{
	content = new T[NR * NC];

	for (int i = 0; i < NR * NC; i++)
		content[i] = original.content[i];

	needToRecalcDecomposed = true;
}

template<class T, int NR, int NC>
inline Matrix2D<T, NR, NC>& Matrix2D<T, NR, NC>::operator=(const Matrix2D<T, NR, NC>& original) {
	for (int i = 0; i < NR * NC; i++)
		content[i] = original.content[i];

	needToRecalcDecomposed = true;

	return *this;  
}

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::transpose()
{
	if (NR != NC)
		throw std::out_of_range("Cannot transponse a non-square matrix");
	else 
	{	
		for (int i = 0; i < NR; i++)
			for (int j = i+1; j < NC; j++)
			{	
				T temp = content[i * NC + j];
				content[i * NC + j] = content[j * NR + i];
				content[j * NR + i] = temp;
			}


		
		needToRecalcDecomposed = true;
	}
}

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::scalarMultiply(const T& factor)
{
	
	for (int i = 0; i < NR * NC; i++)
		content[i] = factor*content[i];

	needToRecalcDecomposed = true;
}

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::matrixMultiply(Matrix2D<T, NC, NR>& rightHandMatrix, Matrix2D<T, NR, NC>& resultMatrix,
	int startRow, int endRow, int startCol, int endCol)
{
	\T temp;
	if (0 < startRow && startRow <= endRow && endRow <= NR)
	{
		if (0 < startCol && startCol <= endCol && endRow <= NC)
		{
			for (int i = startRow; i <= endRow; i++)
			{
				for (int j = startCol; j <= endCol; j++)
				{
					temp = 0;
					for (int k = 1; k <= NC; k++)
					{
						temp += value(i, k) * rightHandMatrix.value(k, j);
					}
					resultMatrix.set(i, j, temp);
				}
			}
		}
		else
		{
			throw std::out_of_range("Bad column limits on matrix multiply");
		}
	}
	else
	{
		throw std::out_of_range("Bad row limits on matrix multiply");
	}
}

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::matrixMultiplyParallel(Matrix2D<T, NC, NR>& otherMatrix, Matrix2D<T, NR, NR>& resultMatrix)
{
	int multiThreadCutOff = 10;
	if (NC <= multiThreadCutOff)
		matrixMultiply(otherMatrix, resultMatrix); 
	else
	{
		using namespace std;
		int numbThreads = 4;
		currThread = 1; 
		rightHandMatrixPtr = &otherMatrix;
		resultMatrixPtr = &resultMatrix;

		vector<thread> theThreads;
		theThreads.resize(numbThreads); 

		int lastThread;
		for (int i = 0; i < numbThreads; i++)
		{
			lastThread = currThread;
			thread aThread(Matrix2D<T, NR, NC>::threadEntry, this);
			theThreads[i].swap(aThread);
			
			while (lastThread == currThread); 
		}

		for (int i = 0; i < numbThreads; i++)
		{
			theThreads[i].join(); 
		}

	}
}

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::threadEntry(Matrix2D<T, NR, NC>* thisPtr)
{
	
	switch (thisPtr->currThread) {
	case 1:
		thisPtr->currThread++;
		thisPtr->matrixMultiply(*(thisPtr->rightHandMatrixPtr), *(thisPtr->resultMatrixPtr), 1, NR / 2, 1, NC / 2);
		break;
	case 2:
		thisPtr->currThread++;
		thisPtr->matrixMultiply(*(thisPtr->rightHandMatrixPtr), *(thisPtr->resultMatrixPtr), 1, NR / 2, NC / 2 + 1, NC);
		break;
	case 3:
		thisPtr->currThread++;
		thisPtr->matrixMultiply(*(thisPtr->rightHandMatrixPtr), *(thisPtr->resultMatrixPtr), NR / 2 + 1, NR, 1, NC / 2);
		break;
	case 4:
		thisPtr->currThread++;
		thisPtr->matrixMultiply(*(thisPtr->rightHandMatrixPtr), *(thisPtr->resultMatrixPtr), NR / 2 + 1, NR, NC / 2 + 1, NC);
		break;
	}
}

template<class T, int NR, int NC>
bool Matrix2D<T, NR, NC>::decomposeLUP()
{
	int i, j, k, imax, temp;
	T maxA, absA, tempContent;

	if (decomposed == nullptr)
		decomposed = new T[NR * NC];

	for (i = 0; i < NR; i++)
		for (j = 0; j < NC; j++)
			decomposed[i * NC + j] = content[i * NC + j];

	if (permutation == nullptr)
		permutation = new int[NR + 1];  
	for (i = 0; i <= NR; i++)
		permutation[i] = i;

	for (i = 0; i < NR; i++) {  
		maxA = 0.0;
		imax = i;
		for (k = i; k < NR; k++)
			if ((absA = fabs(decomposed[k * NC + i])) > maxA) {
				maxA = absA;
				imax = i;
			}

		if (maxA < 1e-20) {  
			needToRecalcDecomposed = true;
			return false;
		}
		else {
			if (imax != i) {
				for (int j = 0; j < NC; j++) {
					std::swap(decomposed[i * NC + j], decomposed[imax * NC + j]);
				}

				std::swap(permutation[i], permutation[imax]);

				permutation[NR]++;
			}

			for (j = i + 1; j < NR; j++) {

				decomposed[j * NC + i] /= decomposed[i * NC + i];
				for (k = i + 1; k < NC; k++)
					decomposed[j * NC + k] -= decomposed[j * NC + i] * decomposed[i * NC + k];
			}
		}

	}
	needToRecalcDecomposed = false;  
	return true;
}


template<class T, int NR, int NC>
const T& Matrix2D<T, NR, NC>::determinant()
{
	if (needToRecalcDecomposed)
		decomposeLUP();

	if (needToRecalcDecomposed) 
		throw std::out_of_range("Matrix is degenerate.");
	else {

		T det = decomposed[0];

		for (int i = 1; i < NR; i++)
			det *= decomposed[i * NC + i];

		return (permutation[NR] - NR) % 2 == 0 ? det : -det;

		if ((permutation[NR] - NR) % 2 == 0)
			return det;
		else
			return -det;
	}
}

template<class T, int NR, int NC>
void Matrix2D<T, NR, NC>::invert()
{
	if (needToRecalcDecomposed)
		decomposeLUP();

	if (needToRecalcDecomposed) 
		throw std::out_of_range("Matrix is degenerate.");
	else {

		for (int j = 0; j < NC; j++) {
			for (int i = 0; i < NR; i++) {
				content[i * NC + j] = permutation[i] == j ? 1.0 : 0.0;

				for (int k = 0; k < i; k++)
					content[i * NC + j] -= decomposed[i * NC + k] * content[k * NC + j];
			}

			for (int i = NR - 1; i >= 0; i--) {
				for (int k = i + 1; k < NC; k++)
					content[i * NC + j] -= decomposed[i * NC + k] * content[k * NC + j];

				content[i * NC + j] /= decomposed[i * NC + i];
			}
		}

		needToRecalcDecomposed = true;
	}
}
