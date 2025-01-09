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
#include <fstream>
#include <sstream>
#include "Train.hpp"
#include "Route.hpp"

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
     * @param schedule Schedule of the station
     * @param intermediateStops Intermediate stops at the station
     * @param startStation Start station of the route
     * @param endStation End station of the route
     * @param name Name of the station
     */

    Station(Train* trainName = nullptr, int platformCount = 0, 
        const std::vector<Route*>& schedule = {}, 
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

    /**
     * @brief Add a new station to the system.
     * 
     * @param trainName Train associated with the station
     * @param platformCount Number of platforms at the station
     * @param intermediateStops Intermediate stops at the station
     * @param startStation Start station of the route
     * @param endStation End station of the route
     * @param name Name of the station
     */

    static void addStation(Train* trainName, int platformCount,
                         const std::vector<Route*>& intermediateStops,
                         Train* startStation, Train* endStation,
                         const std::string& name) {
        Station newStation(trainName, platformCount, {}, intermediateStops,
                         startStation, endStation, name);
        m_stations.push_back(newStation);
    }

    /**
     * @brief Remove a station from the system.
     * 
     * @param name Name of the station to remove
     * @return true if the station was removed successfully
     * @return false if the station was not found
     */

    static bool removeStation(const std::string& name) {
        auto it = std::find_if(m_stations.begin(), m_stations.end(),
                              [&name](const Station& station) {
                                  return station.getName() == name;
                              });
        if (it != m_stations.end()) {
            m_stations.erase(it);
            return true;
        }
        return false;
    }

    /**
     * @brief Display information about a station.
     * 
     * @param name Name of the station to display
     */

    static void displayStationInfo(const std::string& name) {
        auto it = std::find_if(m_stations.begin(), m_stations.end(),
                              [&name](const Station& station) {
                                  return station.getName() == name;
                              });
        if (it != m_stations.end()) {
            std::cout << "Station Information:\n"
                     << "Name: " << it->getName() << "\n"
                     << "Platform Count: " << it->getPlatformCount() << "\n"
                     << "Intermediate Stops Count: "
                     << it->getIntermediateStops().size() << "\n";
        } else {
            std::cout << "Station with name " << name << " not found.\n";
        }
    }

    std::string getName() const {
        return m_name;
    }
};
}