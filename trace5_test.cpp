#include <iostream>
#include "RadixTree.h"

int main() {
    RadixTree tree;
    
    // Test the new order from Main.cpp
    tree.insert("test"); tree.insert("team"); tree.insert("tea"); tree.insert("tester");
    tree.insert("toast"); tree.insert("toasting"); tree.insert("slow"); tree.insert("slower"); tree.insert("slowly");
    
    std::cout << "After initial: test=" << (tree.search("test") ? "OK" : "FAIL") << ", tester=" << (tree.search("tester") ? "OK" : "FAIL") << "\n";
    
    tree.insert("test");
    std::cout << "After test again: test=" << (tree.search("test") ? "OK" : "FAIL") << "\n";
    
    tree.insert("testing");
    std::cout << "After testing: test=" << (tree.search("test") ? "OK" : "FAIL") << ", tester=" << (tree.search("tester") ? "OK" : "FAIL") << ", testing=" << (tree.search("testing") ? "OK" : "FAIL") << "\n";
    
    tree.insert("te");
    std::cout << "After te: test=" << (tree.search("test") ? "OK" : "FAIL") << ", tester=" << (tree.search("tester") ? "OK" : "FAIL") << "\n";
    
    return 0;
}
