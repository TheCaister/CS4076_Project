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
vector<Room*> ZorkUL::allRooms;
Stack<Room*> ZorkUL::recentRooms;

int main(int argc, char *argv[]) {

    //ZorkUL::parser = new Parser();
    Parser* parser = new Parser();
    ZorkUL::setParser(parser);

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

    ZorkUL::setAllRooms(ZorkUL::createRooms());
    w.addStringToConsole(Dialogues::welcome);

    //ZorkUL::updateRoom(ZorkUL::getAllRooms().at(0), windowPtr);
    ZorkUL::updateRoom(ZorkUL::getCurrentRoom(), windowPtr);

    string roomDescription = ZorkUL::getCurrentRoom()->getShortDescription();
    w.addStringToConsole(Dialogues::printCurrentRoom(roomDescription));

    delete worldleEngine;
    delete ZorkUL::getParser();
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

void ZorkUL::setAllRooms(vector<Room *> rooms){
    ZorkUL::allRooms = rooms;
}

vector<Room*> ZorkUL::getAllRooms(){
    return ZorkUL::allRooms;
}

// Cleaning up heap stuff
void ZorkUL::deleteAll(){
    currentRoom = NULL;

    for(auto& room : ZorkUL::getAllRooms()){
        delete room;
    }

    for(auto& item : ZorkUL::itemsInInventory){
        delete item;
    }

    ZorkUL::getAllRooms().clear();
    ZorkUL::itemsInInventory.clear();

}

vector<Room*> ZorkUL::createRooms()  {
    using namespace Constants;
    using namespace GoalCheckFunctions;
    using namespace InteractFunctions;
    Room *city_centre, *station, *cafe, *cave, *chinese_restaurant,
            *claw_machine, *conveyor_sushi, *lively_alley, *noodle_stall,
            *under_bridge;
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
                                          " every direction. Keep an eye out for tourist traps.", NIGHT_CITY_GIF);
    sewer_a = new WordleRoom(100, "Sewers", "The city sewers. Hidden from the glamour of life above-ground, you can't help"
                                            " but gag at the foul odours emanating from this area. Maybe it's best that you leave soon...",
                             SEWER_GIF);
    train = new WordleRoom(pen, "Train", "The local metro train.", TRAIN_GIF);
    station = new GoalRoom(&(checkFinalGoalFunc),"Station", "", STATION_PIC);
    pei_street = new GoalRoom(&(checkPeiCompleteFunc),"Pei Street", "The northern street.",
                              BUSY_STREET);
    chinese_restaurant = new Room("Chinese Restaurant", "The local Chinese restaurant. Your stomach rumbles as the scent of"
                                                        " fried pork and soy sauce emanates from the establishment.", CHINESE_RESTAURANT_PIC);
    cafe = new Room("Cafe", "You arrive at one of the popular cafes in town. Nobody bats an eye when you entered."
                            " They are too busy either lost in their conversations or working with headphones on.", CAFE);
    cave = new Room("Cave", "You head out further away from the big city and ended up in a cave."
                            " The walls are damp and there's not a soul in sight.", CAVE_PIC);
    claw_machine = new Room("Alley with Claw Machine", "As you wander about, you see a claw machine on the side of"
                                                       " the street. Maybe you can try to grab something to sell later on?", CLAW_MACHINE);
    conveyor_sushi = new Room("Conveyor Sushi Restaurant", "You arrive at a restaurant that serves sushi on convenient conveyor belts."
                                                           " The prices here aren't exorbitant so maybe you can treat yourself every now and again?",
                              CONVEYOR_SUSHI);
    lively_alley = new Room("Lively alley", "As you head further down the alley, you come across what seems to be"
                                            " a gang of people moping about. This could be their hangout spot. Provoking"
                                            " them might not be a good idea.", LIVELY_ALLEY);
    noodle_stall = new Room("Noodle Stall", "You see a small noodle stall, operated by a lone man. \"You"
                                            " must have had it rough, huh? I feel sorry for kids like you.\"", NOODLE_STALL);
    under_bridge = new Room("Bridge", "You cross under a bridge. There doesn't seem to be anyone here.", UNDER_BRIDGE);

    *city_centre + frog;
    *pei_street + weird_magazine;

    // Setting exits for each room
    //             (N, E, S, W)
    city_centre->setExits(pei_street, station, sewer_a, under_bridge);
    sewer_a->setExits(city_centre, NULL, NULL, NULL);
    station->setExits(NULL, train, NULL, city_centre);
    train->setExits(NULL, NULL, NULL, station);
    pei_street->setExits(cafe, chinese_restaurant, city_centre, claw_machine);
    chinese_restaurant->setExits(NULL, NULL, NULL, pei_street);
    cafe->setExits(NULL, NULL, pei_street, NULL);
    cave->setExits(under_bridge, NULL, NULL, NULL);
    claw_machine->setExits(conveyor_sushi, pei_street, noodle_stall, lively_alley);
    conveyor_sushi->setExits(NULL, NULL, claw_machine, NULL);
    lively_alley->setExits(NULL, claw_machine, NULL, NULL);
    noodle_stall->setExits(claw_machine, NULL, NULL, NULL);
    under_bridge->setExits(NULL, city_centre, cave, NULL);

    allRooms.push_back(city_centre);
    allRooms.push_back(sewer_a);
    allRooms.push_back(station);
    allRooms.push_back(train);
    allRooms.push_back(pei_street);
    allRooms.push_back(chinese_restaurant);
    allRooms.push_back(cafe);
    allRooms.push_back(cave);
    allRooms.push_back(claw_machine);
    allRooms.push_back(conveyor_sushi);
    allRooms.push_back(lively_alley);
    allRooms.push_back(noodle_stall);
    allRooms.push_back(under_bridge);

    // Start off at this room.
    currentRoom = station;

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
    if(WordleEngine::getWordleStatus() == WordleEngine::WORDLE_PROGRESS){
        output += WordleEngine::evaluateInput(command.getCommandWord());

        // If it's a success, give the reward of that particular room
        if(WordleEngine::getWordleStatus() == WordleEngine::WORDLE_SUCCESS){
            output += ZorkUL::giveReward();
        }

        //return output;
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
                if((currentRoom->getTypeOfRoom() & Room::WORDLE) == Room::WORDLE){
                    output += Dialogues::welcomeWordle + "\n";
                }

                output += currentRoom->getLongDescription();
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
                output += currentRoom->getLongDescription();
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
            }
            else{
                // Adding to player's inventory and removing from the room
                currentRoom->itemsInRoom.push_back(itemsInInventory.at(location));
                itemsInInventory.erase(itemsInInventory.begin() + location);

                output += currentRoom->getLongDescription();
            }



            //return output;

        }
    }

    else if(commandWord.compare("interact") == 0){
        output += currentRoom->interactionFunc(currentRoom);
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
                output += currentRoom->getLongDescription();

                //                return currentRoom->longDescription();
            }
        }
    }

    else if (commandWord.compare("quit") == 0) {
        if (command.hasSecondWord())
            output += "Overdefined input. If you want to quit, please type 'quit' in the input console or click the 'quit' button.";

        //            return "Overdefined input. If you want to quit, please type 'quit' in the input console or click the 'quit' button.";
        else{
            ZorkUL::deleteAll();
        }

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

    Room* previousRoom = currentRoom;

    // Going back to the previous room
    if(direction.compare("back") == 0){
        Room* nextRoom = recentRooms.pop();
        if(nextRoom != NULL){
            currentRoom = nextRoom;
            return true;
        }
        else{
            return false;
        }

    }
    else{
        // Try to leave current room.
        Room* nextRoom = currentRoom->nextRoom(direction);

        if (nextRoom == NULL){
            //cout << "underdefined input"<< endl;
            return false;}
        else {
            currentRoom = nextRoom;
            recentRooms.push(previousRoom);
            //cout << currentRoom->longDescription() << endl;
            return true;
        }

    }
}

// Update the background
void ZorkUL::updateRoom(Room *room, MainWindow *window){
    currentRoom = room;
    window->updateBackground(room->getBackgroundPath());

    if((room->getTypeOfRoom() & Room::WORDLE) == Room::WORDLE){
        //window->addStringToConsole(Dialogues::welcomeWordle);
        //window->overwriteConsole(Dialogues::welcomeWordle + room->getShortDescription());
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

void ZorkUL::setParser(Parser *parser){
    ZorkUL::parser = parser;
}

Parser* ZorkUL::getParser(){
    return ZorkUL::parser;
}
