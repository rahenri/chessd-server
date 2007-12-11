#ifndef XMPPJID_HH
#define XMPPJID_HH

#include <string>

namespace XMPP {

	class Jid {
		public:
			Jid(const Jid& jid);
			explicit Jid(const std::string& jid);
			explicit Jid(const std::string& node,
					const std::string& domain,
					const std::string& resource="");
			Jid();
			~Jid();

			const std::string& node() const { return this->_node; }
			std::string& node() { return this->_node; }

			const std::string& domain() const { return this->_domain; }
			std::string& domain() { return this->_domain; }

			const std::string& resource() const { return this->_resource; }
			std::string& resource() { return this->_resource; }

			std::string full() const;

			bool empty() const {
				return this->node().empty() and this->domain().empty() and this->resource().empty();
			}

			void swap(Jid& jid) {
				this->node().swap(jid.node());
				this->domain().swap(jid.domain());
				this->resource().swap(jid.resource());
			}

			bool operator==(const Jid& jid) const {
				return this->node() == jid.node() and
					this->domain() == jid.domain() and
					this->resource() == jid.resource();
			}
			bool operator!=(const Jid& jid) const {
				return this->node() != jid.node() or
					this->domain() != jid.domain() or
					this->resource() != jid.resource();
			}

			bool operator<(const Jid& jid) const {
				if(this->node() < jid.node())
					return true;
				else if(this->node() > jid.node())
					return false;
				else if(this->resource() < jid.resource())
					return true;
				else if(this->resource() > jid.resource())
					return false;
				else if(this->domain() < jid.domain())
					return true;
				else
					return false;
			}

			bool operator>(const Jid& jid) const {
				if(this->node() > jid.node())
					return true;
				else if(this->node() < jid.node())
					return false;
				else if(this->resource() > jid.resource())
					return true;
				else if(this->resource() < jid.resource())
					return false;
				else if(this->domain() > jid.domain())
					return true;
				else
					return false;
			}

			bool parcialCompare(const Jid& jid) const {
				return this->node() == jid.node() and
					this->domain() == jid.domain();
			}

		private:
			std::string _node, _domain, _resource;
	};

}

/* Overloaded swap function for the Jid class */
namespace std {
	inline void swap(XMPP::Jid& j1, XMPP::Jid& j2) {
		j1.swap(j2);
	}
}

#endif
