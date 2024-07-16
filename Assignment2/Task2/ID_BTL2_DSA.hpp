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
    vector<vector<int>> leftVector(pointList.begin() + start, pointList.begin() + mid + 1);
    vector<vector<int>> rightVector(pointList.begin() + mid + 1, pointList.begin() + end + 1);
    int i = 0, j = 0, k = start;
    while (i < leftVector.size() && j < rightVector.size()) {
        if (leftVector[i][alpha] <= rightVector[j][alpha]) {
            pointList[k++] = leftVector[i++];
        } else {
            pointList[k++] = rightVector[j++];
        }
    }

    while (i < leftVector.size()) {
        pointList[k++] = leftVector[i++];
    }

    while (j < rightVector.size()) {
        pointList[k++] = rightVector[j++];
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
        // cout<<"\n";
        // for (int i = 0; i < sortedList.size(); i++) {
        //     for (int j=0; j < sortedList[0].size(); j++) {
        //         cout<<sortedList[i][j]<<",";
        //     }
        //     cout<<" ";
        // }
        // cout<<"\n";
        int mid = (sortedList.size()-1)/2;
        while (mid>=1 && sortedList[mid][alpha] == sortedList[mid-1][alpha]) {
            --mid;
        }
        // for (int i=0; i < sortedList[0].size(); i++) {
        //     cout<<sortedList[mid][i]<<",";
        // }
        // cout<<"\n";
        Node* newRoot = new Node(sortedList[mid]);
        newRoot->left = buildTreeWithParams(vector<vector<int>> (sortedList.begin(), sortedList.begin() + mid), level + 1);
        newRoot->right = buildTreeWithParams(vector<vector<int>> (sortedList.begin()+ mid + 1, sortedList.end()), level + 1);
        return newRoot;
    }
    double getDistance(const vector<int> &target, const vector<int> &nodeData) {
        double distance = 0.00;
        int length = target.size();
        for (int i=0; i< length; i++) {
            distance += ((target[i] - nodeData[i]) * (target[i] - nodeData[i]));
        }
        return sqrt(distance);
    }
    void findNearestNeighbour(Node* current, const vector<int> &target, Node* &best, int level) {
        if (current == nullptr) {
            return;
        }
        if (current != nullptr && current->left == nullptr && current->right == nullptr && best == nullptr) {
            best = current;
            // cout<<"\n";
            // for (int i=0; i< best->data.size(); i++) {
            //     cout<<best->data[i]<<" ";
            // }
            return;
        }

        int alpha = level % this->k;
        if (target[alpha] < current->data[alpha]) {
            if (current->left == nullptr && best == nullptr) {
                best = current;
                // cout<<"\n";
                // for (int i=0; i< best->data.size(); i++) {
                //     cout<<best->data[i]<<" ";
                // }
                // return;
            }
            findNearestNeighbour(current->left, target, best, level+1);
            double d = abs(current->data[alpha] - target[alpha]);
            double r = getDistance(target, current->data);
            double R = getDistance(target, best->data);
            // if (d >= R && r > R) {
            //     return;
            // }
            if (r < R) {
                best = current;
                // cout<<"\n";
                // for (int i=0; i< best->data.size(); i++) {
                //     cout<<best->data[i]<<" ";
                // }
            }
            if (d < R) {
                findNearestNeighbour(current->right, target, best, level+1);
            }
        } else if (target[alpha] >= current->data[alpha])  {
            if (current->right == nullptr && best == nullptr) {
                best = current;
                // cout<<"\n";
                // for (int i=0; i< best->data.size(); i++) {
                //     cout<<best->data[i]<<" ";
                // }
                // return;
            }
            findNearestNeighbour(current->right, target, best, level+1);
            double d = abs(current->data[alpha] - target[alpha]);
            double r = getDistance(target, current->data);
            double R = getDistance(target, best->data);
            // if (d >= R && r > R) {
            //     return;
            // }
            if (r < R) {
                best = current;
                // cout<<"\n";
                // for (int i=0; i< best->data.size(); i++) {
                //     cout<<best->data[i]<<" ";
                // }
            }
            if (d < R) {
                findNearestNeighbour(current->left, target, best, level+1);
            }
        }
        return;
    }
    void sortBestList (vector<Node *> &bestList, const vector<int> &target) {
        int length = bestList.size();
        for (int i=0; i < length -1; ++i) {
            bool flag = false;
            for (int j = 0; j < length - i - 1; ++j) {
                if (getDistance(bestList[j]->data, target) > getDistance(bestList[j+1]->data, target)) {
                    swap(bestList[j], bestList[j+1]);
                    flag = true;
                }
            }
            if (flag == false) {
                break;
            }
        }
    }
void findkNearestNeighbour(Node* current, const vector<int>& target, int k, vector<Node*>& bestList, int level) {
    if (current == nullptr) {
        return;
    }
    if (current->left == nullptr && current->right == nullptr && bestList.size() == 0) {
        bestList.push_back(current);
        return;
    }
    int alpha = level % this->k;
    //     cout<<"\n";
    //     for (int i=0; i< int(bestList.size()); i++) {
    //         for (int j=0; j< bestList[i]->data.size(); j++) {
    //         cout<<bestList[i]->data[j]<<",";
    //     }
    //     cout<<" ";
    //     }

    if (target[alpha] < current->data[alpha]) {
        if (current->left == nullptr && current->right != nullptr && bestList.size() == 0) {
            bestList.push_back(current);
            double R = getDistance(bestList.back()->data, target);
            double d = abs(current->data[alpha] - target[alpha]);
            if (d < R || bestList.size() < k) {
                findkNearestNeighbour(current->right, target, k, bestList, level + 1);
            }
        } else {
            findkNearestNeighbour(current->left, target, k, bestList, level + 1);
            double r = getDistance(current->data, target);
            double R = getDistance(bestList.back()->data, target);
            double d = abs(current->data[alpha] - target[alpha]);
            if (bestList.size() < k) {
                bestList.push_back(current);
                sortBestList(bestList, target);
            } else if (r < R) {
                bestList.back() = current;
                sortBestList(bestList, target);
            }
            if (d < getDistance(bestList.back()->data, target) || bestList.size() < k) {
                findkNearestNeighbour(current->right, target, k, bestList, level + 1);
            }
        }
    } else if (target[alpha] >= current->data[alpha]) {
        if (current->right == nullptr && current->left != nullptr && bestList.size() == 0) {
            bestList.push_back(current);       
            double R = getDistance(bestList.back()->data, target);
            double d = abs(current->data[alpha] - target[alpha]);
            if (d < R || bestList.size() < k) {
                findkNearestNeighbour(current->left, target, k, bestList, level + 1);
            }
        } else {
            findkNearestNeighbour(current->right, target, k, bestList, level + 1);
            double r = getDistance(current->data, target);
            double R = getDistance(bestList.back()->data, target);
            double d = abs(current->data[alpha] - target[alpha]);
            if (bestList.size() < k) {
                bestList.push_back(current);
                sortBestList(bestList, target);
            } else if (r < R) {
                bestList.back() = current;
                sortBestList(bestList, target);
            }
            if (d < getDistance(bestList.back()->data, target) || bestList.size() < k) {
                findkNearestNeighbour(current->left, target, k, bestList, level + 1);
            }
        }
    }
    return;
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
        delete root;
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
    void nearestNeighbour(const vector<int> &target, Node* &best) {
        best = nullptr;
        findNearestNeighbour(root, target, best, 0);
        return;
    };
    void kNearestNeighbour(const vector<int> &target, int k, vector<Node *> &bestList) {
        findkNearestNeighbour(root, target, k, bestList, 0);
        return;
    };

};



// Please add more or modify as needed
