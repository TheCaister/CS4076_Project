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
    using namespace useItemFunctions;

    Room *city_centre, *cave, *chinese_restaurant,
            *claw_machine, *conveyor_sushi, *lively_alley, *noodle_stall,
            *under_bridge;
    GoalRoom *sewer_a, *pei_street, *train, *cafe, *train_station;
    Item *frog, *weird_magazine, *pen;
    frog = new Item(&(useItemDefault), "frog", ItemDialogues::frog);
    weird_magazine = new Item(&(useItemDefault), "weird magazine", ItemDialogues::weirdMagazine);
    pen = new Item(&(useItemDefault),"pen", ItemDialogues::pen);
    vector<Room*> allRooms;

    // Adding all rooms
    city_centre = new Room("City Centre", RoomDialogues::cityCentre, NIGHT_CITY_GIF);
    sewer_a = new WordleRoom(100, "Sewers", RoomDialogues::sewers, SEWER_GIF);
    train = new WordleRoom(pen, "Train", "The local metro train.", TRAIN_GIF);
    train_station = new GoalRoom(&(interactPlainGoal), &(checkFinalGoalFunc),"Station", "", STATION_PIC);
    pei_street = new GoalRoom(&(interactPlainGoal), &(checkPeiCompleteFunc),"Pei Street", "The northern street.", BUSY_STREET);
    chinese_restaurant = new Room(&(interactPlain), "Chinese Restaurant", RoomDialogues::chineseRestaurant, CHINESE_RESTAURANT_PIC);
    cafe = new GoalRoom(&(interactCafe), &(checkCafeCompleteFunc), "Cafe", RoomDialogues::cafe, CAFE);
    cave = new Room(&(interactPlain), "Cave", RoomDialogues::cave, CAVE_PIC);
    claw_machine = new Room(&(interactPlain), "Alley with Claw Machine", RoomDialogues::clawMachine, CLAW_MACHINE);
    conveyor_sushi = new Room(&(interactPlain), "Conveyor Sushi Restaurant", RoomDialogues::conveyorSushi, CONVEYOR_SUSHI);
    lively_alley = new Room(&(interactPlain), "Lively alley", RoomDialogues::livelyAlley, LIVELY_ALLEY);
    noodle_stall = new Room(&(interactPlain), "Noodle Stall", RoomDialogues::noodleStall, NOODLE_STALL);
    under_bridge = new Room(&(interactPlain), "Bridge", RoomDialogues::underBridge, UNDER_BRIDGE);


    *city_centre + frog;
    *pei_street + weird_magazine;

    // Setting exits for each room
    //             (N, E, S, W)
    city_centre->setExits(pei_street, train_station, sewer_a, under_bridge);
    sewer_a->setExits(city_centre, NULL, NULL, NULL);
    train_station->setExits(NULL, train, NULL, city_centre);
    train->setExits(NULL, NULL, NULL, train_station);
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
    allRooms.push_back(train_station);
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
    currentRoom = train_station;

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

        if(WordleEngine::getWordleStatus() == WordleEngine::WORDLE_PROGRESS){
            return output;
        }
        // If it's a success, give the reward of that particular room
        else if(WordleEngine::getWordleStatus() == WordleEngine::WORDLE_SUCCESS){
            output += ZorkUL::giveReward();
        }
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
            else{
                output += itemsInInventory.at(location)->useFunc(itemsInInventory.at(location));

            }

            //return "Using this item...";
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
//        Room* interactRoom = (Room*) currentRoom;
//        output += interactRoom->interactionFunc(interactRoom);
        if((currentRoom->getTypeOfRoom() & Room::GOAL) == Room::GOAL){
            GoalRoom* room = (GoalRoom*) currentRoom;
            output += room->interactFunc(room);
        }
        else{
            output += currentRoom->interactFunc(currentRoom);

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

int ZorkUL::getGoalMoney(){
    return ZorkUL::goalMoney;
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
