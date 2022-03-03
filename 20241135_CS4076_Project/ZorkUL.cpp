#include <iostream>
#include <cstdlib>
#include "mainwindow.h"

#include <QApplication>

#include <QTextStream>
#include <QDebug>
#include "dialogues.h"
#include "constants.h"
#include "Errors.h"

using namespace std;
#include "ZorkUL.h"

Parser *ZorkUL::parser;
Room *ZorkUL::currentRoom;
Quantities *ZorkUL::allQuantities;
vector<Item*> ZorkUL::itemsInInventory;
bool ZorkUL::keysPresent[5];


//Room *a, *b, *c, *d, *e, *f, *g, *h, *i, *newRoom;

int main(int argc, char *argv[]) {
    ZorkUL::parser = new Parser();
    // For printing stuff out in the output pane and debugging
    QTextStream out(stdout);

    WordleEngine *worldleEngine = new WordleEngine();
    QApplication a(argc, argv);
    MainWindow w;
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
    w.addStringToConsole(Dialogues::printCurrentRoom(ZorkUL::getCurrentRoom()->shortDescription()));


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

    createRooms();
}

vector<Room*> ZorkUL::createRooms()  {
    using namespace Constants;
    //Room *a, *b, *c, *d, *e, *f, *g, *h, *i, *newRoom;
    Room *city_centre, *sewer_a, *train, *station;
    Item *frog = new Item("frog", "The frog stares at you, eyes gleaming with... passion?");

    vector<Room*> allRooms;

    // Adding all rooms
    city_centre = new Room("City Centre", Constants::NIGHT_CITY_GIF);
    sewer_a = new Room("Sewer", Constants::SEWER_GIF, Room::WORDLE);
    train = new GoalRoom("Train", Constants::TRAIN_GIF, Room::WORDLE);
    station = new Room("Station", Constants::STATION_PIC);

    *city_centre + frog;
    vector<Item*> allItems = city_centre->itemsInRoom;

    // Setting exits for each room
    //             (N, E, S, W)
    city_centre->setExits(NULL, station, sewer_a, NULL);
    sewer_a->setExits(city_centre, NULL, NULL, NULL);
    station->setExits(NULL, train, NULL, city_centre);
    train->setExits(NULL, NULL, NULL, station);

    allRooms.push_back(city_centre);
    allRooms.push_back(sewer_a);
    allRooms.push_back(station);
    allRooms.push_back(train);

    // Start off at this room.
    currentRoom = city_centre;

    return allRooms;
}

/**
 * Given a command, process (that is: execute) the command.
 * If this command ends the ZorkUL game, true is returned, otherwise false is
 * returned.
 */
string ZorkUL::processCommand(Command command, MainWindow *window) {

    // If we're in a Wordle game, treat the input as a Wordle attempt
    if(WordleEngine::wordleStatus == WordleEngine::WORDLE_PROGRESS){
        return WordleEngine::evaluateInput(command.getCommandWord());
    }

    //cout << command.getCommandWord() << endl;



    if (command.isUnknown()) {
        //cout << "invalid input"<< endl;
        return "";
    }

    string commandWord = command.getCommandWord();
    if (commandWord.compare("info") == 0)
        return printHelp();

    else if (commandWord.compare("map") == 0)
    {
        return "Map to be implemented soon.\n";
    }

    else if (commandWord.compare("go") == 0){

        try{
            // If the goRoom command is successful, return the room's long description.
            // Otherwise, throw an error message.
            if(goRoom(command)){
                ZorkUL::updateRoom(currentRoom, window);
                return currentRoom->longDescription();

            }
            else{
                throw NoRoomError();
            }
        }
        catch(NoRoomError& errorMessage){
            return errorMessage.what();
        }
    }

    else if (commandWord.compare("use") == 0){
        if(!command.hasSecondWord()){
            return "What the hell do you want to use, punk?!";
        }
        else{
            //return "Using this item...";
            //Item* item = itemsInInventory.at(0);
            //string dialogue = item->getUsedDialogue();
            return itemsInInventory.at(0)->getUsedDialogue() + "\n";
        }
    }

    else if (commandWord.compare("take") == 0)
    {
        string output = "";
        if (!command.hasSecondWord()) {
            //cout << "What the hell do you want to take, punk?!"<< endl;
            return "What the hell do you want to take, punk?!\n";
        }
        else{
            output += "You're trying to take " + command.getSecondWord() + "\n";
            int location = currentRoom->isItemInRoom(command.getSecondWord());
            if (location  < 0 ){
                output += "Unfortunately, this item is not in this room.\n";
            }
            else{
                output += "This item is in the room!\n";
                output += "Index number: " + to_string(location) + "\n";


                // Adding to player's inventory and removing from the room
                itemsInInventory.push_back(currentRoom->itemsInRoom.at(location));
                //string dialogue = itemsInInventory.at(0)->getUsedDialogue();
                currentRoom->itemsInRoom.erase(currentRoom->itemsInRoom.begin() + location);
                //                cout << "item is in room" << endl;
                //                cout << "index number " << + location << endl;
                //                cout << endl;
                //                cout << currentRoom->longDescription() << endl;
                output += currentRoom->longDescription();
                return output;
            }

        }

    }

    else if (commandWord.compare("put") == 0)
    {
        string output = "";
        if (!command.hasSecondWord()) {
            //cout << "What the hell do you want to take, punk?!"<< endl;
            return "What the hell do you want to put, punk?!\n";
        }
        else{
            output += "You're trying to put " + command.getSecondWord() + "\n";
            int location = findItemIndex(command.getSecondWord());


            if(location == -1){
                return "Item not in inventory.";
            }

            // Adding to player's inventory and removing from the room
            //itemsInInventory.push_back(currentRoom->itemsInRoom.at(location));
            //currentRoom->itemsInRoom.erase(currentRoom->itemsInRoom.begin() + location);
            currentRoom->itemsInRoom.push_back(itemsInInventory.at(location));
            itemsInInventory.erase(itemsInInventory.begin() + location);

            output += currentRoom->longDescription();

            return output;

        }
    }
    /*
    {
    if (!command.hasSecondWord()) {
        cout << "incomplete input"<< endl;
        }
        else
            if (command.hasSecondWord()) {
            cout << "you're adding " + command.getSecondWord() << endl;
            itemsInRoom.push_Back;
        }
    }
*/
    else if (commandWord.compare("quit") == 0) {
        if (command.hasSecondWord())
            return "Overdefined input. If you want to quit, please type 'quit' in the input console or click the 'quit' button.";
        else
            exit(0); /**signal to quit*/
    }

    return "";
}
/** COMMANDS **/
string ZorkUL::printHelp() {
    string output = "";
    output += "Valid inputs are: ";
    output += ZorkUL::parser->commandsInString();
    return output;
}

bool ZorkUL::goRoom(Command command) {
    if (!command.hasSecondWord()) {
        //cout << "incomplete input"<< endl;
        return false;
    }

    string direction = command.getSecondWord();

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

    if(room->getTypeOfRoom() == Room::WORDLE){
        window->addStringToConsole(Dialogues::welcomeWordle);
        WordleEngine::initialiseWordleEngine();
        WordleEngine::startWordleGame();
    }
}

// Finding the index of an item in the Zork inventory
int ZorkUL::findItemIndex(const string& item){
    int sizeItems = (ZorkUL::itemsInInventory.size());

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

Room *ZorkUL::getCurrentRoom(){
    return ZorkUL::currentRoom;
}
