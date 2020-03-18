//
// Created by shenk on 14.02.2020.
//

#include "GameField.h"

GameField::GameField():
fieldHeight(0),
fieldWidth(0),
field(nullptr)
{}

GameField::GameField(int fieldSize):
fieldHeight(fieldSize),
fieldWidth(fieldSize)
{

    field = new FieldCell* [fieldSize];
    for (int i=0; i<fieldSize; i++){
        field[i] = new FieldCell [fieldSize];
    }
}

GameField::GameField(int fieldHeight, int fieldWidth):
fieldHeight(fieldHeight),
fieldWidth(fieldWidth)
{
    field = new FieldCell* [fieldHeight];
    for (int i=0; i<fieldHeight; i++){
        field[i] = new FieldCell [fieldWidth];
    }
}

void GameField::deleteObject(int x, int y) {

    field[y][x].eraseObject();

}

bool GameField::addObject(GameObject *object, int x, int y) {

    if (object->isOnField){
        std::cout << "GameField: Object " << object << " already on field." << std::endl;
        return false;
    }

    bool isInBorder = x < fieldWidth && y < fieldHeight && x >= 0 && y >= 0;

    if (isInBorder && field[y][x].isEmpty()){

        field[y][x].setObject(object);
        object->position = Point(x, y);
        object->isOnField = true;

    } else{

        std::cout << "Impossible to add Object " << object << " to field." << std::endl;
        return false;

    }

    return true;
}

void GameField::deleteObject(GameObject *object) {

    deleteObject(object->position.x, object->position.y);

}

void GameField::moveObject(const Point &p1, const Point &p2) {

    if (!field[p1.y][p1.x].isEmpty() && field[p2.y][p2.x].isEmpty()){

        field[p2.y][p2.x] = std::move(field[p1.y][p1.x]);
        field[p2.y][p2.x].getObject()->position = p2;

        field[p1.y][p1.x].eraseObject();

    } else{

        std::cout << "GameField: Impossible to move object." << std::endl;

    }

}

void GameField::moveObject(GameObject *object, const Point &p2) {

    Point p1 = object->getPosition();
    moveObject(p1, p2);

}

void GameField::deleteObject(const Point &point) {

    deleteObject(point.x, point.y);

}

int GameField::getHeight() const{
    return fieldHeight;
}

int GameField::getWidth() const{
    return fieldWidth;
}

FieldCell *GameField::getCell(const Point &p) const{

    if (p.x < fieldWidth && p.y < fieldHeight)
        return &field[p.y][p.x];
    return nullptr;
}

GameField::~GameField() {

    for (int i=0; i<fieldHeight; i++){
        delete []field[i];
    }

    delete []field;

}

std::ostream &operator<<(std::ostream &stream, const GameField &field) {

    for (int y=0; y<field.fieldHeight; y++){
        for (int x=0; x<field.fieldWidth; x++){
            stream << field.field[y][x];
        }
        stream << std::endl;
    }

    return stream;

}

void GameField::onUnitMove(Unit *unit, Point p) {

    FieldCell *cell = getCell(p);

    if (!cell->isEmpty() && cell->getObject()->getType() == ObjectType::NEUTRAL_OBJECT){

        NeutralObject *neutralObject = static_cast<NeutralObject*>(cell->getObject());

        switch (unit->getUnitType()){
            case UnitType::INFANTRY:
                neutralObject->setStrategy(new InfantryStrategy());
                break;
            case UnitType::ARCHER:
                neutralObject->setStrategy(new ArcherStrategy());
                break;
            case UnitType::WIZARD:
                neutralObject->setStrategy(new WizardStrategy());
                break;
        }

        (*unit) << neutralObject;
        cell->eraseObject();
    }

    moveObject(unit->getPosition(), p);

}

void GameField::onUnitDestroy(Unit *unit) {

    deleteObject(unit->getPosition());

}

bool GameField::addObject(GameObject *object, Point position) {

    return addObject(object, position.x, position.y);

}

void GameField::onBaseNewUnitCreated(Unit *unit, Point position) {

    bool isPossibleAdd = addObject(unit, position);
    if (isPossibleAdd)
        unit->addObserver(this);

}

bool GameField::addBase(Base *base, Point position) {

    return addBase(base, position.x, position.y);

}

bool GameField::addBase(Base *base, int x, int y) {

    bool isPossibleAdd = addObject(base, x, y);
    if (isPossibleAdd)
        base->addObserver(this);

}

void GameField::onUnitAttack(Unit *unit, Unit *other) {

    Landscape *landscape = getCell(unit->getPosition())->getLandscape();
    LandscapeProxy landscapeProxy(landscape);
    int resDamage = unit->getWeapon().getDamage()*landscapeProxy.getDamageFactor(unit->getWeapon().getType())
                    -other->getArmor().getDamageAbsorption()*landscapeProxy.getAbsorptionFactor(other->getArmor().getType());
    if (resDamage < 0) resDamage = 0;
    other->damage(resDamage);

}

