#include "pch.h"
#include "gtest/gtest.h"
#include "B-trees.h"
#include<string>
#include<algorithm>
#include<vector>
#include <ctime>
#include <set>
#include <unordered_map>


TEST(BTreeTest, T) {
    bTree tree(5); // Assuming a B-tree of order 3
    tree.insert("01100", "a");
    tree.insert("01101", "b");
    tree.insert("01100", "c");
    tree.insert("01100", "d");

    tree.Delete("01100", "a");
    testing::internal::CaptureStdout();
    tree.inOrder();
    std::string output = testing::internal::GetCapturedStdout();
    // Define the expected output based on the provided B-tree structure
    std::string expectedOutput = "01101 b , \n";

    // Check if the actual output matches the expected output
    EXPECT_EQ(output, expectedOutput);
}

TEST(BTreeTest, T1) {
    bTree tree(3); // Assuming a B-tree of order 3
    tree.insert("123", "a");
    tree.insert("456", "b");
    tree.insert("789", "c");
    tree.insert("987", "d");

    // Set up the output stream buffer for testing
    testing::internal::CaptureStdout();

    // Perform in-order traversal and capture the output
    tree.inOrder();
    std::string output = testing::internal::GetCapturedStdout();

    // Define the expected output based on the provided B-tree structure
    std::string expectedOutput = "123 a , 456 b , 789 c , 987 d , \n";

    // Check if the actual output matches the expected output
    EXPECT_EQ(output, expectedOutput);
}

// Test the level-order traversal functionality
TEST(BTreeTest, T2) {
    bTree tree(3); // Assuming a B-tree of order 3
    tree.insert("123", "a");
    tree.insert("456", "b");
    tree.insert("789", "c");
    tree.insert("987", "d");

    // Set up the output stream buffer for testing
    testing::internal::CaptureStdout();

    // Perform level-order traversal and capture the output
    tree.levelOrderTraversal();
    std::string output = testing::internal::GetCapturedStdout();

    // Define the expected output based on the provided B-tree structure
    std::string expectedOutput = "456 \n123 789 987 \n";

    // Check if the actual output matches the expected output
    EXPECT_EQ(output, expectedOutput);
}

TEST(BTreeTest, T3) {
    bTree t2(4);
    string v = "a";

    // Insert double-digit values in increasing order
    for (int i = 10; i <= 99; i++) {
        string c = to_string(i);
        t2.insert(c, v);
    }

    // Shuffle the values for random order
    vector<int> values;
    for (int i = 10; i <= 99; i++) {
        values.push_back(i);
    }
    random_shuffle(values.begin(), values.end());

    // Create a vector containing elements from 0 to 49
    vector<int> shuffledIndices;
    for (int i = 0; i < 50; i++) {
        shuffledIndices.push_back(values[i]);
    }

    // Set up the output stream buffer for testing
    testing::internal::CaptureStdout();

    // Display initial state
    cout << "Initial state:" << endl;
    t2.inOrder();
    std::string initialInOrder = testing::internal::GetCapturedStdout();
    cout << endl;

    // Delete 50 elements randomly
    for (int i = 0; i < 50; i++) {
        string c = to_string(values[i]);
        t2.Delete(c, v);
    }

    // Set up the output stream buffer for testing
    testing::internal::CaptureStdout();

    // Display state after deletion
    t2.inOrder();
    std::string afterDeletionInOrder = testing::internal::GetCapturedStdout();
    cout << endl;

    // Generate expected in-order values after deletion dynamically
    std::string expectedInOrder;
    for (int i = 10; i <= 99; i++) {
        auto it = std::find(shuffledIndices.begin(), shuffledIndices.end(), i);
        if (it == shuffledIndices.end()) {
            // i is not present in the deleted values, include it in expectedInOrder
            expectedInOrder += to_string(i) + " a , ";
        }
    }
    expectedInOrder += "\n";

    // Check if the size of the tree has decreased after deletion
    EXPECT_LT(afterDeletionInOrder.size(), initialInOrder.size());

    // Check if the actual in-order traversal matches the expected in-order values
    EXPECT_EQ(afterDeletionInOrder, expectedInOrder);
}

TEST(BTreeTest, T4) {
    bTree t3(5);
    string v = "a";

    // Insert three-digit values in increasing order
    for (int i = 100; i <= 200; i++) {
        string c = to_string(i);
        t3.insert(c, v);
    }

    // Shuffle the values for random order
    vector<int> values;
    for (int i = 100; i <= 200; i++) {
        values.push_back(i);
    }
    random_shuffle(values.begin(), values.end());

    // Create a vector containing elements from 0 to 49
    vector<int> shuffledIndices;
    for (int i = 0; i < 50; i++) {
        shuffledIndices.push_back(values[i]);
    }

    // Set up the output stream buffer for testing
    testing::internal::CaptureStdout();

    // Display initial state
    cout << "Initial state:" << endl;
    t3.inOrder();
    std::string initialInOrder = testing::internal::GetCapturedStdout();
    cout << endl;

    // Delete 50 elements randomly
    for (int i = 0; i < 22; i++) {
        string c = to_string(values[i]);
        t3.Delete(c, v);
    }
    for (int i = 22; i < 50; i++) {
        string c = to_string(values[i]);
        t3.Delete(c, v);
    }

    // Set up the output stream buffer for testing
    testing::internal::CaptureStdout();

    // Display state after deletion
    t3.inOrder();
    std::string afterDeletionInOrder = testing::internal::GetCapturedStdout();
    cout << endl;

    // Generate expected in-order values after deletion dynamically
    std::string expectedInOrder;
    for (int i = 100; i <= 200; i++) {
        auto it = std::find(shuffledIndices.begin(), shuffledIndices.end(), i);
        if (it == shuffledIndices.end()) {
            // i is not present in the deleted values, include it in expectedInOrder
            expectedInOrder += to_string(i) + " a , ";
        }
    }
    expectedInOrder += "\n";

    // Check if the actual in-order traversal matches the expected in-order values
    EXPECT_EQ(afterDeletionInOrder, expectedInOrder);
}

TEST(BTreeTest, T5) {
    bTree t3(10);
    string v = "a";

    // Insert 200 unique random three-digit values
    srand(static_cast<unsigned>(time(0)));
    set<int> insertedValues; // To keep track of inserted values
    while (insertedValues.size() < 200) {
        int random_value = rand() % (900) + 100; // Random three-digit value
        if (insertedValues.find(random_value) == insertedValues.end()) {
            string c = to_string(random_value);
            t3.insert(c, v);
            insertedValues.insert(random_value);
        }
    }

    // Convert set to a vector for shuffling
    vector<int> values(insertedValues.begin(), insertedValues.end());
    // Shuffle the values for random order
    random_shuffle(values.begin(), values.end());

    // Create a vector containing elements from 0 to 49
    vector<int> shuffledIndices;
    for (int i = 0; i < 50; i++) {
        shuffledIndices.push_back(values[i]);
    }

    // Set up the output stream buffer for testing
    testing::internal::CaptureStdout();

    // Display initial state
    cout << "Initial state:" << endl;
    t3.inOrder();
    std::string initialInOrder = testing::internal::GetCapturedStdout();
    cout << endl;

    // Delete 50 elements randomly
    for (int i = 0; i < 50; i++) {
        string c = to_string(values[i]);
        t3.Delete(c, v);
    }

    // Set up the output stream buffer for testing
    testing::internal::CaptureStdout();

    // Display state after deletion
    t3.inOrder();
    std::string afterDeletionInOrder = testing::internal::GetCapturedStdout();
    cout << endl;

    // Generate expected in-order values after deletion dynamically
    std::string expectedInOrder;
    for (int i : insertedValues) {
        if (i < 100 || i >= 1000) {
            // Ignore values outside the three-digit range
            continue;
        }
        auto it = std::find(shuffledIndices.begin(), shuffledIndices.end(), i);
        if (it == shuffledIndices.end()) {
            // i is not present in the deleted values, include it in expectedInOrder
            expectedInOrder += to_string(i) + " a , ";
        }
    }
    expectedInOrder += "\n";

    // Check if the actual in-order traversal matches the expected in-order values
    EXPECT_EQ(afterDeletionInOrder, expectedInOrder);
}
TEST(BTreeTest, T6) {
    bTree t3(10);
    string v = "a";

    // Insert 200 random three-digit values (allowing duplicates)
    srand(42);
    vector<int> insertedValues;

    for (int i = 0; i < 50; i++) {
        int random_value = rand() % (900) + 100; // Random three-digit value
        string c = to_string(random_value);
        t3.insert(c, v);
        insertedValues.push_back(random_value);
    }
    // Shuffle the values for random order
    random_shuffle(insertedValues.begin(), insertedValues.end());

    // Create a vector containing elements from 0 to 49
    vector<int> shuffledIndices;
    for (int i = 0; i < 10; i++) {
        shuffledIndices.push_back(insertedValues[i]);
    }
    // Set up the output stream buffer for testing
    testing::internal::CaptureStdout();

    // Display initial state
    cout << "Initial state:" << endl;
    t3.inOrder();
    std::string initialInOrder = testing::internal::GetCapturedStdout();
    cout << endl;

    // Delete 50 elements randomly
    for (int i = 0; i < 10; i++) {
        string c = to_string(insertedValues[i]);
        t3.Delete(c, v);
    }


    // Set up the output stream buffer for testing
    testing::internal::CaptureStdout();

    // Display state after deletion
    t3.inOrder();
    std::string afterDeletionInOrder = testing::internal::GetCapturedStdout();
    cout << endl;

    // Generate expected in-order values after deletion dynamically
    std::string expectedInOrder;
    sort(insertedValues.begin(), insertedValues.end());

    for (int i : insertedValues) {
        if (i < 100 || i >= 1000) {
            // Ignore values outside the three-digit range
            continue;
        }
        auto it = std::find(shuffledIndices.begin(), shuffledIndices.end(), i);
        if (it == shuffledIndices.end()) {
            // i is not present in the deleted values, include it in expectedInOrder
            expectedInOrder += to_string(i) + " a , ";
        }
    }
    expectedInOrder += "\n";
    // Check if the actual in-order traversal matches the expected in-order values
    EXPECT_EQ(afterDeletionInOrder, expectedInOrder);
}

TEST(BTreeTest, T7) {
    bTree t3(10);
    string v = "a";

    // Insert 200 random three-digit values (allowing duplicates)
    srand(42);
    vector<int> insertedValues;

    for (int i = 0; i < 200; i++) {
        int random_value = rand() % (900) + 100; // Random three-digit value
        string c = to_string(random_value);

        t3.insert(c, v);
        insertedValues.push_back(random_value);
    }
    // Shuffle the values for random order
    random_shuffle(insertedValues.begin(), insertedValues.end());

    // Create a vector containing elements from 0 to 49
    vector<int> shuffledIndices;
    for (int i = 0; i < 50; i++) {
        shuffledIndices.push_back(insertedValues[i]);
    }
    // Set up the output stream buffer for testing

    // Display initial state
   /* cout << "Initial state:" << endl;
    t3.inOrder();*/

    // Delete 50 elements randomly
    for (int i = 0; i < 50; i++) {
        string c = to_string(insertedValues[i]);
        t3.Delete(c, v);
    }
    /*  t3.printTree();
      t3.inOrder();*/
      // Set up the output stream buffer for testing
    testing::internal::CaptureStdout();

    // Display state after deletion
    t3.inOrder();
    std::string afterDeletionInOrder = testing::internal::GetCapturedStdout();
    cout << endl;

    // Generate expected in-order values after deletion dynamically
    std::string expectedInOrder;
    sort(insertedValues.begin(), insertedValues.end());

    for (int i : insertedValues) {
        if (i < 100 || i >= 1000) {
            // Ignore values outside the three-digit range
            continue;
        }
        auto it = std::find(shuffledIndices.begin(), shuffledIndices.end(), i);
        if (it == shuffledIndices.end()) {
            // i is not present in the deleted values, include it in expectedInOrder
            expectedInOrder += to_string(i) + " a , ";
        }
    }
    expectedInOrder += "\n";
    // Check if the actual in-order traversal matches the expected in-order values
    EXPECT_EQ(afterDeletionInOrder, expectedInOrder);
}

TEST(BTreeTest, DeleteRootUntilNull) {
    bTree t(3);
    string v = "a";

    // Insert double-digit values in increasing order
    for (int i = 10; i <= 20; i++) {
        string c = to_string(i);
        t.insert(c, v);
    }

    // Set up the output stream buffer for testing
    testing::internal::CaptureStdout();

    // Display initial state
    cout << "Initial state:" << endl;
    t.inOrder();
    std::string initialState = testing::internal::GetCapturedStdout();
    cout << endl;

    // Delete the root repeatedly until it becomes nullptr
    while (t.get_root() != nullptr) {
        // Set up the output stream buffer for testing
        testing::internal::CaptureStdout();

        // Delete the root
        string key = t.get_root()->keys[0];
        t.Delete(key, v);

        // Display state after deletion
        t.inOrder();
        std::string afterDeletionState = testing::internal::GetCapturedStdout();
        cout << endl;

        // Check if the size of the tree has decreased after deletion
        EXPECT_LT(afterDeletionState.size(), initialState.size());

        // Update the initial state for the next iteration
        initialState = afterDeletionState;
    }

}

TEST(BTreeTest, DeleteNonExistentValues) {
    bTree t(3);
    string v = "a";

    // Insert double-digit values in increasing order
    for (int i = 10; i <= 99; i++) {
        string c = to_string(i);
        t.insert(c, v);
    }

    // Values that are not present in the tree
    vector<int> nonExistentValues = { 100, 105, 202, 500, 101 };

    // Set up the output stream buffer for testing
    testing::internal::CaptureStdout();

    // Display initial state
    t.inOrder();
    std::string initialState = testing::internal::GetCapturedStdout();
    cout << endl;

    // Delete non-existent values
    for (int i : nonExistentValues) {
        string c = to_string(i);

        // Set up the output stream buffer for testing
        testing::internal::CaptureStdout();

        // Delete the non-existent value
        t.Delete(c, v);

        // Display state after deletion
        t.inOrder();
        std::string afterDeletionState = testing::internal::GetCapturedStdout();
        cout << endl;

        // Check if the size of the tree remains the same after attempting deletion
        EXPECT_EQ(afterDeletionState, initialState);
    }
}

TEST(BTreeTest, InsertAndDelete) {
    bTree t(4);
    string v = "a";
    string valueToInsert = "42";

    // Set up the output stream buffer for testing
    testing::internal::CaptureStdout();

    // Insert a value into the tree
    cout << "Inserting value:" << endl;
    t.insert(valueToInsert, v);
    t.inOrder();
    std::string initialState = testing::internal::GetCapturedStdout();
    cout << endl;

    // Set up the output stream buffer for testing
    testing::internal::CaptureStdout();

    // Delete the inserted value
    t.Delete(valueToInsert, v);
    t.inOrder();
    std::string afterDeletionState = testing::internal::GetCapturedStdout();

    // Check if the tree is empty after deleting the value
    EXPECT_EQ(afterDeletionState, "\n");
}

int main(int argc, char** argv) {
    // Initialize Google Test
    ::testing::InitGoogleTest(&argc, argv);

    // Run the tests
    return RUN_ALL_TESTS();
}
