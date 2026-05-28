#ifndef EXTRACT_HPP
#define EXTRACT_HPP
#include <string>
#include <vector>
#include <unordered_map>

class Extract {
private:
    short ants;
    std::unordered_map<std::string, short> rooms;
    std::unordered_map<std::string, std::vector<std::string>> connections;

public:
    Extract();
    bool readFile(const std::string& fileName);
    short getants() const;
    const std::unordered_map<std::string, short>& getRooms() const;
    const std::unordered_map<std::string, std::vector<std::string>>& getConnections() const;
};

#endif