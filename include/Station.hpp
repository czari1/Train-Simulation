/**
 * @file Station.hpp
 * @brief Header file for the Station class.
 * 
 * This file defines the `Station` class, which represents a railway station in the system.
 * The class provides functionality to manage station details, including platform count,
 * train associations, intermediate stops, and start/end stations.
 * 
 * @author [Cezary Jaros]
 * @date [09.01.2025]
 */
#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace CJ {
class Route;
class Train;

/**
 * @class Station
 * @brief Represents a railway station in the system.
 * 
 * This class provides functionality to manage station details, including platform count,
 * train associations, intermediate stops, and start/end stations.
 */

class Station {
private:
    Train* m_trainName;
    int m_platformCount; 
    std::vector<Route*> m_intermediateStops;
    std::string m_name;
    Train* m_startStation;
    Train* m_endStation;
    static std::vector<Station> m_stations;

public:

    /**
     * @brief Construct a new Station object
     * 
     * @param trainName Train associated with the station
     * @param platformCount Number of platforms at the station
     * @param intermediateStops Intermediate stops at the station
     * @param startStation Start station of the route
     * @param endStation End station of the route
     * @param name Name of the station
     */

    Station(Train* trainName = nullptr, int platformCount = 0,  
        const std::vector<Route*>& intermediateStops = {}, 
        Train* startStation = 0 , 
        Train* endStation = 0 ,
        const std::string& name = "")
        : m_trainName(trainName), m_platformCount(platformCount), 
         m_intermediateStops(intermediateStops), 
          m_startStation(startStation), m_endStation(endStation),
          m_name(name) {}

    

    void setTrainName(Train* trainName) {
        m_trainName = trainName;
    }

    void setPlatformCount(int platformCount) {
    if (platformCount <= 0) {
        std::cerr << "Platform count must be greater than 0\n";
        throw std::invalid_argument("Platform count must be greater than 0");
    }
    m_platformCount = platformCount;
}

    void setIntermediateStops(std::vector<Route*> intermiediateStops) {
        m_intermediateStops = intermiediateStops;
    }

    void setStartStation(Train* startStation) {
        m_startStation = startStation;
    }

    void setEndStation(Train* endStation) {
        m_endStation = endStation;
    }

    void setName(const std::string& name) {
        m_name = name;
    }

    Train* getTrainName() const {
        return m_trainName;
    }

    int getPlatformCount() const {
        return m_platformCount;
    }

    std::vector<Route*> getIntermediateStops() const {
        return m_intermediateStops;
    }

    Train* getStartStation() const {
        return m_startStation;
    }

    Train* getEndStation() const {
        return m_endStation;
    }

    std::string getName() const {
        return m_name;
    }

};
}