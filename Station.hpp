#pragma once
#include <string>
#include <vector>
#include <algorithm> // for std::find and std::remove
#include <iostream>  // for std::cout
#include "Train.hpp" 
#include "Route.hpp" 

namespace CJ {
class Train;
class Route;

class Station {
private:
    std::string m_name;
    int m_platformCount;
    std::vector<Route*> m_schedule;
    std::vector<Route*> m_intermediateStops;
    std::string m_startStation;
    std::string m_endStation;

public:
    void setName(std::string name) {
        m_name = name;
    }

    void setPlatformCount(int platformCount) {
        m_platformCount = platformCount;
    }

    void setSchedule(std::vector<Route*> schedule) {
        m_schedule = schedule;
    }

    void setIntermediateStops(std::vector<Route*> intermiediateStops) {
        m_intermediateStops = intermiediateStops;
    }

    void setStartStation(std::string startStation) {
        m_startStation = startStation;
    }

    void setEndStation(std::string endStation) {
        m_endStation = endStation;
    }

    std::string getName() const{
        return m_name;
    }

    int getPlatformCount() const {
        return m_platformCount;
    }
    
    std::vector<Route*> getSchedule() const {
        return m_schedule;
    }

    std::vector<Route*> getIntermediateStops() const {
        return m_intermediateStops;
    }

    std::string getStartStation() const {
        return m_startStation;
    }

    std::string getEndStation() const {
        return m_endStation;
    }

};

} // namespace CJ
