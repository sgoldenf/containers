class ListTest : public ::testing::Test {
 protected:
  containers::list<int> list;
  std::list<int> std_list;
  containers::list<int>::iterator i1;
  std::list<int>::const_iterator i2;
  void SetUp() {
    for (int i = 0; i < 5; ++i) {
      list.push_back(i);
      std_list.push_back(i);
    }
    i1 = list.begin();
    i2 = std_list.begin();
  }
  void eq_list(const containers::list<int>& list,
               const std::list<int>& std_list);
  void eq_from_end(const containers::list<int>& list,
                   const std::list<int>& std_list);
};

void ListTest::eq_list(const containers::list<int>& list,
                       const std::list<int>& std_list) {
  EXPECT_EQ(list.size(), std_list.size());
  EXPECT_EQ(list.front(), std_list.front());
  EXPECT_EQ(list.back(), std_list.back());
  i1 = list.begin();
  i2 = std_list.begin();
  while (i2 != std_list.end()) {
    EXPECT_EQ(*i1, *i2);
    ++i1;
    ++i2;
  }
}

void ListTest::eq_from_end(const containers::list<int>& list,
                           const std::list<int>& std_list) {
  i1 = list.end();
  i2 = std_list.end();
  if (std_list.begin() != std_list.end()) {
    --i1;
    --i2;
    while (i2 != std_list.begin()) {
      EXPECT_EQ(*i1, *i2);
      --i1;
      --i2;
    }
    EXPECT_EQ(*i1, *i2);
  }
}

TEST_F(ListTest, default_and_parameterized_constructors_size) {
  containers::list<int> l1(3);
  std::list<int> l2(3);
  eq_list(l1, l2);
  eq_from_end(l1, l2);
}

TEST_F(ListTest, initializer_list_constructor_begin_end_front_back) {
  std::initializer_list<int> init{1, 2, 3, 4};
  containers::list<int> l1(init);
  std::list<int> l2(init);
  eq_list(l1, l2);
  eq_from_end(l1, l2);
}

TEST_F(ListTest, copy_constructor) {
  containers::list<int> copy(list);
  std::list<int> std_copy(std_list);
  eq_list(copy, std_copy);
  eq_from_end(copy, std_copy);
}

TEST_F(ListTest, move_constructor_and_assignment) {
  containers::list<int> moved1(std::move(list));
  containers::list<int>::iterator i = moved1.begin();
  eq_list(moved1, std_list);
  eq_from_end(moved1, std_list);
  containers::list<int> moved2 = std::move(moved1);
  eq_list(moved2, std_list);
  eq_from_end(moved2, std_list);
}

TEST_F(ListTest, empty_clear) {
  EXPECT_FALSE(list.empty());
  list.clear();
  EXPECT_TRUE(list.empty());
}

TEST_F(ListTest, insert) {
  ++i1;
  ++i2;
  i1 = list.insert(i1, 5);
  i2 = std_list.insert(i2, 5);
  EXPECT_EQ(*i1, *i2);
  eq_list(list, std_list);
  eq_from_end(list, std_list);
  list.clear();
  std_list.clear();
  i1 = list.insert(list.begin(), 1);
  i2 = std_list.insert(std_list.begin(), 1);
  EXPECT_EQ(*i1, *i2);
  i1 = list.insert(list.end(), 2);
  i2 = std_list.insert(std_list.end(), 2);
  EXPECT_EQ(*i1, *i2);
  eq_list(list, std_list);
  eq_from_end(list, std_list);
}

TEST_F(ListTest, erase) {
  ++i1;
  ++i2;
  list.erase(i1);
  std_list.erase(i2);
  list.erase(list.begin());
  std_list.erase(std_list.begin());
  list.erase(--(list.end()));
  std_list.erase(--(std_list.end()));
  eq_list(list, std_list);
  eq_from_end(list, std_list);
  list.clear();
  std_list.clear();
  list.push_back(1);
  std_list.push_back(1);
  list.erase(list.begin());
  std_list.erase(std_list.begin());
  eq_list(list, std_list);
  eq_from_end(list, std_list);
}

TEST_F(ListTest, push_back) {
  list.push_back(5);
  std_list.push_back(5);
  eq_list(list, std_list);
  eq_from_end(list, std_list);
  list.clear();
  std_list.clear();
  list.push_back(1);
  std_list.push_back(1);
  list.push_back(2);
  std_list.push_back(2);
  eq_list(list, std_list);
  eq_from_end(list, std_list);
}

TEST_F(ListTest, pop_back) {
  while (!(list.empty())) list.pop_back();
  EXPECT_TRUE(list.empty());
}

TEST_F(ListTest, push_front) {
  list.push_front(5);
  std_list.push_front(5);
  eq_list(list, std_list);
  eq_from_end(list, std_list);
  list.clear();
  std_list.clear();
  list.push_front(1);
  std_list.push_front(1);
  list.push_front(2);
  std_list.push_front(2);
  eq_list(list, std_list);
  eq_from_end(list, std_list);
}

TEST_F(ListTest, pop_front) {
  while (!(list.empty())) list.pop_back();
  EXPECT_TRUE(list.empty());
}

TEST_F(ListTest, swap) {
  std::initializer_list<int> init{1, 2, 3, 4};
  containers::list<int> list2(init);
  std::list<int> std_list2(init);
  list.swap(list2);
  eq_list(list, std_list2);
  eq_from_end(list, std_list2);
  eq_list(list2, std_list);
  eq_from_end(list2, std_list);
}

TEST_F(ListTest, merge) {
  std::initializer_list<int> init1{3, 1, 7, 6, 4, 5, 2};
  std::initializer_list<int> init2{6, 1, 5, 1, 1, 9, -2, -1};
  containers::list<int> list1(init1);
  std::list<int> std_list1(init1);
  containers::list<int> list2(init2);
  std::list<int> std_list2(init2);
  list1.sort();
  list2.sort();
  std_list1.sort();
  std_list2.sort();
  list1.merge(list2);
  std_list1.merge(std_list2);
  eq_list(list1, std_list1);
  eq_from_end(list1, std_list1);
  EXPECT_TRUE(list2.empty());
  EXPECT_TRUE(std_list2.empty());
  list2.merge(list1);
  eq_list(list2, std_list1);
  eq_from_end(list2, std_list1);
  EXPECT_TRUE(list1.empty());
}

TEST_F(ListTest, splice) {
  std::initializer_list<int> init{2, 3, 6, 4, 3};
  containers::list<int> list2(init);
  std::list<int> std_list2(init);
  containers::list<int> list3(list2);
  std::list<int> std_list3(std_list2);
  list.splice(++(list.cbegin()), list2);
  std_list.splice(++(std_list.cbegin()), std_list2);
  list.splice(list.cbegin(), list3);
  std_list.splice(std_list.cbegin(), std_list3);
  eq_list(list, std_list);
  eq_from_end(list, std_list);
}

TEST_F(ListTest, reverse) {
  list.reverse();
  std_list.reverse();
  eq_list(list, std_list);
  eq_from_end(list, std_list);
}

TEST_F(ListTest, unique) {
  std::initializer_list<int> init{1, 2, 2, 3, 3, 2, 1, 1, 2};
  containers::list<int> l(init);
  std::list<int> std_l(init);
  l.unique();
  std_l.unique();
  eq_list(l, std_l);
  eq_from_end(l, std_l);
}

TEST_F(ListTest, sort) {
  std::initializer_list<int> init{2, 3, 6, 4, 3};
  containers::list<int> l(init);
  std::list<int> std_l(init);
  list.sort();
  std_list.sort();
  eq_list(l, std_l);
  eq_from_end(l, std_l);
}

TEST_F(ListTest, max_size) { EXPECT_EQ(list.max_size(), std_list.max_size()); }

TEST_F(ListTest, emplace) {
  containers::list<int>::iterator i =
      list.emplace((++(list.cbegin())), 5, 6, 7);
  EXPECT_EQ(*i, 5);
  std::list<int> test = {-3, -2, -1, 0, 5, 6, 7, 1, 2, 3, 4, 8, 9};
  list.emplace_back(8, 9);
  list.emplace_front(-3, -2, -1);
  ;
  eq_list(list, test);
  eq_from_end(list, test);
}
