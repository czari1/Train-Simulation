#include "Train.hpp"
#include "Station.hpp"
#include "Route.hpp"
#include <iostream>

namespace CJ {

std::vector<Train> Train::m_trains;

Train trainInstance1("Express_101", 160, 400, 1001, "Warsaw", "Krakow", 8, nullptr, nullptr, nullptr, nullptr);
Train trainInstance2("InterCity_202", 140, 350, 1002, "Gdansk", "Wroclaw", 6, nullptr, nullptr, nullptr, nullptr);
Train trainInstance3("Regional_303", 120, 250, 1003, "Poznan", "Lodz", 4, nullptr, nullptr, nullptr, nullptr);

void initializeTrains() {
    Train::addTrain("Express_101", 160, 400, 1001, "Warsaw", "Krakow", 8, nullptr, nullptr, nullptr, nullptr);
    Train::addTrain("InterCity_202", 140, 350, 1002, "Gdansk", "Wroclaw", 6, nullptr, nullptr, nullptr, nullptr);
    Train::addTrain("Regional_303", 120, 250, 1003, "Poznan", "Lodz", 4, nullptr, nullptr, nullptr, nullptr);
}

}