#include "../include/Train.hpp"
#include "../include/Route.hpp"
#include "../include/Station.hpp"
#include <stdexcept>
#include <iostream>

namespace CJ {

    Train::Train(const std::string& trainName, int speed, int capacity, int id,
        int wagonCount, std::shared_ptr<Route> departureTimeHour,
        std::shared_ptr<Route> departureTimeMinute,
        std::shared_ptr<Route> arrivalTimeHour,
        std::shared_ptr<Route> arrivalTimeMinute)
        : m_trainName(trainName)
        , m_speed(speed)  
        , m_capacity(capacity)  
        , m_id(id)
        , m_wagonCount(wagonCount)  
        , m_departureTimeHour(departureTimeHour)
        , m_departureTimeMinute(departureTimeMinute)
        , m_arrivalTimeHour(arrivalTimeHour)
        , m_arrivalTimeMinute(arrivalTimeMinute)
{

    if (m_speed <= 0) {
        m_speed = 1;  
    }
    if (m_capacity <= 0) {
        m_capacity = 1;  
    }
    if (m_wagonCount <= 0) {
        m_wagonCount = 1;  
    }
}

void Train::setTrainName(const std::string& trainName) {
    m_trainName = trainName;
}

void Train::setSpeed(int speed) {
    if (speed <= 0) {
        throw std::invalid_argument("Speed must be greater than 0");
    }
    m_speed = speed;
}

void Train::setCapacity(int capacity) {
    if (capacity <= 0) {
        throw std::invalid_argument("Capacity must be greater than 0");
    }
    m_capacity = capacity;
}

void Train::setId(int id) {
    if (id <= 0) {
        throw std::invalid_argument("ID must be greater than 0");
    }
    m_id = id;
}

void Train::setWagonCount(int wagonCount) {
    if (wagonCount <= 0) {
        throw std::invalid_argument("Wagon count must be greater than 0");
    }
    m_wagonCount = wagonCount;
}

void Train::setDepartureTimeHour(std::shared_ptr<Route> departureTimeHour) {
    m_departureTimeHour = departureTimeHour;
}

void Train::setDepartureTimeMinute(std::shared_ptr<Route> departureTimeMinute) {
    m_departureTimeMinute = departureTimeMinute;
}

void Train::setArrivalTimeHour(std::shared_ptr<Route> arrivalTimeHour) {
    m_arrivalTimeHour = arrivalTimeHour;
}

void Train::setArrivalTimeMinute(std::shared_ptr<Route> arrivalTimeMinute) {
    m_arrivalTimeMinute = arrivalTimeMinute;
}

std::string Train::getTrainName() const {
    return m_trainName;
}

int Train::getSpeed() const {
    return m_speed;
}

int Train::getCapacity() const {
    return m_capacity;
}

int Train::getId() const {
    return m_id;
}

int Train::getWagonCount() const {
    return m_wagonCount;
}

std::shared_ptr<Route> Train::getDepartureTimeHour() const {
    return m_departureTimeHour;
}

std::shared_ptr<Route> Train::getDepartureTimeMinute() const {
    return m_departureTimeMinute;
}

std::shared_ptr<Route> Train::getArrivalTimeHour() const {
    return m_arrivalTimeHour;
}

std::shared_ptr<Route> Train::getArrivalTimeMinute() const {
    return m_arrivalTimeMinute;
}

} 