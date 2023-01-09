TEST(test_def_constructor_stack, def_constr_1) {
  containers::stack<double> st;
  containers::stack<double> st_1;
  containers::stack<double> st_2;
  containers::stack<double> st_3;
  containers::stack<double> st_4;
  containers::stack<double> st_5;
  containers::stack<double> st_6;
  containers::stack<double> st_7;
  containers::stack<double> st_8;
  for (auto i = 0.0; i < 100; ++i) {
    st.push(i);
    ASSERT_EQ(st.top(), i);
  }
}

TEST(test_list, list_1) {
  std::initializer_list<double> c1{0, 10, 2, 0, 0, 4, 6, 7, 100.432, 1.1234567};
  containers::stack<double> st(c1);
  for (auto i = c1.end() - 1; i != c1.begin(); --i) {
    ASSERT_EQ(st.top(), *i);
    st.pop();
  }
}

TEST(test_list, list_2) {
  std::initializer_list<double> c1{-10000000000000000000.9999999999999999999,
                                   0.1,
                                   -1,
                                   +0,
                                   -0,
                                   0.0101010101010,
                                   0.000000000001,
                                   -1000.999999999999998,
                                   -1000000.00000000,
                                   -1000000};
  containers::stack<double> st(c1);
  for (auto i = c1.end() - 1; i != c1.begin(); --i) {
    ASSERT_EQ(st.top(), *i);
    st.pop();
  }
}

TEST(test_list, list_3) {
  std::initializer_list<double> c1{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::initializer_list<double> c4(c1);

  containers::stack<double> st(c4);
  for (auto i = c4.end() - 1; i != c4.begin(); --i) {
    ASSERT_EQ(st.top(), *i);
    st.pop();
  }
}

TEST(copy_test, copy_1) {
  std::initializer_list<int> c1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  containers::stack<int> st(c1);
  containers::stack<int> st_copy(st);
  for (auto i = c1.end() - 1; i != c1.begin(); --i) {
    ASSERT_EQ(st.top(), st_copy.top());
    st.pop();
    st_copy.pop();
  }
}

TEST(copy_test, copy_2) {
  std::initializer_list<double> c1{-10000000000000000000.9999999999999999999,
                                   0.1,
                                   -1,
                                   +0,
                                   -0,
                                   0.0101010101010,
                                   0.000000000001,
                                   -1000.999999999999998,
                                   -1000000.00000000,
                                   -1000000};
  containers::stack<double> st(c1);
  containers::stack<double> st_copy(st);
  for (auto i = c1.end() - 1; i != c1.begin(); --i) {
    ASSERT_EQ(st.top(), st_copy.top());
    st.pop();
    st_copy.pop();
  }
}

TEST(test_move_constr, move_constr_1) {
  std::initializer_list<double> c1{-10000000000000000000.9999999999999999999,
                                   0.1,
                                   -1,
                                   +0,
                                   -0,
                                   0.0101010101010,
                                   0.000000000001,
                                   -1000.999999999999998,
                                   -1000000.00000000,
                                   -1000000};
  containers::stack<double> st(c1);
  containers::stack<double> st_move(std::move(st));
  for (auto i = c1.end() - 1; i != c1.begin(); --i) {
    ASSERT_EQ(st_move.top(), *i);
    st_move.pop();
  }
}

TEST(test_push_constr, push_1) {
  containers::stack<double> st;
  for (auto i = 0.0; i < 10; ++i) {
    st.push(i);
    ASSERT_EQ(st.top(), i);
  }

  for (auto j = 111.0; j < 2000; ++j) {
    st.push(j);
    ASSERT_EQ(st.top(), j);
  }
}

TEST(test_push_constr, push_2) {
  containers::stack<double> st;
  for (auto i = 0.0; i < 10; ++i) {
    st.push(i);
    ASSERT_EQ(st.top(), i);
  }

  for (auto j = 111.0; j < 2000; ++j) {
    st.push(j);
    ASSERT_EQ(st.top(), j);
  }
}

TEST(test_size, size_1) {
  containers::stack<int> st;
  for (auto i = 0; i < 1000; ++i) {
    ASSERT_EQ(st.size(), i);
    st.push(i);
  }
}

TEST(test_empty, empty_1) {
  containers::stack<int> st;
  int a = 1;
  ASSERT_EQ(st.empty(), 1);
  st.push(a);
  ASSERT_EQ(st.empty(), 0);
  st.pop();
  ASSERT_EQ(st.empty(), 1);
}

TEST(test_swap, swap_1) {
  std::initializer_list<int> c1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::initializer_list<int> c2{9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
  containers::stack<int> st_1(c1);
  containers::stack<int> st_2(c2);
  st_1.swap(st_2);
  st_2.swap(st_1);
  ASSERT_EQ(st_1.top(), *c2.begin());
  ASSERT_EQ(st_2.top(), *c1.begin());
}

TEST(test_eq, eq_1) {
  std::initializer_list<int> c1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::initializer_list<int> c2{9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
  containers::stack<int> st_1(c1);
  containers::stack<int> st_2(c2);
  st_1 = std::move(st_2);
  st_2 = std::move(st_1);
  ASSERT_EQ(st_1.top(), *c2.begin());
  ASSERT_EQ(st_2.top(), *c1.begin());
}

TEST(bonus_test, test_emplace) {  //
  containers::stack<int> st_my;

  st_my.emplace_front(100);
  ASSERT_EQ(st_my.top(), 100);
  st_my.emplace_front(200);
  ASSERT_EQ(st_my.top(), 200);
  st_my.emplace_front(1000000);
  ASSERT_EQ(st_my.top(), 1000000);
}
