#include <fstream>
#include <iostream>
#include <unordered_map>
#include <limits>

std::unordered_map<std::string_view, std::string_view>
parse_console_args(int argc, char **argv);

void configure_io(int argc, char **argv);

void configure_io(
    const std::unordered_map<std::string_view, std::string_view> &args);

inline void skip_line(std::istream &in) {
  in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
