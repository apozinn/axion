#pragma once

#include "string"
#include "dpp/dpp.h"

namespace Utils
{
    dpp::component GenerateButton(std::string label, const dpp::component_style &style, std::string _id);
};