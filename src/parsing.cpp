#include <regex>
#include <iostream>
#include "parsing.h"

results_t parse_list(std::istream* in){
	std::string line;
	results_t results;
	std::regex names_format("([A-Za-z \\-]+?)\\s+-\\s+([A-Za-z \\-]+?)");
	std::regex names_and_score("([A-Za-z \\-]+?)\\s+(\\d+)\\s*-\\s*(\\d+)\\s+([A-Za-z \\-]+?)");

	while(getline(*(in),line)){
		std::smatch matches;

		if(regex_match(line, matches, names_format)){
			results.push_back(outcome_t(matches[1].str(),
			                            matches[2].str(),
			                            1,1));
		}
		else if(regex_match(line,matches, names_and_score)){
			auto score1 = stoi(matches[2].str());
			auto score2 = stoi(matches[3].str());
			results.push_back(outcome_t(matches[1].str(),
			                            matches[4].str(),
			                            score1,
			                            score2));
		}
		else{
			throw std::runtime_error("Datafile in incorrect format.");
		}
	}
	return results;
}

results_t parse_tree(std::istream* in){

	auto trim = [](std::string str){
		str.erase(0,str.find_first_not_of(' '));
		str.erase(str.find_last_not_of(' ')+1);
		return str;
	};

	if(in==nullptr) throw std::invalid_argument("Input file cannot be null pointer.");
	if(!in->good()) throw std::runtime_error("Input file cannot be read from.");

	results_t results;
	std::queue<std::pair<std::string,std::string>> players;

	std::string line;
	std::regex name_regex("(\\d*\\.?\\s*)?([A-Za-z \\-\\.]+)\\s*");

	std::regex score_regex("(\\d)\\s*[\\-\\/]\\s*(\\d)\\s*|ret\\.?");

	std::pair<std::string,std::string> opponents;
	bool first_opponent_set=false;
	bool burn_in = true;
	while(getline(*in,line)){
		std::smatch m;
		if(std::regex_match(line,m,name_regex)){ // if line is a name

			std::string name = trim(m[2].str());

			if(players.empty()){
				// add to list of names
				if(first_opponent_set){
					opponents.second = name;
					players.push(opponents);
				}
				else{
					opponents.first = name;
				}

				first_opponent_set = !first_opponent_set;
				continue;
			}

			auto names = players.front();


			if(!(match_name(name, names.first) || match_name(name, names.second))){
				// if name is not the expected name

				if(burn_in){ // and we don't have full list of names yet.
					// add to list of names
					if(first_opponent_set){
						opponents.second = name;
						players.push(opponents);
					}
					else{
						opponents.first = name;
					}

					first_opponent_set = !first_opponent_set;
				}
				else{
					throw std::runtime_error("Unexpected name '" + name+ "'." +
					                         "Expected '"+players.front().first+"'" +
					                         " OR '"+players.front().second+"'.");
				}
			}
			else{ // we have the expected name

				// ensure names.first is the winner
				if(!match_name(name,names.first))
					swap(names.first,names.second);

				// Move forward in the list of expected players
				players.pop();

				burn_in = false; // competitors must be known now

				std::string score_line;
				std::smatch sm;
				getline(*in,score_line);

				// Extract score from matches
				int score1=0,score2=0;
				while(std::regex_search(score_line,sm,score_regex)){
					if(stoi(sm[1].str())>stoi(sm[2].str())){
						score1++;
					}
					else{
						score2++;
					}
					score_line=score_line.substr(sm[0].str().length());
				}
				bool walkover=score_line=="wo";

				if(score1+score2<1 && !walkover){
					throw std::runtime_error("Expected score line. Got '"+score_line+"'");
				}

				// Add outcome to list of results
				results.push_back(outcome_t(names.first,names.second,score1,score2));

				// Add winner to competitors
				if(first_opponent_set){
					opponents.second = names.first;
					players.push(opponents);
				}
				else{
					opponents.first = names.first;
				}

				first_opponent_set = !first_opponent_set;
			}
		}
		else{
			throw std::runtime_error("Expected name, got '"+line+"'");
		}

	}
	return results;
}