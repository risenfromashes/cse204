#include <cstring>
#include <iostream>

#include <optional>
#include <concepts>

#include "list.h"

enum class ListImplementationType { ARRAY_LIST, LINKED_LIST };

template <template <typename> typename R>
concept ImplementsList = std::derived_from<R<int>, cse204::List<int>>; 

/* Selects a list implentation type based on console args */
std::optional<ListImplementationType> selectListImplementation(int argc,
                                                                char **argv);
