class ArrayTest : public ::testing::Test {
 protected:
  containers::array<int, 5> array;
  std::array<int, 5> std_array;
  void eq_array(containers::array<int, 5>& array,
                std::array<int, 5>& std_array);
};

void ArrayTest::eq_array(containers::array<int, 5>& array,
                         std::array<int, 5>& std_array) {
  containers::array<int, 5>::iterator i1 = array.begin();
  std::array<int, 5>::iterator i2 = std_array.begin();
  while (i2 != std_array.end()) {
    EXPECT_EQ(*i1, *i2);
    ++i1;
    ++i2;
  }
  EXPECT_EQ(array.size(), std_array.size());
  EXPECT_EQ(array.front(), std_array.front());
  EXPECT_EQ(array.back(), std_array.back());
}

TEST_F(ArrayTest, default_constructor_pushback) {
  EXPECT_EQ(array.size(), std_array.size());
}

TEST_F(ArrayTest, initializer_constructor) {
  std::initializer_list<int> init{1, 2, 3, 4, 5};
  containers::array<int, 5> init_array(init);
  std::array<int, 5> std_init_array{1, 2, 3, 4, 5};
  eq_array(init_array, std_init_array);
}

TEST_F(ArrayTest, copy_constructor) {
  array.fill(0);
  std_array.fill(0);
  containers::array<int, 5> copy(array);
  std::array<int, 5> std_copy(std_array);
  eq_array(copy, std_copy);
}

TEST_F(ArrayTest, move_constructor_and_assignment) {
  array.fill(0);
  std_array.fill(0);
  containers::array<int, 5> moved(std::move(array));
  std::array<int, 5> std_moved(std::move(std_array));
  eq_array(moved, std_moved);
  containers::array<int, 5> moved2 = std::move(moved);
  std::array<int, 5> std_moved2 = std::move(std_moved);
  eq_array(moved2, std_moved2);
}

TEST_F(ArrayTest, at_access_operator) {
  array.fill(0);
  std_array.fill(0);
  EXPECT_EQ(array.at(3), std_array.at(3));
  EXPECT_EQ(array[3], std_array[3]);
}

TEST_F(ArrayTest, max_size) {
  EXPECT_EQ(array.max_size(), std_array.max_size());
}

TEST_F(ArrayTest, size) { EXPECT_EQ(array.size(), std_array.size()); }

TEST_F(ArrayTest, empty) { EXPECT_EQ(array.empty(), std_array.empty()); }

TEST_F(ArrayTest, swap) {
  array.fill(0);
  std_array.fill(0);
  std::initializer_list<int> init{5, 6, 7, 8, 9};
  containers::array<int, 5> array2(init);
  std::array<int, 5> std_array2{5, 6, 7, 8, 9};
  array.swap(array2);
  std_array.swap(std_array2);
  eq_array(array, std_array);
  eq_array(array2, std_array2);
}

TEST_F(ArrayTest, fill) {
  array.fill(3);
  std_array.fill(3);
  eq_array(array, std_array);
}
