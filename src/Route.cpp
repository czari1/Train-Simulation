#include "../include/Route.hpp"
#include "../include/Train.hpp"
#include "../include/Station.hpp"
#include <stdexcept>

namespace CJ {

    Route::Route(int depHour, int depMin, int arrHour, int arrMin,
        int duration,
        std::shared_ptr<Train> trainPtr,
        std::shared_ptr<Station> startStation,
        std::shared_ptr<Station> endStation,
        const std::vector<std::string>& intermediateStops)
        : m_departureTimeHour(depHour), m_departureTimeMinute(depMin),
        m_arrivalTimeHour(arrHour), m_arrivalTimeMinute(arrMin),
        m_duration(duration),
        m_assignedTrain(trainPtr), m_startStation(startStation),
        m_endStation(endStation),
        m_intermediateStops(intermediateStops)  {
    // Validate time values
    if (depHour < 0 || depHour > 23) {
        throw std::invalid_argument("Departure hour must be between 0 and 23");
    }
    if (depMin < 0 || depMin > 59) {
        throw std::invalid_argument("Departure minute must be between 0 and 59");
    }
    if (arrHour < 0 || arrHour > 23) {
        throw std::invalid_argument("Arrival hour must be between 0 and 23");
    }
    if (arrMin < 0 || arrMin > 59) {
        throw std::invalid_argument("Arrival minute must be between 0 and 59");
    }
    if (duration <= 0) {
        throw std::invalid_argument("Duration must be greater than 0");
    }
}

void Route::setDepartureTimeHour(int departureTimeHour) {
    if (departureTimeHour < 0 || departureTimeHour > 23) {
        throw std::invalid_argument("Departure hour must be between 0 and 23");
    }
    m_departureTimeHour = departureTimeHour;
}

void Route::setDepartureTimeMinute(int departureTimeMinute) {
    if (departureTimeMinute < 0 || departureTimeMinute > 59) {
        throw std::invalid_argument("Departure minute must be between 0 and 59");
    }
    m_departureTimeMinute = departureTimeMinute;
}

void Route::setArrivalTimeHour(int arrivalTimeHour) {
    if (arrivalTimeHour < 0 || arrivalTimeHour > 23) {
        throw std::invalid_argument("Arrival hour must be between 0 and 23");
    }
    m_arrivalTimeHour = arrivalTimeHour;
}

void Route::setArrivalTimeMinute(int arrivalTimeMinute) {
    if (arrivalTimeMinute < 0 || arrivalTimeMinute > 59) {
        throw std::invalid_argument("Arrival minute must be between 0 and 59");
    }
    m_arrivalTimeMinute = arrivalTimeMinute;
}

void Route::setDuration(int duration) {
    if (duration <= 0) {
        throw std::invalid_argument("Duration must be greater than 0");
    }
    m_duration = duration;
}

void Route::setIntermediateStops(const std::vector<std::string>& intermediateStops) {
    m_intermediateStops = intermediateStops;
}

void Route::setTrainAssignment(std::shared_ptr<Train> train) {
    m_assignedTrain = train;
}

int Route::getDepartureTimeHour() const {
    return m_departureTimeHour;
}

int Route::getDepartureTimeMinute() const {
    return m_departureTimeMinute;
}

int Route::getArrivalTimeHour() const {
    return m_arrivalTimeHour;
}

int Route::getArrivalTimeMinute() const {
    return m_arrivalTimeMinute;
}

int Route::getDuration() const {
    return m_duration;
}

std::shared_ptr<Train> Route::getAssignedTrain() const {
    return m_assignedTrain;
}

const std::vector<std::string>& Route::getIntermediateStops() const {
    return m_intermediateStops;
}

void Route::addIntermediateStop(const std::string& stationName) {
    m_intermediateStops.push_back(stationName);
}

int Route::calculateTravelTime() const {
    return m_duration + m_intermediateStops.size() * 3; // 3 minutes per intermediate stop
}

std::string Route::getStartStation() const {
    return !m_intermediateStops.empty() ? m_intermediateStops.front() : "";
}

std::string Route::getEndStation() const {
    return !m_intermediateStops.empty() ? m_intermediateStops.back() : "";
}

} // namespace CJ