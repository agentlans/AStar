#ifndef _ASTARALGORITHM
#define _ASTARALGORITHM

#include <algorithm>
#include <functional>
#include <vector>

#include "PriorityQueue.hpp"

// Maps object of class T to a number
template <class T, class Num, class Hash = std::hash<T>> class DefaultMap {
public:
  void set(const T &x, const Num &y) { map[x] = y; }
  Num get(const T &x) {
    if (map.count(x) > 0) {
      return map[x];
    } else {
      return std::numeric_limits<Num>::max();
    }
  }
  void clear() { map.clear(); }

private:
  std::unordered_map<T, Num, Hash> map;
};

// Does A* search algorithm and saves the solution
template <class T, class Num, class Hash = std::hash<T>> class AStarSearch {
public:
  typedef std::function<bool(const T &)> Goal;
  typedef std::function<void(std::vector<T> &neighbours, const T &x)>
      Neighbours;
  typedef std::function<Num(const T &)> Heuristic;
  typedef std::function<double(const T &, const T &)> Distance;
  // Set the graph description
  void set_neighbours(const Neighbours &nn) { neighbours = nn; }
  void set_heuristic(const Heuristic &h = [](const T &x) { return 0; }) {
    heuristic = h;
  }
  void set_distance(const Distance &d) { distance = d; }
  // Set the endpoints
  void set_start(const T &start) {
    open_set.insert(start, heuristic(start));
    g_score.set(start, 0);
  }
  void set_goal(const T &g) {
    goal = [g](const T &x) { return x == g; };
  }
  // Resets the distance data
  void clear() {
    open_set.clear();
    g_score.clear();
  }
  // Actually do the algorithm
  bool solve() {
    std::vector<T> neighbourhood;
    while (!open_set.empty()) {
      auto current = open_set.pop();
      T curr = current.first;
      if (goal(curr)) {
        // Found a solution
        T el = curr;
        path = {el};
        while (come_from.count(el) > 0) {
          el = come_from[el];
          path.push_back(el);
        }
        std::reverse(path.begin(), path.end());
        // Save the length of the path
        path_length = g_score.get(curr);
        return true;
      }
      // For each neighbour, see if any shorter paths from neighbour to goal
      neighbours(neighbourhood, curr);
      for (const auto &neighbour : neighbourhood) {
        double g = g_score.get(curr) + distance(curr, neighbour);
        if (g < g_score.get(neighbour)) {
          // Found a shorter path to curr than previously
          come_from[neighbour] = curr;
          g_score.set(neighbour, g);
          // Update the priority queue
          Num new_g = g + heuristic(neighbour);
          if (open_set.contains(neighbour)) {
            open_set.update(neighbour, new_g);
          } else {
            open_set.insert(neighbour, new_g);
          }
        }
      }
    }
    return false;
  }
  // Get features of the solution
  Num get_length() { return path_length; }
  void get_path(std::vector<T> &p) { p = path; }

private:
  PriorityQueue<T, Num> open_set;
  std::unordered_map<T, T, Hash> come_from;
  DefaultMap<T, Num> g_score;

  // Path from starting node to solution
  std::vector<T> path;
  Num path_length;

  Goal goal;
  Neighbours neighbours;
  Heuristic heuristic;
  Distance distance;
};

#endif