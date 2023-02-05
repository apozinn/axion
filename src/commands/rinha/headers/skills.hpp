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

class Skill {
	public:
		string name;
		string type;
		int code;
		int limit;
		string description;
	vector<Skill> create(int luck, string classe) {
		vector<Skill> rooster_skill; 
		vector<int> passed_skills;
		int skills_length;

		vector<Skill> skills_v = {
			{ name: "Fúria em ascensão", type: "atk", code: 1, limit: 3,
			description: "4 ataques com 10 de dano, receba 10% de bônus se a stamina atual for maior que 20." },
			{ name: "Voo do dominador", type: "atk", code: 2, limit: 2,
			description: "50 de dano, se a defesa inimiga for menor que 30, receba 20% de bônus no dano." },
			{ name: "Garras da agônia", type: "atk", code: 3, limit: 1,
			description: "60 de dano, se a velocidade inimiga for menor que a sua, atordoe-o por 1 turno. Moral acima de 30 aumenta a chance de critíco em 30%." },

			{ name: "Cura divína", type: "cura", code: 4, limit: 3,
			description: "20 pontos de cura, se a moral atual for maior que 25, receba 30% de bônus. " },
			{ name: "Pedido de um pecador", type: "cura", code: 5, limit: 2,
			description: "Ative o status 'regeneração acelerada', recuperação de vida aumenta em 40% por turno." },
			{ name: "Salvação penosa", type: "cura", code: 6, limit: 1,
			description: "Se a vida atual for menor que 20, cure 40 pontos de vida e 30% de bônus na defesa." },

			{ name: "Gólem de milho", type: "magíco", code: 7, limit: 3,
			description: "Crie um gólem que difere ataques em conjuntos aos seus, cada ataque com 15 de dano com duração de 3 turnos." },
			{ name: "Terremoto de Yazdekan", type: "magíco", code: 8, limit: 2,
			description: "30 de dano, diminua a defesa do inimigo em 50%." },
			{ name: "Correntes de elo", type: "magíco", code: 9, limit: 1,
			description: "Imobilize o inimigo por 3 turnos." },

			{ name: "Penas de aço", type: "defesa", code: 10, limit: 3,
			description: "Aumente a defesa em 10%, se a moral for maior que 25, receba bônus de mais 20%." },
			{ name: "Escudo de papel", type: "defesa", code: 11, limit: 2,
			description: "Bloqueia todo ataque inimigo fisíco por um turno." },
			{ name: "Enderucimento de bico", type: "defesa", code: 12, limit: 1,
			description: "Aumente a defesa atual em x3, se a velocidade e stamina forem maiores que 30, chance critíco será de 40%." },

			{ name: "Efeitos de uma ída a boca", type: "weakening", code: 13, limit: 3,
			description: "Cause fraqueza e atordoamento no inimigo por 1 turno." },
			{ name: "Vingança dos humilhados", type: "weakening", code: 14, limit: 2,
			description: "Ao usar após receber um critíco, remova toda a velocidade e moral do inimigo." },
			{ name: "Moleza do amanhecer", type: "weakening", code: 15, limit: 1,
			description: "Faça o inimigo ter fraqueza e lerdeza por 3 rounds." },

			{ name: "Anabólizante de boi", type: "boost", code: 16, limit: 3,
			description: "Aumente o ataque em 20% por 1 turno, se a velocidade for maior que 30, chance de critíco aumenta em 25%." },
			{ name: "Bomba de milho", type: "boost", code: 17, limit: 2,
			description: "Aumente a defesa e ataque em 50% por 1 turno." },
			{ name: "Gatorade de uva", type: "boost", code: 18, limit: 1,
			description: "Aumente sua velocidade e estamina em x2, ataques lançados após essa skill tem 80% de chance de critíco." },

			{ name: "Benção do deus galo", type: "unic", code: 19, limit: 1,
			description: "usar essa habilidade 2 turnos antes da morte, o faz retornar com 20 pontos de vida." },
			{ name: "O avarento", type: "unic", code: 20, limit: 1,
			description: "Se estiver duelando em um partida apostada, receba %5 de aumenta nos ataques a cada 100 moedas." },
			{ name: "Rugido do galo-rex", type: "unic", code: 21, limit: 1,
			description: "Atordoe o inimigo e o faça perder todas as skills por 3 turnos." },
			{ name: "So acaba quando termina", type: "unic", code: 22, limit: 1,
			description: "Usar esta habilidade antes de receber um critíco que deixe sua vida abaixo de 10, fará inimigo perder instantaneamente." }
		};

		if(luck <= 40) skills_length=1;
		if(luck > 40 && luck <= 65) skills_length=2;
		if(luck > 65 && luck <= 80) skills_length=3;
		if(luck > 80 && luck <= 90) skills_length=3;
		if(luck > 90 && luck <= 98) skills_length=4;
		if(luck >= 99) skills_length=5;

		while(rooster_skill.size() < skills_length) {
			Skill random_skill = skills_v.at(randomInt(0, skills_v.size()-1));
			string skl_type = random_skill.type;
		 
		    if (std::find(passed_skills.begin(), passed_skills.end(), random_skill.code) != passed_skills.end()) {}
		    else {
				if(classe == "Guerreiro") {
					if(skl_type == "atk" || skl_type == "defesa" || skl_type == "weakening" || skl_type == "boost") {
						rooster_skill.push_back(random_skill);
						passed_skills.push_back(random_skill.code);
					}
				}
				if(classe == "Curandeiro") {
					if(skl_type == "cura" || skl_type == "defesa" || skl_type == "weakening" || skl_type == "boost") {
						rooster_skill.push_back(random_skill);
						passed_skills.push_back(random_skill.code);
					}
				}
				if(classe == "Mago") {
					if(skl_type == "magíco" || skl_type == "defesa" || skl_type == "weakening" || skl_type == "boost") {
						rooster_skill.push_back(random_skill);
						passed_skills.push_back(random_skill.code);
					}
				}
				if(classe == "Assasíno") {
					if(skl_type == "atk" || skl_type == "defesa" || skl_type == "weakening" || skl_type == "boost") {
						rooster_skill.push_back(random_skill);
						passed_skills.push_back(random_skill.code);
					}
				}
				if(classe == "Tank") {
					if(skl_type == "atk" || skl_type == "defesa" || skl_type == "weakening" || skl_type == "boost") {
						rooster_skill.push_back(random_skill);
						passed_skills.push_back(random_skill.code);
					}
				}
				if(luck > 90) {
					Skill unic_skill = skills_v.at(randomInt(18, skills_v.size()-1));
					rooster_skill.push_back(unic_skill);
					passed_skills.push_back(unic_skill.code);
				}
		    }
		}
		return rooster_skill;
	}
};