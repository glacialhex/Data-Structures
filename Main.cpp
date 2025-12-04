#include <iostream>
#include <functional>
#include <string>
#include "RadixTree.h"

    int main() {
        RadixTree tree;

        std::cout << "Test 1: Inserting words into Radix Tree\n";
        std::cout << "========================================\n\n";

        // Test basic insertion
        std::cout << "Inserting 'test': ";
        tree.insert("test");
        std::cout << "Done\n";

        std::cout << "Inserting 'team': ";
        tree.insert("team");
        std::cout << "Done\n";

        std::cout << "Inserting 'tea': ";
        tree.insert("tea");
        std::cout << "Done\n";

        std::cout << "Inserting 'tester': ";
        tree.insert("tester");
        std::cout << "Done\n";

        std::cout << "Inserting 'toast': ";
        tree.insert("toast");
        std::cout << "Done\n";

        std::cout << "Inserting 'toasting': ";
        tree.insert("toasting");
        std::cout << "Done\n";

        std::cout << "Inserting 'slow': ";
        tree.insert("slow");
        std::cout << "Done\n";

        std::cout << "Inserting 'slower': ";
        tree.insert("slower");
        std::cout << "Done\n";

        std::cout << "Inserting 'slowly': ";
        tree.insert("slowly");
        std::cout << "Done\n";

        std::cout << "\nTest 2: Testing edge cases\n";
        std::cout << "==========================\n\n";

        // Test edge cases
        std::cout << "Inserting 'test' again (should mark as ended): ";
        tree.insert("test");
        std::cout << "Done\n";

        std::cout << "Inserting 'te' (should split 'tea'): ";
        tree.insert("te");
        std::cout << "Done\n";

        std::cout << "Inserting 'testing' (should become child of 'test'): ";
        tree.insert("testing");
        std::cout << "Done\n";

        std::cout << "Inserting 'to' (should create partial overlap with 'toast'): ";
        tree.insert("to");
        std::cout << "Done\n";

        std::cout << "Inserting 'term' (should create partial overlap with 'team'): ";
        tree.insert("term");
        std::cout << "Done\n";

        std::cout << "\nTest 3: Testing root-level insertions\n";
        std::cout << "=====================================\n\n";

        std::cout << "Inserting 'apple' (different starting letter): ";
        tree.insert("apple");
        std::cout << "Done\n";

        std::cout << "Inserting 'application' (should be child of 'apple'): ";
        tree.insert("application");
        std::cout << "Done\n";

        std::cout << "Inserting 'ape' (partial overlap with 'apple'): ";
        tree.insert("ape");
        std::cout << "Done\n";

        std::cout << "\nAll insertions completed!\n";

        return 0;
    }


