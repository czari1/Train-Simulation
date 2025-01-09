#include <iostream>
#include "../include/Train.hpp"
#include "../include/Route.hpp"
#include "../include/Station.hpp"
#include "../include/CLI.hpp"

namespace CJ {

    void initializeTrains();
    void initializeStations();
    void linkRouteWithStations();
    
    extern Route routeInstance1;
    extern Route routeInstance2;
    extern Route routeInstance3;

} 

int main() {
    CJ::initializeTrains();
    CJ::initializeStations();
    CJ::linkRouteWithStations();

    std::cout << "\nTesting Train Display:\n";
    CJ::Train::displayTrainInfo(1001);
    CJ::Train::displayTrainInfo(1002);

    std::cout << "\nTesting Station Display:\n";
    CJ::Station::displayStationInfo("Warsaw Central");
    CJ::Station::displayStationInfo("Krakow Main");

    std::cout << "\nTesting Route Display:\n";
    CJ::routeInstance1.displayRoute();
    CJ::routeInstance2.displayRoute();

    std::cout << "\nTesting Deletion:\n";
    if (CJ::Train::deleteTrain(1001)) {
        std::cout << "Train 1001 successfully deleted\n";
    }
    if (CJ::Station::removeStation("Warsaw Central")) {
        std::cout << "Warsaw Central station successfully deleted\n";
    }

    CJ::CLI::run();

    return 0;
}
