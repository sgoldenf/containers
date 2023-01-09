class SetTest : public ::testing::Test {
 protected:
  containers::set<int> set{8,  20,  -14, -18, 1, -18, -8,  -20, -14, -12, -9,
                           15, -19, -17, -3,  7, 4,   -12, -17, -14, -20};
  std::set<int> std_set{8,  20,  -14, -18, 1, -18, -8,  -20, -14, -12, -9,
                        15, -19, -17, -3,  7, 4,   -12, -17, -14, -20};
  void eq_set(containers::set<int> set, std::set<int> std_set);
};

void SetTest::eq_set(containers::set<int> set, std::set<int> std_set) {
  EXPECT_EQ(set.size(), std_set.size());
  containers::set<int>::iterator i1 = set.begin();
  std::set<int>::iterator i2 = std_set.begin();
  while (i2 != std_set.end()) {
    EXPECT_EQ(*i1, *i2);
    ++i1;
    ++i2;
  }
  i1 = --(set.end());
  i2 = --(std_set.end());
  while (i2 != std_set.begin()) {
    EXPECT_EQ(*i1, *i2);
    --i1;
    --i2;
  }
  EXPECT_EQ(*i1, *i2);
}

TEST(set, default_constructor_empty) {
  containers::set<int> set;
  EXPECT_TRUE(set.empty());
}

TEST_F(SetTest, init_constructor_insert) { eq_set(set, std_set); }

TEST_F(SetTest, copy_constructor) {
  containers::set<int> copy(set);
  eq_set(copy, std_set);
}

TEST_F(SetTest, move_constructor_assignment) {
  containers::set<int> move1(std::move(set));
  eq_set(move1, std_set);
  containers::set<int> move2 = std::move(move1);
  eq_set(move2, std_set);
}

TEST_F(SetTest, max_size) { EXPECT_EQ(set.max_size(), std_set.max_size()); }

TEST_F(SetTest, clear) {
  set.clear();
  EXPECT_TRUE(set.empty());
}

TEST_F(SetTest, erase) {
  set.erase(set.begin());
  std_set.erase(std_set.begin());
  set.erase(++(set.begin()));
  std_set.erase(++(std_set.begin()));
  set.erase(--(set.end()));
  std_set.erase(--(std_set.end()));
  eq_set(set, std_set);
}

TEST_F(SetTest, swap) {
  containers::set<int> empty;
  std::set<int> std_empty;
  set.swap(empty);
  std_set.swap(std_empty);
  eq_set(empty, std_empty);
  EXPECT_TRUE(set.empty());
}

TEST_F(SetTest, merge) {
  std::initializer_list<int> init = {14, 8, 13, 15, 6, 7, 13, 9, 16, 8};
  containers::set<int> set2(init);
  std::set<int> std_set2(init);
  set.merge(set2);
  std_set.merge(std_set2);
  eq_set(set, std_set);
  eq_set(set2, std_set2);
  containers::set<int> set3;
  std::set<int> std_set3;
  set3.merge(set2);
  std_set3.merge(std_set2);
  eq_set(set3, std_set3);
  EXPECT_EQ(set2.empty(), std_set2.empty());
}

TEST_F(SetTest, find) {
  containers::set<int>::iterator i = set.find(10);
  EXPECT_EQ(i, set.end());
  i = set.find(20);
  EXPECT_EQ(i, --(set.end()));
  i = set.find(-20);
  EXPECT_EQ(i, set.begin());
  i = set.find(-19);
  EXPECT_EQ(i, (++(set.begin())));
}

TEST_F(SetTest, contains) {
  EXPECT_TRUE(set.contains(8));
  EXPECT_TRUE(set.contains(-18));
  EXPECT_TRUE(set.contains(15));
  EXPECT_FALSE(set.contains(10));
}

TEST_F(SetTest, emplace) {
  set.emplace(0, 1, 2, 3, 4);
  for (int i = 0; i < 5; ++i) std_set.insert(i);
  eq_set(set, std_set);
}
