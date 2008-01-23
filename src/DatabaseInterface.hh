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

#ifndef DATABASEINTERFACE_HH
#define DATABASEINTERFACE_HH

#include <pqxx/pqxx>

#include "GameDatabase.hh"

#include "RatingDatabase.hh"

class DatabaseInterface : public pqxx::transactor<>
{
    public:

        DatabaseInterface(pqxx::work&);

        GameDatabase game_database;

        RatingDatabase rating_database;

    private:

        
};

#endif
