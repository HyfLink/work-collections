#include "LeastTransferRoutes.h"

template <typename FrIter> bool has_intersection(FrIter first1, FrIter last1, FrIter first2, FrIter last2) {
    while (first1 != last1 && first2 != last2) {
        if (*first2 == *first1) {
            return true;
        } else if (*first1 < *first2) {
            ++first1;
        } else {
            ++first2;
        }
    }
    return false;
}

template <typename Container> bool has_intersection(const Container &x, const Container &y) {
    return has_intersection(x.begin(), x.end(), y.begin(), y.end());
}

LeastTransferRoutes::LeastTransferRoutes(const StopVec &stops, const LineVec &lines)
    : IRoutesPlanner(), stops(stops), lines(lines), graph(build_graph(stops, lines)) {}

auto LeastTransferRoutes::build_graph(const StopVec &stops, const LineVec &lines) -> Graph {
    // graph[i] specifies the adjacency stops(or nodes) of stops[i]
    // stops[i] and stops[j] has and edge if they are adjacent in some lines

    Graph result(stops.size(), AdjacencyList{});
    for (size_t l = 0; l < lines.size(); ++l) {
        for (size_t s = 1; s < lines[l].size(); ++s) {
            result[lines[l][s - 0]].push_back({ lines[l][s - 1], l });
            result[lines[l][s - 1]].push_back({ lines[l][s - 0], l });
        }
    }
    return result;
}

auto LeastTransferRoutes::route(Coord src_coord, Coord dst_coord) -> std::vector<StopRoute> {
    // implemented by dfs and backtrace

    const Index src = stops.find_closest_stop(src_coord); // index of the source stop
    const Index dst = stops.find_closest_stop(dst_coord); // index of the destination stop

    std::vector<bool>          visited(stops.size(), false); // visited[i] is true specifies stops[i] is visited
    std::vector<GraphNode>     stack1;                       // current route
    std::vector<AdjacencyList> stack2;                       // stack2[i] is stack1[i]'s adjacency list
    std::vector<StopRoute>     result;                       // the return value
    size_t                     transfers = 0;                // current number of transfers
    size_t threshold = std::numeric_limits<size_t>::max();   // transfers threshold (current minimum transfers)

    stack1.reserve(stops.size()); // reserve vectors
    stack2.reserve(stops.size()); //
    result.reserve(stops.size()); //

    // dfs, implementation of 'transfer_next'
    // push the next stop to stack
    auto goto_next = [&](Index stop, Index line) {
        visited[stop] = true;
        stack1.emplace_back(stop, line);
        stack2.emplace_back();
        stack2.back().reserve(graph[stop].size());
        std::copy_if(graph[stop].begin(), graph[stop].end(), std::back_inserter(stack2.back()),
                     [&](const GraphNode &node) { return !visited[std::get<0>(node)]; });
    };

    // dfs, update 'transfers' and call 'goto_next'
    auto transfer_next = [&](const GraphNode &node) {
        auto [next_stop, next_line] = node;
        transfers += (next_line != std::get<1>(stack1.back())); // update transfers
        goto_next(next_stop, next_line);
    };

    // backtrace, update 'transfers' and
    // pop the current stop from stack
    auto transfer_prev = [&] {
        auto [this_stop, this_line] = stack1.back();
        stack1.pop_back();
        stack2.pop_back();
        transfers -= (this_line != std::get<1>(stack1.back()));
        visited[this_stop] = false;
    };

    for (goto_next(src, -1); !stack1.empty();) {
        // if no way to go or 'transfers' is to large
        //      go back to the previous stop
        // else
        //      go to the next stop
        if (stack2.back().empty() || transfers >= threshold) {
            transfer_prev();
        } else {
            const auto &node = stack2.back().back();
            stack2.back().pop_back();
            transfer_next(node);
        }

        if (std::get<0>(stack1.back()) == dst) {
            // we make it to the destination
            if (transfers == 0) {
                return {}; // no need to take subway
            } else if (transfers < threshold) {
                // remove old results and update threshold
                // because we find the better result
                result.clear();
                threshold = transfers;
            }
            // record the result
            result.emplace_back();
            result.back().transfers = transfers;
            result.back().reserve(stack1.size());
            std::transform(stack1.begin(), stack1.end(), std::back_inserter(result.back()),
                           [](const GraphNode &node) { return std::get<0>(node); });

            // go back and continue
            transfer_prev();
        }
    }

    return result;
}
