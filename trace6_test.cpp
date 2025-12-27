#include <iostream>
#include "RadixTree.h"

int main() {
    RadixTree tree;
    
    tree.insert("test"); tree.insert("team"); tree.insert("tea"); tree.insert("tester");
    tree.insert("toast"); tree.insert("toasting"); tree.insert("slow"); tree.insert("slower"); tree.insert("slowly");
    
    std::cout << "After initial: tester=" << (tree.search("tester") ? "OK" : "FAIL") << "\n";
    
    tree.insert("test");
    std::cout << "After test again: tester=" << (tree.search("tester") ? "OK" : "FAIL") << "\n";
    
    tree.insert("to");
    std::cout << "After to: tester=" << (tree.search("tester") ? "OK" : "FAIL") << "\n";
    
    return 0;
}
