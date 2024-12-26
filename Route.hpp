#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "Train.hpp"
#include "Station.hpp"

namespace CJ {
class Station;
class Train;

class Route {
private:
    int m_departureTimeMinute;
    int m_departureTimeHour;
    int m_arriveTimeMinute;
    int m_arriveTimeHour;
    Station* m_name;
    Station* m_startStation;
    Station* m_endStation;
    int m_duration; // Duration in minutes
    std::vector<std::string> m_intermediateStops;
    std::vector<std::string> m_schedule;

public:
    //Settery

    void setDepartureTimeHour(int departureTimeHour) {
        m_departureTimeHour = departureTimeHour;
    }

    void setDepartureTimeMinute(int departureTimeMinute) {
        m_departureTimeMinute = departureTimeMinute;
    }

    void setArrivalTimeHour(int arrivalTimeHour) {
        m_arriveTimeHour = arrivalTimeHour;
    }

    void setArrivalTimeMinute(int arrivalTimeMinute) {
        m_arriveTimeMinute = arrivalTimeMinute;
    }

    void setName(Station* name) {
        m_name = name;
    }

    void setStartStation(Station* startStation){
        m_startStation = startStation;
    }

    void setEndStation(Station* endStation) {
        m_endStation = endStation;
    }

    void setDuration(int duration){
        m_duration = duration;
    }

    void setIntermediateStops(std::vector<std::string> intermediateStops) {
        m_intermediateStops = intermediateStops;
    }

    //Gettery

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
        return m_departureTimeMinute;
    }

    Station* getName() const {
        return m_name;
    }

    Station* getStartStation() const {
        return m_startStation;
    }

    Station* getEndStation() const {
        return m_endStation;
    }

    int getDuration() const {
        return m_duration;
    }

    std::vector<std::string> getSchedule() const {
        return m_schedule;
    }

    std::vector<std::string> getIntermediateStops() const {
        return m_intermediateStops;
    }
};

} // namespace CJ
