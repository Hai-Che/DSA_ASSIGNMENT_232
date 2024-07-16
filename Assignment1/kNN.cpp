#include "kNN.hpp"

void train_test_split(Dataset &X, Dataset &Y, double test_size,
                      Dataset &X_train, Dataset &X_test, Dataset &Y_train, Dataset &Y_test)
{
    if (test_size >= 1 || test_size <= 0 || X.getData()->length() != Y.getData()->length())
    {
        return;
    }
    int rows = X.getData()->length();
    double floorDouble = 1.0e-15;
    double rowChild = (1 - test_size) * rows;
    if (abs(round(rowChild) - rowChild) < rows * floorDouble)
    {
        rowChild = round(rowChild);
    }
    X_train = X.extract(0, rowChild - 1, 0, -1);
    X_test = X.extract(rowChild, -1, 0, -1);
    Y_train = Y.extract(0, rowChild - 1, 0, -1);
    Y_test = Y.extract(rowChild, -1, 0, -1);
}