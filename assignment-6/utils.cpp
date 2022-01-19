#include "utils.h"

std::unordered_map<std::string_view, std::string_view>
parse_console_args(int argc, char **argv) {
  std::unordered_map<std::string_view, std::string_view> args;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (i + 1 < argc) {
        args.emplace(argv[i] + 1 + (argv[i][1] == '-'), argv[i + 1]);
        if (argv[i + 1][0] == '-') {
          args.emplace(argv[i] + 1 + (argv[i][1] == '-'), "true");
        }
      } else {
        args.emplace(argv[i] + 1 + (argv[i][1] == '-'), "true");
      }
    }
  }
  return args;
}

void configure_io(
    const std::unordered_map<std::string_view, std::string_view> &args) {
  std::string file_in = "input.txt";
  std::string file_out = "output.txt";

  if (args.contains("o")) {
    file_out = args.at("o");
  }
  if (args.contains("i")) {
    file_in = args.at("i");
  }

  if (file_in != "console") {
    static std::ifstream in(file_in);
    std::cin.rdbuf(in.rdbuf());
  }
  if (file_out != "console") {
    static std::ofstream out(file_out);
    std::cout.rdbuf(out.rdbuf());
  }
}

void configure_io(int argc, char **argv) {
  configure_io(parse_console_args(argc, argv));
}
