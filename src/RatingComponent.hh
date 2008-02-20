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

#ifndef RATINGCOMPONENT_HH
#define RATINGCOMPONENT_HH

#include <map>
#include <set>
#include <memory>

#include "ComponentBase.hh"
#include "DatabaseManager.hh"

/*! \brief This is the component that handles user info requests */
class RatingComponent : public ComponentBase {
	public:
		/*! \brief Constructor
		 *
		 * \param core_interface is the interface to the core.
		 * \param config is the configuration for this component.
		 */
		RatingComponent(
            const XML::Tag& config,
            const XMPP::ErrorHandler& handleError,
            DatabaseManager& database);

		/*! \brief Destructor
		 *
		 * Closes server connection if available
		 */
		virtual ~RatingComponent();

		/*! \brief Closes the conenction to the server */
		//void close();


	private:

        void onClose();

        void onError(const std::string& error);

		/* several Handlers for the incoming events. */

		/*! \brief Handle an incoming match iq. */
		void handleRating(const XMPP::Stanza& stanza);

        /*! \brief This is a transaction that reds users ratings. */
		void fetchRating(const XMPP::Stanza& stanza, DatabaseInterface& database);

        XMPP::ErrorHandler error_handler;

        DatabaseManager& database;
};

#endif
