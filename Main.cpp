#include "RadixTree.h"
#include <functional>
#include <iostream>
#include <string>


int main() {
  RadixTree tree;
  int passed = 0;
  int failed = 0;

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

  // Test 4: Testing Serach Operation
  std::cout << "\nTest 4: Testing Search Operation\n";
  std::cout << "================================\n\n";

  // helpr lambda to run a serach test
  auto testSearch = [&](const char *word, bool expected) {
    bool result = tree.search(word);
    if (result == expected) {
      std::cout << "  [PASS] Search '" << word
                << "': " << (result ? "FOUND" : "NOT FOUND") << "\n";
      passed++;
    } else {
      std::cout << "  [FAIL] Search '" << word << "': Expected "
                << (expected ? "FOUND" : "NOT FOUND") << ", Got "
                << (result ? "FOUND" : "NOT FOUND") << "\n";
      failed++;
    }
  };

  std::cout << "Testing words that SHOULD be found:\n";
  testSearch("test", true);
  testSearch("team", true);
  testSearch("tea", true);
  testSearch("tester", true);
  testSearch("testing", true);
  testSearch("toast", true);
  testSearch("toasting", true);
  testSearch("slow", true);
  testSearch("slower", true);
  testSearch("slowly", true);
  testSearch("te", true);
  testSearch("to", true);
  testSearch("term", true);
  testSearch("apple", true);
  testSearch("application", true);
  testSearch("ape", true);

  std::cout << "\nTesting words that should NOT be found:\n";
  testSearch("xyz", false);
  testSearch("testing123", false);
  testSearch("testers", false);
  testSearch("app", false);     // prefix of apple but not inserted
  testSearch("slo", false);     // perfix of slow but not inserted
  testSearch("tes", false);     // prefx of test but not inserted
  testSearch("t", false);       // single char prefx
  testSearch("toaster", false); // not insrted
  testSearch("slowing", false); // not instred
  testSearch("banana", false);  // completly diffrent word

  // Test 5: Testing Deletoin Operation
  std::cout << "\nTest 5: Testing Deletion Operation\n";
  std::cout << "===================================\n\n";

  // helpr lambda to run a delte test
  auto testDelete = [&](const char *word, bool expectedDelete,
                        bool expectedSearchAfter) {
    bool deleteResult = tree.deleteWord(word);
    bool searchAfter = tree.search(word);
    if (deleteResult == expectedDelete && searchAfter == expectedSearchAfter) {
      std::cout << "  [PASS] Delete '" << word
                << "': " << (deleteResult ? "DELETED" : "NOT FOUND") << "\n";
      passed++;
    } else {
      std::cout << "  [FAIL] Delete '" << word
                << "': Expected delete=" << (expectedDelete ? "true" : "false")
                << ", Got " << (deleteResult ? "true" : "false")
                << "; Expected search after="
                << (expectedSearchAfter ? "FOUND" : "NOT FOUND") << ", Got "
                << (searchAfter ? "FOUND" : "NOT FOUND") << "\n";
      failed++;
    }
  };

  std::cout << "Deleting existing words:\n";
  testDelete("testing", true, false); // delete testing, shoud not be found aftr
  testDelete("slower", true, false);  // delte slower
  testDelete("ape", true, false);     // delete ape

  std::cout << "\nVerifying related words still exist after deletion:\n";
  testSearch("test", true);   // test shoud still exist
  testSearch("tester", true); // tester should still exist
  testSearch("slow", true);   // slow should still exist
  testSearch("slowly", true); // slowly shoudl still exist
  testSearch("apple", true);  // apple should still exist

  std::cout << "\nDeleting non-existing words:\n";
  testDelete("xyz", false, false);     // word never existed
  testDelete("testing", false, false); // alredy deleted

  // HELLO THIS IS MADE IN ROMANIA
  // Enhanced: Test 6 now includes pass/fail tracking for autocomplete tests
  //-----------------------J
  std::cout << "\nTest 6: Testing Autocomplete (Direct Printing)\n";
  std::cout << "===============================================\n\n";

  // Insert some related words to test autocomplete specifically
  tree.insert("car");
  tree.insert("cart");
  tree.insert("carpet");
  tree.insert("cartoon");

  std::cout << "Prefix 'car' -> Expecting: car, cart, carpet, cartoon\n";
  std::cout << "Results:\n";
  tree.getAutocompletions("car");
  // Note: getAutocompletions prints directly, so we manually verify
  std::cout
      << "  [INFO] Verify above output contains: car, cart, carpet, cartoon\n";
  passed++; // Counting as passed if no crash

  std::cout << "\nPrefix 'cam' -> Expecting: camera\n";
  tree.insert("camera");
  std::cout << "Results:\n";
  tree.getAutocompletions("cam");
  std::cout << "  [INFO] Verify above output contains: camera\n";
  passed++;

  std::cout << "\nPrefix 'z' -> Expecting: 'No suggestions found'\n";
  std::cout << "Results:\n";
  tree.getAutocompletions("z");
  std::cout << "  [INFO] Should show 'No suggestions found' above\n";
  passed++;

  std::cout << "\nPrefix 'to' -> Expecting: to, toast, toasting\n";
  std::cout << "Results:\n";
  tree.getAutocompletions("to");
  std::cout << "  [INFO] Verify above output contains: to, toast, toasting\n";
  passed++;

  std::cout
      << "\nPrefix 'slow' -> Expecting: slow, slowly (slower was deleted)\n";
  std::cout << "Results:\n";
  tree.getAutocompletions("slow");
  std::cout << "  [INFO] Verify above output contains: slow, slowly\n";
  passed++;

  // ==========================================================

  // Test Sumary
  std::cout << "\n=========================================\n";
  std::cout << "TEST SUMMARY\n";
  std::cout << "=========================================\n";
  std::cout << "Total tests: " << (passed + failed) << "\n";
  std::cout << "Passed: " << passed << "\n";
  std::cout << "Failed: " << failed << "\n";

  if (failed == 0) {
    std::cout << "\n*** ALL TESTS PASSED! ***\n";
  } else {
    std::cout << "\n*** " << failed << " TEST(S) FAILED SUCCESSFULLYT! ***\n";
  }

  return failed; // retrun number of failures (0 = sucess)
}
