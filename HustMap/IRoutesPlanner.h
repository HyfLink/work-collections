#pragma once

#include "HustMapTypes.h"

class IRoutesPlanner {
public:
    constexpr IRoutesPlanner() = default;
    virtual ~IRoutesPlanner() = default;

    virtual std::vector<StopRoute> route(Coord src, Coord dst) = 0;
};