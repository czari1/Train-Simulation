#include "../include/Train.hpp"
#include "../include/Route.hpp"
#include "../include/Station.hpp"
#include <iostream>
#include "../include/Management.hpp"
namespace CJ {

std::vector<Station> Management::m_stations;

void initializeStations() {
    CJ::Management::addStation(nullptr, 5, {}, nullptr, nullptr, "Warsaw Central");
    CJ::Management::addStation(nullptr, 4, {}, nullptr, nullptr, "Krakow Main");
    CJ::Management::addStation(nullptr, 3, {}, nullptr, nullptr, "Gdansk Central");
}

}