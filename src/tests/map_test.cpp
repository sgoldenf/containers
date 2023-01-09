class MapTest : public ::testing::Test {
 protected:
  std::pair<int, std::string> p1{3, "pomodoro"};
  std::pair<int, std::string> p2{-1, "cantaloupes"};
  std::pair<int, std::string> p3{1, "focaccia"};
  std::pair<int, std::string> p4{2, "chives"};
  std::pair<int, std::string> p5{11, "chile peppers"};
  std::pair<int, std::string> p6{7, "beans"};
  std::pair<int, std::string> p7{-16, "papayas"};
  std::pair<int, std::string> p8{-12, "focaccia"};
  std::pair<int, std::string> p9{-8, "dates"};
  std::pair<int, std::string> p10{5, "cashew nut"};
  std::pair<int, std::string> p11{-15, "acorn squash"};
  std::pair<int, std::string> p12{12, "bean sauce"};
  std::pair<int, std::string> p13{-2, "mozzarella"};
  std::pair<int, std::string> p14{-17, "dried leeks"};
  std::pair<int, std::string> p15{14, "allspice"};
  std::pair<int, std::string> p16{-4, "succotash"};
  std::pair<int, std::string> p17{-5, "sazon"};
  std::pair<int, std::string> p18{-1, "marmalade"};
  std::pair<int, std::string> p19{-7, "bruschetta"};
  std::pair<int, std::string> p20{-15, "chambord"};
  std::pair<int, std::string> p21{15, "unsweetened chocolate"};
  std::pair<int, std::string> p22{13, "squid"};
  std::pair<int, std::string> p23{2, "plum tomatoes"};
  std::pair<int, std::string> p24{10, "amaretto"};
  std::pair<int, std::string> p25{7, "ginger ale"};
  std::pair<int, std::string> p26{-13, "blueberries"};
  std::pair<int, std::string> p27{1, "black olives"};
  std::pair<int, std::string> p28{-11, "ginger ale"};
  std::pair<int, std::string> p29{-11, "lettuce"};
  std::pair<int, std::string> p30{-8, "buckwheat"};
  std::pair<int, std::string> p31{-3, "sesame seeds"};
  std::pair<int, std::string> p32{-16, "swiss cheese"};
  std::pair<int, std::string> p33{12, "date sugar"};
  std::pair<int, std::string> p34{15, "Romano cheese"};
  std::pair<int, std::string> p35{7, "almond extract"};
  std::pair<int, std::string> p36{-10, "cucumbers"};
  std::pair<int, std::string> p37{-11, "Worcestershire sauce"};
  containers::map<int, std::string> map{
      p1,  p2,  p3,  p4,  p5,  p6,  p7,  p8,  p9,  p10, p11, p12, p13,
      p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26,
      p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37};
  std::map<int, std::string> std_map{
      p1,  p2,  p3,  p4,  p5,  p6,  p7,  p8,  p9,  p10, p11, p12, p13,
      p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26,
      p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37};
  void eq_map(containers::map<int, std::string> map,
              std::map<int, std::string> std_map);
};

void MapTest::eq_map(containers::map<int, std::string> map,
                     std::map<int, std::string> std_map) {
  EXPECT_EQ(map.size(), std_map.size());
  containers::map<int, std::string>::iterator i1 = map.begin();
  std::map<int, std::string>::iterator i2 = std_map.begin();
  while (i2 != std_map.end()) {
    EXPECT_EQ(*i1, *i2);
    ++i1;
    ++i2;
  }
  i1 = --(map.end());
  i2 = --(std_map.end());
  while (i2 != std_map.begin()) {
    EXPECT_EQ(*i1, *i2);
    --i1;
    --i2;
  }
  EXPECT_EQ(*i1, *i2);
}

TEST(map, default_constructor_empty_size) {
  containers::map<int, std::string> map;
  EXPECT_TRUE(map.empty());
}

TEST_F(MapTest, init_constructor_insert) { eq_map(map, std_map); }

TEST_F(MapTest, copy_constructor) {
  containers::map<int, std::string> copy(map);
  eq_map(copy, std_map);
}

TEST_F(MapTest, move_constructor_assignment) {
  containers::map<int, std::string> move1(std::move(map));
  eq_map(move1, std_map);
  containers::map<int, std::string> move2 = std::move(move1);
  eq_map(move2, std_map);
}

TEST_F(MapTest, element_access) {
  EXPECT_EQ(map.at(3), std_map.at(3));
  EXPECT_EQ(map.at(-17), std_map.at(-17));
  EXPECT_EQ(map.at(15), std_map.at(15));
  EXPECT_EQ(map[3], std_map[3]);
  EXPECT_EQ(map[50], std_map[50]);
}

TEST_F(MapTest, max_size) { EXPECT_EQ(map.max_size(), std_map.max_size()); }

TEST_F(MapTest, clear) {
  map.clear();
  EXPECT_TRUE(map.empty());
}

TEST_F(MapTest, insert) {
  std::pair<containers::map<int, std::string>::iterator, bool> res =
      map.insert(std::pair<int, std::string>(-28, "brandy"));
  EXPECT_EQ(std::get<0>(res), map.begin());
  EXPECT_TRUE(std::get<1>(res));
  res = map.insert(42, "celery seeds");
  EXPECT_EQ(std::get<0>(res), --(map.end()));
  EXPECT_TRUE(std::get<1>(res));
  res = map.insert_or_assign(29, "lemon juice");
  EXPECT_EQ(std::get<0>(res), --(--(map.end())));
  EXPECT_TRUE(std::get<1>(res));
  res = map.insert_or_assign(1, "jicama");
  EXPECT_EQ(std::get<0>(*(std::get<0>(res))), 1);
  EXPECT_EQ(std::get<1>(*(std::get<0>(res))), "jicama");
  EXPECT_FALSE(std::get<1>(res));
  std_map.insert(std::pair<int, std::string>(-28, "brandy"));
  std_map.insert_or_assign(42, "celery seeds");
  std_map.insert_or_assign(29, "lemon juice");
  std_map.insert_or_assign(1, "jicama");
  eq_map(map, std_map);
}

TEST_F(MapTest, erase) {
  map.erase(map.begin());
  std_map.erase(std_map.begin());
  map.erase(++(map.begin()));
  std_map.erase(++(std_map.begin()));
  map.erase(--(map.end()));
  std_map.erase(--(std_map.end()));
  eq_map(map, std_map);
}

TEST_F(MapTest, swap) {
  containers::map<int, std::string> empty;
  std::map<int, std::string> std_empty;
  map.swap(empty);
  std_map.swap(std_empty);
  eq_map(empty, std_empty);
  EXPECT_TRUE(map.empty());
}

TEST_F(MapTest, merge) {
  std::pair<int, std::string> pair1{17, "cantaloupes"};
  std::pair<int, std::string> pair2{3, "anchovies"};
  std::pair<int, std::string> pair3{-20, "veal"};
  std::pair<int, std::string> pair4{25, "leeks"};
  std::pair<int, std::string> pair5{0, "milk"};
  std::pair<int, std::string> pair6{8, "red snapper"};
  std::pair<int, std::string> pair7{-16, "green onions"};
  std::pair<int, std::string> pair8{-8, "artichokes"};
  std::pair<int, std::string> pair9{7, "zinfandel wine"};
  containers::map<int, std::string> map2{pair1, pair2, pair3, pair4, pair5,
                                         pair6, pair7, pair8, pair9};
  std::map<int, std::string> std_map2{pair1, pair2, pair3, pair4, pair5,
                                      pair6, pair7, pair8, pair9};
  map.merge(map2);
  std_map.merge(std_map2);
  eq_map(map, std_map);
  eq_map(map2, std_map2);
  containers::map<int, std::string> map3;
  std::map<int, std::string> std_map3;
  map3.merge(map2);
  std_map3.merge(std_map2);
  eq_map(map3, std_map3);
  EXPECT_TRUE(map2.empty());
}

TEST_F(MapTest, contains) {
  EXPECT_FALSE(map.contains(8));
  EXPECT_FALSE(map.contains(-18));
  EXPECT_TRUE(map.contains(15));
  EXPECT_TRUE(map.contains(10));
}

TEST_F(MapTest, emplace) {
  std::pair<int, std::string> pair1{17, "cantaloupes"};
  std::pair<int, std::string> pair2{3, "anchovies"};
  std::pair<int, std::string> pair3{-20, "veal"};
  std::pair<int, std::string> pair4{25, "leeks"};
  std::pair<int, std::string> pair5{0, "milk"};
  map.emplace(pair1, pair2, pair3, pair4, pair5);
  std_map.insert(pair1);
  std_map.insert(pair2);
  std_map.insert(pair3);
  std_map.insert(pair4);
  std_map.insert(pair5);
  eq_map(map, std_map);
}
