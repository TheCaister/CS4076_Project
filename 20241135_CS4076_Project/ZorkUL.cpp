#include <iostream>
#include <cstdlib>
#include "mainwindow.h"

#include <QApplication>

#include <QTextStream>
#include <QDebug>
#include "dialogues.h"

using namespace std;
#include "ZorkUL.h"

Parser *ZorkUL::parser;
Room *ZorkUL::currentRoom;

//Room *a, *b, *c, *d, *e, *f, *g, *h, *i, *newRoom;

int main(int argc, char *argv[]) {
    ZorkUL::parser = new Parser();
    // For printing stuff out in the output pane and debugging
    QTextStream out(stdout);
    //ZorkUL temp;
    //temp.play();

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
    createRooms();
}


vector<Room*> ZorkUL::createRooms()  {
    Room *a, *b, *c, *d, *e, *f, *g, *h, *i, *newRoom;
    vector<Room*> allRooms;

    // Adding all rooms
    a = new Room("a");
    a->addItem(new Item("x", 1, 11));
    a->addItem(new Item("y", 2, 22));
    b = new Room("b");
    b->addItem(new Item("xx", 3, 33));
    b->addItem(new Item("yy", 4, 44));
    c = new Room("c");
    d = new Room("d");
    e = new Room("e");
    f = new Room("f");
    g = new Room("g");
    h = new Room("h");
    i = new Room("i");
    newRoom = new Room("New Room");

    // Setting exits for each room
    //             (N, E, S, W)
    a->setExits(f, b, d, c);
    b->setExits(newRoom, NULL, NULL, a);
    c->setExits(NULL, a, NULL, NULL);
    d->setExits(a, e, NULL, i);
    e->setExits(NULL, NULL, NULL, d);
    f->setExits(NULL, g, a, h);
    g->setExits(NULL, NULL, NULL, f);
    h->setExits(NULL, f, NULL, NULL);
    i->setExits(NULL, d, NULL, NULL);

    newRoom->setExits(NULL, NULL, b, NULL);

    allRooms.push_back(a);
    allRooms.push_back(b);
    allRooms.push_back(c);
    allRooms.push_back(d);
    allRooms.push_back(e);
    allRooms.push_back(f);
    allRooms.push_back(g);
    allRooms.push_back(h);
    allRooms.push_back(i);
    allRooms.push_back(newRoom);

    // Start off at this room.
    currentRoom = a;

    return allRooms;
}

/**
 *  Main play routine.  Loops until end of play.
 */
//void ZorkUL::play() {
//    printWelcome();

//    // Enter the main command loop.  Here we repeatedly read commands and
//    // execute them until the ZorkUL game is over.

//    bool finished = false;
//    while (!finished) {
//        // Create pointer to command and give it a command.
//        Command* command = ZorkUL::parser->getCommand();
//        // Pass dereferenced command and check for end of game.
//        //finished = processCommand(*command);
//        // Free the memory allocated by "parser.getCommand()"
//        //   with ("return new Command(...)")
//        delete command;
//    }
//    cout << endl;
//    cout << "end" << endl;
//}

void ZorkUL::printWelcome() {
    cout << "start"<< endl;
    cout << "info for help"<< endl;
    cout << endl;
    cout << currentRoom->longDescription() << endl;
}

/**
 * Given a command, process (that is: execute) the command.
 * If this command ends the ZorkUL game, true is returned, otherwise false is
 * returned.
 */
string ZorkUL::processCommand(Command command) {
    cout << command.getCommandWord() << endl;

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
        //        cout << "[h] --- [f] --- [g]" << endl;
        //        cout << "         |         " << endl;
        //        cout << "         |         " << endl;
        //        cout << "[c] --- [a] --- [b]" << endl;
        //        cout << "         |         " << endl;
        //        cout << "         |         " << endl;
        //        cout << "[i] --- [d] --- [e]" << endl;
    }

    else if (commandWord.compare("go") == 0)
        if(goRoom(command)){
            return currentRoom->longDescription();
        }
        else{
            return Dialogues::noMoreRooms;
        }


    // Attempting to write teleport code
    //    else if(commandWord.compare("Teleport random") == 0){
    //        cout << "Teleporting to random room..." << endl;

    //        //Room rooms [10] = {*a, *b, *c, *d, *e, *f, *g, *h, *i, *newRoom};
    //        int randomIndex = rand() % 10;
    //        //currentRoom = &rooms[randomIndex];
    //        cout << currentRoom->longDescription() << endl;

    //    }

    // Attempting to write teleport code
    //    else if(commandWord.compare("Teleport") == 0){
    //        cout << "Which room would you like to teleport to?" << endl;
    //        int choice;

    //        //Room rooms [10] = {*a, *b, *c, *d, *e, *f, *g, *h, *i, *newRoom};
    //        // int numberOfRooms = sizeof rooms/ sizeof (rooms[0]);

    //        // cout << "Amount of rooms: " << numberOfRooms << endl;

    //        //        for(int i = 0; i < numberOfRooms; i++){
    //        //            Room iterRoom = rooms[i];
    //        //            cout << i << ": " << iterRoom.shortDescription() << endl;
    //        //        }

    //        cout << "Enter selection here: ";
    //        cin >> choice;

    //        cout << "You have chosen: " << choice << endl;
    //        // currentRoom = &rooms[choice];
    //        cout << currentRoom->longDescription() << endl;

    //    }

    else if (commandWord.compare("take") == 0)
    {
        if (!command.hasSecondWord()) {
            cout << "incomplete input"<< endl;
        }
        else
            if (command.hasSecondWord()) {
                cout << "you're trying to take " + command.getSecondWord() << endl;
                int location = currentRoom->isItemInRoom(command.getSecondWord());
                if (location  < 0 )
                    cout << "item is not in room" << endl;
                else
                    cout << "item is in room" << endl;
                cout << "index number " << + location << endl;
                cout << endl;
                cout << currentRoom->longDescription() << endl;
            }
    }

    else if (commandWord.compare("put") == 0)
    {

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
            return "overdefined input";
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
        cout << "incomplete input"<< endl;
        return false;
    }

    string direction = command.getSecondWord();

    // Try to leave current room.
    Room* nextRoom = currentRoom->nextRoom(direction);

    if (nextRoom == NULL){
        cout << "underdefined input"<< endl;
        return false;}
    else {
        currentRoom = nextRoom;
        cout << currentRoom->longDescription() << endl;
        return true;
    }
}

string ZorkUL::go(string direction) {
    //Make the direction lowercase
    //transform(direction.begin(), direction.end(), direction.begin(),:: tolower);
    //Move to the next room
    Room* nextRoom = currentRoom->nextRoom(direction);
    if (nextRoom == NULL)
        return("direction null");
    else
    {
        currentRoom = nextRoom;
        return currentRoom->longDescription();
    }
}

void ZorkUL::updateRoom(Room *room, MainWindow *window){
    currentRoom = room;
    window->updateBackground(room->getBackgroundPath());
}

Room *ZorkUL::getCurrentRoom(){
    return ZorkUL::currentRoom;
}
