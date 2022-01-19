#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <stack>
#include <vector>

#include "utils.h"

constexpr int inf = std::numeric_limits<int>::max();

int main(int argc, char **argv) {
  configure_io(argc, argv);
  int t;
  std::cin >> t;
  while (t--) {
    int n_board, n_dice, n_ladder, n_snake;

    std::cin >> n_dice >> n_board;

    std::vector<int> jumps(n_board + 1, 0);

    std::cin >> n_ladder;
    for (int i = 0; i < n_ladder; i++) {
      int from, to;
      std::cin >> from >> to;
      jumps[from] = to;
    }

    std::cin >> n_snake;
    for (int i = 0; i < n_snake; i++) {
      int from, to;
      std::cin >> from >> to;
      jumps[from] = to;
    }

    struct SquareInfo {
      int n_shortest_path; // number of moves in shortest path
      int prev_square;     // immediate last square in shortest path
    };

    struct GameState {
      int n_roll;    // number of rolls
      int board_pos; // index in board
    };

    std::vector<SquareInfo> squares(n_board + 1, {inf, 0});
    squares[1] = {0, 0};

    std::queue<GameState> bfs_queue;
    bfs_queue.push({0, 1});

    while (!bfs_queue.empty()) {
      auto state = bfs_queue.front();
      bfs_queue.pop();

      for (int i = 1; i <= n_dice; i++) {
        int next = state.board_pos + i;
        if (next <= n_board &&
            (state.n_roll + 1) < squares[next].n_shortest_path) {
          squares[next] = {state.n_roll + 1, state.board_pos};

          if (!jumps[next]) {
            bfs_queue.push({state.n_roll + 1, next});
          } else {
            while (jumps[next]) { // a ladder or a snake
              int dest = jumps[next];
              if ((state.n_roll + 1) < squares[dest].n_shortest_path) {
                squares[dest] = {state.n_roll + 1, next};
                bfs_queue.push({state.n_roll + 1, dest});
              }
              next = dest;
            }
          }
        }
      }
    }

    if (squares[n_board].n_shortest_path < inf) {

      std::cout << squares[n_board].n_shortest_path << std::endl;
      std::stack<int> shortest_path;

      shortest_path.push(n_board);
      while (squares[shortest_path.top()].prev_square > 0) {
        assert(squares[shortest_path.top()].prev_square != shortest_path.top());
        shortest_path.push(squares[shortest_path.top()].prev_square);
      }

      while (!shortest_path.empty()) {
        std::cout << shortest_path.top();
        shortest_path.pop();
        if (!shortest_path.empty()) {
          std::cout << " -> ";
        }
      }
      std::cout << std::endl;

    } else {
      std::cout << -1 << std::endl;
      std::cout << "No solution" << std::endl;
    }

    bool allreachable = true;
    for (int i = 1; i <= n_board; i++) {
      if (squares[i].n_shortest_path == inf) {
        if (allreachable) {
          allreachable = false;
        }
        std::cout << i << ' ';
      }
    }

    if (allreachable) {
      std::cout << "All reachable" << std::endl;
    } else {
      std::cout << std::endl;
    }

    std::cout << std::endl;
  }
}
