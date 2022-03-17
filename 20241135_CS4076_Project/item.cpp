#include "item.h"

Item::Item (string description, int weightGrams, int value/**, int weaponCheck*/) {
    this->description = description;
    this->setWeight(weightGrams);
    this->value = value;
	/**weaponCheck(isWeapon);*/
}

Item::Item(string description) : usedDialogue("Used the item.") {
    this->description = description;
}

Item::Item(string description, string usedDialogue) : itemType(HINT){
    this->description = description;
    this->usedDialogue = usedDialogue;
}

Item::Item(string description, string usedDialogue, typeOfItem itemType){
    this->description = description;
    this->usedDialogue = usedDialogue;
    this->itemType = itemType;
}

Item::Item(const Item& other) : description(other.description), longDescription(other.longDescription), value(other.value)/*weaponCheck(other.weaponCheck)*/{
    // Make the copied item slightly heavier.
    int randomWeight = rand() % 10;
    this->weightGrams = other.weightGrams + randomWeight;
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

