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

#include "RootNode.hh"
#include <boost/bind.hpp>

using namespace XML;
using namespace std;

namespace XMPP {

	RootNode::RootNode(const StanzaHandler& send_stanza,
			const XMPP::Jid& jid,
			const std::string& name,
			const std::string& category,
			const std::string& type) :
		Node(send_stanza, jid, name, category, type) {
			this->node_handlers.insert(make_pair("",boost::bind(&Node::handleStanza, this, _1)));
		}

	RootNode::~RootNode() { }

	void RootNode::handleStanza(Stanza* stanza) {
		HandlerMap::const_iterator it;
		it = this->node_handlers.find(stanza->to().node());
		if(it != this->node_handlers.end())
			it->second(stanza);
		else {
			this->sendStanza(Stanza::createErrorStanza(stanza, "cancel", "item-not-found"));
		}
	}

	void RootNode::setNodeHandler(const string& node, const StanzaHandler& handler) {
		this->node_handlers.insert(make_pair(node,handler));
	}

	void RootNode::removeNodeHandler(const string& node) {
		this->node_handlers.erase(node);
	}
}
