TEST(test_pop_push, push_and_pop) {
  containers::queue<double> my_q;
  std::queue<double> orig_q;
  double a = 3;
  double a_1 = 5;
  double a_2 = 36;
  double a_3 = 1.000001;
  double a_4 = -0;
  double a_5 = -0.1919191919191919191919;
  double a_6 = 0.111211111;
  my_q.push(a);
  my_q.push(a_1);
  my_q.push(a_2);
  my_q.push(a_3);
  my_q.push(a_4);
  my_q.push(a_5);
  my_q.push(a_6);
  orig_q.push(a);
  orig_q.push(a_1);
  orig_q.push(a_2);
  orig_q.push(a_3);
  orig_q.push(a_4);
  orig_q.push(a_5);
  orig_q.push(a_6);
  for (auto i = 0; i < 7; ++i) {
    ASSERT_EQ(my_q.front(), orig_q.front());
    ASSERT_EQ(my_q.back(), orig_q.back());
    my_q.pop();
    orig_q.pop();
  }
}

TEST(test_constructor_1, constructor_list) {
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
  containers::queue<double> q_1(c1);
  for (auto i = c1.begin(); i != c1.end() - 1; ++i) {
    ASSERT_EQ(q_1.front(), *i);
    q_1.pop();
  }
}

TEST(test_constructor_2, constructor_copy) {
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
  containers::queue<double> q_1(c1);
  containers::queue<double> q_2(q_1);
  for (auto i = c1.begin(); i != c1.end() - 1; ++i) {
    ASSERT_EQ(q_1.front(), q_2.front());
    q_1.pop();
    q_2.pop();
  }
}

TEST(test_constructor_3, constructor_move) {
  std::initializer_list<double> c1{1, 2, 3, 4, 5};
  containers::queue<double> q_1(c1);
  containers::queue<double> q_2(std::move(q_1));
  for (auto i = c1.begin(); i != c1.end() - 1; ++i) {
    ASSERT_EQ(q_2.front(), *i);
    q_2.pop();
  }
}

TEST(test_operator, operator_eq) {
  containers::queue<double> q_1;
  containers::queue<double> q_2;
  double a = 1.3;
  double b = 2.6;
  double c = 300.1;
  q_1.push(a);
  q_1.push(b);
  q_1.push(c);
  q_2 = std::move(q_1);
  ASSERT_EQ(q_2.front(), a);
  ASSERT_EQ(q_2.empty(), 0);
  q_2.pop();
  ASSERT_EQ(q_2.front(), b);
  ASSERT_EQ(q_2.empty(), 0);
  q_2.pop();
  ASSERT_EQ(q_2.front(), c);
  ASSERT_EQ(q_2.empty(), 0);
  q_2.pop();
  ASSERT_EQ(q_2.empty(), 1);
  ASSERT_EQ(q_2.size(), 0);
}

TEST(bonus_test_1, test_emplace) {
  containers::queue<int> oq_my;
  oq_my.emplace_back(100);
  oq_my.emplace_back(320);
  ASSERT_EQ(oq_my.front(), 100);
  ASSERT_EQ(oq_my.back(), 320);
}
