/*
 *   Copyright (c) 2007-2008 Raphael H. Ribas,  Eduardo A. Ribas.
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

#include "CoreInterface.hh"
#include "Core.hh"

CoreInterface::CoreInterface() { }

CoreInterface::~CoreInterface() { }

void CoreInterface::startGame(Game* game) {
    Core::singleton().startGame(game);
}

void CoreInterface::endGame(int game_id, GameResult* result) {
    Core::singleton().endGame(game_id, result);
}

void CoreInterface::adjournGame(int game_id, GameResult* result) {
    Core::singleton().adjournGame(game_id, result);
}

void CoreInterface::cancelGame(int game_id) {
    Core::singleton().cancelGame(game_id);
}
