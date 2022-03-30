#include "item.h"
#include "ZorkUL.h"
#include <time.h>


Item::Item (string description, int weightGrams, int value) {
    this->description = description;
    this->setWeight(weightGrams);
    this->value = value;
}

Item::Item(string description, string usedDialogue, typeOfItem itemType){
    this->description = description;
    this->usedDialogue = usedDialogue;
    this->itemType = itemType;
}

Item::Item(string (*useFunc)(Item*), string description,
           string usedDialogue, typeOfItem typeOfItem, int value){
    this->useFunc = useFunc;
    this->description = description;
    this->usedDialogue = usedDialogue;
    this->itemType = typeOfItem;
    this->value = value;
}

Item::Item(const Item& other) : description(other.description), value(other.value){
    // Make the copied item slightly heavier.
    srand(time(NULL));
    int randomWeight = rand() % 10;
    this->weightGrams = other.weightGrams + randomWeight;
}

Item::~Item(){

}

LuckItem::LuckItem(string (*useFunc)(LuckItem*), string description, string usedDialogue, typeOfItem typeOfItem,
                   float successChance, int value)
    :   Item(NULL, description, usedDialogue, typeOfItem, value)
{
    this->successChance = successChance;
    this->useFunc = useFunc;
}

Raffle::Raffle(int value, float successChance, string description,
               string usedDialogue, typeOfItem typeOfItem)
    : LuckItem(&(useItemFunctions::useItemRaffle), description, usedDialogue, typeOfItem, successChance, value){
}


void Item::setWeight(int weightGrams)
{
    if (weightGrams > 9999 || weightGrams < 0)
        //cout << "Weight invalid, must be 0 < weight < 9999" ;
        return;
    else
        this->weightGrams = weightGrams;
}

void Item::setValue(int value)
{
    if (value > 9999 || value < 0)
        //cout << "Value invalid, must be 0 < value < 9999" ;
        return;
    else
        this->value = value;
}


int Item::getValue(){
    return this->value;
}

string Item::getShortDescription()
{
    return this->description;
}

string Item::getUsedDialogue(){
    return this->usedDialogue;
}

Item::typeOfItem Item::getTypeOfItem(){
    return itemType;
}

bool LuckItem::tryChance(){
    srand(time(NULL));

    if(rand() % 100 < (this->successChance * 100)){
        return true;
    }
    else{
        return false;
    }
}

namespace useItemFunctions{
string useItemDefault(Item* item){
    return item->getUsedDialogue();
}

string useOrb(Item* item){
    string output = item->getUsedDialogue();
    ZorkUL::deleteItemByName(item->getShortDescription());
    ZorkUL::enableParadise();
    return output;

}

string useLuckItemDefault(LuckItem* item){
    return item->getUsedDialogue();
}

string useItemRaffle(LuckItem* raffle){
    string output = "";

    if(raffle->tryChance()){
        ZorkUL::changeMoney(raffle->getValue());
        ZorkUL::deleteItemByName(raffle->getShortDescription());
        output += "Success! You have won some money from the raffle!";
    }
    else{
        output += "Oof! You check your numbers but you didn't"
                  " hit any of them! Guess you weren't so lucky this time!";
        ZorkUL::deleteItemByName(raffle->getShortDescription());
    }

    return output;
}
}
