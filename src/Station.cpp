#include "../include/Station.hpp"
#include "../include/Train.hpp"
#include "../include/Route.hpp"
#include <stdexcept>

namespace CJ {

Station::Station(std::shared_ptr<Train> trainName, int platformCount,
                const std::vector<std::shared_ptr<Route>>& intermediateStops,
                std::shared_ptr<Train> startStation, std::shared_ptr<Train> endStation,
                const std::string& name)
    : m_trainName(trainName)
    , m_platformCount(std::max(1, platformCount))  // Ensure at least 1 platform
    , m_intermediateStops(intermediateStops)
    , m_startStation(startStation)
    , m_endStation(endStation)
    , m_name(name) {
    // Additional validation if needed
    if (name.empty()) {
        throw std::invalid_argument("Station name cannot be empty");
    }
}

void Station::setTrainName(std::shared_ptr<Train> trainName) {
    m_trainName = trainName;
}

void Station::setPlatformCount(int platformCount) {
    if (platformCount <= 0) {
        throw std::invalid_argument("Platform count must be greater than 0");
    }
    m_platformCount = platformCount;
}

void Station::setIntermediateStops(const std::vector<std::shared_ptr<Route>>& intermediateStops) {
    m_intermediateStops = intermediateStops;
}

void Station::setStartStation(std::shared_ptr<Train> startStation) {
    m_startStation = startStation;
}

void Station::setEndStation(std::shared_ptr<Train> endStation) {
    m_endStation = endStation;
}

void Station::setName(const std::string& name) {
    m_name = name;
}

std::shared_ptr<Train> Station::getTrainName() const {
    return m_trainName;
}

int Station::getPlatformCount() const {
    return m_platformCount;
}

const std::vector<std::shared_ptr<Route>>& Station::getIntermediateStops() const {
    return m_intermediateStops;
}

std::shared_ptr<Train> Station::getStartStation() const {
    return m_startStation;
}

std::shared_ptr<Train> Station::getEndStation() const {
    return m_endStation;
}

const std::string& Station::getName() const {
    return m_name;
}

} // namespace CJ