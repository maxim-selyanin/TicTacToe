#ifndef TICTACTOE_LINE_H
#define TICTACTOE_LINE_H

#include "TicTacFieldDefinitions.h"
#include <array>

using namespace Definitions;

//3 positions forming line
class Line {
public:
    //definitions
    static constexpr int lineAmount = 8;
    static constexpr int PositionAmount = 3;
    using LineStorage = std::array<Line, lineAmount>;
    enum class Type : int {
        free
        ,wasted
        ,mine
        ,opponents
    };

private:
    using PositionIndexType = Definitions::PositionIndexType;
    using PositionState = Definitions::PositionState;
    //глобальные индексы
    PositionIndexType positionIndices[PositionAmount] = {};
    //состояния клеток в линии
    PositionState positions[PositionAmount] = {};

public:
    //api
    //тип
    [[nodiscard]] Type getType(int without = -1) const;
    //точка пересечения двух линий
    [[nodiscard]] static PositionIndexType getIntersectionPoint(const Line &a, const Line &b);
    //место глобального индекса в локальном массиве positionIndices
    [[nodiscard]] int getLocalIndexOfPosition(PositionIndexType globalIndex) const;
    //находятся ли 2 линии на пересечении, с которого можно получить вилку
    [[nodiscard]] static bool isOnStrikeIntersection(const Line &a, const Line &b
            , Line::Type targetType, bool &pointOnIntersection);

    //количество занятых позиций
    [[nodiscard]] int occupiedPositions() const;
    //Незанятые клетки
    [[nodiscard]] int countFreePositions() const;

    //принадлежность
    [[nodiscard]] bool isMine() const;
    [[nodiscard]] bool isEnemys() const;
    [[nodiscard]] bool nobodys() const;

    //клетка, в которую вынуждает сходить существование данной линии
    [[nodiscard]] PositionIndexType forcedMove() const;
    //есть ли в линии данный индекс
    [[nodiscard]] bool containsGlobalIndex(PositionIndexType index) const;

    //горизонтальные линии поля
    [[nodiscard]] static Line getHorizontalLine(
            const PositionState positions[PositionNumber], int number);
    //вертикальные линии поля
    [[nodiscard]] static Line getVerticalLine(
            const PositionState positions[PositionNumber], int number);
    //диагональные линии поля
    [[nodiscard]] static Line getDiagonalLine(
            const PositionState positions[PositionNumber], int startVal, int interval);
    //все линии данного поля
    [[nodiscard]] static Line::LineStorage getAllLines(const PositionState positions[PositionNumber]);
};

#endif //TICTACTOE_LINE_H
