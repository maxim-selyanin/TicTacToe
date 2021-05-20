#include "Line.h"

Line::LineStorage Line::getAllLines(const PositionState positions[PositionNumber]) {
    Line::LineStorage result{};
    for (int i = 0; i < 3; ++i) {
        result[i] = Line::getHorizontalLine(positions, i);
    }
    for (int i = 3; i < 6; ++i) {
        result[i] = Line::getVerticalLine(positions, i - 3);
    }
    result[6] = Line::getDiagonalLine(positions, 0, 4);
    result[7] = Line::getDiagonalLine(positions, 2, 2);
    return result;
}

Line::Type Line::getType(int without) const {
    unsigned char p[3] = {
            static_cast<unsigned char>(positions[0])
            ,static_cast<unsigned char>(positions[1])
            ,static_cast<unsigned char>(positions[2])
    };

    if (without != -1) {
        p[without] = allBits;
    }

    unsigned char bitVal = p[0] & p[1] & p[2];

    if (bitVal & freeBit) {//линия пуста
        return Type::free;
    } else if (bitVal == 0) {//на линии есть и свои, и вражеские метки
        return Type::wasted;
    } else if (bitVal & mineBit) {//на линии только свои метки
        return Type::mine;
    } else {//на линии только чужие метки
        return Type::opponents;
    }
}

int Line::countFreePositions() const {
    int count = 0;
    for (PositionState state: positions) {
        if (state == PositionState::free) {
            ++count;
        }
    }
    return count;
}

typename Line::PositionIndexType Line::getIntersectionPoint(
        const Line &a
        ,const Line &b)
{
    for (PositionIndexType index1: a.positionIndices) {
        for (PositionIndexType index2: b.positionIndices) {
            if (index1 == index2) {
                return index1;
            }
        }
    }
    return indexErr;
}

int Line::getLocalIndexOfPosition(PositionIndexType globalIndex) const
{
    for (int i = 0; i < 3; ++i) {
        if (this->positionIndices[i] == globalIndex) {
            return i;
        }
    }
    return -1;
}

bool Line::isOnStrikeIntersection(const Line &a
        , const Line &b
        , Line::Type targetType
        , bool &pointOnIntersection)
{
    //смотрим точку пересечения
    PositionIndexType intersectionPoint = Line::getIntersectionPoint(a, b);
    if (intersectionPoint == indexErr) {
        pointOnIntersection = false;
        return false;
    }
    int indexA = a.getLocalIndexOfPosition(intersectionPoint);
    int indexB = b.getLocalIndexOfPosition(intersectionPoint);
    //смотрим типы линий, если удалить точку пересечения
    Line::Type typeA = a.getType(indexA);
    Line::Type typeB = b.getType(indexB);
    pointOnIntersection = (b.positions[indexB] != PositionState::free);
    //если типы равны таргету, то получается опасное пересечение с возможностью вилки
    return typeA == targetType && typeB == targetType;
}

bool Line::isMine() const {
    return this->getType() == Type::mine;
}

bool Line::nobodys() const {
    Type type = this->getType();
    return type == Type::wasted ||  type == Type::free;
}

int Line::occupiedPositions() const {
    return Line::PositionAmount - this->countFreePositions();
}

bool Line::isEnemys() const {
    return this->getType() == Type::opponents;
}

PositionIndexType Line::forcedMove() const {
    int freeIndex = indexErr;
    for (int i = 0; i < 3; ++i) {
        //нашли свободную клетку
        if (positions[i] == PositionState::free) {
            //если это первая свободная
            if (freeIndex == indexErr) {
                freeIndex = positionIndices[i];
            } else {//если это не первая свободная
                return indexErr;//нет форсед мува
            }
        }
    }
    return freeIndex;
}

bool Line::containsGlobalIndex(PositionIndexType index) const {
    for (PositionIndexType i: positionIndices) {
        if (index == i) {
            return true;
        }
    }
    return false;
}

Line Line::getDiagonalLine(
        const PositionState positions[
        PositionNumber], int startVal, int interval) {
    int current = startVal;
    Line line{};
    for (int i = 0; i < 3; ++i) {
        line.positionIndices[i] = current;
        line.positions[i] = positions[
                line.positionIndices[i]
        ];
        current += interval;
    }
    return line;
}

Line Line::getVerticalLine(
        const PositionState positions[
        PositionNumber], int number) {
    Line line{};
    for (int i = 0; i < 3; ++i) {
        line.positionIndices[i] = number + i * 3;
        line.positions[i] = positions[
                line.positionIndices[i]
        ];
    }
    return line;
}

Line Line::getHorizontalLine(
        const PositionState positions[
        PositionNumber], int number) {
    Line line{};
    for (int i = 0; i < 3; ++i) {
        line.positionIndices[i] = number * 3 + i;
        line.positions[i] = positions[
                line.positionIndices[i]
        ];
    }
    return line;
}
