#ifndef TICTACTOE_HEURISTICS_H
#define TICTACTOE_HEURISTICS_H

#include "heuristicsweights.h"
#include "TicTacFieldDefinitions.h"
#include "Line.h"
#include "HeuristicExtremeValues.h"

using namespace HeuristicsWeights;
using namespace Definitions;

namespace Heuristics {
    //анализирует, кто какиеми линиями владеет
    [[nodiscard]] ValueType myLinesHeuristic(
            const PositionState positions[PositionNumber], ResultType result
            , HeuristicExtremeValues values, bool myMove);
    //анализирует, сколько линий из 8 занимают точки каждого игрока
    [[nodiscard]] ValueType pointsOnLinesHeuristic(
            const PositionState positions[PositionNumber], ResultType result,
            HeuristicExtremeValues values, bool myMove);

    //экстремумы и ничейные значения соответствующих эвристик
    [[nodiscard]] HeuristicExtremeValues myLinesHeuristicExtremes();
    [[nodiscard]] HeuristicExtremeValues pointsOnLinesHeuristicExtremes();

    //применяет эвристики, установленные в heuristicsBit
    [[nodiscard]] ValueType totalizingFunction(HeuristicsStorageType heuristicsBit
                                               , const PositionState positions[PositionNumber]
                                               , ResultType result,
                                               HeuristicExtremeValues values, bool myMove);
    [[nodiscard]] HeuristicExtremeValues totalExtremes(HeuristicsStorageType heuristicsBit);


    //часть MyLinesHeuristic. анализирует, кто владеет линиями
    [[nodiscard]] ValueType POLHlines(const Line::LineStorage &lines, bool myMove);
    //часть MyLinesHeuristic. анализирует пересечения
    [[nodiscard]] ValueType POLHintersections(const Line::LineStorage &lines, bool myMove);
    [[nodiscard]] ValueType POLHpartIntersections(const Line::LineStorage &lines, int initialOffset, bool myMove);
}

#endif //TICTACTOE_HEURISTICS_H
