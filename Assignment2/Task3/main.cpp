#include "main.hpp"
// #include "Dataset.hpp"
#include "ID_BTL2_DSA.hpp"



/*
 * Author: Võ Tiến
 * Date: 08.05.2023
 * FB: https://www.facebook.com/profile.php?id=100056605580171
 * FB nhóm: https://www.facebook.com/groups/211867931379013
 */
/*
! cách chạy test case
* cách dùng hàm main.cpp
* b1 : chạy lệnh g++ -o main main.cpp ID_BTL2_DSA.cpp Dataset.o nếu không gì hiện ra thì không bị lỗi code
* b2 : chạy lệnh ./main
*/
















void tc1()
{
    kDTree tree(2);
    tree.insert({5, 6});
    tree.insert({2, 2});
    tree.insert({7, 3});
    tree.insert({2, 8});
    tree.insert({8, 7});
    tree.insert({8, 1});
    tree.insert({9, 4});
    tree.insert({3, 5});
    tree.preorderTraversal();
    OUTPUT << endl;
}

void tc2()
{
    kDTree tree(2);
    tree.insert({5, 6});
    tree.insert({2, 2});
    tree.insert({7, 3});
    tree.insert({2, 8});
    tree.insert({8, 7});
    tree.insert({8, 1});
    tree.insert({9, 4});
    tree.insert({3, 5});
    tree.insert({9, 2});
    tree.preorderTraversal();
    OUTPUT << endl;
}

void tc3()
{
    kDTree tree(2);
    vector<vector<int>> pointList = {{5, 6}, {2, 2}, {7, 3}, {2, 8}, {8, 7}, {8, 1}, {9, 4}, {3, 5}};
    tree.buildTree(pointList);
    tree.preorderTraversal();
    OUTPUT << endl;
}

void tc4()
{
    kDTree tree(2);
    vector<vector<int>> pointList = {{5, 6}, {2, 2}, {7, 3}, {2, 8}, {8, 7}, {8, 1}, {9, 4}, {3, 5}};
    tree.buildTree(pointList);
    OUTPUT << "nodeCount " << tree.nodeCount() << endl;
    OUTPUT << tree.search({2, 2}) << endl;
    OUTPUT << tree.search({9, 3}) << endl;
}

void tc5()
{
    kDTree tree(2);
    tree.insert({5, 6});
    tree.insert({2, 2});
    tree.insert({7, 3});
    tree.insert({2, 8});
    tree.insert({8, 1});
    tree.insert({3, 5});
    tree.insert({9, 2});
    tree.remove({5, 6});
    tree.preorderTraversal();
    OUTPUT << endl;
}

void tc6()
{
    kDTree tree(2);
    tree.insert({5, 6});
    tree.insert({2, 2});
    tree.insert({7, 3});
    tree.insert({2, 8});
    tree.insert({3, 5});
    tree.insert({8, 2});
    tree.insert({8, 7});
    tree.insert({9, 2});
    tree.insert({9, 5});
    kDTreeNode *best = nullptr;
    tree.nearestNeighbour({9, 3}, best);
    OUTPUT << "Nearest neighbour of (9, 3) is " << *best << endl;

}

void tc7()
{
    kDTree tree(2);
    tree.insert({5, 6});
    tree.insert({2, 2});
    tree.insert({7, 3});
    tree.insert({2, 8});
    tree.insert({3, 5});
    tree.insert({8, 2});
    tree.insert({8, 7});
    tree.insert({9, 2});
    tree.insert({9, 5});
    vector<kDTreeNode *> bestList;
    tree.kNearestNeighbour({9, 3}, 5, bestList);
    OUTPUT << "5 Nearest neighbour of (9, 3) are: ";
    for (auto node : bestList)
    {
        OUTPUT << *node << " ";
    }
    OUTPUT << endl;
}


void tc8()
{
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    int nRows, nCols;

    kNN knn;
    Dataset X_train, X_test, y_train, y_test;
    Dataset feature = dataset.extract(0, -1, 1, -1);
    Dataset label = dataset.extract(0, -1, 0, 0);

    train_test_split(feature, label, 0.2, X_train, X_test, y_train, y_test);
    knn.fit(X_train, y_train);
    Dataset y_pred = knn.predict(X_test);

    OUTPUT << "y_pred" << endl;
    knn.print_Y(y_pred);
    OUTPUT << "y_test" << endl;
    knn.print_Y(y_test);
}

void tc9()
{
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    int nRows, nCols;

    kNN knn;
    Dataset X_train, X_test, y_train, y_test;
    Dataset feature = dataset.extract(0, -1, 1, -1);
    Dataset label = dataset.extract(0, -1, 0, 0);

    train_test_split(feature, label, 0.2, X_train, X_test, y_train, y_test);
    knn.fit(X_train, y_train);
    Dataset y_pred = knn.predict(X_test);
    double accuracy = knn.score(y_test, y_pred);
    OUTPUT << "y_pred" << endl;
    knn.print_Y(y_pred);
    OUTPUT << "y_test" << endl;
    knn.print_Y(y_test);
    OUTPUT << "Accuracy: " << accuracy << endl;
}





bool isNumber(string str);
void comparefile();


int main(int argc, const char *argv[])
{
    OUTPUT.open("output.txt");
    if(OUTPUT.fail())
    {
        OUTPUT << "fail ouput" << endl;
        return 1;
    }
    cout << 1 << " ";
    OUTPUT << "START TEST 1 " << endl;
    tc1();
    OUTPUT << "END TEST " << "\n\n";

    cout << 2 << " ";
    OUTPUT << "START TEST 2 " << endl;
    tc2();
    OUTPUT << "END TEST " << "\n\n";

    cout << 3 << " ";
    OUTPUT << "START TEST 3 " << endl;
    tc3();
    OUTPUT << "END TEST " << "\n\n";

    cout << 4 << " ";
    OUTPUT << "START TEST 4 " << endl;
    tc4();
    OUTPUT << "END TEST " << "\n\n";

    cout << 5 << " ";
    OUTPUT << "START TEST 5 " << endl;
    tc5();
    OUTPUT << "END TEST " << "\n\n";

    cout << 6 << " ";
    OUTPUT << "START TEST 6 " << endl;
    tc6();
    OUTPUT << "END TEST " << "\n\n";

    cout << 7 << " ";
    OUTPUT << "START TEST 7 " << endl;
    tc7();
    OUTPUT << "END TEST " << "\n\n";


    cout << 8 << " ";
    OUTPUT << "START TEST 8 " << endl;
    tc8();
    OUTPUT << "END TEST " << "\n\n";

    cout << 9 << " ";
    OUTPUT << "START TEST 9 " << endl;
    tc9();
    OUTPUT << "END TEST " << "\n\n";

    comparefile();
    return 0;   
}


bool isNumber(string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

void comparefile()
{
    cout << endl;
    vector<int> result;

    ifstream read_output("output.txt");
    ifstream read_expect("expect.txt");
    if (read_output.fail() || read_expect.fail())
    {
        cout << "Error reading file" << endl;
        return;
    }
    string s1, s2;
    while (read_output >> s1 && read_expect >> s2)
    {
        if (s1 != s2)
        {
            result.push_back(0);
            break;
        }
    }

    if (result.size() == 0)
    {
        cout << "Success: test "  << endl;
    }
    else
    {
        cout << "Fail : " << endl;
        cout << "link check comparefile: https://www.diffchecker.com/text-compare/" << endl;
    }
}