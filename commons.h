#ifndef TICTACTOE_COMMONS_H
#define TICTACTOE_COMMONS_H

enum class Result : int {
    none = 0
    ,iWin
    ,opponentWin
    ,draw
};

//считает количество единичных битов в символе
int setBitAmount(unsigned char c);

#endif //TICTACTOE_COMMONS_H
