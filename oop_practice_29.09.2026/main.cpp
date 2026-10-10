#include <iostream>
using namespace std;

class Point
{
private:
    int x;
    int y;

public:
    Point()
    {
        x = 0;
        y = 0;
    }

    Point(int first, int second)
    {
        x = first;
        y = second;
    }

    Point operator+(const Point& value) const
    {
        return Point(x + value.x, y + value.y);
    }

    Point operator-(const Point& value) const
    {
        return Point(x - value.x, y - value.y);
    }

    Point operator*(const Point& value) const
    {
        return Point(x * value.x, y * value.y);
    }

    Point operator/(const Point& value) const
    {
        if (value.x == 0 || value.y == 0)
            throw runtime_error("Division by zero");

        return Point(x / value.x, y / value.y);
    }

    bool operator<(const Point& value) const
    {
        return x + y < value.x + value.y;
    }

    bool operator>(const Point& value) const
    {
        return x + y > value.x + value.y;
    }

    Point& operator++()
    {
        ++x;
        ++y;
        return *this;
    }

    Point operator++(int)
    {
        Point previous = *this;
        ++(*this);
        return previous;
    }

    Point& operator--()
    {
        --x;
        --y;
        return *this;
    }

    Point operator--(int)
    {
        Point previous = *this;
        --(*this);
        return previous;
    }

    Point& operator=(int number)
    {
        x = number;
        y = number;
        return *this;
    }

    friend ostream& operator<<(ostream& output, const Point& value)
    {
        output << "(" << value.x << ", " << value.y << ")";
        return output;
    }

    friend istream& operator>>(istream& input, Point& value)
    {
        input >> value.x >> value.y;
        return input;
    }
};


template <class T>
class Matrix
{
private:
    T** elements;
    int rows;
    int columns;

    void Allocate(int height, int width)
    {
        rows = height;
        columns = width;

        elements = new T*[rows]{};

        try
        {
            for (int i = 0; i < rows; i++)
                elements[i] = new T[columns]{};
        }
        catch (...)
        {
            for (int i = 0; i < rows; i++)
                delete[] elements[i];

            delete[] elements;
            elements = nullptr;
            rows = 0;
            columns = 0;
            throw;
        }
    }

    void Clear()
    {
        for (int i = 0; i < rows; i++)
            delete[] elements[i];

        delete[] elements;

        elements = nullptr;
        rows = 0;
        columns = 0;
    }

public:

    Matrix()
    {
        elements = nullptr;
        rows = 0;
        columns = 0;
    }

    Matrix(int height, int width)
    {
        elements = nullptr;
        rows = 0;
        columns = 0;

        if (height <= 0 || width <= 0)
            throw invalid_argument("Invalid matrix size");

        Allocate(height, width);
    }

    Matrix(const Matrix& source)
    {
        elements = nullptr;
        rows = 0;
        columns = 0;

        if (source.rows == 0)
            return;

        Allocate(source.rows, source.columns);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
                elements[i][j] = source.elements[i][j];
        }
    }

    Matrix(Matrix&& source)
    {
        elements = source.elements;
        rows = source.rows;
        columns = source.columns;

        source.elements = nullptr;
        source.rows = 0;
        source.columns = 0;
    }

    ~Matrix()
    {
        Clear();
    }

    Matrix& operator=(const Matrix& source)
    {
        if (this != &source)
        {
            Matrix temporary(source);
            *this = move(temporary);
        }

        return *this;
    }

    Matrix& operator=(Matrix&& source)
    {
        if (this != &source)
        {
            Clear();

            elements = source.elements;
            rows = source.rows;
            columns = source.columns;

            source.elements = nullptr;
            source.rows = 0;
            source.columns = 0;
        }

        return *this;
    }

    Matrix& operator++()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
                ++elements[i][j];
        }

        return *this;
    }

    Matrix operator++(int)
    {
        Matrix previous(*this);
        ++(*this);
        return previous;
    }

    Matrix& operator--()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
                --elements[i][j];
        }

        return *this;
    }

    Matrix operator--(int)
    {
        Matrix previous(*this);
        --(*this);
        return previous;
    }

    Matrix operator+(const Matrix& source) const
    {
        if (rows != source.rows ||
            columns != source.columns)
            throw invalid_argument("Different matrix sizes");

        Matrix answer(rows, columns);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                answer.elements[i][j] =
                    elements[i][j] + source.elements[i][j];
            }
        }

        return answer;
    }

    Matrix operator-(const Matrix& source) const
    {
        if (rows != source.rows ||
            columns != source.columns)
            throw invalid_argument("Different matrix sizes");

        Matrix answer(rows, columns);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                answer.elements[i][j] =
                    elements[i][j] - source.elements[i][j];
            }
        }

        return answer;
    }

    Matrix operator*(const Matrix& source) const
    {
        if (columns != source.rows)
            throw invalid_argument("Cannot multiply matrices");

        Matrix answer(rows, source.columns);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < source.columns; j++)
            {
                answer.elements[i][j] = T{};

                for (int k = 0; k < columns; k++)
                {
                    answer.elements[i][j] =
                        answer.elements[i][j] +
                        elements[i][k] * source.elements[k][j];
                }
            }
        }

        return answer;
    }

    Matrix operator/(const Matrix& source) const
    {
        if (rows != source.rows ||
            columns != source.columns)
            throw invalid_argument("Different matrix sizes");

        Matrix answer(rows, columns);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                answer.elements[i][j] =
                    elements[i][j] / source.elements[i][j];
            }
        }

        return answer;
    }

    T FindMaximum() const
    {
        if (rows == 0 || columns == 0)
            throw runtime_error("Matrix is empty");

        T maximum = elements[0][0];

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                if (elements[i][j] > maximum)
                    maximum = elements[i][j];
            }
        }

        return maximum;
    }

    T FindMinimum() const
    {
        if (rows == 0 || columns == 0)
            throw runtime_error("Matrix is empty");

        T minimum = elements[0][0];

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                if (elements[i][j] < minimum)
                    minimum = elements[i][j];
            }
        }

        return minimum;
    }

    T& operator()(int r, int c)
    {
        if (r < 0 || r >= rows ||
            c < 0 || c >= columns)
            throw out_of_range("Invalid index");

        return elements[r][c];
    }

    friend ostream& operator<<(ostream& output, const Matrix& value)
    {
        for (int i = 0; i < value.rows; i++)
        {
            for (int j = 0; j < value.columns; j++)
                output << value.elements[i][j] << "\t";

            output << endl;
        }

        return output;
    }

    friend istream& operator>>(istream& input,
                               Matrix& value)
    {
        for (int i = 0; i < value.rows; i++)
        {
            for (int j = 0; j < value.columns; j++)
                input >> value.elements[i][j];
        }

        return input;
    }
};


int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    vector<Point> points;

    points.push_back(Point(1, 2));
    points.push_back(Point(3, 4));
    points.push_back(Point(5, 6));

    cout << "Points:" << endl;

    for (int i = 0; i < points.size(); i++)
        cout << points[i] << endl;

    cout << "\nFirst matrix:" << endl;

    Matrix<Point> firstMatrix(2, 4);

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            firstMatrix(i, j) =
                Point(rand() % 10, rand() % 10);
        }
    }

    cout << firstMatrix;

    cout << "\nAfter increment:" << endl;

    firstMatrix++;

    cout << firstMatrix;

    cout << "\nAfter decrement:" << endl;

    --firstMatrix;

    cout << firstMatrix;

    cout << "\nSecond matrix:" << endl;

    Matrix<Point> secondMatrix(2, 4);

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            secondMatrix(i, j) = Point(2, 3);
        }
    }

    cout << secondMatrix;

    cout << "\nSum of matrices:" << endl;

    Matrix<Point> sumMatrix = firstMatrix + secondMatrix;

    cout << sumMatrix;

    cout << "\nMaximum point:" << endl;
    cout << firstMatrix.FindMaximum() << endl;

    cout << "\nMinimum point:" << endl;
    cout << firstMatrix.FindMinimum() << endl;
}
