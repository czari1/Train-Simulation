# Train Simulation System

A C++ application for managing train routes, stations, and schedules. This system allows users to create and manage train routes, stations, and track train assignments through a command-line interface.

## Features

- **Station Management**

  - Add/Remove stations
  - Set platform count for each station
  - Display station information
  - Store station data persistently

- **Train Management**

  - Add/Remove trains
  - Set train properties (speed, capacity, wagon count)
  - Display train information
  - Track train assignments

- **Route Management**
  - Create routes between stations
  - Automatic duration calculation based on departure and arrival times
  - Multiple stops support
  - Display route information

## Technical Details

- **Language**: C++17
- **Database**: SQLite3
- **Build System**: Make
- **Platform**: Windows
- **Dependencies**:
  - SQLite3
  - C++ Standard Library

## Database Schema

The system uses SQLite3 with the following tables:

- `stations`: Stores station information
- `trains`: Stores train information
- `routes`: Stores route information
- `route_stops`: Links routes with their stops
- `train_routes`: Links trains with their assigned routes

## Building the Project

1. Ensure you have the following prerequisites:

   - MinGW-w64 with g++ compiler
   - SQLite3 development libraries
   - Make build system

2. Clone the repository:

   ```bash
   git clone [repository-url]
   cd train-simulation
   ```

3. Build the project:
   ```bash
   mingw32-make
   ```

## Usage

1. Run the application:

   ```bash
   ./train_simulation.exe
   ```

2. Use the menu system to:
   - Add/remove stations
   - Add/remove trains
   - Create routes
   - Assign trains to routes
   - View system information

## Example Operations

1. Adding a Station:

   ```
   2) Station Operations
   1) Add New Station
   Enter station name: Warsaw Central
   Enter platform count: 5
   ```

2. Creating a Route:
   ```
   3) Route Operations
   1) Add Route
   Enter stops: Warsaw Central -> Krakow Main
   Enter departure time: 08:30
   Enter arrival time: 11:45
   ```

## Project Structure

- `src/`: Source files
- `include/`: Header files
- `database/`: SQLite database files
- `obj/`: Compiled object files
- `Makefile`: Build configuration

