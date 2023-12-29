#include <gtest/gtest.h>
#include "Node.hpp"

class NodeTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

// Test the default constructor
TEST_F(NodeTest, DefaultConstructor) {
    Node node;
    EXPECT_EQ(node.next(), &node);
    EXPECT_EQ(node.prev(), &node);
}

// Test the insert method
TEST_F(NodeTest, Insert) {
    Node a, b;
    a.insert(&b);
    EXPECT_EQ(a.next(), &b);
    EXPECT_EQ(b.prev(), &a);
    EXPECT_EQ(b.next(), &a);
}

// Test the remove method
TEST_F(NodeTest, Remove) {
    Node a, b;
    a.insert(&b);
    b.remove();
    EXPECT_EQ(b.next(), &b);
    EXPECT_EQ(b.prev(), &b);
    EXPECT_EQ(a.next(), &a);
}

// Test the splice method
TEST_F(NodeTest, Splice) {
    Node a, b, c, d;
    a.insert(&b); // a <-> b (circular)
    c.insert(&d); // c <-> d (circular)

    a.splice(&c); // Should link a to d and c to b, making the list a <-> d <-> c <-> b (circular)

    EXPECT_EQ(a.next(), &d);
    EXPECT_EQ(d.prev(), &a);
    EXPECT_EQ(d.next(), &c);
    EXPECT_EQ(c.prev(), &d);
    EXPECT_EQ(c.next(), &b);
    EXPECT_EQ(b.prev(), &c);
    EXPECT_EQ(b.next(), &a); // Since it's circular, b should now point back to a
    EXPECT_EQ(a.prev(), &b); // and a's prev should now be b
}



