//
// Created by shenk on 16.03.2020.
//

#ifndef UNTITLED13_SHOWUNITCOMMAND_H
#define UNTITLED13_SHOWUNITCOMMAND_H


#include "../Command.h"

class ShowUnitCommand: public Command {

private:

    Point unitPosition;

public:

    explicit ShowUnitCommand(Point p): unitPosition(p){}
    virtual void execute(GameInfo &gameInfo, GameField &gameField){

        auto object = gameField.getCell(unitPosition)->getObject();
        if (object && object->getType() == ObjectType::UNIT){

            auto unit = dynamic_cast<Unit*>(object);
            std::cout << "Unit info: " << std::endl
            << "\tHP: " << unit->getHealth() << std::endl
            << "\tWeapon: " << unit->getWeapon() << std::endl
            << "\tArmor: " << unit->getArmor() << std::endl;

        } else{
            std::cout << "Empty cell" << std::endl;
        }

    }

};

class ShowUnitCommandHandler: public CommandHandler{

public:

    virtual CommandPtr handle(std::vector<std::string> &cmd){

        int x = std::stoi(cmd[0]);
        int y = std::stoi(cmd[1]);
        Point unitPosition(x, y);
        return CommandPtr(new ShowUnitCommand(unitPosition));

    }

};


#endif //UNTITLED13_SHOWUNITCOMMAND_H
