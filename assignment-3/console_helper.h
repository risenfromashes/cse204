#include <cstring>
#include <iostream>

#include <concepts>
#include <optional>

#include "queue.h"

enum class QueueImplementationType { ARRAY_QUEUE, LINKED_QUEUE };


/* Selects a list implentation type based on console args */
std::optional<QueueImplementationType> selectQueueImplementation(int argc,
                                                                 char **argv);
