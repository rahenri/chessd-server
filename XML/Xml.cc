#include "Xml.hh"
#include "iksutil.hh"

#include "../Util/utils.hh"


using namespace std;
using namespace Util;

namespace XML {
	Item::~Item() { }

	Tag::Tag(const Tag& tag) : _name(tag.name()), _attributes(tag.attributes())   {
		ChildrenList::const_iterator it;
		for(it = tag.children().begin(); it != tag.children().end(); ++it) {
			this->children().push_back(it->clone());
		}
	}

	Tag::~Tag() { }

	void Tag::xml(string& str) const {
		str += '<';
		str += this->name();
		AttributeMap::const_iterator it;
		for(it = this->attributes().begin(); it != this->attributes().end(); ++it) {
			str += ' ';
			str += it->first;
			str += '=';
			str += '"';
			str += it->second;
			str += '"';
		}
		if(this->children().empty()) {
			str += "/>";
		} else {
			str += '>';
			ChildrenList::const_iterator it;
			for(it = this->children().begin(); it != this->children().end(); ++it)
				it->xml(str);
			str += "</";
			str += this->name();
			str += ">";
		}
	}

	Item* Tag::clone() const {
		return new Tag(*this);
	}

	void Tag::swap(Tag& tag) {
		this->name().swap(tag.name());
		this->attributes().swap(tag.attributes());
		this->children().swap(tag.children());
	}

	Item* CData::clone() const {
		return new CData(*this);
	}

	void CData::xml(string& str) const {
		str += this->data();
	}

	string Tag::xml() const {
		string tmp;
		this->xml(tmp);
		return tmp;
	}

	void Tag::clearChildren() {
		this->children().clear();
	}

	TagGenerator::TagGenerator() : last(0) { }

	TagGenerator::~TagGenerator() {
		while(not this->tag_stack.empty()) {
			delete this->tag_stack.top();
			this->tag_stack.pop();
		}
	}

	void TagGenerator::openTag(const string& name) {
		this->tag_stack.push(new Tag(name));
	}

	Tag* TagGenerator::closeTag() {
		Tag *tag = this->tag_stack.top();
		this->tag_stack.pop();
		if(not this->tag_stack.empty())
			this->tag_stack.top()->children().push_back(tag);
		return this->last = tag;
	}

	void TagGenerator::addAttribute(const string& name, const string& value) {
		this->tag_stack.top()->attributes().insert(make_pair(name, value));
	}

	void TagGenerator::addCData(const string& data) {
		this->tag_stack.top()->children().push_back(new CData(data));
	}

	void TagGenerator::addChild(Tag* tag) {
		this->tag_stack.top()->children().push_back(tag);
	}

	Tag* TagGenerator::getLastTag() {
		return this->last;
	}

	bool TagGenerator::empty() const {
		return this->tag_stack.empty();
	}

	const Tag& Tag::getChild(const std::string& name) const {
		foreach(it, this->children()) {
			if(typeid(*it)==typeid(Tag)) {
				const Tag& tag = dynamic_cast<const Tag&>(*it);
				if(tag.name() == name)
					return tag;
			}
		}
		throw "Tag not found";
	}

	Tag& Tag::getChild(const std::string& name) {
		foreach(it, this->children()) {
			if(typeid(*it)==typeid(Tag)) {
				Tag& tag = dynamic_cast<Tag&>(*it);
				if(tag.name() == name)
					return tag;
			}
		}
		throw "Unrecoverable error";
	}

	Description::Description() : type_count(0) { }

	Description::~Description() {
	}

	bool Description::loadFromFile(const std::string& filename) {
		Tag* xml = loadXmlFile(filename);
		if(xml == 0)
			return false;
		if(xml->name()!="xd") {
			delete xml;
			return false;
		}
		bool root_found=false;
		bool error = false;
		foreach(child, xml->children()) {
			if(typeid(*child) == typeid(Tag)) {
				Tag& tag = dynamic_cast<Tag&>(*child);
				if(tag.name() == "root") {
					if(root_found) {
						error = true; break;
					}
					if(not tag.hasAttribute("name")) {
						error = true; break;
					}
					this->root_name = tag.getAttribute("name");
					if(not tag.hasAttribute("type")) {
						if(not this->parseType(tag,"%root_type")) {
							error = true; break;
						}
						this->root_type="%root_type";
					} else {
						this->root_type = tag.getAttribute("type");
					}
					root_found=true;
				} else if(tag.name() == "type") {
					if(not tag.hasAttribute("name")) {
						error = true; break;
					}
					if(not this->parseType(tag, tag.getAttribute("name"))) {
						error = true; break;
					}
				} else {
					error = true; break;
				}
			}
			/* ignore char data */
		}
		delete xml;
		if(not root_found) {
			error = true;
		}
		foreach(type, this->types) foreach(child, type->second.children) {
			if(not has_key(this->types, child->type))
				error = true;
		}
		if(not has_key(this->types, this->root_type))
			error = true;
		return not error;
	}

	bool Description::parseType(const Tag& xml, const std::string& type_name) {
		TypeDesc type;
		bool error = false;
		foreach(child, xml.children()) {
			if(typeid(*child) == typeid(Tag)) {
				const Tag& tag = dynamic_cast<const Tag&>(*child);
				if(tag.name()=="attribute") {
					if(not tag.hasAttribute("name")) {
						error = true; break;
					}
					const std::string& att_name = tag.getAttribute("name");
					bool required;
					if(tag.hasAttribute("required")) {
						if(tag.getAttribute("required") == "true")
							required = true;
						else
							required = false;
					} else {
						required=true;
					}
					bool fixed;
					if(tag.hasAttribute("fixed") and tag.getAttribute("fixed") == "true")
						fixed=true;
					else
						fixed=false;
					std::string def_val;
					if(tag.hasAttribute("default")) {
						def_val=tag.getAttribute("default");
					}
					FieldType att_type;
					if(tag.hasAttribute("format")) {
						if(tag.getAttribute("format") == "string") {
							att_type=FieldString;
						} else if(tag.getAttribute("format") == "number") {
							att_type=FieldNumber;
						} else {
							error=true; break;
						}
					} else {
						att_type=FieldString;
					}
					type.attributes.insert(make_pair(att_name,
								AttributeDesc(att_type, required, fixed, def_val)));
				} else if(tag.name()=="cdata") {
					type.cdata=true;
				} else if(tag.name()=="child") {
					if(not tag.hasAttribute("name")) {
						error = true; break;
					}
					const string& ch_name = tag.getAttribute("name");
					int minOccur;
					if(tag.hasAttribute("min")) {
						if(not isNumber(tag.getAttribute("min"))) {
							error=true; break;
						}
						minOccur=str2int(tag.getAttribute("min"));
					} else {
						minOccur=1;
					}
					int maxOccur;
					if(tag.hasAttribute("max")) {
						if(tag.getAttribute("max") == "unbounded") {
							maxOccur=0x7fffffff; 
						} else {
							if(not isNumber(tag.getAttribute("max"))) {
								error = true; break;
							}
							maxOccur=str2int(tag.getAttribute("max"));
						}
					} else {
						maxOccur=1;
					}
					string ch_type;
					if(tag.hasAttribute("type")) {
						ch_type = tag.getAttribute("type");
					} else {
						ch_type = int2str(this->type_count++);
						if(not this->parseType(tag, ch_type)) {
							error = true; break;
						}
					}
					type.children.push_back(ChildDesc(ch_name, ch_type, minOccur, maxOccur));
				} else if(tag.name()=="any_children") {
					type.any_children = true;
				} else if(tag.name()=="any_Attributes") {
					type.any_attributes = true;
				} else {
					error = true;
					break;
				}
			}
		}
		if(type.any_children and not type.children.empty()) {
			error = true;
		}
		if(type.any_attributes and not type.attributes.empty()) {
			error = true;
		}
		if(error==true) {
			return false;
		} else {
			this->types.insert(make_pair(type_name, type));
		}
		return not error;
	}

	bool Description::validateXML(Tag& tag) {
		if(tag.name() != this->root_name)
			return false;
		return this->_validate(tag, this->types[this->root_type]);
	}

	static bool is_cdata(const Item& item) {
		return typeid(item) == typeid(CData);
	}
	bool Description::_validate(Tag& xml, const TypeDesc& type) {
		/* FIXME it can be done more efficiently */
		if(not type.any_attributes) {
			foreach(attribute, xml.attributes()) {
				if(not has_key(type.attributes, attribute->first))
					return false;
			}
			foreach(att_desc, type.attributes) {
				if(not xml.hasAttribute(att_desc->first)) {
					if(att_desc->second.required)
						return false;
					else
						xml.setAttribute(att_desc->first, att_desc->second.default_value);
				} else {
					const string& val = xml.attributes()[att_desc->first];
					if(att_desc->second.fixed and val!= att_desc->second.default_value) {
						return false;
					} else {
						if(att_desc->second.type == FieldNumber) {
							if(not isNumber(val))
								return false;
						}
					}
				}
			}
		}
		if(not type.any_children) {
			ChildrenList::iterator child = xml.children().begin();
			int last_count = 0;
			foreach(child_desc, type.children) {
				bool done = false;
				while(not done) {
					if(child != xml.children().end() and typeid(*child)==typeid(CData)) {
						const CData& cdata=dynamic_cast<const CData&>(*child);
						if(not type.cdata and not isSpace(cdata.data()))
							return false;
						++child;
					} else if(child != xml.children().end() and typeid(*child)==typeid(Tag) and dynamic_cast<const Tag&>(*child).name()==child_desc->name) {
						++last_count;
						++child;
						if(not this->_validate(dynamic_cast<Tag&>(*child),
									this->types.find(child_desc->type)->second))
							return false;
					} else  {
						if(last_count < child_desc->minOccur or last_count > child_desc->maxOccur)
							return false;
						last_count = 0;
						done = true;
					}
				}
			}
		}
		if(not type.cdata) {
			xml.children().erase_if(is_cdata);
		}
		return true;
	}
	Tag* loadXmlFile(const std::string& filename) {
		return iksReadXMLFile(filename);
	}
}
