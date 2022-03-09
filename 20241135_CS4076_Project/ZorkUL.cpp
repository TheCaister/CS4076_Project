#include <iostream>
#include <cstdlib>
#include "mainwindow.h"

#include <QApplication>

#include <QTextStream>
#include <QDebug>
#include "dialogues.h"
#include "constants.h"
#include "Errors.h"
#include "item.h"

using namespace std;
#include "ZorkUL.h"

Parser *ZorkUL::parser;
Room *ZorkUL::currentRoom;
Quantities *ZorkUL::allQuantities;
vector<Item*> ZorkUL::itemsInInventory;
bool ZorkUL::keysPresent[5];
int ZorkUL::money;

int main(int argc, char *argv[]) {
    ZorkUL::parser = new Parser();
    // For printing stuff out in the output pane and debugging
    QTextStream out(stdout);

    WordleEngine *worldleEngine = new WordleEngine();
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowState(Qt::WindowMaximized);
    MainWindow *windowPtr = &w;


    w.show();
    w.clearConsole();

    // For testing that all valid Wordle words are present.
    //    for (string s : worldleEngine->getAllWords()){
    //        w.addToConsole(s);
    //    }
    vector<Room*> allRooms = ZorkUL::createRooms();
    w.addStringToConsole(Dialogues::welcome);

    ZorkUL::updateRoom(allRooms.at(0), windowPtr);
    w.addStringToConsole(Dialogues::printCurrentRoom(ZorkUL::getCurrentRoom()->getShortDescription()));

    delete worldleEngine;
    delete ZorkUL::parser;
    return a.exec();
}

ZorkUL::ZorkUL() {
    for(int i = 0; i < (int) (sizeof(ZorkUL::keysPresent)/sizeof(ZorkUL::keysPresent[0])); i++){
        this->keysPresent[i] = false;
    }
    ZorkUL::allQuantities->KeysPresent = 0;
    ZorkUL::allQuantities->Bombs = 0;
    ZorkUL::money = 10;

    createRooms();
}

vector<Room*> ZorkUL::createRooms()  {
    using namespace Constants;
    Room *city_centre, *station/*, *chinese_restaurant,*/;
    GoalRoom *sewer_a, *pei_street, *train;
    Item *frog, *weird_magazine, *pen;
    frog = new Item("frog", "The frog stares at you, eyes gleaming with... passion?");
    weird_magazine = new Item("weird magazine", "A dubious magazine that you picked up from the street. Upon further inspection,\n"
                                                " you see that this magazine contains copious amounts of photos with\n"
                                                " people on their knees, pleading for forgiveness. Apparently, there are\n"
                                                " folks out there who are really into apologies.");
    pen = new Item("pen", "A pen that you got from a man in the train. It smells kinda funky and the tip seems slightly chewed on.");
    vector<Room*> allRooms;

    // Adding all rooms
    city_centre = new Room("City Centre", "Main City Centre. Blinding lights violate your eyes from"
                                          " every direction. Keep an eye out for tourist traps.", Constants::NIGHT_CITY_GIF);
    //sewer_a = new WordleRoom("Sewer", Constants::SEWER_GIF, 100);
    sewer_a = new WordleRoom(100, "Sewers", "The city sewers. Hidden from the glamour of life above-ground, you can't help"
                                       " but gag at the foul odours emanating from this area. Maybe it's best that you leave soon...",
                             Constants::SEWER_GIF);
    train = new WordleRoom(pen, "Train", "The local metro train.", Constants::TRAIN_GIF);
    station = new Room("Station", "", Constants::STATION_PIC);
    //chinese_restaurant = new Room("Chinese Restaurant", Constants::CHINESE_RESTAURANT_PIC);
    //pei_street = new GoalRoom("Pei Street", "The northern street.", Constants::BUSY_STREET);
    pei_street = new GoalRoom("Pei Street", "The northern street.",
                              Constants::BUSY_STREET);

    // Setting goal checkers
    sewer_a->setCheckGoalFunction(&(GoalCheckFunctions::checkPlainFunc));
    train->setCheckGoalFunction(&(GoalCheckFunctions::checkPlainFunc));
    pei_street->setCheckGoalFunction(&(GoalCheckFunctions::checkPeiCompleteFunc));

    *city_centre + frog;
    *pei_street + weird_magazine;
    vector<Item*> allItems = city_centre->itemsInRoom;

    // Setting exits for each room
    //             (N, E, S, W)
    city_centre->setExits(pei_street, station, sewer_a, NULL);
    sewer_a->setExits(city_centre, NULL, NULL, NULL);
    station->setExits(NULL, train, NULL, city_centre);
    train->setExits(NULL, NULL, NULL, station);
    pei_street->setExits(NULL, NULL, city_centre, NULL);

    allRooms.push_back(city_centre);
    allRooms.push_back(sewer_a);
    allRooms.push_back(station);
    allRooms.push_back(train);
    allRooms.push_back(pei_street);

    // Start off at this room.
    currentRoom = city_centre;

    return allRooms;
}

void deleteAllRooms(){

}

/**
 * Given a command, process (that is: execute) the command.
 * If this command ends the ZorkUL game, true is returned, otherwise false is
 * returned.
 */
string ZorkUL::processCommand(Command command, MainWindow *window) {
    string output = "";
    // If we're in a Wordle game, treat the input as a Wordle attempt
    if(WordleEngine::wordleStatus == WordleEngine::WORDLE_PROGRESS){
        output += WordleEngine::evaluateInput(command.getCommandWord());

        // If it's a success, give the reward of that particular room
        if(WordleEngine::wordleStatus == WordleEngine::WORDLE_SUCCESS){
            output += giveReward();
        }

        //return output;
    }


    if (command.isUnknown()) {
        //return "";
    }

    string commandWord = command.getCommandWord();

    if (commandWord.compare("info") == 0){
        output += printHelp();
        //return printHelp();
    }
    else if (commandWord.compare("map") == 0)
    {
        output += "Map to be implemented soon.\n";
        //return "Map to be implemented soon.\n";
    }

    else if (commandWord.compare("go") == 0){

        try{
            // If the goRoom command is successful, return the room's long description.
            // Otherwise, throw an error message.
            if(goRoom(command)){
                ZorkUL::updateRoom(currentRoom, window);
                output += currentRoom->longDescription();
                //return currentRoom->longDescription();

            }
            else{
                throw NoRoomError();
            }
        }
        catch(NoRoomError& errorMessage){
            output += errorMessage.what();
            //return errorMessage.what();
        }
    }

    else if (commandWord.compare("use") == 0){
        if(!command.hasSecondWord()){
            output += "What the hell do you want to use, punk?!";
            //return "What the hell do you want to use, punk?!";
        }
        else{
            int location = findItemIndex(command.getSecondWord());

            if(location == -1){
                output += "Item not in inventory.";
                //return "Item not in inventory.";
            }

            //return "Using this item...";
            output += ZorkUL::useItem(*itemsInInventory.at(location));

//            return ZorkUL::useItem(*itemsInInventory.at(location));
            //return itemsInInventory.at(0)->getUsedDialogue() + "\n";
        }
    }

    else if (commandWord.compare("take") == 0)
    {
        if (!command.hasSecondWord()) {
            output += "What the hell do you want to take, punk?!\n";

//            return "What the hell do you want to take, punk?!\n";
        }
        else{
            output += "You're trying to take " + command.getSecondWord() + "\n";
            int location = currentRoom->isItemInRoom(command.getSecondWord());
            if (location < 0 ){
                output += "Unfortunately, this item is not in this room.\n";
                //return output;
            }
            else{
                output += "This item is in the room!\n";
                output += "Index number: " + to_string(location) + "\n";


                // Adding to player's inventory and removing from the room
                itemsInInventory.push_back(currentRoom->itemsInRoom.at(location));
                currentRoom->itemsInRoom.erase(currentRoom->itemsInRoom.begin() + location);
                output += currentRoom->longDescription();
                //return output;
            }

        }

    }

    else if (commandWord.compare("put") == 0)
    {
        if (!command.hasSecondWord()) {
            //cout << "What the hell do you want to take, punk?!"<< endl;
            output += "What the hell do you want to put, punk?!\n";

//            return "What the hell do you want to put, punk?!\n";
        }
        else{
            output += "You're trying to put " + command.getSecondWord() + "\n";
            int location = findItemIndex(command.getSecondWord());


            if(location == -1){
                output += "Item not in inventory.";

//                return "Item not in inventory.";
            }

            // Adding to player's inventory and removing from the room
            currentRoom->itemsInRoom.push_back(itemsInInventory.at(location));
            itemsInInventory.erase(itemsInInventory.begin() + location);

            output += currentRoom->longDescription();

            //return output;

        }
    }

    else if(commandWord.compare("check") == 0){

        if(!command.hasSecondWord()){
            output += "What do you want to check, punk??\n";

//            return "What do you want to check, punk??\n";
        }
        else{
            string secondWord = command.getSecondWord();
            if(secondWord.compare("inventory") == 0|| secondWord.compare("inv") == 0){
                // Print out inventory here
                output += printAllItems();

//                return printAllItems();
            }
            else if(secondWord.compare("room") == 0 || secondWord.compare("area") == 0){
                output += currentRoom->longDescription();

//                return currentRoom->longDescription();
            }
        }
    }
    else if (commandWord.compare("quit") == 0) {
        if (command.hasSecondWord())
            output += "Overdefined input. If you want to quit, please type 'quit' in the input console or click the 'quit' button.";

//            return "Overdefined input. If you want to quit, please type 'quit' in the input console or click the 'quit' button.";
        else
            exit(0); /**signal to quit*/
    }

    if((currentRoom->roomType & RoomProperties::GOAL) == RoomProperties::GOAL){
        GoalRoom* goalRoom = (GoalRoom*) currentRoom;
        output += goalRoom->checkIfGoalCompleted(goalRoom);
    }

    return output;
}
/** COMMANDS **/
string ZorkUL::printHelp() {
    string output = "";
    output += "Valid inputs are: ";
    output += ZorkUL::parser->commandsInString();
    return output;
}

string ZorkUL::printAllItems(){
    string output = "";

    if(itemsInInventory.size() <= 0){
        output += "You have no items!\n";
        return output;
    }

    output += "Here are all the items currently in your inventory: ";
    for(Item* item : itemsInInventory){
        output += item->getShortDescription();
        output += " ";
    }
    output += "\n";
    return output;
}

bool ZorkUL::goRoom(Command command) {
    if (!command.hasSecondWord()) {
        //cout << "incomplete input"<< endl;
        return false;
    }

    string direction = command.getSecondWord();

    // Going back to the previous room
    if(direction.compare("back") == 0){

    }

    // Try to leave current room.
    Room* nextRoom = currentRoom->nextRoom(direction);

    if (nextRoom == NULL){
        //cout << "underdefined input"<< endl;
        return false;}
    else {
        currentRoom = nextRoom;
        //cout << currentRoom->longDescription() << endl;
        return true;
    }


}

// Update the background
void ZorkUL::updateRoom(Room *room, MainWindow *window){
    currentRoom = room;
    window->updateBackground(room->getBackgroundPath());

    if((room->getTypeOfRoom() & Room::WORDLE) == Room::WORDLE){
        window->addStringToConsole(Dialogues::welcomeWordle);
        WordleEngine::initialiseWordleEngine();
        WordleEngine::startWordleGame();
    }
}

// When trying to use an Item
string ZorkUL::useItem(Item& item){
    string output = "";
    switch (item.getTypeOfItem()){
    case(Item::HINT):
        output += item.getUsedDialogue();
        break;
    case(Item::HEALTH):
        break;
    default:
        break;
    }
    return output;
}

// Finding the index of an item in the Zork inventory
int ZorkUL::findItemIndex(const string& item){
    int sizeItems = (int)(ZorkUL::itemsInInventory.size());

    if (ZorkUL::itemsInInventory.size() < 1) {
        return -1;
    }

    else if (ZorkUL::itemsInInventory.size() > 0) {
        int x = 0;
        for (int n = sizeItems; n > 0; n--) {
            // compare item with short description
            int tempFlag = item.compare(ZorkUL::itemsInInventory[x]->getShortDescription());
            if (tempFlag == 0) {
                return x;
            }
            x++;
        }
    }

    return -1;
}

string ZorkUL::giveReward(){
    string output = "";

    // Check to see if the current room is a Goal Room
    if((currentRoom->roomType & RoomProperties::GOAL) == RoomProperties::GOAL){
        GoalRoom* goalRoom = dynamic_cast<GoalRoom*>(currentRoom);

        // Checking to see if the goal has already been completed or not
        if(goalRoom->getGoalStatus() == false){
            // Casting to RewardRoom so that certain features can be accessed.
            RewardRoom* rewardRoom = dynamic_cast<RewardRoom*>(currentRoom);

            switch(rewardRoom->rewardType){
            case RewardRoom::MONEY : {
                int rewardMoney = rewardRoom->giveMoneyReward();
                output += "Congratulations! You have received $" + to_string(rewardMoney) + "!\n";
                ZorkUL::changeMoney(rewardMoney);
                break;
            }

            case RewardRoom::ITEM : {
                ZorkUL::itemsInInventory.push_back(rewardRoom->giveItemReward());
                output += "You have received: " + ZorkUL::itemsInInventory.at(ZorkUL::itemsInInventory.size() - 1)->getShortDescription() + ".\n";
                break;
            }
            case RewardRoom::CLUE : {
                output += "Clue: " + rewardRoom->giveClueReward() + "\n";
                break;
            }
            case RewardRoom::NONE : break;
            default : break;
            }
        }
        goalRoom->setGoalStatus(true);
    }
    else{
        output += "Damn, it looks like this room isn't a goal room.\n";
    }



    return output;
}

void ZorkUL::changeMoney(int moneyAmount){
    ZorkUL::money += moneyAmount;
}

int ZorkUL::getMoney(){
    return ZorkUL::money;
}

Room *ZorkUL::getCurrentRoom(){
    return ZorkUL::currentRoom;
}

