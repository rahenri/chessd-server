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

#ifndef STATE_HH
#define STATE_HH

#include <vector>
#include <string>
#include "Piece.hh"
#include "ChessMove.hh"

class State {
	private:

	public:

		std::string board_fen;

		int _turn;

		int fullmoves;

//	public:

		State();

		State(const std::string& posfen);

		virtual ~State() { }

		const std::string& boardFEN() const;

		int turn() const;

		virtual std::string FEN() const=0;
};

class ChessState : public State {
	private:

	public:
		
		std::string castle;

		Position enpassant;

		int halfmoves;

//	public:

		ChessState();

		ChessState(const std::string& _board_fen);

		virtual std::string FEN() const;
};
#endif
