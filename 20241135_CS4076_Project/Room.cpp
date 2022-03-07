#include "Room.h"
#include "Command.h"

// For checking variable types
#include <typeinfo>
#include <string.h>

Room::Room(string name, string description, string backgroundPath,
           typeOfRoom typeOfRoom, bool hasHiddenItem){
    this->name = name;
    this->description = description;
    this->backgroundPath = backgroundPath;
    this->roomType = typeOfRoom;
    this->hasHiddenItem = hasHiddenItem;
}

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

Room::~Room(){}

void Room::operator+(Item *item){
    this->addItem(item);
}

GoalRoom::GoalRoom(string name, string description, string backgroundPath, typeOfRoom typeOfRoom,
                   bool hasHiddenItem, bool goalCompleted){
    Room(name, description, backgroundPath, typeOfRoom, hasHiddenItem);
    this->goalCompleted = goalCompleted;
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

//WordleRoom::WordleRoom(string description, string backgroundPath, int rewardMoney){
//    this->roomType = (typeOfRoom) (WORDLE | GOAL);

//    this->description = description;
//    this->backgroundPath = backgroundPath;
//    this->RewardRoom::rewardType = MONEY;
//    this->RewardRoom::rewardMoney = rewardMoney;
//}


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

string Room::longDescription() {
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
    // See if the name matches instead of direction first
//    for(map<string, Room*>::iterator roomName = exits.begin(); roomName != exits.end(); roomName++){
//        if(direction.compare(roomName->second->getShortDescription())){
//            return roomName->second;
//        }
//    }

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

void Room::completionEvent(){
    return;
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
        int x = (0);
        for (int n = sizeItems; n > 0; n--) {
            // compare inString with short description
            int tempFlag = inString.compare( itemsInRoom[x]->getShortDescription());
            if (tempFlag == 0) {
                //itemsInRoom.erase(itemsInRoom.begin()+x);
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
    return roomType;
}

string GoalRoom::longDescription(){
    if(!this->goalCompleted){
        return Room::longDescription() + "\nThere seems to be a objective to be completed here...\n";

    }
    else{
        return Room::longDescription() + "\nThe objective in the room is completed.\n";

    }
}

void GoalRoom::completionEvent(){

}

