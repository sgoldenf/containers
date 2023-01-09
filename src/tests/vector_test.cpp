class VectorTest : public ::testing::Test {
 protected:
  containers::vector<int> vector;
  std::vector<int> std_vector;
  containers::vector<int>::iterator i1;
  std::vector<int>::iterator i2;
  void SetUp() {
    for (int i = 0; i < 5; ++i) {
      vector.push_back(i);
      std_vector.push_back(i);
    }
  }
  void eq_vector(containers::vector<int>& vector, std::vector<int>& std_vector);
};

void VectorTest::eq_vector(containers::vector<int>& vector,
                           std::vector<int>& std_vector) {
  i1 = vector.begin();
  i2 = std_vector.begin();
  while (i2 != std_vector.end()) {
    EXPECT_EQ(*i1, *i2);
    ++i1;
    ++i2;
  }
  EXPECT_EQ(vector.size(), std_vector.size());
  EXPECT_EQ(vector.capacity(), std_vector.capacity());
  if (!(std_vector.empty())) {
    EXPECT_EQ(vector.front(), std_vector.front());
    EXPECT_EQ(vector.back(), std_vector.back());
  }
}

TEST_F(VectorTest, default_constructor_pushback) {
  eq_vector(vector, std_vector);
}

TEST_F(VectorTest, parameterized_constructor) {
  containers::vector<int> param_vector(5);
  std::vector<int> std_param_vector(5);
  eq_vector(param_vector, std_param_vector);
}

TEST_F(VectorTest, initializer_constructor) {
  std::initializer_list<int> init{1, 2, 3, 4, 5};
  containers::vector<int> init_vector(init);
  std::vector<int> std_init_vector(init);
  eq_vector(init_vector, std_init_vector);
}

TEST_F(VectorTest, copy_constructor) {
  containers::vector<int> copy(vector);
  std::vector<int> std_copy(std_vector);
  eq_vector(copy, std_copy);
}

TEST_F(VectorTest, move_constructor_and_assignment) {
  containers::vector<int> moved(std::move(vector));
  std::vector<int> std_moved(std::move(std_vector));
  eq_vector(moved, std_moved);
  containers::vector<int> moved2 = std::move(moved);
  std::vector<int> std_moved2 = std::move(std_moved);
  eq_vector(moved2, std_moved2);
}

TEST_F(VectorTest, at_access_operator) {
  EXPECT_EQ(vector.at(3), std_vector.at(3));
  EXPECT_EQ(vector[3], std_vector[3]);
}

TEST_F(VectorTest, data) {
  int* data = vector.data();
  int* std_data = std_vector.data();
  for (int i = 0; i < std_vector.size(); ++i) EXPECT_EQ(data[i], std_data[i]);
}

TEST_F(VectorTest, empty) {
  containers::vector<int> vector2;
  EXPECT_TRUE(vector2.empty());
  vector2.push_back(1);
  EXPECT_FALSE(vector2.empty());
  EXPECT_FALSE(vector.empty());
}

TEST_F(VectorTest, reserve) {
  vector.reserve(4);
  std_vector.reserve(4);
  eq_vector(vector, std_vector);
  vector.reserve(10);
  std_vector.reserve(10);
  eq_vector(vector, std_vector);
}

TEST_F(VectorTest, shrink_to_fit) {
  vector.shrink_to_fit();
  std_vector.shrink_to_fit();
  eq_vector(vector, std_vector);
}

TEST_F(VectorTest, clear) {
  vector.clear();
  EXPECT_TRUE(vector.empty());
  std_vector.clear();
  EXPECT_EQ(vector.size(), std_vector.size());
  EXPECT_EQ(vector.capacity(), std_vector.capacity());
}

TEST_F(VectorTest, insert) {
  containers::vector<int>::iterator i1 = vector.insert(++(vector.begin()), 5);
  std::vector<int>::iterator i2 = std_vector.insert(++(std_vector.begin()), 5);
  EXPECT_EQ(*i1, *i2);
  eq_vector(vector, std_vector);
  vector.shrink_to_fit();
  std_vector.shrink_to_fit();
  i1 = vector.insert(++(vector.begin()), 6);
  i2 = std_vector.insert(++(std_vector.begin()), 6);
  EXPECT_EQ(*i1, *i2);
  i1 = vector.insert(vector.end(), 7);
  i2 = std_vector.insert(std_vector.end(), 7);
  EXPECT_EQ(*i1, *i2);
  eq_vector(vector, std_vector);
}

TEST_F(VectorTest, pop_back) {
  while (!(std_vector.empty())) {
    vector.pop_back();
    std_vector.pop_back();
  }
  eq_vector(vector, std_vector);
}

TEST_F(VectorTest, erase) {
  vector.erase(vector.begin());
  std_vector.erase(std_vector.begin());
  eq_vector(vector, std_vector);
  vector.erase(++(vector.begin()));
  std_vector.erase(++(std_vector.begin()));
  eq_vector(vector, std_vector);
  vector.erase(--(vector.end()));
  std_vector.erase(--(std_vector.end()));
  eq_vector(vector, std_vector);
}

TEST_F(VectorTest, swap) {
  std::initializer_list<int> init{5, 6, 7, 8};
  containers::vector<int> vector2(init);
  std::vector<int> std_vector2(init);
  vector.swap(vector2);
  std_vector.swap(std_vector2);
  eq_vector(vector, std_vector);
  eq_vector(vector2, std_vector2);
}

TEST_F(VectorTest, max_size) {
  EXPECT_EQ(vector.max_size(), std_vector.max_size());
}

TEST_F(VectorTest, emplace) {
  containers::vector<int>::iterator i =
      vector.emplace((++(vector.cbegin())), 5, 6, 7);
  EXPECT_EQ(*i, 5);
  vector.emplace_back(8, 9);
  std_vector.insert((++(std_vector.cbegin())), 7);
  std_vector.insert((++(std_vector.cbegin())), 6);
  std_vector.insert((++(std_vector.cbegin())), 5);
  std_vector.push_back(8);
  std_vector.push_back(9);
  eq_vector(vector, std_vector);
}
