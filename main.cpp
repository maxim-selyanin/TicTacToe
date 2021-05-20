#include "application.h"
#include "TicTacField.h"

int main() {
    application<TicTacField> a;
    //задаём эвристики
    a.getField().initializeHeuristics(MyLines | PointsOnLines );
    //задаём максимальную глубину рекурсии
    a.getPlayer().setRecursionDepth(1);
    //играем
    return a.exec();
}
