
#include "main.hpp"

/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */

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
    virtual double distanceEuclidean(const List<T> *image) = 0;
};

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

    double distanceEuclidean(const List<T> *image)
    {
        double distance = 0.0;
        int index = 0;
        int len1 = this->length();
        int len2 = image->length();
        int len = len1 > len2 ? len2 : len1;
        while (index < len)
        {
            distance += ((this->get(index) - image->get(index)) * (this->get(index) - image->get(index)));
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
                distance += ((this->get(index) - 0) * (this->get(index) - 0));
                index++;
            }
        }
        else
        {
            while (index < len2)
            {
                distance += ((0 - image->get(index)) * (0 - image->get(index)));
                index++;
            }
        }
        return sqrt(distance);
    }
};

class Dataset
{
private:
    List<List<int> *> *data;
    string *NameCol;
    // You may need to define more
public:
    //! Hàm của thầy
    Dataset();
    ~Dataset();
    Dataset(List<List<int> *> *data, string *NameCol);
    Dataset(const Dataset &other);
    Dataset &operator=(const Dataset &other);
    bool loadFromCSV(const char *fileName);
    void getShape(int &nRows, int &nCols) const;
    void columns() const;

    void printHead(int nRows = 5, int nCols = 5) const
    {
        // TODO: implement task 2
    }
    void printTail(int nRows = 5, int nCols = 5) const
    {
        // TODO: implement task 2
    }
    bool drop(int axis = 0, int index = 0, std::string columns = "")
    {
        // TODO: implement task 2
        return true;
    }
    Dataset extract(int startRow = 0, int endRow = -1, int startCol = 0, int endCol = -1) const
    {
        // TODO: implement task 2
        return *this;
    }

    int predict(const List<int> *image)
    {
        // TODO: implement task 2
        return 1;
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
    kNN(int k = 5) : k(5){};
    void fit(const Dataset &X_train, const Dataset &y_train);
    Dataset predict(const Dataset &X_test);
    double score(const Dataset &y_test, const Dataset &y_pred);
};

void train_test_split(Dataset &X, Dataset &y, double test_size,
                      Dataset &X_train, Dataset &X_test, Dataset &y_train, Dataset &y_test);

// Please add more or modify as needed