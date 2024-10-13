#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <queue>
#include <set>
#include <string>
#include <vector>

namespace Constants {
const size_t kNumColumns = 8;
const size_t kSizeColumn = 9;
};  // namespace Constants

class SolitaireSolver {
 private:
  struct Column {
    std::vector<int64_t> state;
    int64_t start_index;

    Column(std::vector<int64_t> stat)
        : state(stat), start_index(stat.size() - 1) {}

    friend bool operator==(const Column& col_1, const Column& col_2) {
      return col_1.state == col_2.state &&
             col_1.start_index == col_2.start_index;
    }

    friend bool operator<(const Column& col_1, const Column& col_2) {
      if (col_1.state != col_2.state) {
        return col_1.state < col_2.state;
      }
      return col_1.start_index < col_2.start_index;
    }

    std::vector<int64_t> Move() {
      if (this->start_index == -1) {
        return std::vector<int64_t>();
      }
      std::vector<int64_t> move_vec;
      size_t start = this->start_index;
      while (start != this->state.size()) {
        move_vec.push_back(state[start]);
        ++start;
      }
      this->DeleteColumn();
      return move_vec;
    }

    bool CanMove(int64_t pusher) {
      return this->state.empty() ? true : pusher < this->Back();
    }

    void CheckDelete() {
      if (this->Back() == 6 &&
          this->state.size() - this->start_index == Constants::kSizeColumn) {
        this->DeleteColumn();
      }
    }

    void Push(std::vector<int64_t> pusher) {
      for (size_t i = 0; i < pusher.size(); ++i) {
        this->state.push_back(pusher[i]);
        if (this->start_index == -1) {
          this->start_index = 0;
        }
      }
      this->CheckDelete();
    }

    int64_t Front() { return start_index == -1 ? 0 : state[start_index]; }

    int64_t Back() { return state.back(); }

    void DeleteColumn() {
      while (this->state.size() != this->start_index) {
        this->state.pop_back();
      }
      --this->start_index;
    }
  };

  std::vector<Column> columns_;
  std::set<std::vector<Column>> states;
  int64_t min = 0;
  
  int64_t DFS(std::vector<Column> columns, int64_t count_moves = 0) {
    if (this->Empty(columns)) {
      return count_moves;
    }
    if (states.find(columns) != states.end()) {
      return count_moves;
    }
    states.insert(columns);
    for (size_t i = 0; i < Constants::kNumColumns; ++i) {
      for (size_t j = 0; j < Constants::kNumColumns; ++j) {
        if (j == i) continue;
        int64_t fr = columns[j].Front();
        if (fr == 0) continue;
        if (columns[i].CanMove(fr)) {
          std::vector<Column> col = columns;
          std::vector<int64_t> stat = col[j].Move();
          col[i].Push(stat);
          ++count_moves;
          count_moves = DFS(col, count_moves);
          if (this->Empty(col) && min == 0) {
            min = count_moves;
          } else if (this->Empty(col)) {
            min = std::min(min, count_moves);
          }
        }
      }
    }
    return count_moves;
  }

 public:
  SolitaireSolver(std::vector<std::vector<int64_t>>& columns) {
    for (auto& column : columns) {
      columns_.emplace_back(column);
    }
  }

  bool Empty(std::vector<Column>& stat) const {
    for (auto column : stat) {
      if (!column.state.empty()) {
        return false;
      }
    }
    return true;
  }

  int64_t GetSolution() { DFS(columns_); return min; }
};

std::vector<std::vector<int64_t>> Input() {
  std::cout << "Введите состояние колонн (8 колонн по 9 карт)" << '\n';
  std::vector<std::vector<int64_t>> columns(
      Constants::kNumColumns, std::vector<int64_t>(Constants::kSizeColumn));
  std::string card_str;
  for (auto& column : columns) {
    for (auto& card : column) {
      std::cin >> card_str;
      if (card_str == "J") {
        card = 11;
      } else if (card_str == "Q") {
        card = 12;
      } else if (card_str == "K") {
        card = 13;
      } else if (card_str == "A") {
        card = 14;
      } else {
        card = std::stoi(card_str);
      }
    }
  }
  return columns;
}

void Print(int64_t min) {
  if (min == 0) {
    std::cout << "Выигрышный исход невозможен" << '\n';
    return;
  }
  std::cout << "Выигрышная стратегия допустима за " << min << " действий"
            << '\n';
}

int main() {
  std::vector<std::vector<int64_t>> columns = Input();
  SolitaireSolver sol(columns);
  int64_t min = sol.GetSolution();
  Print(min);
}
