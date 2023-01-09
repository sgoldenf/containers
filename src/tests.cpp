#include <array>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>

#include "containers.h"
#include "gtest/gtest.h"
#include "tests/array_test.cpp"
#include "tests/list_test.cpp"
#include "tests/map_test.cpp"
#include "tests/multiset_test.cpp"
#include "tests/queue_test.cpp"
#include "tests/set_test.cpp"
#include "tests/stack_test.cpp"
#include "tests/vector_test.cpp"

int main() {
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
