#ifndef TICTACTOE_HEURISTICEXTREMEVALUES_H
#define TICTACTOE_HEURISTICEXTREMEVALUES_H

#include "TicTacFieldDefinitions.h"
using namespace Definitions;

//минимум, максимум и ничейное значение эвристик
struct HeuristicExtremeValues {
    ValueType max = 0;
    ValueType min = 0;
    ValueType drawValue = 0;
    friend HeuristicExtremeValues& operator+=(HeuristicExtremeValues &increased
            , const HeuristicExtremeValues &addendum);
};


#endif //TICTACTOE_HEURISTICEXTREMEVALUES_H
