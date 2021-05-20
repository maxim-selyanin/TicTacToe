#ifndef TICTACTOE_TICTACFIELD_H
#define TICTACTOE_TICTACFIELD_H

#include "myconcepts.h"
#include "commons.h"
#include <vector>
#include <ostream>
#include <array>
#include "TicTacFieldDefinitions.h"
#include "Line.h"
#include "HeuristicExtremeValues.h"

using namespace Definitions;

class TicTacField {
public:
    //public definitions
    using MoveDescriptorType = Definitions::MoveDescriptorType;
    using ValueType = Definitions::ValueType;
    using HashKeyType = Definitions::HashKeyType;
    using ResultType = Definitions::ResultType;

private:
    //move descriptor predefined values
    static constexpr MoveDescriptorType nullMove = 0xFF;
    static constexpr MoveDescriptorType myMoveBit = 0x10;//00010000


    //данные
    HeuristicsStorageType chosenHeuristics = 0;
    HeuristicExtremeValues extremeValues{};

    //positions
    PositionState positions[PositionNumber] = {
        PositionState::free, PositionState::free,PositionState::free
        ,PositionState::free, PositionState::free, PositionState::free
        ,PositionState::free, PositionState::free, PositionState::free
    };
    //result
    ResultType gameResult = ResultType::none;

    //чей первый и текущий ход
    bool myMoveFirst = true;
    bool myMoveNow = true;



    //private methods
    void updateResult();
    //STRONGLY DEPRECATED, USE myMoveMow INSTEAD
    [[nodiscard]] bool isMyMoveNow() const;
    //0, 1 или 2 в зависимости от состояния позиции
    [[nodiscard]] static char positionHashValue(PositionState state);
    //вывод позиции на экран в зависимости от её состояния
    [[nodiscard]] static char positionOutputRepresentation(PositionState state, bool iMoveFirst, int number);

public:
    TicTacField() = default;
    TicTacField(const TicTacField &other);

    //default api
    //сделать ход
    bool makeMove(MoveDescriptorType move);
    //отменить последний ход
    bool undoMove(MoveDescriptorType move);
    //null-move
    [[nodiscard]] static MoveDescriptorType noMove();
    //результат игры
    [[nodiscard]] ResultType result() const;
    //экстремумы эвристики
    [[nodiscard]] ValueType minimalValue() const;
    [[nodiscard]] ValueType maximalValue() const;
    //хэш поля
    [[nodiscard]] HashKeyType getHash() const;
    //эвристическая оценка поля
    [[nodiscard]] ValueType heuristicValue() const;
    //возможные ходы из текущей позиции
    [[nodiscard]] std::vector<MoveDescriptorType> getAvailableMoves(bool myMove) const;

    //additional api
    //устанавливает, какие эвристики используются
    void initializeHeuristics(HeuristicsStorageType h);
    //смотрит результат игры в текущий момент
    [[nodiscard]] ResultType countCurrentResult() const;

    //эквивалентные преобразования поля
    [[nodiscard]] TicTacField turn90() const;
    [[nodiscard]] TicTacField turnAroundVertical() const;
    [[nodiscard]] TicTacField turnAroundHorizontal() const;
    [[nodiscard]] TicTacField turnAroundDiagonal() const;

    //хэш только данного поля
    [[nodiscard]] HashKeyType simpleHash() const;
    //хэш данного и эквивалентных полей (считается как минимальный из вышеперечисленных)
    [[nodiscard]] HashKeyType allHashes() const;

    //устнавливает, кто ходит первый
    void setMoveOrder(bool iMoveFirst);
    //вывод поля в ostream
    friend std::ostream &operator<<(std::ostream &, const TicTacField &);

    //сбрасывает значение поля до изначального
    void drop();
    //длится ли игра?
    [[nodiscard]] bool gameInProgress() const;
};


#endif //TICTACTOE_TICTACFIELD_H
