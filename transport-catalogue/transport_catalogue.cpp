#include "transport_catalogue.h"
#include <optional>
#include <stdexcept>

using namespace std;

namespace tansport_catalogue {

void TransportCatalogue::AddStop(Stop& stop){
    stops_.push_back(std::move(stop));
    pointers_to_all_stops_[stops_.back().stop_name] = &stops_.back();
    stop_info_[stops_.back().stop_name] = {};
}

void TransportCatalogue::AddRoute(Route& route) {
    routes_.push_back(std::move(route));
    pointers_to_all_routes_[routes_.back().route_name] = &routes_.back();

    auto route_ptr = pointers_to_all_routes_[routes_.back().route_name];
    auto unique_stops = std::set<std::string_view>((*route_ptr).stop_names.begin(), (*route_ptr).stop_names.end());

    for(const auto& stop: unique_stops) {
        if(pointers_to_all_stops_.contains(stop)) {
            stop_info_[stop].routes_incl_stop.insert(routes_.back().route_name);
        }
    }
}

const Stop* TransportCatalogue::GetStop(std::string_view stop_name) const {
    auto stop = pointers_to_all_stops_.find(stop_name);
    if(stop != pointers_to_all_stops_.end()) {
        return stop->second;
    }
    return nullptr;
}

const Route* TransportCatalogue::GetRoute(std::string_view route_name) const {
    auto route = pointers_to_all_routes_.find(route_name);
    if(route != pointers_to_all_routes_.end()) {
        return route->second;
    }
    return nullptr;
}

RouteInfo TransportCatalogue::GetRouteInfo(std::string_view route_name) const {
    const Route* route = GetRoute(route_name);
    if(route != nullptr) {
        return (*pointers_to_all_routes_.at(route_name)).route_info;
    }
    return RouteInfo();
}

const StopInfo& TransportCatalogue::GetStopInfo(std::string_view stop_name) const {
    const Stop* stop = GetStop(stop_name);
    if (stop == nullptr) {
        throw std::out_of_range("Stop not found");
    }
    return stop_info_.at(stop_name);
}

}
