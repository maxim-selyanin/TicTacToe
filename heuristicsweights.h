#ifndef TICTACTOE_HEURISTICSWEIGHTS_H
#define TICTACTOE_HEURISTICSWEIGHTS_H

#include "TicTacFieldDefinitions.h"
using namespace Definitions;

//веса игровых ситуаций для эвристики myLines
namespace HeuristicsWeights {
    //множитель для myLines
    static constexpr ValueType MyLinesAdjustment = 10;
    //множитель для pointsOnLines
    static constexpr ValueType PointsOnLinesAdjustment = 1;

    //максимально возможное количество пересечений
    static constexpr int maxLineIntersectionAmount = 21;

    //вес двух меток в строке
    static constexpr ValueType twoPointsWeight = 1 * MyLinesAdjustment;
    //вес трёх меток в строке
    static constexpr ValueType threePointsWeight = 100* MyLinesAdjustment;
    //вес пересечения линий с возможностью удара по обеим без метки на пересечении
    static constexpr ValueType lineIntersectionWeight = 5 * MyLinesAdjustment;
    //вес пересечения линий после удара по пересечению
    static constexpr ValueType usedLineIntersectionWeight = 10 * MyLinesAdjustment;

    //максимально возможное значение примитивной эвристики
    static constexpr ValueType pointsOnLinesHeuristicMax = 9 * PointsOnLinesAdjustment;
}

#endif //TICTACTOE_HEURISTICSWEIGHTS_H
