#ifndef TICTACTOE_PLAYER_H
#define TICTACTOE_PLAYER_H

#include <iostream>
#include "myconcepts.h"
#include "commons.h"
#include <map>
#include <algorithm>

template<HashableField FieldType>
class Player {
    //definitions
    using MoveDescriptorType = typename FieldType::MoveDescriptorType;
    using ValueType = typename FieldType::ValueType;
    using HashKeyType = typename FieldType::HashKeyType;

    using WinCountType = unsigned long long;

    /* предпочтительность последовательности ходов
     * на первом месте оптимальность значения минимакс
     * на втором глубина рекурсии последовательности (чем меньше, тем лучше)
     * на третьем - количество итоговых победных комбинаций, доступных с такой комбинации
     */
    struct SolutionAvailability {
        ValueType value;
        unsigned int depth;
        WinCountType winCount;
        friend bool operator<(const SolutionAvailability &a, const SolutionAvailability &b) {
            if (a.value < b.value//а менее выгодно, чем b
                ||//а приводит к тому же результату медленнее, чем b
                a.value == b.value && a.depth > b.depth
                ||//а оставляет меньше варианов победы, чем b
                a.value == b.value && a.winCount < b.winCount
                    ) {
                return true;
            } else {
                return false;
            }
        }
        friend bool operator>(const SolutionAvailability &a, const SolutionAvailability &b) {
            if (a.value > b.value//а выгоднее b
                ||//а приводит к тому же результату быстрее, чем b
                a.value == b.value && a.depth < b.depth
                ||//а оставляет больше вариантов победы
                a.value == b.value && a.winCount > b.winCount
                    ) {
                return true;
            } else {
                return false;
            }
        }
    };


    //поле
    FieldType &field;
    //максимальная глубина рекурсии
    unsigned int recursionDepth = 2;
    //захешированные позиции
    std::map<HashKeyType, SolutionAvailability> myHash{};
    //производится ли альфа-бета отсечение?
    bool ABPrune = true;


    //рекурсивно считает значение минимакса
    SolutionAvailability getMinimax(unsigned int recursionLevel
            , MoveDescriptorType &resultingMove
            , ValueType alpha, ValueType beta);
    //смотрит, захеширован ли данный ключ
    bool hashed(HashKeyType key) const;
    //возвращает захешированное значение под данным ключом
    SolutionAvailability hashedValue(HashKeyType key) const;
    //хеширует данное значение
    void hash(HashKeyType key, SolutionAvailability value);
    //производит альфа-бета отсечение
    bool alphaBetaPrune(ValueType &alpha, ValueType &beta, ValueType currentValue, bool myMove);

public:
    using ResultType = typename FieldType::ResultType;

    explicit Player(FieldType &f);

    //ход (первый или после хода соперника)
    MoveDescriptorType makeMove();
    MoveDescriptorType makeMove(MoveDescriptorType opponentMove);

    //текущий результат игры
    [[nodiscard]] ResultType getResult() const;
    //выводит поле в out
    void showField(std::ostream &out) const;
    //устанавливает нужную глубину рекурсии
    void setRecursionDepth(unsigned int depth);
    //устанавливает, проводить ли альфа-бета отсечение
    void enableAlphaBetaPruning(bool set);
};

template<HashableField FieldType>
typename Player<FieldType>::MoveDescriptorType
Player<FieldType>::makeMove(Player<FieldType>::MoveDescriptorType opponentMove) {
    if (this->field.makeMove(opponentMove)) {
        showField(std::cout);
        return this->makeMove();
    } else {
        return FieldType::noMove();
    }
}

template<HashableField FieldType>
typename Player<FieldType>::MoveDescriptorType
Player<FieldType>::makeMove() {
    MoveDescriptorType move = FieldType::noMove();
    getMinimax(0, move, field.minimalValue(), field.maximalValue());
    field.makeMove(move);
    if (move != FieldType::noMove()) {
        showField(std::cout);
    }
    return move;
}

template<HashableField FieldType>
void
Player<FieldType>::setRecursionDepth(unsigned int depth) {
    if (depth) {
        recursionDepth = depth*2;//умножаем на два, чтобы каждый ход считался отдельно
    }
}

template<HashableField FieldType>
typename Player<FieldType>::ResultType
Player<FieldType>::getResult() const {
    return field.result();
}

template<HashableField FieldType>
typename Player<FieldType>::SolutionAvailability
Player<FieldType>::getMinimax(unsigned int recursionLevel
        , MoveDescriptorType &resultingMove
        , ValueType alpha, ValueType beta) {
    HashKeyType currentHash = field.getHash();
    if (hashed(currentHash) && recursionLevel > 0) {
        return hashedValue(currentHash);
    }

    if (recursionLevel >= recursionDepth) {
        ValueType hv = field.heuristicValue();
        SolutionAvailability result = {
                hv
                ,recursionLevel
                ,(hv == field.maximalValue())
                 ? static_cast<WinCountType>(1)
                 : static_cast<WinCountType>(0)
        };
        //не хешируем эвристические значения, так как они неточные
        return result;
    }

    bool myMove = !(recursionLevel % 2);

    struct {
        SolutionAvailability solution;
        MoveDescriptorType move;
    } bestMove {
            {
                    myMove
                    ? field.minimalValue()
                    : field.maximalValue()
                    ,0
                    ,0
            }
            , FieldType::noMove()
    };
    WinCountType myWinCount = 0;

    auto availableMoves = field.getAvailableMoves(myMove);

    if (availableMoves.begin() == availableMoves.end()) {//нет возможных ходов
        ValueType hv = field.heuristicValue();
        SolutionAvailability result = {hv
                ,recursionLevel
                ,(hv == field.maximalValue())
                 ? static_cast<WinCountType>(1)
                 : static_cast<WinCountType>(0)
        };
        //хэшируем финальное значение
        this->hash(currentHash, result);
        return result;
    }

    for (MoveDescriptorType move: availableMoves) {
        class TmpMove {
            FieldType &f;
            MoveDescriptorType m;
        public:
            TmpMove(const TmpMove&) = delete;
            TmpMove(TmpMove&&) = delete;
            explicit TmpMove(FieldType &field, MoveDescriptorType tmpMove)
                    : f(field)
                    , m(tmpMove)
            {f.makeMove(m);}
            ~TmpMove() {f.undoMove(m);}
        } temporaryMove(field, move);

        SolutionAvailability testValue = getMinimax(recursionLevel + 1, resultingMove, alpha, beta);

        myWinCount += testValue.winCount;

        if (bestMove.move == FieldType::noMove()) {//первый вариант хода
            bestMove.solution = testValue;
            bestMove.move = move;
        } else {
            if (myMove) {//мой ход
                if (testValue > bestMove.solution) {//лучше предыдущего значения
                    bestMove.solution = testValue;
                    bestMove.move = move;
                }
            } else {//ход противника
                if (testValue < bestMove.solution) {//хуже предыдущего значения
                    bestMove.solution = testValue;
                    bestMove.move = move;
                }
            }
        }

        //альфа-бета отсечение
        if (this->ABPrune && alphaBetaPrune(alpha, beta, testValue.value, myMove)) {break;}
    }

    SolutionAvailability result = {bestMove.solution.value
            , bestMove.solution.depth
            , myWinCount};

    if (recursionLevel == 0) {//изначальный вызов
        resultingMove = bestMove.move;
    }

    //итоговые значения тоже не хешируем, так как они могут основываться на эвристиках и быть неточными

    return result;
}

template<HashableField FieldType>
bool Player<FieldType>::hashed(HashKeyType key) const {
    return this->myHash.find(key) != this->myHash.end();
}

template<HashableField FieldType>
void Player<FieldType>::hash(HashKeyType key, SolutionAvailability value) {
    this->myHash[key] = value;
}

template<HashableField FieldType>
typename Player<FieldType>::SolutionAvailability
Player<FieldType>::hashedValue(HashKeyType key) const {
    return this->myHash.find(key)->second;
}

template<HashableField FieldType>
Player<FieldType>::Player(FieldType &f)
        : field(f)
{}

template<HashableField FieldType>
void Player<FieldType>::showField(std::ostream &out) const {
    out << field;
}

template<HashableField FieldType>
bool Player<FieldType>::alphaBetaPrune(ValueType &alpha, ValueType &beta
                                       , ValueType currentValue, bool myMove)
{
    if (myMove) {
        alpha = std::max(alpha, currentValue);
    } else {
        beta = std::min(beta, currentValue);
    }
    return beta < alpha;
}

template<HashableField FieldType>
void Player<FieldType>::enableAlphaBetaPruning(bool set) {
    this->ABPrune = set;
}

#endif //TICTACTOE_PLAYER_H
