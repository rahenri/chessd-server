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

#ifndef RATINGDATABASE_HH
#define RATINGDATABASE_HH

#include <pqxx/pqxx>

#include <string>

#include <map>

#include "Rating.hh"


class RatingDatabase {
    public:

        RatingDatabase(pqxx::work& w);

        //Rating getRating(const std::string& user, const std::string& category);

        std::vector<std::pair<std::string, Rating> > getRatings(const std::string& user, const std::string& category);

        Rating getRatingForUpdate(const std::string& user, const std::string& category);

        void setRating(const std::string& user, const std::string& category, const Rating& rating);

        std::string getUserType(const std::string& user);

    private:

        pqxx::work& work;
};

#endif
