#include "console_helper.h"

std::optional<StackImplementationType> selectStackImplementation(int argc,
                                                                 char **argv) {
  if (argc > 1) {
    if (std::strcmp(argv[1], "-ll") == 0 ||
        std::strcmp(argv[1], "--linkedstack") == 0) {
      return StackImplementationType::LINKED_STACK;
    } else if (std::strcmp(argv[1], "-arr") == 0 ||
               std::strcmp(argv[1], "--arraystack") == 0) {
      return StackImplementationType::ARRAY_STACK;
    } else if (std::strcmp(argv[1], "-arr2") == 0 ||
               std::strcmp(argv[1], "--array2stack") == 0) {
      return StackImplementationType::ARRAY2STACK;
    }
  }

  // show help
  std::cout << "Please provide one of the following flags:" << std::endl
            << std::endl
            << "-arr, --arraystacktTo test the array based "
               "implementation of the stack interface"
            << std::endl
            << "-ll, --linkedstack\tTo test the linked list "
               "implementation of the stack interface"
            << std::endl
            << "-arr2, --array2stack\t(Only Applicable for the Dishwasher "
               "problem) Test the stacks with a single array container"
            << std::endl
            << std::endl;
  return std::nullopt;
}
