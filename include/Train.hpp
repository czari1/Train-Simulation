/**
 * @file Train.hpp
 * @brief Header file for the Train class.
 * 
 * This file defines the `Train` class, which represents a train in the system.
 * The class provides functionality to manage train details, including train name, speed,
 * capacity, start and end stations, departure and arrival times, and wagon count.
 */

 #pragma once
 #include <string>
 #include <vector>
 #include <memory>
 
 namespace CJ {
 
 // Forward declarations
 class Route;
 class Station;
 
 /**
  * @class Train
  * @brief Represents a train in the system.
  * 
  * This class provides functionality to manage train details, including train name, speed,
  * capacity, start and end stations, departure and arrival times, and wagon count.
  */
 class Train {
 private:
     std::string m_trainName;
     int m_speed;
     int m_capacity;
     int m_id;
     std::string m_startStation;
     std::string m_endStation;
     int m_wagonCount;
     std::shared_ptr<Route> m_departureTimeHour;
     std::shared_ptr<Route> m_departureTimeMinute;
     std::shared_ptr<Route> m_arrivalTimeHour;
     std::shared_ptr<Route> m_arrivalTimeMinute;
 
 public:
     /**
      * @brief Construct a new Train object
      * 
      * @param trainName Name of the train
      * @param speed Speed of the train in km/h
      * @param capacity Passenger capacity of the train
      * @param id Unique identifier for the train
      * @param startStation Start station of the train
      * @param endStation End station of the train
      * @param wagonCount Number of wagons on the train
      * @param departureTimeHour Departure hour
      * @param departureTimeMinute Departure minute
      * @param arrivalTimeHour Arrival hour
      * @param arrivalTimeMinute Arrival minute
      */
     Train(const std::string& trainName = "", 
           int speed = 0, 
           int capacity = 0, 
           int id = 0, 
           const std::string& startStation = "", 
           const std::string& endStation = "", 
           int wagonCount = 0, 
           std::shared_ptr<Route> departureTimeHour = nullptr,
           std::shared_ptr<Route> departureTimeMinute = nullptr, 
           std::shared_ptr<Route> arrivalTimeHour = nullptr,
           std::shared_ptr<Route> arrivalTimeMinute = nullptr);
 
     // Setters
     void setTrainName(const std::string& trainName);
     void setSpeed(int speed);
     void setCapacity(int capacity);
     void setId(int id);
     void setWagonCount(int wagonCount);
     void setStartStation(const std::string& startStation);
     void setEndStation(const std::string& endStation);
     void setDepartureTimeHour(std::shared_ptr<Route> departureTimeHour);
     void setDepartureTimeMinute(std::shared_ptr<Route> departureTimeMinute);
     void setArrivalTimeHour(std::shared_ptr<Route> arrivalTimeHour);
     void setArrivalTimeMinute(std::shared_ptr<Route> arrivalTimeMinute);
 
     // Getters
     std::string getTrainName() const;
     int getSpeed() const;
     int getCapacity() const;
     int getId() const;
     const std::string& getStartStation() const;
     const std::string& getEndStation() const;
     int getWagonCount() const;
     std::shared_ptr<Route> getDepartureTimeHour() const;
     std::shared_ptr<Route> getDepartureTimeMinute() const;
     std::shared_ptr<Route> getArrivalTimeHour() const;
     std::shared_ptr<Route> getArrivalTimeMinute() const;
 };
 
 } // namespace CJ