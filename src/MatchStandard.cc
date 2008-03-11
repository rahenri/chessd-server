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

#include "MatchStandard.hh"
#include "GameStandard.hh"
#include "Util/utils.hh"

#include <memory>

/*
 * MatchRuleStandard Stuff
*/
MatchRuleStandard::MatchRuleStandard() : MatchRuleChess("standard") {
}

MatchRuleStandard::~MatchRuleStandard() {
}

Match* MatchRuleStandard::checkOffer(const XML::Tag& _match_offer, const TeamDatabase&) const {
	return new MatchStandard(this->getPlayersTag(_match_offer));
}

bool MatchRuleStandard::isTimeValid(const XML::Tag& _player) const {
	if(_player.hasAttribute("time")) {
		if(11u * Util::Minutes <= Util::Time(_player.getAttribute("time"),Util::Seconds))
			return true;
	}
	return false;
}

/*
 * MatchStandard Stuff
*/
MatchStandard::MatchStandard(const std::vector<XML::Tag>& players) :
	MatchChess(players,"standard")
{
}

MatchStandard::~MatchStandard() {
}

Game* MatchStandard::createGame() const {
	return new GameStandard(getPlayersFromXML(this->_match_players));
}

/*
 * MatchChessStandardAdjourn Stuff
*/

Game* MatchChessStandardAdjourn::createGame() const {
	return new GameStandard(this->history);
}
