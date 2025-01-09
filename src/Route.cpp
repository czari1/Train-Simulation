#include "Route.hpp"
#include "Station.hpp"
#include "Train.hpp"
#include <iostream>

namespace CJ {

Route routeInstance1(8, 30, 11, 45, nullptr, nullptr, nullptr, 195, 
    {"Warsaw Central", "Lodz Widzew", "Czestochowa", "Krakow Main"});
Route routeInstance2(9, 15, 13, 30, nullptr, nullptr, nullptr, 255,
    {"Gdansk Central", "Bydgoszcz", "Poznan", "Wroclaw Main"});
Route routeInstance3(7, 0, 9, 30, nullptr, nullptr, nullptr, 150,
    {"Warsaw Central", "Radom", "Kielce", "Krakow Main"});

void linkRouteWithStations() {
    routeInstance1.addIntermediateStop("Lodz Widzew");
    routeInstance1.addIntermediateStop("Czestochowa");
    
    routeInstance2.addIntermediateStop("Bydgoszcz");
    routeInstance2.addIntermediateStop("Poznan");
    
    routeInstance3.addIntermediateStop("Radom");
    routeInstance3.addIntermediateStop("Kielce");
}

}