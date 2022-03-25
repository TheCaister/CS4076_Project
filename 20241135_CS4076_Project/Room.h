#ifndef ROOM_H_
#define ROOM_H_

#include "item.h"
#include "Stack.h"
#include "constants.h"

using namespace std;
using std::vector;
using namespace Constants;

class Room;
class GoalRoom;

// All the different goal conditions that can be assigned
// to a room
namespace GoalCheckFunctions{
string checkPlainFunc(GoalRoom*);

string checkPeiCompleteFunc(GoalRoom*);

string checkCafeCompleteFunc(GoalRoom*);

string checkFinalGoalFunc(GoalRoom*);
}

// All the different interact functions that can
// be assigned to a room
namespace InteractFunctions{
string interactPlain(Room*);
string interactDescription(Room*);
string interactPlainGoal(GoalRoom*);
string interactCafe(GoalRoom*);
string interactClawMachine(Room*);
string interactChineseRestaurant(Room*);
}

// Abstract class holding properties and features that rooms will have
class RoomProperties{
public:
    virtual bool hasItems() = 0;

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
    Room(string (*interactFunc)(Room*)=&(InteractFunctions::interactPlain), string name="Generic Area", string description="No description.", string backgroundPath=Constants::NIGHT_CITY_GIF,
         typeOfRoom typeOfRoom=NORMAL, bool hasHiddenItem=false);
    Room(const Room& other);

    virtual ~Room();

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
    void setAllItems(vector<Item*> items);
    void deleteAllItems();
    bool deleteItemByName(string name);

    // Display different long descriptions for different types of rooms.
    virtual string getLongDescription();

    bool hasItems() override;


    void setInteractionFunction(string (*interactionFunc)(Room*));


protected:
    string name;
    string description;
    map<string, Room*> exits;
    string exitString();
    vector <Item*> itemsInRoom;
    string backgroundPath;
    typeOfRoom roomType;
    Room* nextRoom(string direction);
    string capitaliseFirst(string input);
    // Function to check if a particular goal for a room is completed.
    string (*interactFunc)(Room*);
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
    virtual ~RewardRoom();
};



// Subclass GoalRoom with a specific goal to complete
class GoalRoom : public Room{
private:
    bool goalCompleted;


public:
    GoalRoom(string name="Generic Goal Area", string description="No description.",
             string backgroundPath=Constants::NIGHT_CITY_GIF, typeOfRoom=GOAL,
             bool hasHiddenItem=false, bool goalCompleted=false);
    GoalRoom(string(*goalFunc)(GoalRoom*)=&(GoalCheckFunctions::checkPlainFunc), string name="Generic Goal Area", string description="No description.",
             string backgroundPath=Constants::NIGHT_CITY_GIF, typeOfRoom=GOAL,
             bool hasHiddenItem=false, bool goalCompleted=false);
    GoalRoom(string(*interactFunc)(GoalRoom*)=&(InteractFunctions::interactPlainGoal), string(*goalFunc)(GoalRoom*)=&(GoalCheckFunctions::checkPlainFunc), string name="Generic Goal Area", string description="No description.",
             string backgroundPath=Constants::NIGHT_CITY_GIF, typeOfRoom=GOAL,
             bool hasHiddenItem=false, bool goalCompleted=false);

    virtual ~GoalRoom();

    // Unique description for long descriptions
    string getLongDescription() override;
    void setGoalStatus(bool status);
    bool getGoalStatus();

    // Function to check if a particular goal for a room is completed.
    string (*checkIfGoalCompleted)(GoalRoom* room);

    string (*interactFunc)(GoalRoom* room);
    void setCheckGoalFunction(string (*checkGoalCompleted)(GoalRoom* room));

};

class WordleRoom : public GoalRoom, public RewardRoom{
public:
    WordleRoom(string description, string backgroundPath, int moneyReward);
//    WordleRoom(int moneyReward=0, string name="Generic Wordle Area", string description="No description.", string backgroundPath=Constants::NIGHT_CITY_GIF,
//               typeOfRoom=(typeOfRoom) (WORDLE | GOAL), bool hasHiddenItem=false, bool goalCompleted=false);
//    WordleRoom(string clueReward=0, string name="Generic Wordle Area", string description="No description.", string backgroundPath=Constants::NIGHT_CITY_GIF,
//               typeOfRoom=(typeOfRoom) (WORDLE | GOAL), bool hasHiddenItem=false, bool goalCompleted=false);
//    WordleRoom(Item* itemReward=0, string name="Generic Wordle Area", string description="No description.", string backgroundPath=Constants::NIGHT_CITY_GIF,
//               typeOfRoom=(typeOfRoom) (WORDLE | GOAL), bool hasHiddenItem=false, bool goalCompleted=false);

    WordleRoom(string(*interactFunc)(GoalRoom*)=&(InteractFunctions::interactPlainGoal), int moneyReward=0, string name="Generic Wordle Area", string description="No description.", string backgroundPath=Constants::NIGHT_CITY_GIF,
               typeOfRoom=(typeOfRoom) (WORDLE | GOAL), bool hasHiddenItem=false, bool goalCompleted=false);
    WordleRoom(string(*interactFunc)(GoalRoom*)=&(InteractFunctions::interactPlainGoal), string clueReward=0, string name="Generic Wordle Area", string description="No description.", string backgroundPath=Constants::NIGHT_CITY_GIF,
               typeOfRoom=(typeOfRoom) (WORDLE | GOAL), bool hasHiddenItem=false, bool goalCompleted=false);
    WordleRoom(string(*interactFunc)(GoalRoom*)=&(InteractFunctions::interactPlainGoal), Item* itemReward=0, string name="Generic Wordle Area", string description="No description.", string backgroundPath=Constants::NIGHT_CITY_GIF,
               typeOfRoom=(typeOfRoom) (WORDLE | GOAL), bool hasHiddenItem=false, bool goalCompleted=false);

};

//// Room that contains a stack
//class StackRoom : public Room{
//private:
//    // A stack of items?
//    Stack<Item*> roomStack;
//};


#endif
