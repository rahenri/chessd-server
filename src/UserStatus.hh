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

#ifndef USERSTATUS_HH
#define USERSTATUS_HH

struct UserStatus {
    UserStatus() : available(false), games_playing(0), multigame(false) { }
    bool available;
    int games_playing;
    bool multigame;

    bool canPlay() const {
        return this->available and (this->games_playing <= 0 or this->multigame);
    }

};

#endif
