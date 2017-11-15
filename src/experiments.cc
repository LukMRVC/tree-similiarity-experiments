#include "experiments.h"

int main(int argc, char** argv) {
  Timing timing;
  // Initialized Timing object
  Timing::Interval * tfib25 = timing.create_enroll("fib25");
  //start timing
  tfib25->start();
  //execute what you want to time
  int sum = 0;
  for (int i = 0; i < 1000000; ++i) {
    ++sum;
  }
  // stop timing
  tfib25->stop();
  std::cout << timing;

  parser::BracketNotationParser bnp;
  // Verify the input format before parsing.
  if (!bnp.validate_input(argv[1])) {
    std::cerr << "Incorrect format of source tree. Is the number of opening and closing brackets equal?" << std::endl;
    return -1;
  }

}
