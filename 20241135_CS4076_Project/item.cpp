#include "item.h"
#include "ZorkUL.h"
#include <time.h>


Item::Item (string description, int weightGrams, int value/**, int weaponCheck*/) {
    this->description = description;
    this->setWeight(weightGrams);
    this->value = value;
    //weaponCheck(isWeapon);
}

Item::Item(string description, string usedDialogue, typeOfItem itemType){
    this->description = description;
    this->usedDialogue = usedDialogue;
    this->itemType = itemType;
}

Item::Item(string (*useFunc)(Item*), string description,
           string usedDialogue, typeOfItem typeOfItem){
    this->useFunc = useFunc;
    this->description = description;
    this->usedDialogue = usedDialogue;
    this->itemType = typeOfItem;
}

Item::Item(const Item& other) : description(other.description), value(other.value)/*weaponCheck(other.weaponCheck)*/{
    // Make the copied item slightly heavier.
    int randomWeight = rand() % 10;
    this->weightGrams = other.weightGrams + randomWeight;
}

Item::~Item(){

}

LuckItem::LuckItem(string (*useFunc)(LuckItem*), string description, string usedDialogue, typeOfItem typeOfItem, float successChance)
    :   Item(NULL, description, usedDialogue, typeOfItem)
{
    this->successChance = successChance;
}

Raffle::Raffle(string(*useFunc)(LuckItem* raffle), float successChance, int value, string description,
               string usedDialogue, typeOfItem typeOfItem)
    : LuckItem(useFunc, description, usedDialogue, typeOfItem, successChance){

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

void Raffle::consumeItem(){

}

namespace useItemFunctions{
string useItemDefault(Item* item){
    return item->getUsedDialogue();
}

string useLuckItemDefault(LuckItem* item){
    return item->getUsedDialogue();
}

string useItemRaffle(LuckItem* raffle){
    string output = "";

    if(raffle->tryChance()){
//        int raffleIndex = ZorkUL::findItemIndex(raffle->LuckItem::getShortDescription());
        ZorkUL::changeMoney(raffle->LuckItem::getValue());
        ZorkUL::deleteItemByName(raffle->LuckItem::getShortDescription());
        output += "Success! You have won some money from the raffle!";
        //ZorkUL::
    }
    else{
        output += "Oof! Guess you weren't so lucky this time!";
        ZorkUL::deleteItemByName(raffle->LuckItem::getShortDescription());
    }

    return output;
    //return raffle->Item::getUsedDialogue();
}
}
