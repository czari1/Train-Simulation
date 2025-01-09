
/**
 * @file Train.hpp
 * @brief Header file for the Train class.
 * 
 * This file defines the `Train` class, which represents a train in the system.
 * The class provides functionality to manage train details, including train name, speed,
 * capacity, start and end stations, departure and arrival times, and wagon count.
 * 
 * @author [Cezary Jaros]
 * @date [09.01.2025]
 */

#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Station.hpp"
#include "Route.hpp"

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

    static std::vector<Train> m_trains;

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
          std::string startStation = nullptr, std::string endStation = nullptr, 
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
        m_speed = speed;
    }

    void setCapacity(int capacity) {
        m_capacity = capacity;
    }

    void setId(int id) {
        m_id = id;
    }

    void setStartStation(std::string startStation) {
        m_startStation = startStation;
    }

    void setEndStation(std::string endStation) {
        m_endStation = endStation;
    }

    void setWagonCount(int wagonCount) {
        m_wagonCount = wagonCount;
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
    

    /**
     * @brief Add a new train to the system.
     * 
     * @param trainName Name of the train
     * @param speed Speed of the train in km/h
     * @param capacity Passenger capacity of the train
     * @param id Unique identifier for the train
     * @param startStation Start station of the train
     * @param endStation End station of the train
     * @param wagonCount Number of wagons on the train
     * @param depHour Departure hour
     * @param depMin Departure minute
     * @param arrHour Arrival hour
     * @param arrMin Arrival minute
     */
        
    static void addTrain(const std::string& trainName, int speed, int capacity, int id,
                        const std::string& startStation, const std::string& endStation,
                        int wagonCount, Route* depHour, Route* depMin,
                        Route* arrHour, Route* arrMin) {
        Train newTrain(trainName, speed, capacity, id, startStation, endStation,
                      wagonCount, depHour, depMin, arrHour, arrMin);
        m_trains.push_back(newTrain);
    }

    /**
     * @brief Delete a train from the system.
     * 
     * @param id ID of the train to delete
     * @return true if the train was deleted successfully
     */

    static bool deleteTrain(int id) {
        auto it = std::find_if(m_trains.begin(), m_trains.end(),
                              [id](const Train& train) { return train.getId() == id; });
        if (it != m_trains.end()) {
            m_trains.erase(it);
            return true;
        }
        return false;
    }

    /**
     * @brief Display detailed information about a train.
     * 
     * @param id ID of the train to display
     */

    static void displayTrainInfo(int id) {
        auto it = std::find_if(m_trains.begin(), m_trains.end(),
                              [id](const Train& train) { return train.getId() == id; });
        if (it != m_trains.end()) {
            std::cout << "Train Information:\n"
                     << "Name: " << it->getTrainName() << "\n"
                     << "ID: " << it->getId() << "\n"
                     << "Speed: " << it->getSpeed() << " km/h\n"
                     << "Capacity: " << it->getCapacity() << " passengers\n"
                     << "Start Station: " << it->getStartStation() << "\n"
                     << "End Station: " << it->getEndStation() << "\n"
                     << "Wagon Count: " << it->getWagonCount() << "\n";
        } else {
            std::cout << "Train with ID " << id << " not found.\n";
        }
    }
};

}