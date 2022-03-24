#include "Room.h"
#include "Command.h"
#include "ZorkUL.h"

#include <string.h>
#include "dialogues.h"


namespace InteractFunctions{
string interactPlain(Room* room){
    return "Plain interaction.";
}

string interactPlainGoal(GoalRoom* room){
    return "Objective to be completed";
}

string interactDescription(Room* room){
    return room->getShortDescription();
}
string interactClawMachine(Room* room){
    string output = "";

    if(ZorkUL::getMoney() >= 2){
        ZorkUL::changeMoney(-2);
        Raffle* raffle = new Raffle(100, 0.2);
        ZorkUL::addItem(raffle);
        output += "You put in a bit of money and fiddle with the claw machine"
                  " for a bit. Luckily, you managed to snag a raffle ticket!";
        vector<Item*> items = ZorkUL::getAllItems();
    }
    else{
        output += "You don't have enough money for the claw machine.";
    }

    return output;
}

string interactCafe(GoalRoom* room){
    if(room->getGoalStatus()){
        return InteractDialogues::cafeCompleteInteract;
    }
    else{
        return InteractDialogues::cafeNotCompleteInteract;
    }
}
}

Room::Room(string name, string description, string backgroundPath,
           typeOfRoom typeOfRoom, bool hasHiddenItem){
    this->name = name;
    this->description = description;
    this->backgroundPath = backgroundPath;
    this->roomType = typeOfRoom;
    this->hasHiddenItem = hasHiddenItem;
    this->interactFunc = &(InteractFunctions::interactDescription);
}

Room::Room(string(*interactFunc)(Room*), string name, string description, string backgroundPath,
           typeOfRoom typeOfRoom, bool hasHiddenItem){
    this->name = name;
    this->description = description;
    this->backgroundPath = backgroundPath;
    this->roomType = typeOfRoom;
    this->hasHiddenItem = hasHiddenItem;
    this->interactFunc = interactFunc;
}

// Copy constructor
Room::Room(const Room& other){
    if(hasHiddenItem){
        return;
        //this->hiddenItem = new Item(other.hiddenItem);
    }
    else{
        this->itemsInRoom = other.itemsInRoom;
        this->hiddenClue = other.hiddenClue;
    }
}

Room::~Room(){
    for(auto& item : this->getAllItems()){
        delete item;
    }

    this->getAllItems().clear();

}

void Room::operator+(Item *item){
    this->addItem(item);
}

void Room::setAllItems(vector<Item*> items){
    this->itemsInRoom = items;
}

bool Room::deleteItemByName(string name){
    vector<Item*> items = this->getAllItems();
    for(int i = 0; i < (int) (items.size()); i++){
//        if(items.at(i)->getShortDescription().compare(name) == 0){
        if(ZorkUL::compareIgnoreCase(items.at(i)->getShortDescription(), name)){
            delete items.at(i);
            items.erase(items.begin() + i);
            this->setAllItems(items);
            return true;
        }
    }
    return false;
}

void Room::deleteAllItems(){
    for(auto& iPtr : this->getAllItems()){
        delete iPtr;
    }
    this->getAllItems().clear();
}

GoalRoom::GoalRoom(string name, string description, string backgroundPath, typeOfRoom typeOfRoom,
                   bool hasHiddenItem, bool goalCompleted)
    : Room(name, description, backgroundPath, typeOfRoom, hasHiddenItem),
      checkIfGoalCompleted(&(GoalCheckFunctions::checkPlainFunc)){
    this->goalCompleted = goalCompleted;

    if(checkIfGoalCompleted != NULL){
        this->setCheckGoalFunction(checkIfGoalCompleted);
    }
}

GoalRoom::GoalRoom(string(*goalFunc)(GoalRoom*), string name, string description,
                   string backgroundPath, typeOfRoom roomType,
                   bool hasHiddenItem, bool goalCompleted) : Room(name, description, backgroundPath, roomType, hasHiddenItem){
    this->goalCompleted = goalCompleted;
    this->checkIfGoalCompleted = goalFunc;

}

GoalRoom::GoalRoom(string(*interactFunc)(GoalRoom*), string(*goalFunc)(GoalRoom*), string name, string description,
                   string backgroundPath, typeOfRoom roomType, bool hasHiddenItem, bool goalCompleted)
    : Room(name, description, backgroundPath, roomType, hasHiddenItem){
    this->goalCompleted = goalCompleted;
    this->checkIfGoalCompleted = goalFunc;
    this->interactFunc = interactFunc;

}

GoalRoom::~GoalRoom(){}

void GoalRoom::setGoalStatus(bool status){
    this->goalCompleted = status;
}

bool GoalRoom::getGoalStatus(){
    return this->goalCompleted;
}

RewardRoom::RewardRoom(){}
RewardRoom::~RewardRoom(){}

// For giving monetary reward
int RewardRoom::giveMoneyReward(){
    int reward = 0;
    if(this->rewardType == MONEY){
        reward = this->rewardMoney;
        this->rewardMoney = 0;
    }
    return reward;
}

string RewardRoom::giveClueReward(){
    string reward = "";
    if(this->rewardType == CLUE){
        reward = this->rewardClue;
    }
    return reward;
}

Item* RewardRoom::giveItemReward(){
    Item* reward = NULL;
    if(this->rewardType == ITEM){
        reward = this->rewardItem;
        this->rewardItem = NULL;
    }
    return reward;
}

WordleRoom::WordleRoom(string(*interactFunc)(GoalRoom*), int moneyReward, string name, string description, string backgroundPath,
                       typeOfRoom typeOfRoom, bool hasHiddenItem, bool goalCompleted)
    : GoalRoom(interactFunc, name, description, backgroundPath, typeOfRoom, hasHiddenItem, goalCompleted){
    this->rewardType = MONEY;
    this->RewardRoom::rewardMoney = moneyReward;
    this->interactFunc = interactFunc;
}

WordleRoom::WordleRoom(string(*interactFunc)(GoalRoom*), string clueReward, string name, string description, string backgroundPath,
                       typeOfRoom typeOfRoom, bool hasHiddenItem, bool goalCompleted)
    : GoalRoom(interactFunc, name, description, backgroundPath, typeOfRoom, hasHiddenItem, goalCompleted){
    this->rewardType = CLUE;
    this->RewardRoom::rewardClue = clueReward;
    this->GoalRoom::setGoalStatus(goalCompleted);
    this->interactFunc = interactFunc;
}

WordleRoom::WordleRoom(string(*interactFunc)(GoalRoom*), Item* itemReward, string name, string description, string backgroundPath,
                       typeOfRoom typeOfRoom, bool hasHiddenItem, bool goalCompleted)
    : GoalRoom(interactFunc, name, description, backgroundPath, typeOfRoom, hasHiddenItem, goalCompleted){
    this->rewardType = ITEM;
    this->RewardRoom::rewardItem = itemReward;
    this->GoalRoom::setGoalStatus(goalCompleted);
    this->interactFunc = interactFunc;
}


void Room::setExits(Room *north, Room *east, Room *south, Room *west) {
    if (north != NULL)
        exits["north"] = north;
    if (east != NULL)
        exits["east"] = east;
    if (south != NULL)
        exits["south"] = south;
    if (west != NULL)
        exits["west"] = west;
}

string Room::getShortDescription() {
    return description;
}

string Room::getLongDescription() {
    return "Room: " + description + "\n-\n" + displayItem() + exitString();
}

string Room::exitString() {
    string returnString = "\n-\nExits: ";
    for (map<string, Room*>::iterator i = exits.begin(); i != exits.end(); i++)
        // Loop through map
        returnString += "  " + capitaliseFirst(i->first);
    //returnString += "  " + i->first;	// access the "first" element of the pair (direction as a string)
    returnString += "\n";
    return returnString;
}

Room* Room::nextRoom(string direction) {

    map<string, Room*>::iterator next = exits.find(direction); //returns an iterator for the "pair"
    if (next == exits.end())
        return NULL; // if exits.end() was returned, there's no room in that direction.
    return next->second; // If there is a room, remove the "second" (Room*)
    // part of the "pair" (<string, Room*>) and return it.
}

bool Room::hasItems(){
    if(itemsInRoom.size() == 0){
        return false;
    }
    else{
        return true;
    }
}

void Room::addItem(Item *inItem) {
    //cout <<endl;
    //cout << "Just added" + inItem->getLongDescription();
    itemsInRoom.push_back(inItem);
}

string Room::displayItem() {
    string tempString = "Items in room: ";
    int sizeItems = (int)(itemsInRoom.size());
    if (itemsInRoom.size() < 1) {
        tempString = "There are no items in this room.";
    }
    else if (itemsInRoom.size() > 0) {
        int x = (0);
        for (int n = sizeItems; n > 0; n--) {
            tempString = tempString + itemsInRoom[x]->getShortDescription() + "  " ;
            x++;
        }
    }
    return tempString;
}

int Room::numberOfItems() {
    return (int)itemsInRoom.size();
}

int Room::isItemInRoom(string inString)
{
    int sizeItems = (int)(itemsInRoom.size());
    if (itemsInRoom.size() < 1) {
        return -1;
    }
    else if (itemsInRoom.size() > 0) {
        int x = 0;
        for (int n = sizeItems; n > 0; n--) {
            // compare inString with short description
            if(ZorkUL::compareIgnoreCase(inString, itemsInRoom[x]->getShortDescription())){
                return x;
            }
            x++;
        }
    }
    return -1;
}

string Room::getBackgroundPath(){
    return this->backgroundPath;
}

// Capitalises the first letter
string Room::capitaliseFirst(string input){
    input[0] = toupper(input[0]);
    return input;
}

Room::typeOfRoom Room::getTypeOfRoom(){
    return this->roomType;
}

vector<Item*> Room::getAllItems(){
    return this->itemsInRoom;
}

string GoalRoom::getLongDescription(){
    if(!this->goalCompleted){
        return Room::getLongDescription() + "\nThere seems to be a objective to be completed here...\n";

    }
    else{
        return Room::getLongDescription() + "\nThe objective in the room is completed.\n";

    }
}

void GoalRoom::setCheckGoalFunction(string (*checkIfGoalCompleted)(GoalRoom *)){
    this->checkIfGoalCompleted = checkIfGoalCompleted;
}

namespace GoalCheckFunctions{
string checkPlainFunc(GoalRoom* room){
    return "";
}

string checkPeiCompleteFunc(GoalRoom* currentRoom){
    string output = "";
    string goalItem = "frog";

    // I want to remove a particular item from the list that matches the name of the goal item.
    if(currentRoom->deleteItemByName(goalItem)){
        currentRoom->setGoalStatus(true);
        output += CompletionDialogues::peiComplete;
        ZorkUL::changeMoney(1000);

    }

    return output;
}

string checkCafeCompleteFunc(GoalRoom* currentRoom){
    string output = "";
    string goalItem = "pen";

    if(currentRoom->deleteItemByName(goalItem)){
        currentRoom->setGoalStatus(true);
        output += CompletionDialogues::cafeComplete;
        ZorkUL::changeMoney(2);
    }

    if(currentRoom->getGoalStatus() == false){
        output += CompletionDialogues::cafeNotComplete;
    }


    return output;
}

string checkFinalGoalFunc(GoalRoom* room){
    string output = "";

    if(ZorkUL::getMoney() >= ZorkUL::getGoalMoney()){
        room->setGoalStatus(true);
        output += CompletionDialogues::finalGoalComplete;
    }
    else{
        output += CompletionDialogues::finalGoalNotComplete;
    }
    return output;
}
}

