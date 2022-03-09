#ifndef ROOM_H_
#define ROOM_H_

#include <map>
#include <string>
#include <cctype>
#include <vector>

#include <QDebug>

#include "item.h"
#include "Stack.h"
#include "constants.h"

using namespace std;
using std::vector;
using namespace Constants;



// Class holding properties and features that rooms will have
class RoomProperties{
public:
    virtual bool hasItems() = 0;
    virtual void completionEvent() = 0;
    // Using bitmask
    enum typeOfRoom : int {WORDLE = 1, NORMAL = 2, GOAL = 4, STACK = 8};
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

    Room(string name="Generic Area", string description="No description.", string backgroundPath=Constants::NIGHT_CITY_GIF,
         typeOfRoom typeOfRoom=NORMAL, bool hasHiddenItem=false);
    Room(const Room& other);

    ~Room();

    void operator+(Item* item);

    int numberOfItems();
    void setExits(Room *north, Room *east, Room *south, Room *west);
    string getShortDescription();
    void addItem(Item *inItem);
    string displayItem();
    int isItemInRoom(string inString);
    void removeItemFromRoom(int location);
    string getBackgroundPath();
    void setHiddenItem(Item* item);
    void setHiddenClue(string string);
    typeOfRoom getTypeOfRoom();
    vector<Item*> getAllItems();

    // Display different long descriptions for different types of rooms.
    virtual string longDescription();

    bool hasItems() override;
    void completionEvent() override;

protected:
    string name;
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

// Room with reward
class RewardRoom{
public:
    enum typeOfReward : int{ITEM, MONEY, CLUE, NONE};
protected:
    union {
        Item* rewardItem;
        int rewardMoney;
        string rewardClue;
        vector<Item*> rewardItemsMultiple;
    };

public:
    Item* giveItemReward();
    int giveMoneyReward();
    string giveClueReward();
    vector<Item*> giveMultipleItemsReward();

    typeOfReward rewardType;

    RewardRoom();
    ~RewardRoom();
};



// Subclass GoalRoom with a specific goal to complete
class GoalRoom : public Room{
private:
    bool goalCompleted;

public:
    GoalRoom(string name="Generic Goal Area", string description="No description.",
             string backgroundPath=Constants::NIGHT_CITY_GIF, typeOfRoom=GOAL,
             bool hasHiddenItem=false, bool goalCompleted=false);
    ~GoalRoom();

    // Unique description for long descriptions
    string longDescription() override;
    void setGoalStatus(bool status);
    bool getGoalStatus();

    // Function to check if a particular goal for a room is completed.
    string (*checkIfGoalCompleted)(GoalRoom* room);
    void setCheckGoalFunction(string (*checkGoalCompleted)(GoalRoom* room));

    void completionEvent() override;
};

class WordleRoom : public GoalRoom, public RewardRoom{
public:
    WordleRoom(string description, string backgroundPath, int moneyReward);
    WordleRoom(int moneyReward=0, string name="Generic Wordle Area", string description="No description.", string backgroundPath=Constants::NIGHT_CITY_GIF,
               typeOfRoom=(typeOfRoom) (WORDLE | GOAL), bool hasHiddenItem=false, bool goalCompleted=false);
    WordleRoom(string clueReward=0, string name="Generic Wordle Area", string description="No description.", string backgroundPath=Constants::NIGHT_CITY_GIF,
               typeOfRoom=(typeOfRoom) (WORDLE | GOAL), bool hasHiddenItem=false, bool goalCompleted=false);
    WordleRoom(Item* itemReward=0, string name="Generic Wordle Area", string description="No description.", string backgroundPath=Constants::NIGHT_CITY_GIF,
               typeOfRoom=(typeOfRoom) (WORDLE | GOAL), bool hasHiddenItem=false, bool goalCompleted=false);
};

// Room that contains a stack
class StackRoom : public Room{
private:
    // A stack of items?
    Stack<Item*> roomStack;
};

namespace GoalCheckFunctions{
string checkPlainFunc(GoalRoom* room);

string checkPeiCompleteFunc(GoalRoom* room);
}


#endif
