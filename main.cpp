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

  int operator-(const Point& rhs) const {
    return abs(rhs.row - row) + abs(rhs.col - col);
  }
};

struct Ride {
  int id;
  Point start;
  Point end;
  long long earliest_start;
  long long latest_finish;

  bool operator<(const Ride& rhs) const {
    if (earliest_start == rhs.earliest_start) {
      return Distance() > rhs.Distance();
    }
    return earliest_start < rhs.earliest_start;
  }

  int Distance() const {
    return abs(end.row - start.row) + abs(end.col - start.col);
  }
};

struct Car {
  long long first_free_time;
  long long score;
  Point cur_position;
  std::vector<Ride> rides;

  bool operator<(const Car& rhs) const {
    return rhs.first_free_time < first_free_time;
  }

  int CanScheduleRide(const Ride& ride) {
    const int distance = ride.Distance();
    int pickup_time = first_free_time + (cur_position - ride.start);
    long long wasted_time = 99999999999;
    if (ride.earliest_start >= pickup_time) {
      wasted_time = ride.earliest_start - pickup_time;
      pickup_time = ride.earliest_start;
    }
    if (pickup_time + distance < ride.latest_finish) {
      return wasted_time;
    }
    return -2;
  }

  bool ScheduleRide(const Ride& ride) {
    const int distance = ride.Distance();
    int pickup_time = first_free_time + (cur_position - ride.start);
    int wasted_time = 0;
    if (ride.earliest_start > pickup_time) {
      pickup_time = ride.earliest_start;
      wasted_time = ride.earliest_start - pickup_time;
    }
    if (pickup_time + distance < ride.latest_finish) {
      first_free_time = pickup_time + distance;
      rides.push_back(ride);
      cur_position = ride.end;
      score += distance - wasted_time;
      return true;
    }
    return false;
  }

  void Print() const {
    std::cout << rides.size();
    for (const Ride& ride : rides) {
      std::cout << ' ' << ride.id;
    }
    std::cout << std::endl;
  }
};

bool NewCarSort(const Car& car1, const Car& car2) {
  return car1.score < car2.score;
}

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
    std::vector<Ride> missed_rides;
    sort(rides.begin(), rides.end());
    for (const Ride& ride : rides) {
      Car* best_car = nullptr;
      int min_wasted_time = -1;
      for (Car& car : cars) {
        const int car_wasted_time = car.CanScheduleRide(ride);
        if (car_wasted_time != -2 &&
            (!best_car || car_wasted_time < min_wasted_time)) {
          best_car = &car;
          min_wasted_time = car_wasted_time;
        }
      }
      if (best_car == nullptr)
        missed_rides.push_back(ride);
      else
        best_car->ScheduleRide(ride);
    }
    std::vector<Ride> new_missed_rides;
    for (int i = 0; i < 10; i++) {
      std::sort(cars.begin(), cars.end(), NewCarSort);
      std::vector<Car*> new_cars;
      for (int j = 0; j < std::min(8, number_of_cars); j++) {
        for (Ride& ride : cars[j].rides) missed_rides.push_back(ride);
        cars[j].cur_position.col = 0;
        cars[j].cur_position.row = 0;
        cars[j].first_free_time = 0;
        cars[j].score = 0;
        cars[j].rides.clear();
        new_cars.push_back(&cars[j]);
      }
      std::sort(missed_rides.begin(), missed_rides.end());
      for (const Ride& ride : missed_rides) {
        Car* best_car = nullptr;
        int min_wasted_time = -1;
        for (Car* car : new_cars) {
          const int car_wasted_time = car->CanScheduleRide(ride);
          if (car_wasted_time != -2 &&
              (!best_car || car_wasted_time < min_wasted_time)) {
            best_car = car;
            min_wasted_time = car_wasted_time;
          }
        }
        if (best_car == nullptr)
          new_missed_rides.push_back(ride);
        else
          best_car->ScheduleRide(ride);
      }
      missed_rides.clear();
      missed_rides = new_missed_rides;
      new_missed_rides.clear();
    }
    for (Car& car : cars) {
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
