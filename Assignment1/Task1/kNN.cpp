#include "kNN.hpp"


//**********************************************************************
//* class Image thừa kế từ List<T>
//**********************************************************************





//**********************************************************************
//* class Dataset
//**********************************************************************

Dataset::Dataset()
{
  //TODO: implement task 2
}
Dataset::~Dataset()
{
    data->clear();
}
Dataset::Dataset(const Dataset& other)
{
    //TODO: implement task 2
}
Dataset::Dataset(List<List<int>*>* data, string* NameCol)
{
    //TODO: implement task 2
}
Dataset& Dataset::operator=(const Dataset& other)
{
    //TODO: implement task 2
    return *this;
}
bool Dataset::loadFromCSV(const char* fileName)
{   
      //TODO: implement task 2
      return true;
}
void Dataset::getShape(int& nRows, int& nCols) const
{
    //TODO: implement task 2
}
void Dataset::columns() const
{
    //TODO: implement task 2
}




//**********************************************************************
//* Class kNN
//**********************************************************************


void kNN::fit(const Dataset& X_train, const Dataset& y_train)
{
    this->X_train = X_train;
    this->Y_train = y_train;
}

Dataset kNN::predict(const Dataset& X_test)
{
    //TODO: implement task 3
    return X_test;
}

double kNN::score(const Dataset& y_test, const Dataset& y_pred)
{
    //TODO: implement task 
    return 0.0;
}



//**********************************************************************
//* hàm train_test_split
//**********************************************************************
void train_test_split(Dataset& X, Dataset& y, double test_size, 
                        Dataset& X_train, Dataset& X_test, 
                        Dataset& y_train, Dataset& y_test)
{
    //TODO: implement task 3
}
