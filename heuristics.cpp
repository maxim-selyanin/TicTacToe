#include "Heuristics.h"

ValueType Heuristics::totalizingFunction(HeuristicsStorageType heuristicsBit,
                                         const PositionState positions[PositionNumber], ResultType result,
                                         HeuristicExtremeValues values, bool myMove) {
    ValueType value = 0;
    AvailableHeuristics dummy = AvailableHeuristics::PointsOnLines;
    switch (dummy) {
        case AvailableHeuristics::PointsOnLines: {
            if (PointsOnLines & heuristicsBit) {
                value += Heuristics::pointsOnLinesHeuristic(positions, result, values, myMove);
            }
        }
        case AvailableHeuristics::MyLines: {
            if (MyLines & heuristicsBit) {
                value += Heuristics::myLinesHeuristic(positions, result, values, myMove);
            }
        }
    }
    return value;
}

ValueType Heuristics::pointsOnLinesHeuristic(
        const PositionState *positions, ResultType result
        , HeuristicExtremeValues values, bool) {
    switch (result) {
        case ResultType::iWin: return values.max;
        case ResultType::opponentWin: return values.min;
        case ResultType::draw: return values.drawValue;
        case ResultType::none: break;
    }

    unsigned char myResult = 0;
    unsigned char opponentsResult = 0;
    for (int i = 0; i < PositionNumber; ++i) {
        switch (positions[i]) {
            case PositionState::free: {
                break;
            }
            case PositionState::mine: {
                myResult |= static_cast<unsigned char>(positionCodes[i]);
                break;
            }
            case PositionState::opponents: {
                opponentsResult |= static_cast<unsigned char>(positionCodes[i]);
                break;
            }
        }
    }

    return PointsOnLinesAdjustment * (setBitAmount(myResult) - setBitAmount(opponentsResult));
}

ValueType Heuristics::myLinesHeuristic(
        const PositionState *positions, ResultType result
        , HeuristicExtremeValues values, bool myMove) {
    switch (result) {
        case ResultType::iWin: return values.max;
        case ResultType::opponentWin: return values.min;
        case ResultType::draw: return values.drawValue;
        case ResultType::none: break;
    }

    //horizontal-vertical-diagonal
    Line::LineStorage lines = Line::getAllLines(positions);
    //обе составляющих эвристики
    return POLHlines(lines, myMove) + POLHintersections(lines, myMove);
}

ValueType Heuristics::POLHlines(const Line::LineStorage &lines
, bool myMove)
{
    ValueType myResult = 0;
    for (const Line &line: lines) {
        if (line.nobodys()) {
            continue;
        }
        bool myLine = line.isMine();
        int occupiedPoints = line.occupiedPositions();
        int adjustment = (myLine) ? 1 : -1;
        if (myLine == myMove) {
            //если occupiedPoints = 1 и ход того, у кого эта точка, то берём вес для 2 точек
            //если occupiedPoints = 2 и ход того же, то соответственно для 3
            ValueType weight;
            switch (occupiedPoints) {
                case 1: {
                    weight = twoPointsWeight;
                    break;
                }
                case 2: {
                    weight = threePointsWeight;
                    break;
                }
                default: {
                    weight = 0;
                    break;
                }
            }
            myResult += weight * adjustment;
        }
    }
    return myResult;
}

ValueType Heuristics::POLHintersections(const Line::LineStorage &lines
, bool myMove) {
    return POLHpartIntersections(lines, 0, myMove) + POLHpartIntersections(lines, 3, myMove);
}

ValueType Heuristics::POLHpartIntersections(const Line::LineStorage &lines
, int initialOffset
, bool myMove) {
    ValueType myResult = 0;
    const int amount = 3;
    for (int i = initialOffset; i < initialOffset + amount; ++i) {
        Line::Type type = lines[i].getType();
        //свободные и ничьи не рассматриваем
        if (lines[i].nobodys()) {
            continue;
        }
        bool myLines = lines[i].isMine();

        for (int j = initialOffset + amount; j < Line::lineAmount; ++j) {
            if (type == lines[j].getType()) {//пересекаются линии одного типа
                bool pointOnIntersection;
                //и они пересекаются опасным образом
                if (Line::isOnStrikeIntersection(lines[i], lines[j], type, pointOnIntersection)) {
                    //мой ход и моё пересечение
                    if (myMove && myLines) {
                        myResult += lineIntersectionWeight;
                    }
                    //ход противника и его пересечение
                    if (!myMove && !myLines) {
                        myResult -= lineIntersectionWeight;
                    }
                    //если на опасном пересечении уже есть пойнт
                    if (pointOnIntersection) {
                        int adjustment = myLines ? 1 : -1;
                        myResult += usedLineIntersectionWeight*adjustment;
                    }
                    //если ход и пересечение у разных игроков, то пересечение нейтрализуется
                }
            }
        }
    }
    return myResult;
}

HeuristicExtremeValues Heuristics::myLinesHeuristicExtremes() {
    int MLmaxVal = (twoPointsWeight + threePointsWeight) * Line::lineAmount
                   + lineIntersectionWeight * maxLineIntersectionAmount;
    HeuristicExtremeValues extreme{};
    extreme.max = MLmaxVal;
    extreme.min = -extreme.max;
    extreme.drawValue = 0;
    return extreme;
}

HeuristicExtremeValues Heuristics::pointsOnLinesHeuristicExtremes() {
    HeuristicExtremeValues extreme{};
    extreme.max = pointsOnLinesHeuristicMax;
    extreme.min = -extreme.max;
    extreme.drawValue = 0;
    return extreme;
}

HeuristicExtremeValues Heuristics::totalExtremes(HeuristicsStorageType heuristicsBit) {
    HeuristicExtremeValues value{};
    AvailableHeuristics dummy = AvailableHeuristics::PointsOnLines;
    switch (dummy) {
        case AvailableHeuristics::PointsOnLines: {
            if (PointsOnLines & heuristicsBit) {
                value += Heuristics::pointsOnLinesHeuristicExtremes();
            }
        }
        case AvailableHeuristics::MyLines: {
            if (MyLines & heuristicsBit) {
                value += Heuristics::myLinesHeuristicExtremes();
            }
        }
    }
    return value;
}
