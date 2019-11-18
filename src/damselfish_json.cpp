// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017 Jonathan Metzgar
// All rights reserved.
//
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.If not, see <https://www.gnu.org/licenses/>.
//
// For any other type of licensing, please contact me at jmetzgar@outlook.com
#include <sstream>
#include <damselfish_json.hpp>

namespace Df
{
	const std::string JSON::NullString = "";

	JSON::JSON(JSON::Type whichType)
	{
		type_ = whichType;
	}

	JSON::JSON(const JSON &json)
	{
		type_ = json.type_;
		if (IsNumber())
			dval_ = json.dval_;
		else if (IsString())
			sval_ = json.sval_;
		else if (IsArray())
			copyArray(json.array_);
		else if (IsObject())
			copyMap(json.map_);
	}

	JSON::JSON(const JSONPtr &json)
	{
		if (!json)
			return;
		type_ = json->type_;
		if (IsNumber())
			dval_ = json->dval_;
		else if (IsString())
			sval_ = json->sval_;
		else if (IsArray())
			copyArray(json->array_);
		else if (IsObject())
			copyMap(json->map_);
	}

	JSON::~JSON()
	{
		sval_.clear();
		array_.clear();
		map_.clear();
	}

	JSONPtr JSON::operator=(const JSONPtr &rhs)
	{
		Clear();
		if (!rhs)
			return shared_from_this();
		type_ = rhs->type_;
		switch (type_)
		{
		case Type::Number:
			dval_ = rhs->dval_;
			break;
		case Type::String:
			sval_ = rhs->sval_;
			break;
		case Type::Array:
			copyArray(rhs->array_);
			break;
		case Type::Object:
			copyMap(rhs->map_);
			break;
		case Type::True:
			break;
		case Type::False:
			break;
		case Type::Null:
			break;
		}
		return shared_from_this();
	}

	bool JSON::Equals(const JSONPtr &rhs) const
	{
		if (!rhs || rhs->type_ != type_)
			return false;

		if (type_ == Type::Null)
			return true;
		if (type_ == Type::True)
			return true;
		if (type_ == Type::False)
			return true;
		if (type_ == Type::Number)
			return dval_ == rhs->dval_;
		if (type_ == Type::String)
			return sval_ == rhs->sval_;

		if (type_ == Type::Array)
		{
			if (Length() != rhs->Length())
				return false;

			size_t length = array_.size();
			for (size_t i = 0; i < length; i++)
			{
				// This should actually never be the case because nullptr != JSON(JSON::Type::Null).
				if (!array_[i] || !rhs->array_[i])
					return false;
				if (array_[i] && rhs->array_[i] && !array_[i]->Equals(rhs->array_[i]))
					return false;
			}

			return true;
		}

		if (type_ == Type::Object)
		{
			if (Length() != rhs->Length())
				return false;

			auto m1It = map_.begin();
			auto m2It = rhs->map_.begin();
			while (m1It != map_.end())
			{
				// are the keys the same?
				if (m1It->first != m2It->first)
					return false;
				// are the values the same?
				if (!m1It->second->Equals(m2It->second))
					return false;
				m1It++;
				m2It++;
			}
		}

		return true;
	}

	void JSON::copyArray(const std::vector<JSONPtr> &a)
	{
		size_t length = a.size();
		for (size_t i = 0; i < length; i++)
		{
			JSONPtr json = JSON::New((a[i]));
			array_.push_back(json);
		}
	}

	void JSON::copyMap(const std::map<std::string, JSONPtr> &m)
	{
		for (auto it = m.begin(); it != m.end(); it++)
		{
			JSONPtr json = JSON::New((it->second));
			map_[it->first] = json;
		}
	}

	std::string JSON::Serialize() const
	{
		std::ostringstream ostr;

		if (IsNull())
			return "null";
		else if (IsTrue())
			return "true";
		else if (IsFalse())
			return "false";
		else if (IsNumber())
		{
			ostr << dval_;
		}
		else if (IsString())
		{
			ostr << "\"";
			for (size_t i = 0, len = sval_.length(); i < len; i++)
			{
				if (sval_[i] == '\"')
					ostr << "\\\"";
				else
					ostr << sval_[i];
			}
			ostr << "\"";
		}
		else if (IsArray())
		{
			ostr << "[";
			int i = 0;
			for (const JSONPtr &j : array_)
			{
				const JSON *json = j.get();
				if (!json)
					continue;

				if (i != 0)
					ostr << ", ";
				else
					ostr << " ";
				ostr << json->Serialize();
				i++;
			}
			ostr << " ]";
		}
		else if (IsObject())
		{
			ostr << "{";
			int i = 0;
			for (const std::pair<std::string, JSONPtr> &j : map_)
			{
				const JSON *json = j.second.get();
				if (!json)
					continue;

				if (i != 0)
					ostr << ", ";
				else
					ostr << " ";
				ostr << "\"" << j.first << "\" : ";
				ostr << json->Serialize();
				i++;
			}
			ostr << " }";
		}

		return ostr.str();
	}

	bool JSON::Deserialize(const std::string &json)
	{
		Clear();

		// Let's LEX it!
		TokenVector tokens;
		lex_quick_l2_parse(json, tokens);

		return DeserializeParseTokens(tokens);
	}

	long JSON::DeserializeParseTokens(const TokenVector &tokens, size_t startIndex)
	{
		size_t len = tokens.size();
		for (size_t i = startIndex; i < len; i++)
		{
			if (tokens[i].IsIntegerOrDouble())
			{
				*this = tokens[i].dval;
				return (long)i;
			}
			else if (tokens[i].IsIdentifier())
			{
				if (tokens[i].sval == "null")
					;
				else if (tokens[i].sval == "true")
					*this = true;
				else if (tokens[i].sval == "false")
					*this = false;
				else
					return -1;
				return (long)i;
			}
			else if (tokens[i].IsString())
			{
				*this = tokens[i].sval;
				return (long)i;
			}
			else if (tokens[i].IsType(TokenType::TT1_LBRACE))
			{
				// Object
				type_ = Type::Object;

				i++;
				while (i < len)
				{
					// next we're expecting a STRING, a COLON, and a value, and an optional comma etc...
					if (i + 3 > len)
						break;

					std::string key;
					if (tokens[i].IsString())
					{
						key = tokens[i].sval;
						i++;
					}
					else
					{
						return -1;
					}

					if (tokens[i].IsType(TokenType::TT1_COLON))
					{
						i++;
					}
					else
					{
						return -1;
					}

					JSONPtr value = JSON::New();
					i = value->DeserializeParseTokens(tokens, i);
					if (i < 0)
						return -1;
					map_[key] = value;
					i++;

					if (i + 1 >= len)
						return -1;
					if (tokens[i].IsType(TokenType::TT1_COMMA))
					{
						// we're expecting another object now
						i++;
					}
					else
					{
						break;
					}
				}
				if (!tokens[i].IsType(TokenType::TT1_RBRACE))
					return -1;
				return (long)i;
			}
			else if (tokens[i].IsType(TokenType::TT1_LBRACKET))
			{
				// Array
				type_ = Type::Array;

				i++;
				while (i < len)
				{
					// next we're expecting a a value and an optional comma etc...
					if (i + 1 > len)
						break;

					JSONPtr value = JSON::New();
					i = value->DeserializeParseTokens(tokens, i);
					if (i < 0)
						return -1;
					array_.push_back(value);
					i++;

					if (i + 1 >= len)
						return -1;
					if (tokens[i].IsType(TokenType::TT1_COMMA))
					{
						// we're expecting another value now
						i++;
					}
					else
					{
						break;
					}
				}
				if (!tokens[i].IsType(TokenType::TT1_RBRACKET))
					return -1;
				return (long)i;
			}
			else
			{
				return -1;
			}
		}
		return -1;
	}
} // namespace Df