#pragma once

#include <iostream>
#include <string>
#include <dpp/dpp.h>
#include <vector>
#include <cctype>
#include <random>
#include <algorithm>

#include "../../../utils/randoms.hpp"

using namespace std;

class Rewards {
	public:
		int corn;
		int user_xp;
		int rooster_xp;
		int uprank;
		int golden_spike;
		string box;
	void create(string opponent_rarity) {
		vector<vector<int>> min_max_props;
		if(opponent_rarity == "Comum") min_max_props = {{20, 50}, {10, 20}, {20, 30}, {15, 30}, {0}, {0, 25}};
		if(opponent_rarity == "Raro") min_max_props = {{50, 100}, {20, 40}, {30, 50}, {25, 50}, {0}, {0, 30}};
		if(opponent_rarity == "Epíco") min_max_props = {{100, 150}, {40, 60}, {50, 70}, {40, 80}, {1}, {0, 65}};
		if(opponent_rarity == "Lendário") min_max_props = {{150, 200}, {60, 80}, {70, 90}, {70, 90}, {1}, {0, 70}};
		if(opponent_rarity == "Mitíco") min_max_props = {{200, 250}, {80, 100}, {90, 100}, {90, 110}, {1}, {0, 91}};
		if(opponent_rarity == "Celestial") min_max_props = {{250, 200}, {100, 120}, {100, 130}, {110, 140}, {1}, {0, 100}};

		corn = randomInt(min_max_props[0][0], min_max_props[0][1]);
		user_xp = randomInt(min_max_props[1][0], min_max_props[1][1]);
		rooster_xp = randomInt(min_max_props[2][0], min_max_props[2][1]);
		uprank = randomInt(min_max_props[3][0], min_max_props[3][1]);
		golden_spike = 0;

		int box_luck = randomInt(min_max_props[5][0], min_max_props[5][1]);
		if(box_luck >= 21) box = "Caixa comum";
		if(box_luck >= 60) box = "Caixa rara";
		if(box_luck >= 90) box = "Caixa mistíca";
		if(box_luck >= 98) box = "Caixa sagrada";

		if(min_max_props[4][0] > 0) {
			if(randomInt(1, 4) == 4) {
				golden_spike = 1;
			} 
		}
	}
};