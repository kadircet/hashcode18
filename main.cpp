#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

struct Point {
  int row;
  int col;

  friend std::istream& operator>>(std::istream& is, Point& p) {
    is >> p.row >> p.col;
    return is;
  }
};

struct Ride {
  int id;
  Point start;
  Point end;
  long long earliest_start;
  long long latest_finish;

  bool operator<(const Ride& rhs) const {
    return earliest_start < rhs.earliest_start;
  }

  int Distance() const {
    return abs(end.row - start.row) + abs(end.col - start.col);
  }
};

struct Car {
  long long first_free_time;
  Point cur_position;
  Point destionation;
  std::vector<Ride> rides;

  bool operator<(const Car& rhs) const {
    return rhs.first_free_time < first_free_time;
  }

  void ScheduleRide(const Ride& ride) {
    const int distance = ride.Distance();
    int pickup_time = ride.earliest_start;
    if (first_free_time > pickup_time) pickup_time = first_free_time;
    if (pickup_time + distance < ride.latest_finish) {
      first_free_time = pickup_time + distance;
      rides.push_back(ride);
    }
  }

  void Print() const {
    std::cout << rides.size();
    for (const Ride& ride : rides) {
      std::cout << ' ' << ride.id;
    }
    std::cout << std::endl;
  }
};

class FleetScheduler {
 public:
  void ReadInput() {
    std::cin >> number_of_rows >> number_of_cols >> number_of_cars >>
        number_of_rides >> per_ride_bonus >> number_of_steps;
    cars.resize(number_of_cars);
    rides.resize(number_of_rides);
    int i = 0;
    for (Ride& ride : rides) {
      ride.id = i++;
      std::cin >> ride.start >> ride.end >> ride.earliest_start >>
          ride.latest_finish;
    }
  }

  void AssignRides() {
    sort(rides.begin(), rides.end());
    std::priority_queue<Car> car_queue;
    for (Car& car : cars) {
      car_queue.push(car);
    }
    for (Ride& ride : rides) {
      Car car = car_queue.top();
      car_queue.pop();
      car.ScheduleRide(ride);
      car_queue.push(car);
    }
    while (!car_queue.empty()) {
      Car car = car_queue.top();
      car_queue.pop();
      car.Print();
    }
  }

 private:
  std::vector<Car> cars;
  std::vector<Ride> rides;
  int number_of_rows;
  int number_of_cols;
  int number_of_cars;
  int number_of_rides;
  int per_ride_bonus;
  long long number_of_steps;
};

int main() {
  FleetScheduler fleet_scheduler;
  fleet_scheduler.ReadInput();
  fleet_scheduler.AssignRides();

  return 0;
}
