#ifndef ROOM_H_
#define ROOM_H_

#include <map>
#include <string>
#include <vector>
#include "item.h"
using namespace std;
using std::vector;


class RoomProperties{
public:
    virtual bool hasItems() = 0;

};

class Room : public RoomProperties{
    friend class ZorkUL;

public:
    enum typeOfRoom : int {WORDLE, NORMAL};
    bool hasHiddenItem;

    // A room can have either a hidden or a hidden clue to be revealed.
    union {
        Item* hiddenItem;
        string hiddenClue;

    };

protected:
    string description;
    map<string, Room*> exits;
    string exitString();
    vector <Item> itemsInRoom;
    string backgroundPath;
    typeOfRoom roomType;
    Room* nextRoom(string direction);


public:
    Room(string description);
    Room(string description, string backgroundPath);
    Room(string description, string backgroundPath, typeOfRoom typeOfRoom);
    Room(const Room& other);

    ~Room();

    void operator+(Item* item);

    int numberOfItems();
    void setExits(Room *north, Room *east, Room *south, Room *west);
    string shortDescription();
    // Display different long descriptions for different types of rooms.
    virtual string longDescription();

    void addItem(Item *inItem);
    string displayItem();
    int isItemInRoom(string inString);
    void removeItemFromRoom(int location);
    string getBackgroundPath();

    void setHiddenItem(Item* item);
    void setHiddenClue(string string);

    typeOfRoom getTypeOfRoom(){
        return roomType;
    }

    bool hasItems() override;
};

// Subclass GoalRoom with a specific goal to complete
class GoalRoom : public Room{
private:
    bool goalCompleted;

public:
    GoalRoom(string description);
    GoalRoom(string description, string backgroundPath);
    GoalRoom(string description, string backgroundPath, typeOfRoom typeOfRoom);
    string longDescription() override;
    void completeGoal();
    void getGoalStatus();
};


#endif
