#include "utils/utils.hpp"

namespace Utils
{
    dpp::component GenerateButton(std::string label, const dpp::component_style &style, std::string _id)
    {
        auto button = dpp::component()
                          .set_label(label)
                          .set_type(dpp::cot_button)
                          .set_style(style)
                          .set_id(_id);
        return button;
    }
}