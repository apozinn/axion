#include "rooster/utils.hpp"

namespace RoosterUtils
{
    std::vector<Rooster> CreateUserRooster(int length);

    std::string AvailableRooters(std::vector<Rooster> &roosters)
    {
        std::string rts;
        for (auto rt : roosters)
        {
            rts.append(
                fmt::format("{}**{}** ({} - {})\n[ life: {} | atk: {} | def: {} | level: {} ]",
                            !rts.size() ? "" : "\n",
                            rt.name,
                            rt.classe,
                            rt.rarity,
                            std::to_string(rt.life),
                            std::to_string(rt.atk),
                            std::to_string(rt.def),
                            std::to_string(rt.level)));
        }
        return rts;
    }
}
