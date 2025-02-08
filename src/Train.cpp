#include "../include/Train.hpp"
#include "../include/Route.hpp"
#include "../include/Station.hpp"
#include <iostream>
#include "../include/Management.hpp"

namespace CJ {

std::vector<Train> Train::m_trains;

void initializeTrains() {
    CJ::Management::addTrain("Express_101", 160, 400, 1001, "Warsaw", "Krakow", 8, nullptr, nullptr, nullptr, nullptr);
    CJ::Management::addTrain("InterCity_202", 140, 350, 1002, "Gdansk", "Wroclaw", 6, nullptr, nullptr, nullptr, nullptr);
    CJ::Management::addTrain("Regional_303", 120, 250, 1003, "Poznan", "Lodz", 4, nullptr, nullptr, nullptr, nullptr);
}

}