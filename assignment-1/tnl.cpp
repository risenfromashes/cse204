#include "arraylist.h"
#include "linkedlist.h"

#include <memory>

#include "console_helper.h"

class TNLStop {

  enum StopFlagBits { RICKSHAW = 1, BUS = 1 << 1, TRAIN = 1 << 2 };

  uint8_t m_stop_flags = 0;

  void set(StopFlagBits bit, bool val) {
    if (val) {
      m_stop_flags |= bit;
    } else {
      m_stop_flags &= ~bit;
    }
  }

  bool get(StopFlagBits bit) { return m_stop_flags & bit; }

public:
  TNLStop(bool rs = false, bool bs = false, bool ts = false) {
    setRS(rs);
    setBS(bs);
    setTS(ts);
  }

  void setRS(bool val) { set(RICKSHAW, val); }
  void setBS(bool val) { set(BUS, val); }
  void setTS(bool val) { set(TRAIN, val); }

  bool isRS() { return get(RICKSHAW); };
  bool isBS() { return get(BUS); };
  bool isTS() { return get(TRAIN); };
};

template <template <typename> typename list_type>
requires ImplementsList<list_type>
class TNL {
  list_type<std::shared_ptr<TNLStop>> m_list;

public:
  void process() {
    int K, L, M, T;
    std::cin >> K;
    for (int i = 0; i < K; i++) {
      m_list.append(std::make_shared<TNLStop>(true));
    }

    std::cin >> L;
    for (int i = 0, B; i < L; i++) {
      std::cin >> B;
      m_list.moveToPos(B);
      m_list.getValue()->setBS(true);
    }

    std::cin >> M;
    for (int i = 0, T; i < M; i++) {
      std::cin >> T;
      m_list.moveToPos(T);
      m_list.getValue()->setTS(true);
    }

    std::cin >> T;
    switch (T) {
    case 1:
      task1();
      break;
    default:
      break;
    }
  }

  void task1() {
    m_list.moveToStart();
    for (int i = 0; i < m_list.length(); i++, m_list.next()) {
      auto stop = m_list.getValue();
      if (stop->isRS()) {
        std::cout << i;
      }
      if (i < m_list.length() - 1) {
        std::cout << ',';
      }
    }
    std::cout << std::endl;

    m_list.moveToStart();
    for (int i = 0; i < m_list.length(); i++, m_list.next()) {
      auto stop = m_list.getValue();
      if (stop->isBS()) {
        std::cout << i;
      }
      if (i < m_list.length() - 1) {
        std::cout << ',';
      }
    }
    std::cout << std::endl;

    m_list.moveToStart();
    for (int i = 0; i < m_list.length(); i++, m_list.next()) {
      auto stop = m_list.getValue();
      if (stop->isTS()) {
        std::cout << i;
      }
      if (i < m_list.length() - 1) {
        std::cout << ',';
      }
    }
    std::cout << std::endl;
  }
};

int main(int argc, char **argv) {
  auto list_type = selectListImplementation(argc, argv);
  if (list_type.has_value()) {
    switch (list_type.value()) {
    case ListImplementationType::LINKED_LIST:
      TNL<cse204::LinkedList>().process();
      break;
    case ListImplementationType::ARRAY_LIST:
      TNL<cse204::ArrayList>().process();
      break;
    }
  }
  return 0;
}
