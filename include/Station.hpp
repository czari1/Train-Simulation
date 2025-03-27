#pragma once
#include <string>
#include <vector>
#include <memory>

namespace CJ {

class Route;
class Train;

class Station {
private:
    std::string m_name;                                      
    int m_platformCount;                                     
    std::shared_ptr<Train> m_trainName;                     
    std::vector<std::shared_ptr<Route>> m_intermediateStops; 
    std::shared_ptr<Train> m_startStation;                  
    std::shared_ptr<Train> m_endStation;                    

public:
   
    Station(std::shared_ptr<Train> trainName = nullptr, 
            int platformCount = 0,  
            const std::vector<std::shared_ptr<Route>>& intermediateStops = {}, 
            std::shared_ptr<Train> startStation = nullptr, 
            std::shared_ptr<Train> endStation = nullptr,
            const std::string& name = "");


    void setTrainName(std::shared_ptr<Train> trainName);
    void setPlatformCount(int platformCount);
    void setIntermediateStops(const std::vector<std::shared_ptr<Route>>& intermediateStops);
    void setStartStation(std::shared_ptr<Train> startStation);
    void setEndStation(std::shared_ptr<Train> endStation);
    void setName(const std::string& name);


    std::shared_ptr<Train> getTrainName() const;
    int getPlatformCount() const;
    const std::vector<std::shared_ptr<Route>>& getIntermediateStops() const;
    std::shared_ptr<Train> getStartStation() const;
    std::shared_ptr<Train> getEndStation() const;
    const std::string& getName() const;
};

} // namespace CJ