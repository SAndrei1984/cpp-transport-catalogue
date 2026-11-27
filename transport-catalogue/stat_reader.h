
#pragma once

#include <iosfwd>
#include <string_view>

#include "transport_catalogue.h"
#include "input_reader.h"

struct RequestDescription {
    explicit operator bool() const {
        return !request.empty();
    }

    bool operator!() const {
        return !operator bool();
    }

    std::string request;      // Название команды
    std::string id;           // id маршрута или остановки
    std::string description;  // Параметры команды на будущее, если пригодится.
};

void ParseAndPrintStat(const tansport_catalogue::TransportCatalogue& transport_catalogue, std::string_view request,
                       std::ostream& output);

