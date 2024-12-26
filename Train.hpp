#pragma once
#include <string>
#include <iostream>
#include "Station.hpp"
#include "Route.hpp"

namespace CJ {

class Route;
class Station;

class Train {
private:
    std::string m_name;
    int m_speed;
    int m_capacity;
    int m_id;
    Station* m_startStation;
    Station* m_endStation;
    int m_wagonCount;
    Route* m_departureTimeHour;
    Route* m_departureTimeMinute;
    Route* m_arrivalTimeHour;
    Route* m_arrivalTimeMinute;

public:
    // Setters
    void setName(const std::string& name) {
        m_name = name;
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

    void setStartStation(Station* startStation) {
        m_startStation = startStation;
    }

    void setEndStation(Station* endStation) {
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

    // Getters
    std::string getName() const {
        return m_name;
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

    Station* getStartStation() const {
        return m_startStation;
    }

    Station* getEndStation() const {
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

} // namespace CJ
