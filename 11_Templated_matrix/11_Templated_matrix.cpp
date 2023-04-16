#include <stdio.h>
template <class T, const int NR, const int NC>
class MatrixTemplate
{
private:
    T* mat;
public:
    MatrixTemplate();
    ~MatrixTemplate();
    void Set(int row, int column, double value);
    T Get(int row, int column) const;
};

template <class T, const int NR, const int NC>
MatrixTemplate<T, NR, NC>::MatrixTemplate()
{
    mat = new T[NC * NR];
}

template <class T, const int NR, const int NC>
MatrixTemplate<T, NR, NC>::~MatrixTemplate()
{

    if (nullptr != mat)
    {
        delete[] mat;
    }
    mat = nullptr;
}

template <class T, const int NR, const int NC>
void MatrixTemplate <T, NR, NC>::Set(int row, int column, double value)
{
    mat[(column - 1) * NC + (row - 1)] = value;
}
template <class T, const int NR, const int NC>
T MatrixTemplate<T, NR, NC>::Get(int row, int column) const
{
    return mat[(column - 1) * NC + (row - 1)];
}

class Matrix4x4 : public MatrixTemplate <double, 4, 4>
{
public:
    void Transpose();
    void Print() const;
};

void Matrix4x4::Transpose()
{
    double transpose[16];
    int row, column;
    for (int i = 0; i < 16; ++i)
    {
        row = 1 + i / 4;
        column = 1 + i % 4;
        double value = Get(row, column);
        transpose[(row - 1) * 4 + (column - 1)] = value;
    }

    for (int i = 0; i < 16; ++i)
    {
        row = (i / 4) + 1;
        column = (i % 4) + 1;
        double value_transpose = transpose[(column - 1) * 4 + (row - 1)];
        Set(row, column, value_transpose);
    }
}

void Matrix4x4::Print() const
{
    int row, column;
    for (int i = 0; i < 16; ++i)
    {
        row = (i / 4) + 1;
        column = (i % 4) + 1;
        double elem = Get(row, column);
        if (i % 4 == 0)
        {
            if (i == 0)
            {
                printf("%6f ", elem);
                continue;
            }
            else
            {
                printf("\n");
            }
        }
        printf("%6f ", elem);
    }
    printf("\n");

}

int main(void)
{
    Matrix4x4 mat;
    const double v[] =
    {
        6.0,1.0,4.0,9.0,
        9.0,8.0,6.0,1.0,
        7.0,2.0,9.0,4.0,
        1.0,7.0,5.0,9.0
    };
    for (int i = 0; i < 16; ++i)
    {
        const int r = 1 + i / 4;
        const int c = 1 + i % 4;
        mat.Set(r, c, v[i]);
    }
    mat.Print();
    mat.Transpose();
    printf("\n");
    mat.Print();
    // If you go for extra credit, uncomment the following three lines.
    // printf("\n");
    // mat.Invert();
    // mat.Print();
    return 0;
}
