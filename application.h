#ifndef TICTACTOE_APPLICATION_H
#define TICTACTOE_APPLICATION_H

#include "Player.h"

template<HashableField FieldType>
class application {
    FieldType field{};
    Player<FieldType> player{field};

    //смотрим кто ходит первый
    bool myMoveFirst(bool &exit);
    //считываем ход юзера
    static typename FieldType::MoveDescriptorType getUserMove();
    //выводим финальное сообщение
    static bool printResultMessage(typename FieldType::ResultType result);
    //игра. true, если завершилась штатно, false, если нет
    bool singlePlay();

public:
    application();
    FieldType &getField();
    Player<FieldType> &getPlayer();
    int exec();

};

template<HashableField FieldType>
bool application<FieldType>::singlePlay() {
    struct FieldDropper {
        FieldType &f;
        explicit FieldDropper(FieldType &field):f(field){}
        ~FieldDropper() {f.drop();}
    } fieldDropper(field);

    bool exit = false;
    bool iMoveFirst = myMoveFirst(exit);
    if (exit) {
        return false;
    }

    field.setMoveOrder(iMoveFirst);

    if (iMoveFirst) {
        player.makeMove();
        std::cout << "enter the number of the point you want to move to\n";
    } else {
        std::cout << "enter the number of the point you want to move to\n";
        std::cout << field;
    }

    while (field.gameInProgress()) {
        player.makeMove(getUserMove());
    }

    return printResultMessage(player.getResult());
}

template<HashableField FieldType>
typename FieldType::MoveDescriptorType application<FieldType>::getUserMove() {
    char c;
    while (true) {
        std::cin >> c;
        if (c >= '1' && c <= '9') {
            return typename FieldType::MoveDescriptorType(c - 1 - '0');
        }
    }
}

template<HashableField FieldType>
bool application<FieldType>::myMoveFirst(bool &exit) {
    std::cout << "choose who move first\n"
              << "you: 1\n"
              << "computer: 2\n"
              << "exit: 0\n";
    char c;
    while (true) {
        std::cin >> c;
        switch (c) {
            case '1': return false;
            case '2': return true;
            case '0': {
                exit = true;
                return true;
            }
            default: break;
        }
    }
}

template<HashableField FieldType>
int application<FieldType>::exec() {
    while (singlePlay()) {}
    std::cout << "\nthe end\n";
    return 0;
}

template<HashableField FieldType>
FieldType &application<FieldType>::getField() {
    return field;
}

template<HashableField FieldType>
Player<FieldType> &application<FieldType>::getPlayer() {
    return player;
}

template<HashableField FieldType>
bool application<FieldType>::printResultMessage(typename FieldType::ResultType result) {
    switch (result) {
        case Player<FieldType>::ResultType::none: {
            std::cout << "\nwhat?\n";
            return false;
        }
        case Player<FieldType>::ResultType::iWin: {
            std::cout << "\nyou lose\n";
            break;
        }
        case Player<FieldType>::ResultType::opponentWin: {
            std::cout << "\nyou win\n";
            break;
        }
        case Player<FieldType>::ResultType::draw: {
            std::cout << "\ndraw\n";
            break;
        }
    }
    return true;
}

template<HashableField FieldType>
application<FieldType>::application()
: field{}
, player(field)
{
    //
}

#endif //TICTACTOE_APPLICATION_H
