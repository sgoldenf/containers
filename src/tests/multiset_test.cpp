class MultisetTest : public ::testing::Test {
 protected:
  containers::multiset<int> multiset{8,   20,  -14, -18, 1,   -18, -8,
                                     -20, -14, -12, -9,  15,  -19, -17,
                                     -3,  7,   4,   -12, -17, -14, -20};
  std::multiset<int> std_multiset{8,   20,  -14, -18, 1,   -18, -8,
                                  -20, -14, -12, -9,  15,  -19, -17,
                                  -3,  7,   4,   -12, -17, -14, -20};
  void eq_set(containers::multiset<int> multiset,
              std::multiset<int> std_multiset);
};

void MultisetTest::eq_set(containers::multiset<int> multiset,
                          std::multiset<int> std_multiset) {
  EXPECT_EQ(multiset.size(), std_multiset.size());
  containers::multiset<int>::iterator i1 = multiset.begin();
  std::multiset<int>::iterator i2 = std_multiset.begin();
  while (i2 != std_multiset.end()) {
    EXPECT_EQ(*i1, *i2);
    ++i1;
    ++i2;
  }
  i1 = --(multiset.end());
  i2 = --(std_multiset.end());
  while (i2 != std_multiset.begin()) {
    EXPECT_EQ(*i1, *i2);
    --i1;
    --i2;
  }
  EXPECT_EQ(*i1, *i2);
}

TEST(multiset, default_constructor_empty) {
  containers::multiset<int> multiset;
  EXPECT_TRUE(multiset.empty());
}

TEST_F(MultisetTest, init_constructor_insert) {
  eq_set(multiset, std_multiset);
}

TEST_F(MultisetTest, copy_constructor) {
  containers::multiset<int> copy(multiset);
  eq_set(copy, std_multiset);
}

TEST_F(MultisetTest, move_constructor_assignment) {
  containers::multiset<int> move1(std::move(multiset));
  eq_set(move1, std_multiset);
  containers::multiset<int> move2 = std::move(move1);
  eq_set(move2, std_multiset);
}

TEST_F(MultisetTest, max_size) {
  EXPECT_EQ(multiset.max_size(), std_multiset.max_size());
}

TEST_F(MultisetTest, clear) {
  multiset.clear();
  EXPECT_TRUE(multiset.empty());
}

TEST_F(MultisetTest, erase) {
  multiset.erase(multiset.begin());
  std_multiset.erase(std_multiset.begin());
  multiset.erase(++(multiset.begin()));
  std_multiset.erase(++(std_multiset.begin()));
  multiset.erase(--(multiset.end()));
  std_multiset.erase(--(std_multiset.end()));
  eq_set(multiset, std_multiset);
}

TEST_F(MultisetTest, swap) {
  containers::multiset<int> empty;
  std::multiset<int> std_empty;
  multiset.swap(empty);
  std_multiset.swap(std_empty);
  eq_set(empty, std_empty);
  EXPECT_TRUE(multiset.empty());
}

TEST_F(MultisetTest, merge) {
  std::initializer_list<int> init = {14, 8, 13, 15, 6, 7, 13, 9, 16, 8};
  containers::multiset<int> multiset2(init);
  std::multiset<int> std_multiset2(init);
  multiset.merge(multiset2);
  std_multiset.merge(std_multiset2);
  eq_set(multiset, std_multiset);
  EXPECT_EQ(multiset2.empty(), std_multiset2.empty());
  multiset2.merge(multiset);
  std_multiset2.merge(std_multiset);
  eq_set(multiset2, std_multiset2);
  EXPECT_EQ(multiset.empty(), std_multiset.empty());
}

TEST_F(MultisetTest, count) {
  EXPECT_EQ(multiset.count(-14), std_multiset.count(-14));
}

TEST_F(MultisetTest, find) {
  containers::multiset<int>::iterator i = multiset.find(10);
  EXPECT_EQ(i, multiset.end());
  i = multiset.find(20);
  EXPECT_EQ(i, --(multiset.end()));
  i = multiset.find(-20);
  EXPECT_EQ(i, multiset.begin());
  i = multiset.find(-19);
  containers::multiset<int>::iterator test = ++(multiset.begin());
  ++test;
  EXPECT_EQ(i, test);
}

TEST_F(MultisetTest, contains) {
  EXPECT_TRUE(multiset.contains(8));
  EXPECT_TRUE(multiset.contains(-18));
  EXPECT_TRUE(multiset.contains(15));
  EXPECT_FALSE(multiset.contains(10));
}

TEST_F(MultisetTest, equal_range_bound) {
  containers::multiset<int>::iterator i1 = multiset.find(-14);
  containers::multiset<int>::iterator i2 = multiset.find(-12);
  std::pair<containers::multiset<int>::iterator,
            containers::multiset<int>::iterator>
      pair = multiset.equal_range(-14);
  EXPECT_EQ(i1, std::get<0>(pair));
  EXPECT_EQ(i2, std::get<1>(pair));
  containers::multiset<int>::iterator low = multiset.lower_bound(-14);
  containers::multiset<int>::iterator up = multiset.upper_bound(-14);
  EXPECT_EQ(i1, low);
  EXPECT_EQ(i2, up);
}

TEST_F(MultisetTest, emplace) {
  multiset.emplace(0, 1, 2, 3, 4);
  for (int i = 0; i < 5; ++i) std_multiset.insert(i);
  eq_set(multiset, std_multiset);
}
