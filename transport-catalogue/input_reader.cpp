#include "input_reader.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <set>
#include <iostream>

using namespace std::literals;

/**
 * Парсит строку вида "10.123,  -30.1837" и возвращает пару координат (широта, долгота)
 */
Coordinates ParseCoordinates(std::string_view str) {
    static const double nan = std::nan("");

    auto not_space = str.find_first_not_of(' ');
    auto comma = str.find(',');

    if (comma == str.npos) {
        return {nan, nan};
    }

    auto not_space2 = str.find_first_not_of(' ', comma + 1);

    double lat = std::stod(std::string(str.substr(not_space, comma - not_space)));
    double lng = std::stod(std::string(str.substr(not_space2)));

    return {lat, lng};
}

/**
 * Удаляет пробелы в начале и конце строки
 */
std::string_view Trim(std::string_view string) {
    const auto start = string.find_first_not_of(' ');
    if (start == string.npos) {
        return {};
    }
    return string.substr(start, string.find_last_not_of(' ') + 1 - start);
}

/**
 * Разбивает строку string на n строк, с помощью указанного символа-разделителя delim
 */
std::vector<std::string_view> Split(std::string_view string, char delim) {
    std::vector<std::string_view> result;

    size_t pos = 0;
    while ((pos = string.find_first_not_of(' ', pos)) < string.length()) {
        auto delim_pos = string.find(delim, pos);
        if (delim_pos == string.npos) {
            delim_pos = string.size();
        }
        if (auto substr = Trim(string.substr(pos, delim_pos - pos)); !substr.empty()) {
            result.push_back(substr);
        }
        pos = delim_pos + 1;
    }

    return result;
}

/**
 * Парсит маршрут.
 * Для кольцевого маршрута (A>B>C>A) возвращает массив названий остановок [A,B,C,A]
 * Для некольцевого маршрута (A-B-C-D) возвращает массив названий остановок [A,B,C,D,C,B,A]
 */
std::vector<std::string_view> ParseRoute(std::string_view route) {
    if (route.find('>') != route.npos) {
        return Split(route, '>');
    }

    auto stops = Split(route, '-');
    std::vector<std::string_view> results(stops.begin(), stops.end());
    results.insert(results.end(), std::next(stops.rbegin()), stops.rend());

    return results;
}

CommandDescription ParseCommandDescription(std::string_view line) {
    auto colon_pos = line.find(':');
    if (colon_pos == line.npos) {
        return {};
    }

    auto space_pos = line.find(' ');
    if (space_pos >= colon_pos) {
        return {};
    }

    auto not_space = line.find_first_not_of(' ', space_pos);
    if (not_space >= colon_pos) {
        return {};
    }

    return {std::string(line.substr(0, space_pos)),
            std::string(Trim(line.substr(not_space, colon_pos - not_space))),
            std::string(line.substr(colon_pos + 1))};
}

void InputReader::ParseLine(std::string_view line) {
    auto command_description = ParseCommandDescription(line);
    if (command_description) {
        commands_.push_back(std::move(command_description));
    }
}

double RouteLength(tansport_catalogue::TransportCatalogue& catalogue, std::vector<std::string_view> route) {
    double dist = 0;
    auto previous_stop_coord = (*catalogue.GetStop(route[0]))->coordinates;
    for(int i = 1; i < route.size(); ++i) {
        auto curr_stop_coord = (*catalogue.GetStop(route[i]))->coordinates;
        dist += ComputeDistance(previous_stop_coord, curr_stop_coord);
        //std::cout << "dist " << i << ": " << dist << std::endl;
        previous_stop_coord.lat = curr_stop_coord.lat;
        previous_stop_coord.lng = curr_stop_coord.lng;
    }
    return dist;
}

void InputReader::ApplyCommands([[maybe_unused]] tansport_catalogue::TransportCatalogue& catalogue) const {
    for(auto& item : commands_){
        if(item.command == "Bus"s) {
            continue;
        }
        tansport_catalogue::Stop stop;
        stop.stop_name = item.id;
        auto coordinates_string = Trim(item.description);
        stop.coordinates = ParseCoordinates(coordinates_string);
        catalogue.AddStop(stop);
    }

    for(auto& item : commands_){
        if(item.command == "Stop"s) {
            continue;
        }
        tansport_catalogue::Route route;
        route.route_name = item.id;

        auto route_string = Trim(item.description);
        route.stop_names = ParseRoute(route_string);

        route.route_info.number_of_stops = route.stop_names.size();
        route.route_info.number_unique_stops = std::set<std::string_view>(route.stop_names.begin(), route.stop_names.end()).size();
        route.route_info.route_length = RouteLength(catalogue, route.stop_names);

        /*
        std::cout << "Route: " << "\"" << route.route_name << "\"" << std::endl;
        int i =0;
        for(auto& stop: route.stop_names){
            ++i;
            std::cout << "Stop: " << i << ": " << "\"" << stop << "\"" << std::endl;
        }
        */
        //AddStopInfo(catalogue, route);
        catalogue.AddRoute(route);
    }
}







































