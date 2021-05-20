#ifndef TICTACTOE_TICTACFIELDDEFINITIONS_H
#define TICTACTOE_TICTACFIELDDEFINITIONS_H

#include "commons.h"

namespace Definitions {
    //общие определения типов
    using MoveDescriptorType = unsigned char;
    using ValueType = int;
    using HashKeyType = int;
    using ResultType = Result;

    //типы для эвристик
    using HeuristicsStorageType = unsigned char;
    enum AvailableHeuristics : HeuristicsStorageType {
        PointsOnLines = 0x01 //00000001
        ,MyLines = 0x02 //00000010
    };

    //private definitions
    //количество клеток
    static constexpr int PositionNumber = 9;

    //output format
    static constexpr char cross = 'x';
    static constexpr char zero = 'o';

    //position code
    enum class PositionCode : unsigned char {
        leftUp = 0x8C//10001100
        ,up = 0x0A//00001010
        ,rightUp = 0x49//01001001
        ,right = 0x11//00010001
        ,rightDown = 0xA1//10100001
        ,down = 0x22//00100010
        ,leftDown = 0x64//01100100
        ,left = 0x14//00010100
        ,center = 0xD2//11010010
    };
    //коды позиций
    static constexpr PositionCode positionCodes[PositionNumber] = {
            PositionCode::leftUp, PositionCode::up, PositionCode::rightUp
            ,PositionCode::left, PositionCode::center, PositionCode::right
            ,PositionCode::leftDown, PositionCode::down, PositionCode::rightDown
    };

    //номер позиции
    using PositionIndexType = unsigned char;
    static constexpr PositionIndexType indexErr = static_cast<PositionIndexType>(10);

    //position state
    enum class PositionState : unsigned char {
        free = 0x07//00000111
        ,mine = 0x01//00000001
        ,opponents = 0x02//00000010
    };
    //смысл битов в PositionState
    static constexpr unsigned char freeBit = 4;//00000100
    static constexpr unsigned char mineBit = 1;//00000001
    static constexpr unsigned char opponentsBit = 2;//00000010

    static constexpr unsigned char allBits = 0xFF;
}

#endif //TICTACTOE_TICTACFIELDDEFINITIONS_H
