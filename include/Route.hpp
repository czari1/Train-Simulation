/**
 * @file Route.hpp
 * @brief Header file for the Route class.
 * This file defines the `Route` class, which represents a train route in the system.
 * The class provides functionality to manage route details, including departure and arrival times,
 * train associations, intermediate stops, and route duration.
 * 
 */

#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace CJ {
class Station;
class Train;

/**
 * @class Route
 * @brief Represents a train route in the system.
 * 
 * This class provides functionality to manage route details, including departure and arrival times,
 * train associations, intermediate stops, and route duration.
 */

class Route {
private:
    int m_departureTimeMinute;
    int m_departureTimeHour;
    int m_arriveTimeMinute;
    int m_arriveTimeHour;
    Train* m_trainName;
    Train* m_startStation;
    Train* m_endStation;
    int m_duration;
    std::vector<std::string> m_intermediateStops;

    /**
     * @brief Constructs a Route object with specified parameters.
     * 
     * @param depHour Departure hour.
     * @param depMin Departure minute.
     * @param arrHour Arrival hour.
     * @param arrMin Arrival minute.
     * @param trainName Pointer to the train associated with this route.
     * @param startStation Pointer to the start station.
     * @param endStation Pointer to the end station.
     * @param duration Total travel duration in minutes.
     * @param intermediateStops List of intermediate stop names.
     */

public:
    Route(int depHour = 0, int depMin = 0, int arrHour = 0, int arrMin = 0,
          Train* trainName = nullptr, Train* startStation = nullptr, Train* endStation = nullptr, 
          int duration = 0, const std::vector<std::string>& intermediateStops = {})
        : m_departureTimeHour(depHour), m_departureTimeMinute(depMin),
          m_arriveTimeHour(arrHour), m_arriveTimeMinute(arrMin),
          m_trainName(trainName), m_startStation(startStation),
          m_endStation(endStation), m_duration(duration),
          m_intermediateStops(intermediateStops) {}

    
    void setDepartureTimeHour(int departureTimeHour) {
    if (departureTimeHour < 0 || departureTimeHour > 23) {
        std::cerr << "Departure hour must be between 0 and 23\n";
        throw std::invalid_argument("Departure hour must be between 0 and 23");
    }
        m_departureTimeHour = departureTimeHour;
    }

    void setDepartureTimeMinute(int departureTimeMinute) {
        if (departureTimeMinute < 0 || departureTimeMinute > 59) {
            std::cerr << "Departure minute must be between 0 and 59\n";
            throw std::invalid_argument("Departure minute must be between 0 and 59");
        }
        m_departureTimeMinute = departureTimeMinute;
    }

    void setArrivalTimeHour(int arrivalTimeHour) {
        if (arrivalTimeHour < 0 || arrivalTimeHour > 23) {
            std::cerr << "Arrival hour must be between 0 and 23\n";
            throw std::invalid_argument("Arrival hour must be between 0 and 23");
        }
        m_arriveTimeHour = arrivalTimeHour;
    }

    void setArrivalTimeMinute(int arrivalTimeMinute) {
        if (arrivalTimeMinute < 0 || arrivalTimeMinute > 59) {
            std::cerr << "Arrival minute must be between 0 and 59\n";
            throw std::invalid_argument("Arrival minute must be between 0 and 59");
        }
        m_arriveTimeMinute = arrivalTimeMinute;
    }

    void setDuration(int duration) {
        if (duration <= 0) {
            std::cerr << "Duration must be greater than 0\n";
            throw std::invalid_argument("Duration must be greater than 0");
        }
        m_duration = duration;
    }

    void setTrainName(Train* trainName) {
        m_trainName = trainName;
    }

    void setStartStation(Train* startStation) {
        m_startStation = startStation;
    }

    void setEndStation(Train* endStation) {
        m_endStation = endStation;
    }

    void setIntermediateStops(std::vector<std::string> intermediateStops) {
        m_intermediateStops = intermediateStops;
    }

    int getDepartureTimeHour() const {
        return m_departureTimeHour;
    }

    int getDepartureTimeMinute() const {
        return m_departureTimeMinute;
    }

    int getArrivalTimeHour() const {
        return m_arriveTimeHour;
    }

    int getArrivalTimeMinute() const {
        return m_arriveTimeMinute;
    }

    Train* getTrainName() const {
        return m_trainName;
    }

    Train* getStartStation() const {
        return m_startStation;
    }

    Train* getEndStation() const {
        return m_endStation;
    }

    int getDuration() const {
        return m_duration;
    }

    std::vector<std::string> getIntermediateStops() const {
        return m_intermediateStops;
    }

    void addIntermediateStop(const std::string& stationName) {
        m_intermediateStops.push_back(stationName);
    }

    int calculateTravelTime() const {
        return m_duration + m_intermediateStops.size() * 3;
    }
};

}
