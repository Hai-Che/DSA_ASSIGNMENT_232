#include <iostream>
using namespace std;
int main()
{
    double test_size = 0.34;
    int nRows = 50;
    int Cols = 2;
    bool flag = test_size * nRows == int(test_size * nRows);
    cout << flag;
    cout << "0 to " << nRows - int(test_size * nRows) - 1 + int(flag) << "\n";
    cout << nRows - int(test_size * nRows) + int(flag) << " to " << nRows;
    // double score = 1;
    // int length = 5;
    // cout << score / length;
    return 0;
}