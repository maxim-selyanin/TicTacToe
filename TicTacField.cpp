#include "TicTacField.h"
#include <string>
#include <iostream>
#include "Heuristics.h"

using namespace HeuristicsWeights;
using namespace Heuristics;

TicTacField::ValueType TicTacField::minimalValue() const {
    return extremeValues.min;
}

TicTacField::ValueType TicTacField::maximalValue() const {
    return extremeValues.max;
}

TicTacField::ValueType TicTacField::heuristicValue() const {
    return totalizingFunction(chosenHeuristics,
                              this->positions, this->result(), extremeValues, this->myMoveNow);
}

void TicTacField::initializeHeuristics(HeuristicsStorageType h) {
    chosenHeuristics = h;
    extremeValues = totalExtremes(chosenHeuristics);
}

TicTacField::MoveDescriptorType TicTacField::noMove() {
    return TicTacField::nullMove;
}

void TicTacField::updateResult() {
    gameResult = countCurrentResult();
}

TicTacField::ResultType TicTacField::countCurrentResult() const {
    Line::LineStorage lines = Line::getAllLines(positions);
    int wastedCount = 0;
    for (const Line &line: lines) {
        switch (line.getType()) {
            case Line::Type::wasted: {
                ++wastedCount;
                break;
            }
            case Line::Type::free: {
                break;
            }
            case Line::Type::mine: {
                if (line.countFreePositions() == 0) {
                    return ResultType::iWin;
                }
                break;
            }
            case Line::Type::opponents: {
                if (line.countFreePositions() == 0) {
                    return ResultType::opponentWin;
                }
                break;
            }
        }
    }
    if (wastedCount == Line::lineAmount) {
        return ResultType::draw;
    } else {
        return ResultType::none;
    }
}

TicTacField::ResultType TicTacField::result() const {
    return gameResult;
}

TicTacField::HashKeyType TicTacField::allHashes() const {
    std::array<HashKeyType, 7> hashes{};
    TicTacField f(*this);
    for (int i = 0; i < 4; ++i) {
        hashes[i] = f.simpleHash();
        f = f.turn90();
    }

    f = f.turnAroundDiagonal();
    hashes[4] = f.simpleHash();
    f = f.turnAroundDiagonal();//вернулись в изначальное положение

    f = f.turnAroundHorizontal();
    hashes[5] = f.simpleHash();
    f = f.turnAroundHorizontal();//вернулись в изначальное положение

    f = f.turnAroundVertical();
    hashes[6] = f.simpleHash();

    return *std::min_element(hashes.begin(), hashes.end());
}

TicTacField::HashKeyType TicTacField::getHash() const {
    return this->allHashes();
}

TicTacField::HashKeyType TicTacField::simpleHash() const {
    std::string s;
    s.reserve(PositionNumber);
    for (PositionState state: positions) {
        s.push_back(
                TicTacField::positionHashValue(state)
                );
    }
    auto result = static_cast<HashKeyType>(
            std::stoi(s, nullptr, 3)
            );
    return result;
}

char TicTacField::positionHashValue(PositionState state) {
    switch (state) {
        case PositionState::free: {
            return '0';
        }
        case PositionState::mine: {
            return '1';
        }
        case PositionState::opponents: {
            return '2';
        }
    }
}

std::vector<TicTacField::MoveDescriptorType> TicTacField::getAvailableMoves(bool myMove) const {
    if (result() != ResultType::none) {
        return std::vector<MoveDescriptorType>();
    }

    unsigned char myMoveSetter = myMove
            ? myMoveBit
            : 0;
    std::vector<MoveDescriptorType> result{};
    for (PositionIndexType i = 0; i < PositionNumber; ++i) {
        if (this->positions[i] == PositionState::free) {
            result.push_back(
                    i | myMoveSetter
                    );
        }
    }
    return result;
}

bool TicTacField::makeMove(TicTacField::MoveDescriptorType move) {
    if (move == nullMove) {
        return false;
    }

    bool myMove = move & TicTacField::myMoveBit;
    PositionIndexType pos = move & (~myMoveBit);
    if (this->positions[pos] == PositionState::free) {
        this->positions[pos] = myMove
                ? PositionState::mine
                : PositionState::opponents;
        updateResult();
        this->myMoveNow = !myMove;
        return true;
    } else {
        return false;
    }
}

bool TicTacField::undoMove(TicTacField::MoveDescriptorType move) {
    if (move == nullMove) {
        return false;
    }

    PositionIndexType pos = move & (~myMoveBit);
    if (this->positions[pos] == PositionState::free) {
        return false;
    } else {
        this->positions[pos] = PositionState::free;
        updateResult();
        myMoveNow = !myMoveNow;
        return true;
    }
}

std::ostream &operator<<(std::ostream &out, const TicTacField &f) {
    out << '\n';
    for (int i = 0; i < PositionNumber; ++i) {
        if (i % 3 == 0) {
            out << '\n';
        }
        out <<
        TicTacField::positionOutputRepresentation(
                f.positions[i]
                ,f.myMoveFirst
                ,i
                )
                ;
    }
    out << '\n' << '\n';
    return out;
}

char TicTacField::positionOutputRepresentation(PositionState state,
                                               bool iMoveFirst, int number) {
    switch (state) {
        case PositionState::free: {
            return char(number + 1 + int('0'));
        }
        case PositionState::mine: {
            return iMoveFirst
            ? cross
            : zero;
        }
        case PositionState::opponents: {
            return iMoveFirst
                   ? zero
                   : cross;
        }
    }
}

void TicTacField::setMoveOrder(bool iMoveFirst) {
    this->myMoveFirst = iMoveFirst;
    this->myMoveNow = iMoveFirst;
}

void TicTacField::drop() {
    for (PositionState &state: positions) {
        state = PositionState::free;
    }
    gameResult = ResultType::none;
    myMoveFirst = true;
    myMoveNow = true;
}

bool TicTacField::gameInProgress() const {
    return result() == ResultType::none;
}

bool TicTacField::isMyMoveNow() const {
    int count = 0;
    for (PositionState state: positions) {
        if (state == PositionState::free) {
            ++count;
        }
    }
    count = PositionNumber - count;//количество несвободных позиций
    bool odd = count % 2;//нечётность
    return odd != this->myMoveFirst;
}

TicTacField TicTacField::turn90() const {
    TicTacField f(*this);

    std::swap(f.positions[1], f.positions[3]);
    std::swap(f.positions[3], f.positions[7]);
    std::swap(f.positions[7], f.positions[5]);

    std::swap(f.positions[2], f.positions[0]);
    std::swap(f.positions[0], f.positions[6]);
    std::swap(f.positions[6], f.positions[8]);

    return f;
}

TicTacField TicTacField::turnAroundDiagonal() const {
    TicTacField f(*this);
    std::swap(f.positions[3], f.positions[7]);
    std::swap(f.positions[1], f.positions[5]);
    std::swap(f.positions[0], f.positions[8]);
    return f;
}

TicTacField TicTacField::turnAroundHorizontal() const {
    TicTacField f(*this);
    std::swap(f.positions[2], f.positions[8]);
    std::swap(f.positions[1], f.positions[7]);
    std::swap(f.positions[0], f.positions[6]);
    return f;
}

TicTacField TicTacField::turnAroundVertical() const {
    TicTacField f(*this);
    std::swap(f.positions[6], f.positions[8]);
    std::swap(f.positions[3], f.positions[5]);
    std::swap(f.positions[0], f.positions[2]);
    return f;
}

TicTacField::TicTacField(const TicTacField &other)
: extremeValues(other.extremeValues)
, gameResult(other.gameResult)
, myMoveFirst(other.myMoveFirst)
, myMoveNow(other.myMoveNow)
{
    for (int i = 0; i < PositionNumber; ++i) {
        positions[i] = other.positions[i];
    }
}