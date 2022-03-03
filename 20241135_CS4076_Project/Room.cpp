#include "Room.h"
#include "Command.h"

// Setting default background
Room::Room(string description)
    : backgroundPath(":/img/img/night_city.jpg"), roomType(Room::NORMAL){
    this->description = description;
}

Room::Room(string description, string backgroundPath)
    : roomType(Room::NORMAL){
    this->description = description;

    // Setting default background path;
    this->backgroundPath = backgroundPath;
}

Room::Room(string description, string backgroundPath, typeOfRoom typeOfRoom){
    this->description = description;

    // Setting default background path;
    this->backgroundPath = backgroundPath;

    this->roomType = typeOfRoom;
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

GoalRoom::GoalRoom(string description) : Room(description), goalCompleted(false){}

GoalRoom::GoalRoom(string description, string backgroundPath) : Room(description, backgroundPath), goalCompleted(false){}

GoalRoom::GoalRoom(string description, string backgroundPath, typeOfRoom typeOfRoom) : Room(description, backgroundPath, typeOfRoom), goalCompleted(false){}



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

string Room::shortDescription() {
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
    int sizeItems = (itemsInRoom.size());
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
    return itemsInRoom.size();
}

int Room::isItemInRoom(string inString)
{
    int sizeItems = (itemsInRoom.size());
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


