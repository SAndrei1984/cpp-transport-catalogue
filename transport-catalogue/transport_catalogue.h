#ifndef TRANSPORT_CATALOGUE_H
#define TRANSPORT_CATALOGUE_H

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
    //std::pair<double, double> coordinates;
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
    bool stop_incl_in_routes = false;
    std::set<std::string_view> routes_incl_stop;

};

class TransportCatalogue {
    public:
    void AddStop(Stop& stop);
    void AddRoute(Route& route);
    const std::optional<Stop*> GetStop(std::string_view stop_name) const;          // сделать private?
    const std::optional<Route*> GetRoute(const std::string& route_name) const;                  // сделать private?
    const RouteInfo& GetRouteInfo(const std::string& route_name) const;
    const std::optional<StopInfo> GetStopInfo(const std::string& stop_name) const;

    private:
        std::deque<Stop> stops_;
        std::unordered_map<std::string_view, Stop*> pointers_to_all_stops_;
        std::deque<Route> routes_;
        std::unordered_map<std::string_view, Route*> pointers_to_all_routes_;
        std::unordered_map<Stop*, StopInfo> stop_info_;
};

}

#endif
