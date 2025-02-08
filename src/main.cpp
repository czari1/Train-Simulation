#include <iostream>
#include "../include/Train.hpp"
#include "../include/Route.hpp"
#include "../include/Station.hpp"
#include "../include/CLI.hpp"
#include "../include/Management.hpp"

int main() {
    // Initialize the system using Management class
    CJ::Management::initializeSystem();

    std::cout << "\nTesting Train Display:\n";
    CJ::Management::displayTrainInfo(1001);
    CJ::Management::displayTrainInfo(1002);

    std::cout << "\nTesting Station Display:\n";
    CJ::Management::displayStationInfo("Warsaw Central");
    CJ::Management::displayStationInfo("Krakow Main");

    std::cout << "\nTesting Route Display:\n";
    CJ::Management::displayAllRoutes();

    std::cout << "\nTesting Deletion:\n";
    if (CJ::Management::deleteTrain(1001)) {
        std::cout << "Train 1001 successfully deleted\n";
    }
    if (CJ::Management::removeStation("Warsaw Central")) {
        std::cout << "Warsaw Central station successfully deleted\n";
    }
    
    

    CJ::CLI::run();

    return 0;
}