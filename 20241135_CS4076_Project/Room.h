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
    union reward{
        Item* rewardItem;
        int rewardMoney;
        string rewardClue;
    };

public:
    Item* giveItemReward();
    int giveMoneyReward();
    string giveClueReward();

    typeOfReward rewardType;

    RewardRoom();
    ~RewardRoom();
};

// Subclass GoalRoom with a specific goal to complete
class GoalRoom : public Room{
private:
    bool goalCompleted;

public:
    GoalRoom(string name="Generic Goal Area", string description="No description.", string backgroundPath=Constants::NIGHT_CITY_GIF, typeOfRoom=GOAL,
             bool hasHiddenItem=false, bool goalCompleted=false);
    ~GoalRoom();

    // Unique description for long descriptions
    string longDescription() override;
    void setGoalStatus(bool status);
    bool getGoalStatus();

    void completionEvent() override;
};

template <class RewardType>
class WordleRoom : public GoalRoom, public RewardRoom{
public:
    //WordleRoom(string description, string backgroundPath, int moneyReward);
    // Can accept different reward types

    //    WordleRoom(RewardType reward=false, string name="Generic Wordle Area", string description="No description.", string backgroundPath=Constants::NIGHT_CITY_GIF,
    //               typeOfRoom=(typeOfRoom) (WORDLE | GOAL), bool hasHiddenItem=false, bool goalCompleted=false);
    WordleRoom<RewardType>(RewardType reward=false, string name="Generic Wordle Area", string description="No description.", string backgroundPath=Constants::NIGHT_CITY_GIF,
                           typeOfRoom roomType=(typeOfRoom)(WORDLE | GOAL), bool hasHiddenItem=false, bool goalCompleted=false){
        GoalRoom(name, description, backgroundPath, roomType, hasHiddenItem, goalCompleted);

        Item* testItemPtr;
        int testInt;
        string testString;


        qDebug(typeid(reward).name());
        qDebug(typeid(testItemPtr).name());
        qDebug(typeid(testInt).name());
        qDebug(typeid(testString).name());
        // Checking the type of reward passed in and assigning it.
        if(std::string(typeid(reward).name()).compare(std::string(typeid(testItemPtr).name())) == 0){
            qDebug("Bruh");
            this->RewardRoom::reward = reward;
            //this->rewardType = RewardRoom::ITEM;
        }
        else if(std::string(typeid(reward).name()).compare(std::string(typeid(testInt).name())) == 0){
            this->RewardRoom::reward = reward;
            this->rewardType = RewardRoom::MONEY;
        }
        else if(std::string(typeid(reward).name()).compare(std::string(typeid(testString).name())) == 0){
            this->RewardRoom::reward = reward;
            this->rewardType = RewardRoom::CLUE;
        }
        else{
            this->rewardType = RewardRoom::NONE;
        }
    }
};

// Room that contains a stack
class StackRoom : public Room{
private:
    // A stack of items?
    Stack<Item*> roomStack;
};


#endif
