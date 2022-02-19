#ifndef ROOM_H_
#define ROOM_H_

#include <map>
#include <string>
#include <vector>
#include "item.h"
using namespace std;
using std::vector;

class Room {

public:
    enum typeOfRoom : int {WORDLE, NORMAL};

private:
    string description;
    map<string, Room*> exits;
    string exitString();
    vector <Item> itemsInRoom;
    string backgroundPath;
    typeOfRoom roomType;


public:


    Room(string description);
    Room(string description, string backgroundPath);
    Room(string description, string backgroundPath, typeOfRoom typeOfRoom);

    int numberOfItems();
    void setExits(Room *north, Room *east, Room *south, Room *west);
    string shortDescription();
    string longDescription();
    Room* nextRoom(string direction);
    void addItem(Item *inItem);
    string displayItem();
    int isItemInRoom(string inString);
    void removeItemFromRoom(int location);
    string getBackgroundPath();

    typeOfRoom getTypeOfRoom(){
        return roomType;
    }
};

// Subclass GoalRoom with a specific goal to complete
class GoalRoom : public Room{
private:
    bool goalCompleted;

public:
    GoalRoom(string description);
    GoalRoom(string description, string backgroundPath);
    GoalRoom(string description, string backgroundPath, typeOfRoom typeOfRoom);
    void completeGoal();
    void getGoalStatus();
};

#endif
