#include "../include/Train.hpp"
#include "../include/Route.hpp"
#include "../include/Station.hpp"
#include <iostream>

namespace CJ {

std::vector<Station> Station::m_stations;

Station stationInstance1(nullptr, 5, {}, {}, nullptr, nullptr, "Warsaw Central");
Station stationInstance2(nullptr, 4, {}, {}, nullptr, nullptr, "Krakow Main");
Station stationInstance3(nullptr, 3, {}, {}, nullptr, nullptr, "Gdansk Central");

void initializeStations() {
    Station::addStation(nullptr, 5, {}, nullptr, nullptr, "Warsaw Central");
    Station::addStation(nullptr, 4, {}, nullptr, nullptr, "Krakow Main");
    Station::addStation(nullptr, 3, {}, nullptr, nullptr, "Gdansk Central");
}

}