#ifndef ROOM_HPP
#define ROOM_HPP

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;





class Room {
private:
    string name;
    short Antmax;
    short Ant;
    vector<Room*> Direction;




public:
    // Constructor
    Room(const string& name, short Antmax);

    // Getters and Setters
    string getName() const;
    short getAntmax() const;
    short getAnt() const;
    const vector<Room*>& getDirection() const;
    void setAnt(short count);
    void setAntmax(short max);

    // Methods
    void linkRoom(Room* neighborRoom);
    bool enter();
    void leave();
    bool isFull() const;
};


#endif
