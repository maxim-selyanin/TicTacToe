#include "HeuristicExtremeValues.h"

HeuristicExtremeValues &operator+=(HeuristicExtremeValues &increased, const HeuristicExtremeValues &addendum) {
    increased.max += addendum.max;
    increased.min += addendum.min;
    increased.drawValue += addendum.drawValue;
    return increased;
}
