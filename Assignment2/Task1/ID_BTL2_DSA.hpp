#include "main.hpp"
// #include "Dataset.hpp"
/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */
struct Node
{
    vector<int> data;
    Node *left;
    Node *right;
    Node(vector<int> data, Node *left = nullptr, Node *right = nullptr)
    {
        this->data = data;
        this->left = left;
        this->right = right;
    }

    void print() const
    {
        OUTPUT << "(";
        for(int i = 0; i < data.size(); i++)
        {
            OUTPUT << data[i];
            if (i == data.size() - 1) OUTPUT << ")";
            else OUTPUT << ",";
        }
        // OUTPUT << "\n";
        OUTPUT << " ";
    }
};

class kDTree
{
private:
    int k;
    Node *root;
    int count;
private:
    // hàm phụ nếu cần
    Node* deepCopy(const Node* node) {
        if (node == nullptr) {
            return nullptr;
        }
        Node* newNode = new Node(node->data);
        newNode->left = deepCopy(node->left);
        newNode->right = deepCopy(node->right);
        return newNode;
    }
    void inOrderTraversal(Node* node) const {
        if (node == nullptr) {
            return ;
        }
        inOrderTraversal(node->left);
        node->print();
        inOrderTraversal(node->right);
    }
    void preOrderTraversal(Node* node) const {
        if (node == nullptr) {
            return ;
        }
        node->print();
        preOrderTraversal(node->left);
        preOrderTraversal(node->right);
    }
    void postOrderTraversal(Node* node) const {
        if (node == nullptr) {
            return ;
        }
        postOrderTraversal(node->left);
        postOrderTraversal(node->right);
        node->print();
    }
    int getHeight (Node* node) const {
        if (node == nullptr) {
            return 0;
        }
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);
        return max(leftHeight, rightHeight) + 1;
    }
    int countNode (Node* node) const {
        if (node == nullptr) {
            return 0;
        }
        int countInLeft = countNode(node->left);
        int countInRight = countNode(node->right);
        return 1 + countInLeft + countInRight;
    }
    int countLeaf(Node* node) const {
        if (node == nullptr) {
            return 0;
        }
        if (node->left == nullptr && node->right == nullptr) {
            return 1;
        }
        int leftLeaf = countLeaf(node->left);
        int rightLeaf = countLeaf(node->right);
        return leftLeaf + rightLeaf;
    }
    void insertWithParams(Node* &node, const vector<int> &point, int level) {
        if (node == nullptr) {
            node = new Node(point);
            return;
        }
        int alpha = level % this->k;
        if (point[alpha] >= node->data[alpha]) {
            insertWithParams(node->right, point, level+1);
        } else {
            insertWithParams(node->left, point, level+1);
        }
    }
    bool searchWithParams(Node* node, const vector<int> &point, int level) {
        if (node == nullptr) {
            return false;
        }
        int alpha = level % this->k;
        if (point[alpha] == node->data[alpha]) {
            if (node->data == point) {
            return true;
            } else {
                return searchWithParams(node->right, point, level+1);
            }
        } else if (point[alpha] > node->data[alpha]) {
            return searchWithParams(node->right, point, level+1);
        } else {
            return searchWithParams(node->left, point, level+1);
        }
    }
    Node* findMinNode(Node* father, Node* child_left, Node* child_right, int alphaFather) {
        int a,b,c;
        a = father->data[alphaFather];
        child_left != nullptr ? b = child_left->data[alphaFather] : b = 999;
        child_right != nullptr ? c = child_right->data[alphaFather] : c = 999;
        if (c < a && c < b) {
            return child_right;
        } else if (b < a && b <= c) {
            return child_left;
        } else {
            return father;
        }
    }
    Node* nodeReplace(Node* node, int level, int alphaFather) {
        if (node == nullptr) {
            return nullptr;
        }
        int alpha = level % this->k;
        if (alpha == alphaFather) {
            if (node->left == nullptr) {
                return node;
            }
            return nodeReplace(node->left, level+1, alphaFather);
        }
        return findMinNode(node, nodeReplace(node->left, level+1, alphaFather), nodeReplace(node->right, level+1, alphaFather), alphaFather);
    }
    bool removeWithParams(Node* &node, const vector<int> &point, int level) {
        if (node == nullptr) {
            return false;
        }
        int alpha = level % this->k;
        if (point[alpha] > node->data[alpha]) {
            return removeWithParams(node->right, point, level+1);
        } else if (point[alpha] < node->data[alpha]) {
            return removeWithParams(node->left, point, level+1);
        } else {
            if (node->data == point) {
            if (node->right == nullptr && node->left == nullptr) {
                delete node;
                node = nullptr;
                return true;
            }
            if (node->right != nullptr) {
                Node* del = nodeReplace(node->right, level+1, alpha);
                node->data = del->data;
                return removeWithParams(node->right, del->data, level + 1);
            } else {
                Node* del = nodeReplace(node->left, level+1, alpha);
                node->data = del->data;
                node->right = node->left;
                node->left = nullptr;
                return removeWithParams(node->right, del->data, level + 1);
            }
            } else {
                return removeWithParams(node->right, point, level+1);
            }
        }
    }

    void merge (vector<vector<int>> &pointList, int start, int mid, int end, int alpha) {
        int leftSize = mid - start + 1;
        int rightSize = end - mid;
        vector<vector<int>> leftVector(leftSize);
        vector<vector<int>> rightVector(rightSize);
        for (int i = 0; i < leftSize; ++i) {
            leftVector[i] = pointList[start + i];
        }
        for (int j = 0; j < rightSize; ++j) {
            rightVector[j] = pointList[mid + 1 + j];
        }
        int i = 0, j = 0, k = start;
        while (i < leftSize && j < rightSize) {
            if (leftVector[i][alpha] <= rightVector[j][alpha]) {
                pointList[k] = leftVector[i];
                ++i;
            } else {
                pointList[k] = rightVector[j];
                ++j;
            }
            ++k;
        }
        while (i < leftSize) {
            pointList[k] = leftVector[i];
            ++i;
            ++k;
        }
        while (j < rightSize) {
            pointList[k] = rightVector[j];
            ++j;
            ++k;
        }
    }
    void mergeSort(vector<vector<int>> &pointList, int start, int end, int alpha) {
        if (start < end) {
            int mid = start + (end - start) / 2;
            mergeSort(pointList, start, mid, alpha);
            mergeSort(pointList, mid + 1, end, alpha);
            merge(pointList, start, mid, end, alpha);
        }
    }
    Node* buildTreeWithParams (const vector<vector<int>> &pointList, int level) {
        if (pointList.empty()) {
            return nullptr;
        }
        int alpha = level % this->k;
        vector<vector<int>> sortedList = pointList;
        mergeSort(sortedList, 0, sortedList.size() - 1, alpha);
        int mid = (sortedList.size()-1)/2;
        while (sortedList[mid] == sortedList[mid-1]) {
            --mid;
        }
        Node* newRoot = new Node(sortedList[mid]);
        newRoot->left = buildTreeWithParams(vector<vector<int>> (sortedList.begin(), sortedList.begin() + mid ), level + 1);
        newRoot->right = buildTreeWithParams(vector<vector<int>> (sortedList.begin()+ mid + 1, sortedList.end()), level + 1);
        return newRoot;
    }
public:
    kDTree(int k = 2) {
        this->k = k;
        this->root = nullptr;
        this->count = 0;
    };
    ~kDTree() {
        this->count = 0;
        this->k = 0;
        while (count > 0) {
            remove(root->data);
        }
        root = nullptr;
    };

    const kDTree &operator=(const kDTree &other){
        this->k = other.k;
        this->root = deepCopy(other.root);
        this->count = other.count;
        return *this;
    };
    kDTree(const kDTree &other) {
        this->k = other.k;
        this->root = deepCopy(other.root);
        this->count = other.count;
    };
    int nodeCount() const {
        return countNode(root);
    };
    int height() const {
        return getHeight(root);
    };
    int leafCount() const {
        return countLeaf(root);
    };
    void inorderTraversal() const {
        inOrderTraversal(root);
    };
    void preorderTraversal() const{
        preOrderTraversal(root);
    };
    void postorderTraversal() const {
        postOrderTraversal(root);
    };
    void insert(const vector<int> &point) {
        insertWithParams(root, point, 0);
        this->count++;
    };
    void remove(const vector<int> &point) {
        bool check = removeWithParams(root, point, 0);
        if (check) {this->count--;}
    };
    bool search(const vector<int> &point) {
        return searchWithParams(root, point, 0);
    };
    void buildTree(const vector<vector<int>> &pointList) {
        root = buildTreeWithParams(pointList, 0);
    };
    // void nearestNeighbour(const vector<int> &target, Node *best);
    // void kNearestNeighbour(const vector<int> &target, int k, vector<Node *> &bestList);

};



// Please add more or modify as needed
