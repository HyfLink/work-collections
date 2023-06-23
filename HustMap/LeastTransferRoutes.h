#pragma once

#include "IRoutesPlanner.h"

class LeastTransferRoutes final : public IRoutesPlanner {
public:
    LeastTransferRoutes(const StopVec&, const LineVec&);
    ~LeastTransferRoutes() override = default;

    std::vector<StopRoute> route(Coord src, Coord dst) override;

public:
    // GraphNode has two portion { s, l }
    // Graph[i][_] == GraphNode { s, l } means
    //      stop[i] and stop[s] are adjacent in line[l]
    //
    using GraphNode = std::tuple<Index, Index>;
    using AdjacencyList = std::vector<GraphNode>;
    using Graph = std::vector<AdjacencyList>;

private:
    static auto build_graph(const StopVec&, const LineVec&) -> Graph;

private:
    const StopVec& stops;
    const LineVec& lines;
    const Graph graph;
};