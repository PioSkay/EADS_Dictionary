#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
#include <time.h>
#include"Dictionary.h"

Dictionary<std::string, int> readFile(std::string filename) {
    Dictionary<std::string, int> tree;
    std::fstream file(filename);
    if (!file) {
        throw "Could not open the file!";
    }
    std::string line;
    while (getline(file, line, ' ')) {
        std::string local;
        line += ";";
        //according to the instructions I am considering only the letters
        //the thing there is that each individual block can be at some point corrupted
        //characters like: . , \ [ and so on may occure.
        //we do need to prevent ourselves from that
        for (int i = 0; i < line.size(); i++) {
            if ((line[i] >= 'a' and line[i] <= 'z') or (line[i] >= 'A' and line[i] <= 'Z')) {
                local += line[i];
            }
            else {
                if (local.size() != 0) {
                    Dictionary<std::string, int>::iterator it;
                    if (!tree.insert(local, 1, it)) {
                        *it += 1;
                    }
                    local.clear();
                }
            }
        }
    }
    return tree;
}

template<class rule>
bool getFrequency(const Dictionary<std::string, int>& d,
    std::list<Dictionary<std::string, int>::iterator>& elements,
    int max,
    bool exact,
    rule method) 
{
    if (max < 0)
        return false;
    //lets just make sure that elements is empty
    elements.clear();
    //I do prefer to execute this method because it works in O(n) where n is a number of elements
    //I am not using range based loop bacause in case of tree it in not O(n)
    //we still do need the iterators because executeForAllNodes works only if
    //it has const_iterator on the input
    d.executeForAllNodes([&elements, max, &method](auto& it)
        {
            elements.push_back(it);
            if (elements.size() > max) {
                //I am using a little trick here because we can specify the compering
                //method so min_element can find minimal and maximal element. 
                elements.remove(*std::min_element(elements.begin(), elements.end(), method));
            }
        }
    );
    //the problem with exact = false is that we do need to recalculate everything after the insertion of the elements
    //so after the insertion lest re-insert the remaining elements
    if (!exact and elements.size() > max) {
        auto& min = *std::min_element(elements.begin(), elements.end(), method);
        d.executeForAllNodes([&elements, &min](auto& it)
            {
                if (it.getKey() != min.getKey() and it.getInfo() == min.getInfo()) {
                    elements.push_back(it);
                }
            }
        );
    }
    return true;
}
void printMaxFrequency(const Dictionary<std::string, int>& d, int max, bool exact) {
    //list of elements where we are going to pass our results.
    std::list<Dictionary<std::string, int>::iterator> results;
    //the only step is to use getFrequency method.
    if (!getFrequency(d, results, max, exact,
        [](Dictionary<std::string, int>::iterator& a, Dictionary<std::string, int>::iterator& b)->bool
        {
            return a.getInfo() < b.getInfo();
        }
    )) {
        throw "Incorrect parameters!";
    }
    //according to the task at the end we do need to print the list.
    //task does not really specify if the elements do need to be sorted.
    //in the example they are thats why I am going to do it
    //the sorting is done in line 91-94 so if it's not needed just delete the lines
    results.sort([](Dictionary<std::string, int>::iterator const & a, Dictionary<std::string, int>::iterator const& b)->bool
        {
            return a.getInfo() > b.getInfo();
        });
    for (auto& it : results) {
        std::cout << it << std::endl;
    }
}
void printMinFrequency(const Dictionary<std::string, int>& d, int max, bool exact) {
    //list of elements where we are going to pass our results.
    std::list<Dictionary<std::string, int>::iterator> results;
    getFrequency(d, results, max, exact,
        [](const Dictionary<std::string, int>::iterator a, const Dictionary<std::string, int>::iterator b)->bool
        {
            //that's the trick that I have been mentioning.
            return a.getInfo() > b.getInfo();
        }
    );
    results.sort([](Dictionary<std::string, int>::iterator const& a, Dictionary<std::string, int>::iterator const& b)->bool
        {
            return a.getInfo() < b.getInfo();
        });
    for (auto& it : results) {
        std::cout << it << std::endl;
    }
}
void AVLRotations() {
    //according the the task we are supposed to create a graphical representation of the tree
    //for that I am using some additional libs SFML & MEP(this one is developed by me)
    //I ll leave .exe file of the simple project which I have made as a demo for my project
    //controls: type number, press enter, use arrows to navigate, type the same number to delete
    //I ll also include a screenshots and preorder output of the AVL to prove that rotations are ok
    //I have not made any console-graphical method because I just don't see a point of doind that
    {
        Dictionary<int, int> x;
        //right-right case
        x.insert(11, 1); x.insert(12, 1); x.insert(13, 1);
        std::cout << "expected keys: 12, 11, 13" << std::endl;
        x.preorder();
        //screenshot right-right.png
    }
    {
        Dictionary<int, int> x;
        //left-left case
        x.insert(11, 1); x.insert(10, 1); x.insert(9, 1);
        std::cout << "expected keys: 10, 9, 11" << std::endl;
        x.preorder();
        //screenshot left-left.png
    }
    {
        Dictionary<int, int> x;
        //left-right case
        x.insert(12, 1); x.insert(10, 1); x.insert(11, 1);
        std::cout << "expected keys: 11, 10, 12" << std::endl;
        x.preorder();
        //screenshot left-right.png
    }
    {
        Dictionary<int, int> x;
        //right-left case
        x.insert(10, 1); x.insert(12, 1); x.insert(11, 1);
        std::cout << "expected keys: 11, 10, 12" << std::endl;
        x.preorder();
        //screenshot right-left.png
    }
    //remove
    {
        Dictionary<int, int> x;
        x.insert(20, 1); x.insert(10, 1); x.insert(30, 1); x.insert(5, 1); x.insert(15, 1);
        //before screenshot: before1.png
        x.remove(30);
        std::cout << "expected keys: 10, 5, 20, 15" << std::endl;
        x.preorder();
        //screenshot after1.png
        //lets remove the head
        x.remove(10);
        std::cout << "expected keys: 15, 5, 20" << std::endl;
        x.preorder();
        //after1_head.png
    }
    {
        Dictionary<int, int> x;
        x.insert(50, 1); x.insert(40, 1); x.insert(60, 1); x.insert(30, 1); x.insert(45, 1); x.insert(55, 1); x.insert(10, 1);
        //before screenshot: before2.png
        x.remove(55);
        std::cout << "expected keys: 40, 30, 10, 50, 45, 60" << std::endl;
        x.preorder();
        //screenshot after2.png
    }
    {
        Dictionary<int, int> x;
        x.insert(50, 1); x.insert(40, 1); x.insert(60, 1); x.insert(55, 1);
        //before screenshot: before3.png
        x.remove(40);
        std::cout << "expected keys: 55, 50, 60" << std::endl;
        x.preorder();
        //screenshot left-left.png
    }
}
bool iterators() {
    std::cout << "---iterators." << std::endl;
    Dictionary<int, int> x;
    x.insert(10, 1); x.insert(20, 2); x.insert(30, 3); x.insert(50, 4); x.insert(40, 5); x.insert(60, 6);
    //iterators are working from the smallest to the gratest element so:
    std::cout << "1." << std::endl;
    std::cout << "Iterator test:" << std::endl;
    std::cout << "Expected: 10 20 30 40 50 60" << std::endl;
    for (Dictionary<int, int>::iterator it = x.begin(); it != x.end(); ++it) {
        std::cout << it << std::endl;
    }
    //there is also a const_iterator this is the test of the operators
    std::cout << "2." << std::endl;
    std::cout << "const_iterator test:" << std::endl;
    std::cout << "Expected: 10 20 30 40 50 60" << std::endl;
    const Dictionary<int, int> y(x);
    for (Dictionary<int, int>::iterator it = y.begin(); it != y.end(); ++it) {
        std::cout << it << std::endl;
    }
    //I have also implemented reverese iterator class which works as follows:
    std::cout << "3." << std::endl;
    std::cout << "reverese_iterator test:" << std::endl;
    std::cout << "Expected keys: 60 50 40 30 20 10" << std::endl;
    for (Dictionary<int, int>::reverse_iterator it = x.rbegin(); it != x.rend(); ++it) {
        std::cout << it << std::endl;
    }
    //const_reverse_iterator is also here
    std::cout << "4." << std::endl;
    std::cout << "const_reverese_iterator test:" << std::endl;
    std::cout << "Expected keys: 60 50 40 30 20 10" << std::endl;
    for (Dictionary<int, int>::const_reverse_iterator it = x.rbegin(); it != x.rend(); ++it) {
        std::cout << it << std::endl;
    }
    //a quite usefull thing is to have a working ++, -- with begin and end
    std::cout << "5." << std::endl;
    std::cout << "begin() and end() connectivity test & == operator" << std::endl;
    if (x.begin() == ++x.end()) {
        std::cout << "x.begin() == ++x.end() true!" << std::endl;
    }
    else {
        std::cout << "output: false" << std::endl;
        return false;
    }
    std::cout << "6." << std::endl;
    std::cout << "begin() and end() connectivity test & == operator" << std::endl;
    if (--x.begin() == x.end()) {
        std::cout << "--x.begin() == x.end() true!" << std::endl;
    }
    else {
        std::cout << "output: false" << std::endl;
        return false;
    }
    std::cout << "7." << std::endl;
    std::cout << "rbegin() and rend() connectivity test & == operator" << std::endl;
    if (x.rbegin() == ++x.rend()) {
        std::cout << "x.rbegin() == ++x.rend() true!" << std::endl;
    }
    else {
        std::cout << "output: false" << std::endl;
        return false;
    }
    std::cout << "8." << std::endl;
    std::cout << "rbegin() and rend() connectivity test & == operator" << std::endl;
    if (--x.rbegin() == x.rend()) {
        std::cout << "--x.rbegin() == x.rend() true!" << std::endl;
    }
    else {
        std::cout << "output: false" << std::endl;
        return false;
    }
    //ways of accessing the data
    std::cout << "9." << std::endl;
    std::cout << "* operator" << std::endl;
    std::cout << "*x.begin() expected: 1" << std::endl;
    try {
        std::cout << *x.begin() << std::endl;
    }
    catch (...) {
        std::cout << "output: false" << std::endl;
        return false;
    }
    //ways of accessing the data
    std::cout << "10." << std::endl;
    std::cout << "inNull functionality" << std::endl;
    std::cout << "x.end().isNull() expected: true" << std::endl;
    if (x.end().isNull()) {
        std::cout << "output: true" << std::endl;
    }
    else {
        std::cout << "output: false" << std::endl;
        return false;
    }
    return true;
}
bool basicFunctionalities() {
    std::cout << "---Basic functionalities." << std::endl;
    Dictionary<int, int> x;
    std::cout << "1." << std::endl;
    std::cout << "empty() method on an empty x" << std::endl;
    std::cout << "expected: true" << std::endl;
    if (x.empty()) {
        std::cout << "output: true" << std::endl;
    }
    else {
        std::cout << "output: false" << std::endl;
        return false;
    }
    std::cout << "2." << std::endl;
    std::cout << "inserting the node 10, 10" << std::endl;
    x.insert(10, 10);
    std::cout << "!x.empty() and x.begin().getInfo() == 10 and x.begin().getKey() and x.getHeight() == 1" << std::endl;
    std::cout << "expected: true" << std::endl;
    if (!x.empty() and x.begin().getInfo() == 10 and x.begin().getKey() and x.getHeight() == 1) {
        std::cout << "output: true" << std::endl;
    }
    else {
        std::cout << "output: false" << std::endl;
        return false;
    }
    std::cout << "3." << std::endl;
    std::cout << "removing the node 10, 10 (only existing)" << std::endl;
    if (!x.remove(10)) {
        std::cout << "output: false" << std::endl;
        return false;
    }
    std::cout << "x.empty() and x.getHeight() == 0" << std::endl;
    std::cout << "expected: true" << std::endl;
    if (x.empty() and x.getHeight() == 0) {
        std::cout << "output: true" << std::endl;
    }
    else {
        std::cout << "output: false" << std::endl;
        return false;
    }
    std::cout << "4." << std::endl;
    std::cout << "trying to find a non-existing element" << std::endl;
    std::cout << "x.find(11).isNull()" << std::endl;
    std::cout << "expected: true" << std::endl;
    if (x.find(11).isNull()) {
        std::cout << "output: true" << std::endl;
    } else {
        std::cout << "output: false" << std::endl;
        return false;
    }
    x.insert(11, 1); x.insert(12, 2); x.insert(13, 3);
    std::cout << "5." << std::endl;
    std::cout << "trying to insert an existing element" << std::endl;
    std::cout << "!x.insert(11, 32)" << std::endl;
    std::cout << "expected: true" << std::endl;
    if (!x.insert(11, 32)) {
        std::cout << "output: true" << std::endl;
    }
    else {
        std::cout << "output: false" << std::endl;
        return false;
    }
    std::cout << "6." << std::endl;
    std::cout << "trying to find an element with key 11" << std::endl;
    std::cout << "x.find(11).getInfo() == 1" << std::endl;
    std::cout << "expected: true" << std::endl;
    if (x.find(11).getInfo() == 1) {
        std::cout << "output: true" << std::endl;
    }
    else {
        std::cout << "output: false" << std::endl;
        return false;
    }
}
bool readFileTest()
{
    //the read file is going to be tested on my very own testbench files
    //obviously we can use some larger files books etc. but the most important thing here is to prove
    //the correctness of the algorithm and thats why:
    //I go have three file which have the same amount of the "words" where the word is:
    //according to the instruction
    //a set of characters containing large and small letters <a,..., z> and <A,..., Z>
    //a large letters are taken into account when compering them so Cat != cat.
    std::cout << "1. File: first_testbench.txt" << std::endl;
    //first testbench is showing a simple file containing words seperated with spaces
    Dictionary<std::string, int> a = readFile("first_testbench.txt");
    a.preorder();
    std::cout << std::endl;
    std::cout << "Warning! The order of preorder is different bacause of a different order of insertion." << std::endl;
    std::cout << "2. File: first_testbench_notperfect_mixed.txt" << std::endl;
    Dictionary<std::string, int> b = readFile("first_testbench_notperfect_mixed.txt");
    b.preorder();
    std::cout << std::endl;
    //this file shows the exact numeration of every word so it's clear to see that the algorithm is working properly
    std::cout << "3. File: first_testbench_numered.txt" << std::endl;
    Dictionary<std::string, int> c = readFile("first_testbench_numered.txt");
    c.preorder();
    std::cout << std::endl;
    if (a != c)
        return false;
    return true;
}
bool frequencyTest() 
{
    //the frequency testing is starting from an example from the lecture
    Dictionary<std::string, int> d;
    d.insert("Warsaw", 33);
    d.insert("Cracow", 20);
    d.insert("Gdansk", 10);
    d.insert("Szczecin", 10);
    std::cout << "max = 3, exact = true" << std::endl;
    printMaxFrequency(d, 3, true);
    std::cout << "max = 3, exact = false" << std::endl;
    printMaxFrequency(d, 3, false);
    //not enaught elements case
    std::cout << "max = 5, exact = false" << std::endl;
    printMaxFrequency(d, 5, false);
    //negative max
    std::cout << "max = -5, exact = false" << std::endl;
    try {
        printMaxFrequency(d, -5, false);
    }
    catch (const char* err) {
        std::cout << err;
    }
    
    return true;
}
int main()
{
    std::cout << "\n\n AVLRotations() \n\n";
    AVLRotations();
    std::cout << "\n\n basicFunctionalities() \n\n";
    if (!basicFunctionalities()) {
        std::cout << "error";
        return 0;
    }
    std::cout << "\n\n iterators() \n\n";
    if (!iterators()) {
        std::cout << "error";
        return 0;
    }
    std::cout << "\n\n frequencyTest() \n\n";
    if (!frequencyTest()) {
        std::cout << "error";
        return 0;
    }
    std::cout << "\n\n readFileTest() \n\n";
    if (!readFileTest()) {
        std::cout << "error";
        return 0;
    }
    return 0;
}
