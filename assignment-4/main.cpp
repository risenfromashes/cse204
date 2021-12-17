#include <fstream>
#include <iostream>

#include "bstree.h"

int main(int argc, char **argv) {
  std::string path = "../test_input.txt";
  if (argc >= 2) {
    path = argv[1];
  }
  std::ifstream in(path);

  char cmd;
  std::string param;

  cse204::BSTree<int> tree;

  while (in >> cmd >> param) {
    if (cmd == 'T') {
      if (param == "In") {
        tree.foreach ([](int k) { std::cout << k << ' '; },
                      cse204::TraversalOrder::IN_ORDER);
        std::cout << std::endl;
      } else if (param == "Pre") {
        tree.foreach ([](int k) { std::cout << k << ' '; },
                      cse204::TraversalOrder::PRE_ORDER);
        std::cout << std::endl;
      } else if (param == "Post") {
        tree.foreach ([](int k) { std::cout << k << ' '; },
                      cse204::TraversalOrder::POST_ORDER);
        std::cout << std::endl;
      }
    } else {
      int b = std::stoi(param);
      switch (cmd) {
      case 'I':
        tree.insert(b);
        std::cout << tree << std::endl;
        break;
      case 'D':
        try {
          tree.remove(b);
          std::cout << tree << std::endl;
        } catch (std::runtime_error& e) {
          std::cout << "Invalid Operation" << std::endl;
        }
        break;
      case 'F':
        std::cout << (tree.find(b) ? "True" : "False") << std::endl;
        break;
      }
    }
  }
}
