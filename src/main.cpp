#include <iostream>
#include <stdexcept>
#include <filesystem>
#include "../include/Train.hpp"
#include "../include/Route.hpp"
#include "../include/Station.hpp"
#include "../include/CLI.hpp"
#include "../include/Management.hpp"
#include "../include/DatabaseManager.hpp"

int main() {
    try {
        std::cout << "Starting Train Simulation System..." << std::endl;
        
        if (!CJ::Management::initializeSystem()) {
            std::cerr << "Failed to initialize system!" << std::endl;
            return 1;
        }

        std::filesystem::path dbPath = std::filesystem::current_path() / "database" / "train_system.db";
        if (!std::filesystem::exists(dbPath)) {
            std::cerr << "Database file was not created at: " << dbPath << std::endl;
            return 1;
        }

        CJ::DatabaseManager dbManager;

        std::cout << "Database file created successfully at: " << dbPath << std::endl;

        if (dbManager.connect()) {
            std::cout << "Database connection successful" << std::endl;
            dbManager.displayDatabaseContents();
        } else {
            std::cerr << "Failed to connect to database!" << std::endl;
        }
        CJ::CLI Cli;
        Cli.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}