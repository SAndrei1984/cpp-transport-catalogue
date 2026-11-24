#include "transport_catalogue.h"
#include <optional>

using namespace std;

namespace tansport_catalogue {

void TransportCatalogue::AddStop(Stop& stop){
    stops_.push_back(std::move(stop));
    pointers_to_all_stops_[stops_.back().stop_name] = &stops_.back();
    stop_info_[pointers_to_all_stops_[stops_.back().stop_name]] = {false, {}};
}

void TransportCatalogue::AddRoute(Route& route) {
    routes_.push_back(std::move(route));
    pointers_to_all_routes_[routes_.back().route_name] = &routes_.back();

    auto route_ptr = pointers_to_all_routes_[routes_.back().route_name];
    auto unique_stops = std::set<std::string_view>((*route_ptr).stop_names.begin(), (*route_ptr).stop_names.end());

    for(const auto& stop: unique_stops) {
        if(pointers_to_all_stops_.contains(stop)) {
            stop_info_[pointers_to_all_stops_[stop]].stop_incl_in_routes = true;
            stop_info_[pointers_to_all_stops_[stop]].routes_incl_stop.insert(routes_.back().route_name); // Adds to stop_info_ field "routes_incl_stop" over key pointer to Stop by stop name
        }
    }
}

const std::optional<Stop*> TransportCatalogue::GetStop(std::string_view stop_name) const {
    if(pointers_to_all_stops_.count(stop_name) != 0) {
        return pointers_to_all_stops_.at(stop_name);
    }
    return nullopt;
}

const std::optional<Route*> TransportCatalogue::GetRoute(const std::string& route_name) const {
    if(pointers_to_all_routes_.count(route_name) != 0) {
        return pointers_to_all_routes_.at(route_name);
    }
    return nullopt;
}

const RouteInfo& TransportCatalogue::GetRouteInfo(const std::string& route_name) const {
    return (*pointers_to_all_routes_.at(route_name)).route_info;
}

const std::optional<StopInfo> TransportCatalogue::GetStopInfo(const std::string& stop_name) const {
    std::optional<Stop*> value = GetStop(stop_name);
    if(value.has_value()) {
        return stop_info_.at(*value);
    }
    return nullopt;
}

}
