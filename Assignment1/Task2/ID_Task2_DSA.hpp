#include "main.hpp"

struct newPair
{
    double distance;
    int label;
};

template <typename T>
class List
{
public:
    //! Hàm của thầy
    virtual ~List() = default;
    virtual void push_back(T value) = 0;
    virtual void push_front(T value) = 0;
    virtual void insert(int index, T value) = 0;
    virtual void remove(int index) = 0;
    virtual T &get(int index) const = 0;
    virtual int length() const = 0;
    virtual void clear() = 0;
    virtual void print() const = 0;
    virtual void reverse() = 0;

    //! Hàm hỗ trợ thêm
    virtual List<T> *subList(int start, int end) = 0;
    virtual void printStartToEnd(int start, int end) const = 0;
};

//
// class Image
template <typename T>
class Image : public List<T>
{
private:
    class Node
    {
    public:
        T pointer;
        Node *next;

    public:
        Node(T pointer, Node *next = nullptr) : pointer(pointer), next(next) {}
    };

private:
    Node *head;
    Node *tail;
    int size;

public:
    Image()
    {
        head = tail = nullptr;
        size = 0;
    }
    ~Image()
    {
        this->clear();
    }
    void push_back(T value)
    {
        Node *newNode = new Node(value);
        if (size == 0)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
        ++size;
    }
    void push_front(T value)
    {
        Node *newNode = new Node(value);
        if (size == 0)
        {
            head = tail = newNode;
        }
        else
        {
            newNode->next = head;
            head = newNode;
        }
        ++size;
    }
    void insert(int index, T value)
    {
        if (index < 0 || index > size)
        {
            return;
        }
        else
        {
            if (index == 0)
            {
                push_front(value);
                return;
            }
            else if (index == size)
            {
                push_back(value);
                return;
            }
            else
            {
                Node *newNode = new Node(value);
                Node *temp = head;
                while (--index)
                {
                    temp = temp->next;
                }
                newNode->next = temp->next;
                temp->next = newNode;
                ++size;
            }
        }
    }
    void remove(int index)
    {
        if (index < 0 || index >= size)
        {
            return;
        }
        else
        {
            if (size == 1)
            {
                delete head;
                head = tail = nullptr;
            }
            else if (index == 0)
            {
                Node *temp = head;
                head = head->next;
                delete temp;
            }
            else
            {
                Node *temp = head;
                while (--index)
                {
                    temp = temp->next;
                }
                if (temp->next->next)
                {
                    Node *del = temp->next;
                    temp->next = del->next;
                    delete del;
                }
                else
                {
                    Node *del = temp->next;
                    tail = temp;
                    tail->next = nullptr;
                    delete del;
                }
            }
            --size;
        }
    }
    T &get(int index) const
    {
        if (index < 0 || index >= this->size)
            throw std::out_of_range("get(): Out of range");
        Node *temp = head;
        while (index)
        {
            temp = temp->next;
            index--;
        }
        return temp->pointer;
    }
    int length() const
    {
        return size;
    }
    void clear()
    {
        Node *temp = head;
        while (temp)
        {
            temp = temp->next;
            delete head;
            head = temp;
        }
        head = tail = nullptr;
        size = 0;
    }
    void print() const
    {
        // remake later
        if (size == 0)
            OUTPUT << "nullptr" << endl;
        // cout << "nullptr" << endl;
        Node *temp = head;
        for (int i = 0; i < this->size; i++)
        {
            if (i == this->size - 1)
                OUTPUT << temp->pointer << endl;
            // cout << temp->pointer << endl;
            else
                OUTPUT << temp->pointer << " ";
            // cout << temp->pointer << " ";
            temp = temp->next;
        }
    }
    void reverse()
    {
        if (!head || size == 1)
        {
            return;
        }
        else
        {
            Node *prev = head, *current = prev->next, *post = current->next;
            prev->next = nullptr;
            while (post)
            {
                current->next = prev;
                prev = current;
                current = post;
                post = post->next;
            }
            current->next = prev;
            tail = head;
            head = current;
        }
    }
    void printStartToEnd(int start, int end) const
    {
        // remake later
        Node *temp = head;
        for (int i = 0; i < start; i++)
            temp = temp->next;
        for (int i = start; i < end && i < this->size; i++)
        {
            if (i == end - 1 || i == this->size - 1)
                OUTPUT << temp->pointer << endl;
            // cout << temp->pointer << endl;
            else
                OUTPUT << temp->pointer << " ";
            // cout << temp->pointer << " ";
            temp = temp->next;
        }
    }
    List<T> *subList(int start, int end)
    {
        if (this->size <= start)
        {
            return nullptr;
        }
        List<T> *result = new Image<T>();
        Node *temp = head;
        for (int i = 0; i < start; i++)
        {
            temp = temp->next;
        }
        if (end > size)
        {
            while (temp)
            {
                result->push_back(temp->pointer);
                temp = temp->next;
            }
        }
        else
        {
            for (int i = start; i < end; i++)
            {
                result->push_back(temp->pointer);
                temp = temp->next;
            }
        }
        return result;
    }
};

class Dataset
{
private:
    List<List<int> *> *data;
    List<string> *nameCol;
    // You may need to define more
public:
    //! Hàm khởi tạo
    Dataset()
    {
        this->nameCol = new Image<string>();
        this->data = new Image<List<int> *>();
    }
    //! Hàm hủy
    ~Dataset()
    {
        for (int i = 0; i < data->length(); i++)
        {
            data->get(i)->clear();
        }
        for (int i = 0; i < data->length(); i++)
        {
            delete data->get(i);
        }
        delete data;
        delete nameCol;
    }
    Dataset(const Dataset &other)
    {
        this->nameCol = new Image<string>();
        this->data = new Image<List<int> *>();
        if (other.nameCol->length() == 0 && other.data->length() == 0)
        {
        }
        else
        {
            if (other.nameCol != nullptr)
            {
                nameCol = other.nameCol->subList(0, other.nameCol->length());
            }
            for (int i = 0; i < other.data->length(); i++)
            {
                data->push_back(other.data->get(i)->subList(0, other.data->get(0)->length()));
            }
        }
    }
    Dataset &operator=(const Dataset &other)
    {
        if (this == &other)
        {
            return *this;
        }
        int row = other.data->length();
        int col = other.data->length() ? other.data->get(0)->length() : 0;
        this->~Dataset();
        this->nameCol = new Image<string>();
        this->data = new Image<List<int> *>();
        if (!row)
        {
            return *this;
        }
        // if (other.nameCol == nullptr && other.data->length() == 0)
        // {
        // }
        else
        {
            if (other.nameCol != nullptr)
            {
                nameCol = other.nameCol->subList(0, other.nameCol->length());
            }
            for (int i = 0; i < other.data->length(); i++)
            {
                data->push_back(other.data->get(i)->subList(0, other.data->get(0)->length()));
            }
        }
        return *this;
    }
    List<List<int> *> *getData() const
    {
        // ! UPDATE file thay
        return data;
    }
    bool loadFromCSV(const char *fileName)
    {
        ifstream file(fileName);
        //* kiểm tra mở file
        if (file.is_open())
        {
            string str;
            int number;

            //* xử lý hàng đầu tiên chuyển , thành ' ' dùng thư viện stringstream
            file >> str;
            for (int i = 0; i < str.length(); i++)
            {
                if (str[i] == ',')
                    str[i] = ' ';
            }
            stringstream ss(str);
            while (ss >> str)
                nameCol->push_back(str);

            //* xử lý các hàng còn lại , thành ' ' dùng thư viện stringstream
            while (file >> str)
            {
                for (int i = 0; i < str.length(); i++)
                {
                    if (str[i] == ',')
                        str[i] = ' ';
                }
                stringstream ss(str);
                List<int> *temp = new Image<int>();
                while (ss >> number)
                    temp->push_back(number);
                data->push_back(temp);
            }
            return true;
        }
        return false;
    }
    void getShape(int &nRows, int &nCols) const
    {
        nRows = data->length();
        nCols = nRows == 0 ? 0 : data->get(0)->length();
    }
    void columns() const
    {
        if (nameCol != nullptr)
        {
            nameCol->print();
        }
    }
    void printHead(int nRows = 5, int nCols = 5) const
    {
        if (nRows <= 0 || nCols <= 0 || data->length() == 0)
            return;
        nRows = nRows > data->length() ? data->length() : nRows;
        // if (data->length() > 0)
        // {
        nCols = nCols > data->get(0)->length() ? data->get(0)->length() : nCols;
        // }
        // else
        // {
        //     nCols = nCols > nameCol->length() ? nameCol->length() : nCols;
        // }
        // if (data->length() > 0)
        // {
        if (nameCol != nullptr)
            nameCol->printStartToEnd(0, nCols);
        // }
        for (int i = 0; i < nRows; i++)
        {
            data->get(i)->printStartToEnd(0, nCols);
        }
    }
    void printTail(int nRows = 5, int nCols = 5) const
    {
        if (nRows <= 0 || nCols <= 0 || data->length() == 0)
            return;
        int lengthCol = data->get(0)->length();
        int lengthRow = data->length();
        nCols = nCols > lengthCol ? lengthCol : nCols;
        nRows = nRows > lengthRow ? lengthRow : nRows;
        if (nameCol != nullptr)
        {
            nameCol->printStartToEnd(lengthCol - nCols, lengthCol);
        }
        for (int i = lengthRow - nRows; i < lengthRow; i++)
        {
            data->get(i)->printStartToEnd(lengthCol - nCols, lengthCol);
        }
    }
    bool drop(int axis = 0, int index = 0, std::string columns = "")
    {
        if (axis == 0)
        {
            if (index >= data->length() || index < 0)
            {
                return false;
            }
            else
            {
                data->get(index)->clear();
                delete data->get(index);
                data->remove(index);
                // data->remove(index);
                return true;
            }
        }
        else if (axis == 1)
        {
            for (int i = 0; i < nameCol->length(); i++)
            {
                if (nameCol->get(i) == columns && data->length() > 0)
                {
                    if (nameCol->length() == 1)
                    {
                        for (int j = 0; j < data->length(); j++)
                        {
                            data->get(j)->clear();
                            delete data->get(j);
                        }
                        data->clear();
                    }
                    else
                    {
                        nameCol->remove(i);
                        for (int j = 0; j < data->length(); j++)
                        {
                            data->get(j)->remove(i);
                        }
                    }
                    return true;
                }
            }
        }
        return false;
    }
    Dataset extract(int startRow = 0, int endRow = -1, int startCol = 0, int endCol = -1) const
    {
        Dataset ans;
        if (data->length() > 0)
        {
            // cout << "go here\n";
            endRow = (endRow > data->length() || endRow == -1) ? data->length() : endRow;
            endCol = (endCol > data->get(0)->length() || endCol == -1) ? data->get(0)->length() : endCol;
            if (startCol > endCol || startRow > endRow || startCol >= data->get(0)->length() || startRow >= data->length())
            {
                return ans;
            }
            if (this->nameCol->subList(startCol, endCol + 1) != nullptr)
            {
                ans.nameCol = this->nameCol->subList(startCol, endCol + 1);
            }

            // cout << "/////////////////////////\n";
            // cout << "R:" << startRow << "to" << endRow << "\n";
            // cout << "C:" << startCol << "to" << endCol << "\n";
            // cout << "data length:" << data->length() << "\n";
            // cout << "data length:" << nameCol->length() << "\n";
            // cout << "data->get(0) length:" << data->get(0)->length() << "\n";
            // cout << "/////////////////////////\n";
            for (int i = startRow; i < endRow + 1; i++)
            {
                if (i == this->data->length())
                {
                    // cout << "result data length:" << ans.data->length() << "\n";
                    // cout << "result data->get(0) length:" << ans.data->get(0)->get(0) << "\n";
                    // cout << "/////////////////////////\n";
                    return ans;
                }
                ans.data->push_back(this->data->get(i)->subList(startCol, endCol + 1));
            }
            // cout << "result data length:" << ans.data->length() << "\n";
            // cout << "result data->get(0) length:" << ans.data->get(0)->get(0) << "\n";
            // cout << "/////////////////////////\n";
            return ans;
        }
        return ans;
    }
    double distanceEuclidean(const List<int> *x, const List<int> *y) const
    {
        double distance = 0.0;
        int index = 0;
        int len1 = x->length();
        int len2 = y->length();
        int len = len1 > len2 ? len2 : len1;
        while (index < len)
        {
            distance += ((x->get(index) - y->get(index)) * (x->get(index) - y->get(index)));
            index++;
        }
        if (len1 == len2)
        {
            return sqrt(distance);
        }
        else if (index < len1)
        {
            while (index < len1)
            {
                distance += ((x->get(index) - 0) * (x->get(index) - 0));
                index++;
            }
        }
        else
        {
            while (index < len2)
            {
                distance += ((0 - y->get(index)) * (0 - y->get(index)));
                index++;
            }
        }
        return sqrt(distance);
    }

    Dataset predict(const Dataset &X_train, const Dataset &Y_train, const int k) const
    {
        // TODO: implement Task 3
        Dataset result;
        if (data->length() == 0)
        {
            return result;
        }
        result.nameCol = Y_train.nameCol->subList(0, 1);
        for (int i = 0; i < data->length(); i++)
        {
            newPair discs[X_train.data->length()];
            // std::vector<std::pair<double, int>> distances;
            int arr[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            int labelMax = 0, labelIndex = 0;
            for (int j = 0; j < X_train.data->length(); j++)
            {
                discs[j] = {distanceEuclidean(data->get(i), X_train.data->get(j)), Y_train.data->get(j)->get(0)};
            }
            // cout << "\n"
            //      << discs[3].distance << " " << discs[3].label << "\n";
            // sorting
            int n = X_train.data->length();
            for (int j = 0; j < n - 1; ++j)
            {
                for (int h = 0; h < n - j - 1; ++h)
                {
                    if (discs[h].distance > discs[h + 1].distance)
                    {
                        swap(discs[h], discs[h + 1]);
                    }
                }
            }
            //
            // for (int j = 0; j < X_train.data->length(); j++)
            // {
            //     cout << "\n"
            //          << discs[j].distance << " " << discs[j].label << "\n";
            // }
            for (int j = 0; j < k; j++)
            {
                if (j == X_train.data->length())
                {
                    break;
                }
                ++arr[discs[j].label];
                labelMax = arr[discs[j].label] > labelMax ? arr[discs[j].label] : labelMax;
            }
            for (int j = 0; j < 10; j++)
            {
                if (arr[j] == labelMax)
                {
                    labelIndex = j;
                    break;
                }
            }
            List<int> *predict = new Image<int>();
            predict->push_back(labelIndex);
            result.data->push_back(predict);
        }
        return result;
    }
    double score(const Dataset &y_predict) const
    {
        // TODO: implement Task 3
        if (y_predict.nameCol->length() > 1 || y_predict.data->length() == 0 || data->length() != y_predict.data->length())
        {
            return -1;
        }
        double score = 0;
        for (int i = 0; i < data->length(); i++)
        {
            if (data->get(i)->get(0) == y_predict.data->get(i)->get(0))
            {
                ++score;
            }
        }
        // cout << data->get(0)->get(0) << " " << y_predict.data->get(0)->get(0);
        return (score / data->length());
    }
};

class kNN
{
private:
    int k;
    Dataset X_train;
    Dataset Y_train;
    // You may need to define more
public:
    kNN(int k = 5) : k(k){};
    void fit(const Dataset &X_train, const Dataset &y_train)
    {
        this->X_train = X_train;
        this->Y_train = y_train;
    }
    Dataset predict(const Dataset &X_test)
    {
        return X_test.predict(this->X_train, this->Y_train, this->k);
    }
    double score(const Dataset &y_test, const Dataset &y_pred)
    {
        return y_test.score(y_pred);
    }
};

void train_test_split(Dataset &X, Dataset &Y, double test_size,
                      Dataset &X_train, Dataset &X_test, Dataset &Y_train, Dataset &Y_test)
{
    if (X.getData()->length() != Y.getData()->length() || test_size >= 1 || test_size <= 0)
        return;

    double minDouble = 1.0e-15;
    int nRow = X.getData()->length();
    double rowSplit = nRow * (1 - test_size);

    if (abs(round(rowSplit) - rowSplit) < minDouble * nRow)
        rowSplit = round(rowSplit);

    X_train = X.extract(0, rowSplit - 1, 0, -1);
    Y_train = Y.extract(0, rowSplit - 1, 0, -1);

    X_test = X.extract(rowSplit, -1, 0, -1);
    Y_test = Y.extract(rowSplit, -1, 0, -1);
}

// Please add more or modify as needed