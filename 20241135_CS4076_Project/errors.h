#ifndef ERRORS_H
#define ERRORS_H

class NoRoomError
{
public:
    const char* what(){
        return "There are no rooms in this direction. Maybe try another one?\n";
    }
};

#endif // ERRORS_H
