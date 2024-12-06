#include <vector>
#include <iostream>
#include "driver.cpp"

#define TEST_ITERATIONS (1024*256)

int main() {
  vector<int> result = test_driver(TEST_ITERATIONS);

  cout << "histogram of different observations:" << endl;
  cout << "  condition: " << OUTPUT_0 << " result: " << result[0] << endl;
  cout << "  condition: " << OUTPUT_1 << " result: " << result[1] << endl;
  cout << "  condition: " << OUTPUT_2 << " result: " << result[2] << endl;
  cout << "  condition: " << OUTPUT_3 << " result: " << result[3] << endl;
  cout << "relaxed behavior frequency: " << float(result[3])/float(TEST_ITERATIONS) << endl;

  return 0;

}
