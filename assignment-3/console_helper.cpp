#include "console_helper.h"

std::optional<QueueImplementationType> selectQueueImplementation(int argc,
                                                                 char **argv) {
  if (argc > 1) {
    if (std::strcmp(argv[1], "-ll") == 0 ||
        std::strcmp(argv[1], "--linkedqueue") == 0) {
      return QueueImplementationType::LINKED_QUEUE;
    } else if (std::strcmp(argv[1], "-arr") == 0 ||
               std::strcmp(argv[1], "--arrayqueue") == 0) {
      return QueueImplementationType::ARRAY_QUEUE;
    }
  }

  // show help
  std::cout << "Please provide one of the following flags:" << std::endl
            << std::endl
            << "-arr, --arrayqueuetTo test the array based "
               "implementation of the queue interface"
            << std::endl
            << "-ll, --linkedqueue\tTo test the linked list "
               "implementation of the queue interface"
            << std::endl
            << std::endl;
  return std::nullopt;
}
