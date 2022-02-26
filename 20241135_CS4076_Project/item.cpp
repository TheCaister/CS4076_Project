#include "item.h"

Item::Item (string description, int weightGrams, float value/**, int weaponCheck*/) {
    this->description = description;
    this->setWeight(weightGrams);
    this->value = value;
	/**weaponCheck(isWeapon);*/
}

Item::Item(string description) {
    this->description = description;
}

Item::Item(const Item& other) : description(other.description), longDescription(other.longDescription), value(other.value), weaponCheck(other.weaponCheck){
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

void Item::setValue(float value)
{
    if (value > 9999 || value < 0)
       //cout << "Value invalid, must be 0 < value < 9999" ;
        return;
    else
       this->value = value;
}

/**void Item::setWeaponCheck(int isWeapon)
{
    if(isWeapon > 0 || isWeapon < 0)
        cout << "Item not a weapon" ;
    else
        cout << "Item is a weapon" ;
}*/

string Item::getShortDescription()
{
    return this->description;
}

string Item::getLongDescription()
{
    return " item(s), " + this->description + ".\n";
}

