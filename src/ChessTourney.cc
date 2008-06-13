/*
 *   Copyright (c) 2007-2008 C3SL.
 *
 *   This file is part of Chessd.
 *
 *   Chessd is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Chessd is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 */

#include "ChessTourney.hh"
#include "TourneyException.hh"
#include "GameChess.hh"
#include <algorithm>

ChessTourney::ChessTourney(const std::string& __category, const Util::Time& _initial_time, const Util::Time& _inc, int __rounds) : _category(__category), initial_time(_initial_time), inc(_inc), _rounds(__rounds), _missing_rounds(__rounds), tourney('s') {
	tourney_started=false;
	missing_results=false;
	game_attributes["category"]=__category;
	game_attributes["time"]=Util::to_string((int)_initial_time.getSeconds());
	game_attributes["inc"]=Util::to_string((int)_inc.getSeconds());
	game_attributes["rounds"]=Util::to_string(__rounds);
}

const TourneyPlayerList& ChessTourney::players() const {
	return this->_players;
}

void ChessTourney::addPlayer(const TourneyPlayer& p) {
	this->tourney.AddPlayer(this->_players.size(),p.rating.rating());
	this->_players.push_back(p);
	this->player_map[p.jid]=this->_players.size()-1;
}

std::vector<Game*>* ChessTourney::match() {
	if(missing_results==false) {
		if(this->_missing_rounds==0)
			throw tourney_over("Can't make one more round, number of rounds already reached");
		this->_missing_rounds--;
		if(tourney_started)
			this->tourney.MakeAssignments();
		else {
			this->tourney.start();
			tourney_started=true;
		}

		missing_results=true;
		result_set.clear();

		return this->makeGames(this->tourney.getGames());
	}
	return 0;
}

void ChessTourney::addResult(const PlayerResultList& prl) {
	foreach(it,prl) {
		if(result_set.find(it->jid)!=result_set.end())
			throw result_error("Cannot set result, already set");
		if(player_map.find(it->jid)==player_map.end())
			throw result_error("Player not found to set result");
	}
	result_set.insert(prl[0].jid);
	result_set.insert(prl[1].jid);
	missing_results=tourney.SetGameResult(this->player_map[prl[0].jid],this->player_map[prl[1].jid],(prl[0].score!="1/2"?(prl[0].score=="1"?1:0):2))!=2;
	this->_players[player_map[prl[0].jid]].score=tourney.GetPlayerByName(this->player_map[prl[0].jid])->score;
	this->_players[player_map[prl[1].jid]].score=tourney.GetPlayerByName(this->player_map[prl[1].jid])->score;
}

std::vector<Game*>* ChessTourney::makeGames(const std::list<Pairing::Game>& games) const {
	std::vector<Game*>* g = new std::vector<Game*>;
	foreach(it,games) {
		StandardPlayerList players;
		players.push_back(StandardPlayer(this->_players[it->whiteName].jid,this->initial_time,this->inc,White));
		players.push_back(StandardPlayer(this->_players[it->blackName].jid,this->initial_time,this->inc,Black));
		g->push_back(new GameChess(players,this->game_attributes));
	}
	return g;
}