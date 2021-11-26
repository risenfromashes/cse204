#include "console_helper.h"

std::optional<ListImplementationType> selectListImplementation(int argc,
                                                               char **argv) {
  if (argc > 1) {
    if (std::strcmp(argv[1], "-ll") == 0 ||
        std::strcmp(argv[1], "--linkedlist") == 0) {
      return ListImplementationType::LINKED_LIST;
    } else if (std::strcmp(argv[1], "-arr") == 0 ||
               std::strcmp(argv[1], "--arraylist") == 0) {
      return ListImplementationType::ARRAY_LIST;
    }
  }

  // show help
  std::cout << "Please provide one of the following flags:" << std::endl
            << std::endl
            << "-arr, --arraylist\tTo test the array based "
               "implementation of the list interface"
            << std::endl
            << "-ll, --linkedlist\tTo test the linked list "
               "implementation of the list interface"
            << std::endl
            << std::endl;
  return std::nullopt;
}
