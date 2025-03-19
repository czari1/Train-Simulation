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
#include <memory>

namespace CJ {

// Forward declarations
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
    std::shared_ptr<Train> m_trainName;
    int m_platformCount; 
    std::vector<std::shared_ptr<Route>> m_intermediateStops;
    std::string m_name;
    std::shared_ptr<Train> m_startStation;
    std::shared_ptr<Train> m_endStation;

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
    Station(std::shared_ptr<Train> trainName = nullptr, 
            int platformCount = 0,  
            const std::vector<std::shared_ptr<Route>>& intermediateStops = {}, 
            std::shared_ptr<Train> startStation = nullptr, 
            std::shared_ptr<Train> endStation = nullptr,
            const std::string& name = "");

    // Setters
    void setTrainName(std::shared_ptr<Train> trainName);
    void setPlatformCount(int platformCount);
    void setIntermediateStops(const std::vector<std::shared_ptr<Route>>& intermediateStops);
    void setStartStation(std::shared_ptr<Train> startStation);
    void setEndStation(std::shared_ptr<Train> endStation);
    void setName(const std::string& name);

    // Getters
    std::shared_ptr<Train> getTrainName() const;
    int getPlatformCount() const;
    const std::vector<std::shared_ptr<Route>>& getIntermediateStops() const;
    std::shared_ptr<Train> getStartStation() const;
    std::shared_ptr<Train> getEndStation() const;
    const std::string& getName() const;
};

} // namespace CJ