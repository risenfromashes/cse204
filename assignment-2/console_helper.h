#include <cstring>
#include <iostream>

#include <optional>
#include <concepts>

#include "stack.h"

enum class StackImplementationType { ARRAY_STACK, LINKED_STACK, ARRAY2STACK };

template <template <typename> typename R>
concept ImplementsStack = std::derived_from<R<int>, cse204::Stack<int>>; 

/* Selects a list implentation type based on console args */
std::optional<StackImplementationType> selectStackImplementation(int argc,
                                                                char **argv);
