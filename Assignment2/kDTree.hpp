#include "main.hpp"
#include "Dataset.hpp"
/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */
struct kDTreeNode
{
    vector<int> data;
    int label;
    kDTreeNode *left;
    kDTreeNode *right;
    kDTreeNode(vector<int> data, kDTreeNode *left = nullptr, kDTreeNode *right = nullptr)
    {
        this->data = data;
        this->label = 0;
        this->left = left;
        this->right = right;
    }
    kDTreeNode(vector<int> data, int label, kDTreeNode *left = nullptr, kDTreeNode *right = nullptr)
    {
        this->data = data;
        this->label = label;
        this->left = nullptr;
        this->right = nullptr;
    }

    friend ostream &operator<<(ostream &os, const kDTreeNode &node)
    {
        os << "(";
        for (int i = 0; i < node.data.size(); i++)
        {
            os << node.data[i];
            if (i != node.data.size() - 1)
            {
                os << ", ";
            }
        }
        os << ")";
        return os;
    }
};


class kDTree
{
private:
    int k;
    kDTreeNode *root;
    int count;
private:
    kDTreeNode* deepCopy(const kDTreeNode* node) {
        if (node == nullptr) {
            return nullptr;
        }
        kDTreeNode* newNode = new kDTreeNode(node->data);
        newNode->left = deepCopy(node->left);
        newNode->right = deepCopy(node->right);
        return newNode;
    }
    void deleteTree(kDTreeNode* node) {
        if (node == nullptr) {
            return;
        }
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
    int getK() {
        return this->k;
    }
    void setK(int k) {
        this->k = k;
        return;
    }
    void inOrderTraversal(kDTreeNode* node) const {
        if (node == nullptr) {
            return ;
        }
        inOrderTraversal(node->left);
        cout << *node << " ";
        inOrderTraversal(node->right);
    }
    void preOrderTraversal(kDTreeNode* node) const {
        if (node == nullptr) {
            return ;
        }
        cout << *node << " ";
        preOrderTraversal(node->left);
        preOrderTraversal(node->right);
    }
    void postOrderTraversal(kDTreeNode* node) const {
        if (node == nullptr) {
            return ;
        }
        postOrderTraversal(node->left);
        postOrderTraversal(node->right);
        cout << *node << " ";
    }
    int getHeight (kDTreeNode* node) const {
        if (node == nullptr) {
            return 0;
        }
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);
        return max(leftHeight, rightHeight) + 1;
    }
    int countNode (kDTreeNode* node) const {
        if (node == nullptr) {
            return 0;
        }
        int countInLeft = countNode(node->left);
        int countInRight = countNode(node->right);
        return 1 + countInLeft + countInRight;
    }
    int countLeaf(kDTreeNode* node) const {
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
    void insertWithParams(kDTreeNode* &node, const vector<int> &point, int level) {
        if (node == nullptr) {
            node = new kDTreeNode(point);
            return;
        }
        int alpha = level % this->k;
        if (point[alpha] >= node->data[alpha]) {
            insertWithParams(node->right, point, level+1);
        } else {
            insertWithParams(node->left, point, level+1);
        }
    }
    bool searchWithParams(kDTreeNode* node, const vector<int> &point, int level) {
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
    kDTreeNode* findMinNode(kDTreeNode* father, kDTreeNode* child_left, kDTreeNode* child_right, int alphaFather) {
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
    kDTreeNode* nodeReplace(kDTreeNode* node, int level, int alphaFather) {
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
    bool removeWithParams(kDTreeNode* &node, const vector<int> &point, int level) {
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
                kDTreeNode* del = nodeReplace(node->right, level+1, alpha);
                node->data = del->data;
                return removeWithParams(node->right, del->data, level + 1);
            } else {
                kDTreeNode* del = nodeReplace(node->left, level+1, alpha);
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
            pointList[k] = leftVector[i];
            k++;
            i++;
        } else {
            pointList[k] = rightVector[j];
            k++;
            j++;
        }
    }
    while (i < leftVector.size()) {
        pointList[k] = leftVector[i];
        k++;
        i++;
    }
    while (j < rightVector.size()) {
        pointList[k] = rightVector[j];
        k++;
        j++;
    }
    }
    void mergeV2 (vector<vector<int>> &pointList, vector<int> &label, int start, int mid, int end, int alpha) {
        vector<vector<int>> leftVector(pointList.begin() + start, pointList.begin() + mid + 1);
        vector<vector<int>> rightVector(pointList.begin() + mid + 1, pointList.begin() + end + 1);
        vector<int> leftLabel(label.begin() + start, label.begin() + mid + 1);
        vector<int> rightLabel(label.begin() + mid + 1, label.begin() + end + 1);
        int i = 0, j = 0, k = start;
        while (i < leftVector.size() && j < rightVector.size()) {
            if (leftVector[i][alpha] <= rightVector[j][alpha]) {
                pointList[k] = leftVector[i];
                label[k] = leftLabel[i];
                k++;
                i++;
            } else {
                pointList[k] = rightVector[j];
                label[k] = rightLabel[j];
                k++;
                j++;
            }
        }
        while (i < leftVector.size()) {
            pointList[k] = leftVector[i];
            label[k] = leftLabel[i];
            k++;
            i++;
        }
        while (j < rightVector.size()) {
            pointList[k] = rightVector[j];
            label[k] = rightLabel[j];
            k++;
            j++;
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
    void mergeSortV2(vector<vector<int>> &pointList, vector<int> &label, int start, int end, int alpha) {
        if (start < end) {
            int mid = start + (end - start) / 2;
            mergeSortV2(pointList, label, start, mid, alpha);
            mergeSortV2(pointList, label, mid + 1, end, alpha);
            mergeV2(pointList, label, start, mid, end, alpha);
        }
    }
    kDTreeNode* buildTreeWithParams (const vector<vector<int>> &pointList, int level) {
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
        kDTreeNode* newRoot = new kDTreeNode(sortedList[mid]);
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
    void findNearestNeighbour(kDTreeNode* current, const vector<int> &target, kDTreeNode* &best, int level) {
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
    void sortBestList (vector<kDTreeNode *> &bestList, const vector<int> &target) {
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
    void findkNearestNeighbour(kDTreeNode* current, const vector<int>& target, int k, vector<kDTreeNode*>& bestList, int level) {
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
    kDTreeNode* buildTreeLabelWithParams (const vector<vector<int>> &pointList,const vector<int> &label, int level) {
        if (pointList.empty()) {
            return nullptr;
        }
        int alpha = level % this->k;
        vector<vector<int>> sortedList = pointList;
        vector<int> labelList = label;
        mergeSortV2(sortedList, labelList, 0, sortedList.size() - 1, alpha);
        int mid = (sortedList.size()-1)/2;
        while (mid>=1 && sortedList[mid][alpha] == sortedList[mid-1][alpha]) {
            --mid;
        }
        kDTreeNode* newRoot = new kDTreeNode(sortedList[mid], labelList[mid]);
        newRoot->left = buildTreeLabelWithParams(vector<vector<int>> (sortedList.begin(), sortedList.begin() + mid), vector<int> (labelList.begin(), labelList.begin() + mid), level + 1);
        newRoot->right = buildTreeLabelWithParams(vector<vector<int>> (sortedList.begin()+ mid + 1, sortedList.end()), vector<int> (labelList.begin() + mid + 1, labelList.end()), level + 1);
        return newRoot;
    }
public:
    kDTree(int k = 2) {
        this->k = k;
        this->root = nullptr;
        this->count = 0;
    };
    ~kDTree() {
        deleteTree(this->root);
        this->count = 0;
        this->k = 0;
        root = nullptr;
    };

    const kDTree &operator=(const kDTree &other){
        if (this->root != nullptr) {
            deleteTree(this->root);
        }
        this->k = other.k;
        this->root = deepCopy(other.root);
        this->count = other.count;
        return *this;
    };
    kDTree(const kDTree &other) {
        if (this->root != nullptr) {
            deleteTree(this->root);
        }
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
        if (check) {--this->count;}
    };
    bool search(const vector<int> &point) {
        return searchWithParams(root, point, 0);
    };
    void buildTree(const vector<vector<int>> &pointList) {
        root = buildTreeWithParams(pointList, 0);
    };
    void nearestNeighbour(const vector<int> &target, kDTreeNode* &best) {
        best = nullptr;
        findNearestNeighbour(root, target, best, 0);
        return;
    };
    void kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList) {
        findkNearestNeighbour(root, target, k, bestList, 0);
        return;
    };
    void buildTreeLabel(const vector<vector<int>> &pointList,const vector<int> &label) {
        this->count = pointList.size();
        this->root = buildTreeLabelWithParams(pointList, label, 0);
    };
    friend class kNN;
};

class kNN
{
private:
    int k;
    Dataset *X_train;
    Dataset *y_train;
    kDTree  kdtree;

public:
    kNN(int k = 5) : k(k){};
    void fit(Dataset &X_train, Dataset &y_train) {
        this->X_train = &X_train;
        this->y_train = &y_train;
        if (X_train.data.size()) {
            int dimension = X_train.data.front().size();
            kdtree.setK(dimension);
            vector<vector<int>> pointList;
            vector<int> label;
            for (const auto &x : X_train.data) {
                pointList.push_back(vector<int>(x.begin(), x.end()));
            }
            for (const auto &y : y_train.data) {
                label.push_back(y.front());
            }
            kdtree.buildTreeLabel(pointList, label);
        }
        return;
    };
    Dataset predict(Dataset &X_test) {
        Dataset result;
        result.columnName.push_back("label");
        vector<vector<int>> targets;
        for (const auto &x : X_test.data) {
            targets.push_back(vector<int>(x.begin(), x.end()));
        }
        for (int i=0; i<targets.size(); i++) {
            vector<kDTreeNode*> bestList;
            int arr[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            int labelMax = 0, labelIndex = 0;
            kdtree.kNearestNeighbour(targets[i], this->k, bestList);
            // cout<<"\n";
            // for (int i=0; i< int(bestList.size()); i++) {
            //     cout<<bestList[i]->label<<" ";
            // }
            // cout<<"\n";
            for (int j=0; j<bestList.size(); j++) {
                ++arr[bestList[j]->label];
                labelMax = arr[bestList[j]->label] > labelMax ? arr[bestList[j]->label] : labelMax;
            }
            for (int l=0; l<10; l++) {
                if (arr[l] == labelMax)
                {
                    labelIndex = l;
                    break;
                }
            }
            list<int> ans;
            ans.push_back(labelIndex);
            result.data.push_back(ans);
        }
        return result;
    };
    double score(const Dataset &y_test, const Dataset &y_pred) {
        int count=0;
        vector<int> test;
        vector<int> pred;
        for (const auto &y : y_test.data) {
            test.push_back(y.front());
        }
        for (const auto &y : y_pred.data) {
            pred.push_back(y.front());
        }
        for (int i=0; i<y_test.data.size(); i++) {
            if (test[i] == pred[i]) {
                ++count;
            }
        }
        return count*1.0 / y_test.data.size();
    };

};
