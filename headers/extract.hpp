#ifndef EXTRACT_HPP
#define EXTRACT_HPP
#include <string>
#include <vector>
#include <unordered_map>

class Extract {
private:
    short ants;
    std::unordered_map<std::string, short> rooms;
    std::unordered_map<std::string, std::vector<std::string>> connexions;

public:
    Extract();
    bool lireFichier(const std::string& nomFichier);
    short getants() const;
    const std::unordered_map<std::string, short>& getRooms() const;
    const std::unordered_map<std::string, std::vector<std::string>>& getConnexions() const;
};

#endif