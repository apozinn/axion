#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <dpp/dpp.h>
#include <random>
#include <algorithm>

#include "../../../utils/randoms.hpp"

using namespace std;

class Rooster {
	public:
		string name;
		string classe;
		string rarity;
		string weakness;
		vector<Skill> skills;
		vector<int> states;
		int life;
		int current_life;
		int atk;
		int def;
		int stamina;
		int velocity;
		int level;
		int moral;
	void create(int min_rarity=0, int max_rarity=100) {
		vector<string> classes = {"Guerreiro", "Curandeiro", "Mago", "Assasíno", "Tank"};
		int luck = randomInt(min_rarity, max_rarity);
		classe = classes.at(randomInt(0, classes.size()-1));
		vector<float> class_multiplier;
		vector<vector<int>> min_max_props;
		name = randomString(12);
		moral = luck/2;
		level = 1;

		if(classe == "Guerreiro") {
			class_multiplier = {1.30, 1.60, 1.30, 1.20, 1.20};
			weakness = "Tank";
		}
		if(classe == "Curandeiro") {
			class_multiplier = {1.35, 1.25, 1.45, 1.15, 1.15};
			weakness = "Guerreiro";
		}
		if(classe == "Mago") {
			class_multiplier = {1.40, 1.35, 1.40, 1.25, 1.25};
			weakness = "Assasino";
		}
		if(classe == "Assasíno") {
			class_multiplier = {1.15, 1.60, 1.25, 1.55, 1.60};
			weakness = "Tank";
		}
		if(classe == "Tank") {
			class_multiplier = {1.60, 1.35, 1.55, 1.25, 1.15};
			weakness = "Mago";
		}

		if(luck <= 40) {
			min_max_props = {{100, 150}, {25, 55}, {30, 45}, {20, 35}, {25, 45}};
			rarity = "Comum";
		}
		if(luck > 40 && luck <= 65) {
			min_max_props = {{150, 200}, {35, 65}, {35, 60}, {35, 45}, {35, 50}};
			rarity = "Raro";
		}
		if(luck > 65 && luck <= 80) {
			min_max_props = {{300, 350}, {45, 75}, {45, 75}, {40, 65}, {40, 70}};
			rarity = "Epíco";
		}
		if(luck > 80 && luck <= 90) {
			min_max_props = {{350, 400}, {60, 80}, {60, 85}, {55, 75}, {55, 80}};
			rarity = "Lendário";
		}
		if(luck > 90 && luck <= 98) {
			min_max_props = {{400, 450}, {80, 100}, {75, 90}, {65, 80}, {65, 85}};
			rarity = "Mitíco";
		}
		if(luck >= 99)  {
			min_max_props = {{450, 500}, {100, 150}, {100, 165}, {80, 100}, {90, 130}};
			rarity = "Celestial";
		}

		life = randomInt(min_max_props[0][0], min_max_props[0][1])*class_multiplier[0];
		atk = randomInt(min_max_props[1][0], min_max_props[1][1])*class_multiplier[1];
		def = randomInt(min_max_props[2][0], min_max_props[2][1])*class_multiplier[2];
		stamina = randomInt(min_max_props[3][0], min_max_props[3][1])*class_multiplier[3];
		velocity = randomInt(min_max_props[4][0], min_max_props[4][1])*class_multiplier[4];
		Skill rt_skills; skills = rt_skills.create(luck, classe);
		current_life = life;
	}

	void withProps(bsoncxx::array::element& ele) {
		name = std::string(ele["name"].get_string().value);
		classe = std::string(ele["class"].get_string().value);
		rarity = std::string(ele["rarity"].get_string().value);
		weakness = std::string(ele["weakness"].get_string().value);
		
		life = ele["life"].get_int32().value;
		current_life = life;
		atk = ele["atk"].get_int32().value;
		def = ele["def"].get_int32().value;
		stamina = ele["stamina"].get_int32().value;
		velocity = ele["velocity"].get_int32().value;
		level = ele["level"].get_int32().value;
		moral = ele["moral"].get_int32().value;

		for(auto skl : ele["skills"].get_array().value) {
			Skill n_skl = { 
			name: std::string(skl["name"].get_string().value),
			type: std::string(skl["type"].get_string().value),
			code: skl["code"].get_int32().value,
			limit: skl["limit"].get_int32().value,
			description: std::string(skl["description"].get_string().value)};
			skills.push_back(n_skl);
		}
	}
};