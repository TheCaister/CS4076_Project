#ifndef ROOM_H_
#define ROOM_H_

#include <map>
#include <string>
#include <cctype>
#include <vector>
#include "item.h"
#include "Stack.h"
using namespace std;
using std::vector;

// Class holding properties and features that rooms will have
class RoomProperties{
public:
    virtual bool hasItems() = 0;
    enum typeOfRoom : int {WORDLE, NORMAL, GOAL, STACK};
};

class Room : public RoomProperties{
    friend class ZorkUL;

public:
    bool hasHiddenItem;

    // A room can have either a hidden or a hidden clue to be revealed.
    // Placed these in an union to save memory space
    union {
        Item* hiddenItem;
        string hiddenClue;
    };

    Room(string description);
    Room(string description, string backgroundPath);
    Room(string description, string backgroundPath, typeOfRoom typeOfRoom);
    Room(const Room& other);

    ~Room();

    void operator+(Item* item);

    int numberOfItems();
    void setExits(Room *north, Room *east, Room *south, Room *west);
    string shortDescription();
    void addItem(Item *inItem);
    string displayItem();
    int isItemInRoom(string inString);
    void removeItemFromRoom(int location);
    string getBackgroundPath();
    void setHiddenItem(Item* item);
    void setHiddenClue(string string);
    typeOfRoom getTypeOfRoom();

    // Display different long descriptions for different types of rooms.
    virtual string longDescription();

    bool hasItems() override;

protected:
    string description;
    map<string, Room*> exits;
    string exitString();
    vector <Item*> itemsInRoom;
    string backgroundPath;
    typeOfRoom roomType;
    Room* nextRoom(string direction);

private:
    string capitaliseFirst(string input);
};

// Subclass GoalRoom with a specific goal to complete
class GoalRoom : public Room{
private:
    bool goalCompleted;

public:
    GoalRoom(string description);
    GoalRoom(string description, string backgroundPath);
    GoalRoom(string description, string backgroundPath, typeOfRoom typeOfRoom);

    // Unique description for long descriptions
    string longDescription() override;
    void completeGoal();
    void getGoalStatus();
};

class WordleRoom : public Room{

};

// Room that contains a stack
class StackRoom : public Room{
private:
    // A stack of items?
    Stack<Item*> roomStack;
};


#endif
