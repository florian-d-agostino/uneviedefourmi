#ifndef ROOM_HPP
#define ROOM_HPP

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;





class Room {
private:
    string name;
    int Antmax;
    int Ant;
    vector<Room*> Direction;




public:
    // Constructor
    Room(const string& name, int Antmax);

    // Getters
    string getName() const;
    int getAntmax() const;
    int getAnt() const;
    const vector<Room*>& getDirection() const;

    // Methods
    void linkRoom(Room* neighborRoom);
    bool enter();
    void leave();
    bool isFull() const;
};



// Connections dictionaries
unordered_map<string, Room*> buildRoomGraph(
    const unordered_map<string, int>& dico1,
    const unordered_map<string, vector<string>>& dico2
);

#endif
