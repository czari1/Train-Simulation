/**
 * @file Train.hpp
 * @brief Header file for the Train class.
 * 
 * This file defines the `Train` class, which represents a train in the system.
 * The class provides functionality to manage train details, including train name, speed,
 * capacity, start and end stations, departure and arrival times, and wagon count.
 * 
 */

#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace CJ {
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
    Route* m_departureTimeHour;
    Route* m_departureTimeMinute;
    Route* m_arrivalTimeHour;
    Route* m_arrivalTimeMinute;

    

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

    Train(const std::string& trainName = "", int speed = 0, int capacity = 0, int id = 0, 
          std::string startStation = "", std::string endStation = "", 
          int wagonCount = 0, Route* departureTimeHour = nullptr,
          Route* departureTimeMinute = nullptr, Route* arrivalTimeHour = nullptr,
          Route* arrivalTimeMinute = nullptr)
        : m_trainName(trainName), m_speed(speed), m_capacity(capacity), m_id(id),
          m_startStation(startStation), m_endStation(endStation),
          m_wagonCount(wagonCount), m_departureTimeHour(departureTimeHour),
          m_departureTimeMinute(departureTimeMinute), m_arrivalTimeHour(arrivalTimeHour),
          m_arrivalTimeMinute(arrivalTimeMinute) {}

    void setTrainName(std::string trainName) {
        m_trainName = trainName;
    }

    void setSpeed(int speed) {
    if (speed > 0) {
        m_speed = speed;
    } else {
        std::cerr << "Speed must be greater than 0\n";
        throw std::invalid_argument("Speed must be greater than 0");
    }
}

void setCapacity(int capacity) {
    if (capacity > 0) {
        m_capacity = capacity;
    } else {
        std::cerr << "Capacity must be greater than 0\n";
        throw std::invalid_argument("Capacity must be greater than 0");
    }
}

void setId(int id) {
    if (id > 0) {
        m_id = id;
    } else {
        std::cerr << "ID must be greater than 0\n";
        throw std::invalid_argument("ID must be greater than 0");
    }
}

void setWagonCount(int wagonCount) {
    if (wagonCount > 0) {
        m_wagonCount = wagonCount;
    } else {
        std::cerr << "Wagon count must be greater than 0\n";
        throw std::invalid_argument("Wagon count must be greater than 0");
    }
}

    void setStartStation(std::string startStation) {
        m_startStation = startStation;
    }

    void setEndStation(std::string endStation) {
        m_endStation = endStation;
    }

    void setDepartureTimeHour(Route* departureTimeHour) {
        m_departureTimeHour = departureTimeHour;
    }

    void setDepartureTimeMinute(Route* departureTimeMinute) {
        m_departureTimeMinute = departureTimeMinute;
    }

    void setArrivalTimeHour(Route* arrivalTimeHour) {
        m_arrivalTimeHour = arrivalTimeHour;
    }

    void setArrivalTimeMinute(Route* arrivalTimeMinute) {
        m_arrivalTimeMinute = arrivalTimeMinute;
    }

    std::string getTrainName() const {
        return m_trainName;
    }

    int getSpeed() const {
        return m_speed;
    }

    int getCapacity() const {
        return m_capacity;
    }

    int getId() const {
        return m_id;
    }

    std::string getStartStation() const {
        return m_startStation;
    }

    std::string getEndStation() const {
        return m_endStation;
    }

    int getWagonCount() const {
        return m_wagonCount;
    }

    Route* getDepartureTimeHour() const {
        return m_departureTimeHour;
    }

    Route* getDepartureTimeMinute() const {
        return m_departureTimeMinute;
    }

    Route* getArrivalTimeHour() const {
        return m_arrivalTimeHour;
    }

    Route* getArrivalTimeMinute() const {
        return m_arrivalTimeMinute;
    }
};

}