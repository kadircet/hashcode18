#include <iostream>
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
  Point start;
  Point end;
  long long earliest_start;
  long long latest_finish;
};

struct Car {
  long long first_free_time;
  Point cur_position;
  Point destionation;
  std::vector<Ride> rides;
};

class FleetScheduler {
 public:
  void ReadInput() {
    std::cin >> number_of_rows >> number_of_cols >> number_of_cars >>
        number_of_rides >> per_ride_bonus >> number_of_steps;
    cars.resize(number_of_cars);
    rides.resize(number_of_rides);
    for (Ride& ride : rides) {
      std::cin >> ride.start >> ride.end >> ride.earliest_start >>
          ride.latest_finish;
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

  return 0;
}
