#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include <memory>
#include <string>

class Observer;

class Subject {
  std::vector<Observer*> observers;
 public:
  void attach(Observer *o);
  void detach(Observer *o);
  void notifyObservers();
  virtual std::pair<char, std::string> getState(int x, int y) = 0;
  virtual ~Subject() = default;
};

#endif
