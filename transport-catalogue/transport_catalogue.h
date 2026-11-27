#pragma once

#include <unordered_map>
#include <string_view>
#include <string>
#include <deque>
#include <vector>
#include <optional>
#include <set>

#include "geo.h"

namespace tansport_catalogue {

struct Stop {
    std::string stop_name;
    Coordinates coordinates;
};

struct RouteInfo {
    size_t number_of_stops;
    size_t number_unique_stops;
    double route_length;
};

struct Route {
    std::string route_name;
    std::vector<std::string_view> stop_names;
    RouteInfo route_info;
};

struct StopInfo {
    std::set<std::string_view> routes_incl_stop;
};

class TransportCatalogue {
public:
    void AddStop(Stop& stop);
    void AddRoute(Route& route);
    const Stop* GetStop(std::string_view stop_name) const;
    const Route* GetRoute(std::string_view route_name) const;
    RouteInfo GetRouteInfo(std::string_view route_name) const;
    const StopInfo& GetStopInfo(std::string_view stop_name) const;

private:
        std::deque<Stop> stops_;
        std::unordered_map<std::string_view, const Stop*> pointers_to_all_stops_;
        std::deque<Route> routes_;
        std::unordered_map<std::string_view, const Route*> pointers_to_all_routes_;
        std::unordered_map<std::string_view, StopInfo> stop_info_;
};

}
