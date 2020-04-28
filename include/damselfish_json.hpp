// Damselfish
// Copyright (C) 2017-2019 Jonathan Metzgar
// All rights reserved.
//
#ifndef DAMSELFISH_JSON_HPP
#define DAMSELFISH_JSON_HPP

#include <damselfish_lexer.hpp>

namespace Df
{
	class JSON;
	using JSONPtr = std::shared_ptr<JSON>;

	class JSON : public std::enable_shared_from_this<JSON>
	{
	public:
		using SharedPtr = std::shared_ptr<JSON>;

		enum class Type
		{
			Object,
			Array,
			String,
			Number,
			True,
			False,
			Null
		};

		JSON() {}				// for pair to work
		JSON(const JSON& json); // for pair to work

		JSON(Type whichType);
		JSON(const int ival) {
			type_ = Type::Number;
			dval_ = ival;
		}
		JSON(const double dval) {
			type_ = Type::Number;
			dval_ = dval;
		}
		JSON(const std::string& sval) {
			type_ = Type::String;
			sval_ = sval;
		}

		explicit JSON(std::initializer_list<int> numberList) {
			type_ = Type::Array;
			for (auto it = numberList.begin(); it != numberList.end(); it++) {
				array_.push_back(JSON::MakeNumber(*it));
			}
		}

		explicit JSON(const std::vector<int>& numberList) {
			type_ = Type::Array;
			for (auto it = numberList.begin(); it != numberList.end(); it++) {
				array_.push_back(JSON::MakeNumber(*it));
			}
		}

		explicit JSON(std::initializer_list<float> numberList) {
			type_ = Type::Array;
			for (auto it = numberList.begin(); it != numberList.end(); it++) {
				array_.push_back(JSON::MakeNumber(*it));
			}
		}

		explicit JSON(const std::vector<float>& numberList) {
			type_ = Type::Array;
			for (auto it = numberList.begin(); it != numberList.end(); it++) {
				array_.push_back(JSON::MakeNumber(*it));
			}
		}

		explicit JSON(std::initializer_list<double> numberList) {
			type_ = Type::Array;
			for (auto it = numberList.begin(); it != numberList.end(); it++) {
				array_.push_back(JSON::MakeNumber(*it));
			}
		}

		explicit JSON(const std::vector<double>& numberList) {
			type_ = Type::Array;
			for (auto it = numberList.begin(); it != numberList.end(); it++) {
				array_.push_back(JSON::MakeNumber(*it));
			}
		}

		explicit JSON(std::initializer_list<std::string> stringList) {
			type_ = Type::Array;
			for (auto it = stringList.begin(); it != stringList.end(); it++) {
				array_.push_back(JSON::MakeString(*it));
			}
		}

		explicit JSON(const std::vector<std::string>& stringList) {
			type_ = Type::Array;
			for (auto it = stringList.begin(); it != stringList.end(); it++) {
				array_.push_back(JSON::MakeString(*it));
			}
		}

		explicit JSON(std::initializer_list<std::pair<std::string, JSONPtr>> members) {
			type_ = Type::Object;
			for (auto it = members.begin(); it != members.end(); it++) {
				map_[it->first] = it->second;
			}
		}

		explicit JSON(const std::map<std::string, JSONPtr>& members) {
			type_ = Type::Object;
			for (auto it = members.begin(); it != members.end(); it++) {
				map_[it->first] = it->second;
			}
		}

		JSON(const JSONPtr json);
		~JSON();

		static JSONPtr MakeNull() { return std::make_shared<JSON>(Type::Null); }
		static JSONPtr MakeBool(bool state = false) { return std::make_shared<JSON>(state ? Type::True : Type::False); }
		static JSONPtr MakeString(const std::string& sval) { return std::make_shared<JSON>(sval); }
		static JSONPtr MakeString() { return std::make_shared<JSON>(""); }
		static JSONPtr MakeNumber(int ival = 0) { return std::make_shared<JSON>(ival); }
		static JSONPtr MakeNumber(double dval = 0.0) { return std::make_shared<JSON>(dval); }
		static JSONPtr MakeArray() { return std::make_shared<JSON>(Type::Array); }
		static JSONPtr MakeArray(const std::vector<int>& numberList) { return std::make_shared<JSON>(numberList); }
		static JSONPtr MakeArray(std::initializer_list<int> numberList) { return std::make_shared<JSON>(numberList); }
		static JSONPtr MakeArray(const std::vector<float>& numberList) { return std::make_shared<JSON>(numberList); }
		static JSONPtr MakeArray(std::initializer_list<float> numberList) { return std::make_shared<JSON>(numberList); }
		static JSONPtr MakeArray(const std::vector<double>& numberList) { return std::make_shared<JSON>(numberList); }
		static JSONPtr MakeArray(std::initializer_list<double> numberList) { return std::make_shared<JSON>(numberList); }
		static JSONPtr MakeArray(const std::vector<std::string>& stringList) { return std::make_shared<JSON>(stringList); }
		static JSONPtr MakeArray(std::initializer_list<std::string> stringList) { return std::make_shared<JSON>(stringList); }
		static JSONPtr MakeObject() { return std::make_shared<JSON>(Type::Object); }
		static JSONPtr MakeObject(std::initializer_list<std::pair<std::string, JSONPtr>> members) { return std::make_shared<JSON>(members); }
		static JSONPtr MakeObject(const std::map<std::string, JSONPtr> members) { return std::make_shared<JSON>(members); }

		JSONPtr operator=(const JSONPtr rhs);
		inline JSONPtr operator=(int ival) {
			Clear();
			dval_ = ival;
			type_ = Type::Number;
			return shared_from_this();
		}
		inline JSONPtr operator=(double dval) {
			Clear();
			dval_ = dval;
			type_ = Type::Number;
			return shared_from_this();
		}
		inline JSONPtr operator=(const std::string& sval) {
			Clear();
			sval_ = sval;
			type_ = Type::String;
			return shared_from_this();
		}

		inline const bool operator==(int ival) const { return IsNumber() && dval_ == (double)ival; }
		inline const bool operator==(const std::string& sval) const { return IsString() && sval_ == sval; }
		inline const bool operator==(bool bval) const { return (bval && IsTrue()) ? true : (!bval && IsFalse()) ? true : false; }
		inline const bool operator==(const JSONPtr rhs) const { return Equals(rhs); }
		inline const bool operator==(const void* ptr) const {
			if (ptr == nullptr && IsNull())
				return true;
			if (ptr == this)
				return true;
			return false;
		}

		std::string Serialize() const;
		bool Deserialize(const std::string& json);
		long DeserializeParseTokens(const TokenVector& tokens, size_t startIndex = 0);

		inline bool IsNull() const { return type_ == Type::Null; }
		inline bool IsTrue() const { return type_ == Type::True; }
		inline bool IsFalse() const { return type_ == Type::False; }
		inline bool IsBoolean() const { return type_ == Type::True || type_ == Type::False; }
		inline bool IsFinite() const { return type_ == Type::Number && std::isfinite(dval_); }
		inline bool IsNaN() const { return type_ == Type::Number && std::isnan(dval_); }
		inline bool IsNumber() const { return type_ == Type::Number; }
		inline bool IsString() const { return type_ == Type::String; }
		inline bool IsArray() const { return type_ == Type::Array; }
		inline bool IsObject() const { return type_ == Type::Object; }

		static JSONPtr NewArray() { return std::make_shared<JSON>(Type::Array); }
		static JSONPtr NewObject() { return std::make_shared<JSON>(Type::Object); }
		static JSONPtr NewNumber(const double value) { return std::make_shared<JSON>(value); }
		static JSONPtr NewBoolean(const bool value) { return std::make_shared<JSON>(value); }
		static JSONPtr NewString(const std::string& value) { return std::make_shared<JSON>(value); }
		static JSONPtr New(const JSONPtr value) { return std::make_shared<JSON>(value); }
		static JSONPtr New() { return std::make_shared<JSON>(Type::Null); }

		inline int Size() const {
			if (IsArray())
				return (int)array_.size();
			if (IsObject())
				return (int)map_.size();
			return 0;
		}
		inline bool Empty() const {
			if (IsArray())
				return array_.empty();
			if (IsObject())
				return map_.empty();
			return true;
		}
		void Clear() {
			type_ = Type::Null;
			dval_ = 0.0;
			sval_.clear();
			array_.clear();
			map_.clear();
		}

		inline int AsInt() const { return IsNumber() ? (int)dval_ : 0; }
		inline int64_t AsInt64() const { return IsNumber() ? (int64_t)dval_ : 0; }
		inline double AsDouble() const { return IsNumber() ? dval_ : 0.0; }
		inline float AsFloat() const { return IsNumber() ? float(dval_) : 0.0f; }
		inline const std::string AsString() const { return IsString() ? sval_ : Serialize(); }
		inline size_t Length() const { return IsArray() ? array_.size() : IsObject() ? map_.size() : 0; }
		inline std::map<std::string, JSONPtr>& AsObject() noexcept { return map_; }
		inline const std::map<std::string, JSONPtr>& AsObject() const noexcept { return map_; }
		inline std::vector<JSONPtr>& AsArray() noexcept { return array_; }
		inline const std::vector<JSONPtr>& AsArray() const noexcept { return array_; }

		inline std::vector<int> AsIntArray() const noexcept {
			std::vector<int> result;
			for (auto& e : array_) {
				if (e)
					result.push_back(e->AsInt());
			}
			return result;
		}

		inline std::vector<float> AsFloatArray() const noexcept {
			std::vector<float> result;
			for (auto& e : array_) {
				if (e)
					result.push_back(e->AsFloat());
			}
			return result;
		}

		inline std::vector<double> AsDoubleArray() const noexcept {
			std::vector<double> result;
			for (auto& e : array_) {
				if (e)
					result.push_back(e->AsDouble());
			}
			return result;
		}
		inline JSON& AsJSON() noexcept { return *this; }
		inline const JSON& AsJSON() const noexcept { return *this; }

		inline bool Has(const std::string& key) const noexcept {
			if (!IsObject())
				return false;
			if (map_.find(key) == map_.end())
				return false;
			return true;
		}
		inline bool HasKeyOfType(const std::string& key, Type type) const noexcept {
			std::map<std::string, JSONPtr>::const_iterator it = map_.find(key);
			if (it != map_.end()) {
				const JSON* pjson = it->second.get();
				Type t = pjson->type_;
				if (t == type) {
					return true;
				}
			}

			return false;
		}

		inline JSONPtr operator[](int index) {
			if (IsArray())
				return array_.at(index);
			throw std::runtime_error("JSON object is not an array");
		}
		inline const JSONPtr operator[](int index) const {
			if (IsArray())
				return array_.at(index);
			throw std::runtime_error("JSON object is not an array");
		}
		inline JSONPtr operator[](const std::string& key) {
			if (IsObject())
				return map_.at(key);
			throw std::runtime_error("JSON object is not an object");
		}
		inline const JSONPtr operator[](const std::string& key) const {
			if (IsObject())
				return map_.at(key);
			throw std::runtime_error("JSON object is not an object");
		}
		inline const JSONPtr operator[](const char* key) const {
			if (IsObject())
				return map_.at(key);
			throw std::runtime_error("JSON object is not an object");
		}

		inline JSONPtr getElement(int index) {
			if (IsArray())
				return array_.at(index);
			throw std::runtime_error("JSON object is not an array");
		}
		inline const JSONPtr getElement(int index) const {
			if (IsArray())
				return array_.at(index);
			throw std::runtime_error("JSON object is not an array");
		}
		inline JSONPtr getMember(const std::string& key) {
			if (IsObject() && Has(key))
				return map_.at(key);
			return nullptr;
			// throw std::runtime_error("JSON object is not an object");
		}
		inline const JSONPtr getMember(const std::string& key) const {
			if (IsObject() && Has(key))
				return map_.at(key);
			return nullptr;
			// throw std::runtime_error("JSON object is not an object");
		}

		inline bool getMemberAsString(const std::string& key, std::string& value) const {
			if (!IsObject() || !Has(key))
				return false;
			auto j_member = getMember(key);
			if (!j_member)
				return false;
			value = j_member->AsString();
			return true;
		}

		inline bool getMemberAsFloat(const std::string& key, float& value) const {
			if (!IsObject())
				return false;
			auto j_member = getMember(key);
			if (!j_member)
				return false;
			value = j_member->AsFloat();
			return true;
		}

		inline bool getMemberAsDouble(const std::string& key, double& value) const {
			if (!IsObject())
				return false;
			auto j_member = getMember(key);
			if (!j_member)
				return false;
			value = j_member->AsDouble();
			return true;
		}

		inline bool getMemberAsInt(const std::string& key, int& value) const {
			if (!IsObject())
				return false;
			auto j_member = getMember(key);
			if (!j_member)
				return false;
			value = j_member->AsInt();
			return true;
		}

		inline bool getMemberAsInt64(const std::string& key, int64_t& value) const {
			if (!IsObject())
				return false;
			auto j_member = getMember(key);
			if (!j_member)
				return false;
			value = j_member->AsInt64();
			return true;
		}

		inline JSONPtr set(Type whichType) {
			Clear();
			type_ = whichType;
			return shared_from_this();
		}

		inline JSONPtr set(const int ival) {
			Clear();
			type_ = Type::Number;
			dval_ = ival;
			return shared_from_this();
		}

		inline JSONPtr set(const double dval) {
			Clear();
			type_ = Type::Number;
			dval_ = dval;
			return shared_from_this();
		}

		inline JSONPtr set(const std::string& sval) {
			Clear();
			type_ = Type::String;
			sval_ = sval;
			return shared_from_this();
		}

		inline JSONPtr set(std::initializer_list<int> numberList) {
			Clear();
			type_ = Type::Array;
			for (auto it = numberList.begin(); it != numberList.end(); it++) {
				array_.push_back(JSON::MakeNumber(*it));
			}
			return shared_from_this();
		}

		inline JSONPtr set(const std::vector<int>& numberList) {
			Clear();
			type_ = Type::Array;
			for (auto it = numberList.begin(); it != numberList.end(); it++) {
				array_.push_back(JSON::MakeNumber(*it));
			}
			return shared_from_this();
		}

		inline JSONPtr set(std::initializer_list<float> numberList) {
			Clear();
			type_ = Type::Array;
			for (auto it = numberList.begin(); it != numberList.end(); it++) {
				array_.push_back(JSON::MakeNumber(*it));
			}
			return shared_from_this();
		}

		inline JSONPtr set(const std::vector<float>& numberList) {
			Clear();
			type_ = Type::Array;
			for (auto it = numberList.begin(); it != numberList.end(); it++) {
				array_.push_back(JSON::MakeNumber(*it));
			}
			return shared_from_this();
		}

		inline JSONPtr set(std::initializer_list<double> numberList) {
			Clear();
			type_ = Type::Array;
			for (auto it = numberList.begin(); it != numberList.end(); it++) {
				array_.push_back(JSON::MakeNumber(*it));
			}
			return shared_from_this();
		}

		inline JSONPtr set(const std::vector<double>& numberList) {
			Clear();
			type_ = Type::Array;
			for (auto it = numberList.begin(); it != numberList.end(); it++) {
				array_.push_back(JSON::MakeNumber(*it));
			}
			return shared_from_this();
		}

		inline JSONPtr set(std::initializer_list<std::string> stringList) {
			Clear();
			type_ = Type::Array;
			for (auto it = stringList.begin(); it != stringList.end(); it++) {
				array_.push_back(JSON::MakeString(*it));
			}
			return shared_from_this();
		}

		inline JSONPtr set(const std::vector<std::string>& stringList) {
			Clear();
			type_ = Type::Array;
			for (auto it = stringList.begin(); it != stringList.end(); it++) {
				array_.push_back(JSON::MakeString(*it));
			}
			return shared_from_this();
		}

		inline JSONPtr set(std::initializer_list<std::pair<std::string, JSONPtr>> members) {
			Clear();
			type_ = Type::Object;
			for (auto it = members.begin(); it != members.end(); it++) {
				map_[it->first] = it->second;
			}
			return shared_from_this();
		}

		inline JSONPtr set(const std::map<std::string, JSONPtr>& members) {
			Clear();
			type_ = Type::Object;
			for (auto it = members.begin(); it != members.end(); it++) {
				map_[it->first] = it->second;
			}
			return shared_from_this();
		}

		inline JSONPtr set(const JSONPtr json) {
			Clear();
			if (!json)
				return shared_from_this();

			const JSON* pjson = json.get();
			type_ = pjson->type_;
			if (IsNumber())
				dval_ = json->dval_;
			if (IsString())
				sval_ = json->sval_;
			if (IsArray())
				array_ = json->array_;
			if (IsObject())
				map_ = json->map_;
			return shared_from_this();
		}

		inline JSONPtr PushBack(JSONPtr json) {
			if (!json || !IsArray())
				throw std::runtime_error("JSON object is not an array");
			array_.push_back(json);
			return shared_from_this();
		}

		inline JSONPtr PopBack() {
			if (!IsArray())
				throw std::runtime_error("JSON object is not an array");
			if (array_.empty())
				return JSONPtr();
			JSONPtr json = array_.back();
			array_.pop_back();
			return json;
		}

		inline Type GetType() const { return type_; }
		inline bool InstanceOf(const Type t) const { return type_ == t; }
		bool Equals(const JSONPtr rhs) const;

	private:
		Type type_ = Type::Null;
		double dval_ = 0.0;
		std::string sval_;
		std::vector<JSONPtr> array_;
		std::map<std::string, JSONPtr> map_;

		void copyArray(const std::vector<JSONPtr>& a);
		void copyMap(const std::map<std::string, JSONPtr>& m);

		static const std::string NullString;
	};

	inline bool operator==(const JSONPtr lhs, const JSONPtr rhs) {
		if (!lhs || !rhs)
			return false;
		return lhs->Equals(rhs);
	}

	inline bool operator!=(const JSONPtr lhs, const JSONPtr rhs) {
		if (!lhs || !rhs)
			return false;
		return !lhs->Equals(rhs);
	}
} // namespace Df

#endif
