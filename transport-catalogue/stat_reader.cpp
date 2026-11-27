#include "stat_reader.h"
#include <iostream>
#include <iomanip>
#include <set>

std::string_view Trim1(std::string_view string) {
    const auto start = string.find_first_not_of(' ');
    if (start == string.npos) {
        return {};
    }
    return string.substr(start, string.find_last_not_of(' ') + 1 - start);
}

RequestDescription ParseRequestDescription(std::string_view line) {
    auto space_pos = line.find(' ');
    auto not_space = line.find_first_not_of(' ', space_pos);
    auto not_space_last = line.find_last_not_of(' ', space_pos);

    return {std::string(line.substr(0, space_pos)),
            std::string(Trim1(line.substr(not_space, not_space_last - not_space + 1))),
           ""};
}

void ParseAndPrintStat(const tansport_catalogue::TransportCatalogue& transport_catalogue, std::string_view request, std::ostream& output) {
    RequestDescription request_parsed = ParseRequestDescription(request);
    if(request_parsed.request == "Bus") {
        auto route_exist = transport_catalogue.GetRoute(request_parsed.id);
        if(route_exist == nullptr){
            output << request << ": not found" << std::endl;
        } else {
            const tansport_catalogue::RouteInfo route_info = transport_catalogue.GetRouteInfo(request_parsed.id);
            output << request << ": " << route_info.number_of_stops << " stops on route, "
               << route_info.number_unique_stops << " unique stops, "
               << std::setprecision(6) << route_info.route_length << " route length" << std::endl;
        }
    }

    if(request_parsed.request == "Stop") {
        auto stop_info = transport_catalogue.GetStop(request_parsed.id);
        if(stop_info == nullptr){
            output << request << ": not found" << std::endl;
        } else {
            if((transport_catalogue.GetStopInfo(request_parsed.id)).routes_incl_stop.empty()) {
                output << request << ": no buses" << std::endl;
            } else {
                output << request << ": buses";
                for(const auto& route_name: (transport_catalogue.GetStopInfo(request_parsed.id)).routes_incl_stop){
                    output << " " << route_name;
                }
                output << std::endl;
            }
        }
    }
}
