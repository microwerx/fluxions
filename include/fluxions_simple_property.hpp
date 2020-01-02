// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017-2019 Jonathan Metzgar
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
#ifndef FLUXIONS_SIMPLE_PROPERTY_HPP
#define FLUXIONS_SIMPLE_PROPERTY_HPP

#include <fluxions_base.hpp>

namespace Fluxions
{

	enum class PropertyType
	{
		// OpenGL ES 2.0 GLSL Types
		NOTHING = 0,
		STRUCT,
		ARRAY,
		BOOL,
		INT,
		FLOAT,
		VEC2,
		VEC3,
		VEC4,
		BVEC2,
		BVEC3,
		BVEC4,
		IVEC2,
		IVEC3,
		IVEC4,
		MAT2,
		MAT3,
		MAT4,
		SAMPLER2D,
		SAMPLERCUBE,

		// OpenGL ES 3.0 GLSL Types
		UINT,
		UVEC2,
		UVEC3,
		UVEC4,
		MAT2X2,
		MAT2X3,
		MAT2X4,
		MAT3X2,
		MAT3X3,
		MAT3X4,
		MAT4X2,
		MAT4X3,
		MAT4X4,
		SAMPLER3D,
		SAMPLERCUBESHADOW,
		SAMPLER2DSHADOW,
		SAMPLER2DARRAY,
		SAMPLER2DARRAYSHADOW,
		ISAMPLER2D,
		ISAMPLER3D,
		ISAMPLERCUBE,
		ISAMPLER2DARRAY,
		USAMPLER2D,
		USAMPLER3D,
		USAMPLERCUBE,
		USAMPLER2DARRAY,

		// OpenGL ES 3.1 GLSL Types
		ISAMPLER2DMS,
		IIMAGE2D,
		IIMAGE3D,
		IIMAGECUBE,
		IIMAGE2DARRAY,
		ATOMIC_UINT,
		USAMPLER2DMS,
		UIMAGE2D,
		UIMAGE3D,
		UIMAGECUBE,
		UIMAGE2DARRAY,
		SAMPLER2DMS,
		IMAGE2D,
		IMAGE3D,
		IMAGECUBE,
		IMAGE2DARRAY,

		DOUBLE,

		// Used for testing which types are available
		MAXVALUETYPE,

		// These are special types indicating memory buffers
		UNIFORM_BLOCK = 1000,
		BUFFER_BLOCK = 2000,
		STRING = 3000
	};

	const std::string PropertyTypeToString(PropertyType propertyType);

	class SimpleProperty
	{
	private:
		const PropertyType type;

	public:
		SimpleProperty() : type(PropertyType::NOTHING) {}
		SimpleProperty(PropertyType propertyType) : type(propertyType) {}
		virtual ~SimpleProperty() {}

		virtual const std::type_info& GetTypeID() const { return typeid(void); }
		virtual bool empty() const { return true; }

		const PropertyType GetType() const { return type; }
		const std::string GetTypeAsString() const { return PropertyTypeToString(type); }
		virtual const size_t GetSize() const { return 0; }
		const SimpleProperty* GetAsSimplePropertyPtr() const { return this; }
	};

	using SimplePropertyPtr = SimpleProperty *;
	using UniqueSimplePropertyPtr = std::unique_ptr<SimpleProperty>;
	using SharedSimplePropertyPtr = std::shared_ptr<SimpleProperty>;

	template <typename T>
	class TSimpleProperty : public SimpleProperty
	{
	public:
		T value;

		template <typename... Args>
		TSimpleProperty(PropertyType propertyType, Args... args)
			: SimpleProperty(propertyType), value(std::forward<Args>(args)...) {}

		virtual const std::type_info& GetTypeID() const { return typeid(T); }

		const T& Value() const { return value; }
		T& Value() { return value; }
		const T* ValuePtr() const { return &value; }
		T* ValuePtr() { return &value; }
		const T& GetValue() const { return value; }
		void SetValue(const T& newValue) { value = newValue; }
		const T* GetValuePointer() const { return &value; }
		virtual const size_t GetSize() const { return sizeof(value); }
	};

	template <typename T, typename... Args>
	inline std::unique_ptr<TSimpleProperty<T>>
		MakeProperty(PropertyType propertyType, Args... args) {
		return std::unique_ptr<TSimpleProperty<T>>(new TSimpleProperty<T>(propertyType, std::forward<Args>(args)...));
	}

	template <typename T, typename... Args>
	inline std::unique_ptr<TSimpleProperty<T>>
		MakeUniqueProperty(PropertyType propertyType, Args... args) {
		return std::unique_ptr<TSimpleProperty<T>>(new TSimpleProperty<T>(propertyType, std::forward<Args>(args)...));
	}

	template <typename T, typename... Args>
	inline std::shared_ptr<TSimpleProperty<T>>
		MakeSharedProperty(PropertyType propertyType, Args... args) {
		return std::make_shared<TSimpleProperty<T>>(propertyType, std::forward<Args>(args)...);
	}

	template <typename T, typename... Args>
	inline UniqueSimplePropertyPtr MakeSimpleProperty(PropertyType propertyType, Args... args) {
		return std::unique_ptr<SimpleProperty>(new TSimpleProperty<T>(propertyType, std::forward<Args>(args)...));
	}

	template <typename T, typename... Args>
	inline SharedSimplePropertyPtr MakeSharedSimpleProperty(PropertyType propertyType, Args... args) {
		return std::shared_ptr<SimpleProperty>(new TSimpleProperty<T>(propertyType, std::forward<Args>(args)...));
	}

	template <typename T>
	constexpr TSimpleProperty<T>* CastAsPropertyType(SimpleProperty* ptr) {
		if (!ptr || ptr->GetTypeID() != typeid(T))
			return nullptr;

		return static_cast<TSimpleProperty<T>*>(ptr);
	}

	template <typename T>
	constexpr TSimpleProperty<T>* CastAsPropertyType(UniqueSimplePropertyPtr& ptr) {
		return CastAsPropertyType<T>(ptr.get());
	}

	template <typename T>
	constexpr TSimpleProperty<T>* CastAsPropertyType(SharedSimplePropertyPtr& ptr) {
		return CastAsPropertyType<T>(ptr.get());
	}

	template <typename T>
	constexpr const TSimpleProperty<T>* CastAsPropertyType(const SimpleProperty* ptr) {
		if (!ptr || ptr->GetTypeID() != typeid(T))
			return nullptr;

		return static_cast<const TSimpleProperty<T>*>(ptr);
	}

	template <typename T>
	constexpr const TSimpleProperty<T>* CastAsPropertyType(const std::unique_ptr<SimpleProperty>& ptr) {
		return CastAsPropertyType<T>(ptr.get());
	}

	template <typename T>
	constexpr const TSimpleProperty<T>* CastAsPropertyType(const std::shared_ptr<SimpleProperty>& ptr) {
		return CastAsPropertyType<T>(ptr.get());
	}

	template <typename T>
	constexpr const T* CastAsPropertyTypeValuePtr(const SimpleProperty* ptr) {
		const TSimpleProperty<T>* propertyPtr = CastAsPropertyType<T>(ptr);

		if (propertyPtr == nullptr)
			return nullptr;

		if (propertyPtr->GetTypeID() != typeid(T))
			return nullptr;

		return propertyPtr->GetValuePointer();
	}

	template <typename T>
	constexpr const T* CastAsPropertyTypeValuePtr(const UniqueSimplePropertyPtr& ptr) {
		return CastAsPropertyTypeValuePtr<T>(ptr.get());
	}

	template <typename T>
	constexpr const T* CastAsPropertyTypeValuePtr(const SharedSimplePropertyPtr& ptr) {
		return CastAsPropertyTypeValuePtr<T>(ptr.get());
	}

	template <typename T>
	inline std::shared_ptr<TSimpleProperty<T>> DynamicSharedSimplePropertyCast(const SharedSimplePropertyPtr& ptr) {
		if (!ptr || ptr->GetTypeID() != typeid(T))
			return nullptr;

		return std::dynamic_pointer_cast<TSimpleProperty<T>>(ptr);
	}

	template <typename T>
	inline std::unique_ptr<TSimpleProperty<T>> StaticUniqueSimplePropertyCast(UniqueSimplePropertyPtr& ptr) {
		if (!ptr || ptr->GetTypeID() != typeid(T))
			return nullptr;

		return std::unique_ptr<TSimpleProperty<T>>(static_cast<TSimpleProperty<T>*>(ptr.release()));
	}

#define DECLARE_PROPERTY_TYPE(ctype, type, TypeName)                                                             \
	using TypeName##Property = TSimpleProperty<ctype>;                                                           \
	using Unique##TypeName##PropertyPtr = std::unique_ptr<TypeName##Property>;                                   \
	using Shared##TypeName##PropertyPtr = std::shared_ptr<TypeName##Property>;                                   \
	inline Unique##TypeName##PropertyPtr MakeUnique##TypeName##Property(const ctype &value)                      \
	{                                                                                                            \
		return MakeUniqueProperty<ctype>(PropertyType::type, value);                                             \
	}                                                                                                            \
	inline Shared##TypeName##PropertyPtr MakeShared##TypeName##Property(const ctype &value)                      \
	{                                                                                                            \
		return MakeSharedProperty<ctype>(PropertyType::type, value);                                             \
	}                                                                                                            \
	inline const ctype *Get##TypeName##ValuePtr(const SimpleProperty *ptr)                                       \
	{                                                                                                            \
		return CastAsPropertyTypeValuePtr<ctype>(ptr);                                                           \
	}                                                                                                            \
	inline const ctype *Get##TypeName##ValuePtr(const UniqueSimplePropertyPtr &ptr)                              \
	{                                                                                                            \
		return CastAsPropertyTypeValuePtr<ctype>(ptr);                                                           \
	}                                                                                                            \
	inline const ctype *Get##TypeName##ValuePtr(const SharedSimplePropertyPtr &ptr)                              \
	{                                                                                                            \
		return CastAsPropertyTypeValuePtr<ctype>(ptr);                                                           \
	}                                                                                                            \
	inline Shared##TypeName##PropertyPtr CastToShared##TypeName##PropertyPtr(const SharedSimplePropertyPtr &ptr) \
	{                                                                                                            \
		return DynamicSharedSimplePropertyCast<ctype>(ptr);                                                      \
	}                                                                                                            \
	inline Unique##TypeName##PropertyPtr CastToUnique##TypeName##PropertyPtr(UniqueSimplePropertyPtr &ptr)       \
	{                                                                                                            \
		return StaticUniqueSimplePropertyCast<ctype>(ptr);                                                       \
	}

	DECLARE_PROPERTY_TYPE(bool, BOOL, Bool)
	DECLARE_PROPERTY_TYPE(int, INT, Int)
	DECLARE_PROPERTY_TYPE(float, FLOAT, Float)
	DECLARE_PROPERTY_TYPE(Vector2f, VEC2, Vec2)
	DECLARE_PROPERTY_TYPE(Vector3f, VEC3, Vec3)
	DECLARE_PROPERTY_TYPE(Vector4f, VEC4, Vec4)
	DECLARE_PROPERTY_TYPE(Vector2i, BVEC2, BVec2)
	DECLARE_PROPERTY_TYPE(Vector3i, BVEC3, BVec3)
	DECLARE_PROPERTY_TYPE(Vector4i, BVEC4, BVec4)
	DECLARE_PROPERTY_TYPE(Vector2i, IVEC2, IVec2)
	DECLARE_PROPERTY_TYPE(Vector3i, IVEC3, IVec3)
	DECLARE_PROPERTY_TYPE(Vector4i, IVEC4, IVec4)
	DECLARE_PROPERTY_TYPE(Color3f, VEC3, Col3)
	DECLARE_PROPERTY_TYPE(Color4f, VEC4, Col4)
	DECLARE_PROPERTY_TYPE(Matrix2f, MAT2, Mat2)
	DECLARE_PROPERTY_TYPE(Matrix3f, MAT3, Mat3)
	DECLARE_PROPERTY_TYPE(Matrix4f, MAT4, Mat4)
	DECLARE_PROPERTY_TYPE(int, SAMPLER2D, Sampler2D)
	DECLARE_PROPERTY_TYPE(int, SAMPLERCUBE, SamplerCube)
	DECLARE_PROPERTY_TYPE(unsigned, UINT, Uint)
	DECLARE_PROPERTY_TYPE(Vector2ui, UVEC2, UVec2)
	DECLARE_PROPERTY_TYPE(Vector3ui, UVEC3, UVec3)
	DECLARE_PROPERTY_TYPE(Vector4ui, UVEC4, UVec4)
	DECLARE_PROPERTY_TYPE(Matrix2f, MAT2X2, Mat2x2)
	//DECLARE_PROPERTY_TYPE(Matrix2x3f, MAT2X3, Mat2x3)
	//DECLARE_PROPERTY_TYPE(Matrix2x4f, MAT2X4, Mat2x4)
	//DECLARE_PROPERTY_TYPE(Matrix3x2f, MAT3X2, Mat3x2)
	DECLARE_PROPERTY_TYPE(Matrix3f, MAT3X3, Mat3x3)
	//DECLARE_PROPERTY_TYPE(Matrix3x4f, MAT3X4, Mat3x4)
	//DECLARE_PROPERTY_TYPE(Matrix4x2f, MAT4X2, Mat4x2)
	//DECLARE_PROPERTY_TYPE(Matrix4x3f, MAT4X3, Mat4x3)
	DECLARE_PROPERTY_TYPE(Matrix4f, MAT4X4, Mat4x4)
	DECLARE_PROPERTY_TYPE(int, SAMPLER3D, Sampler3D)
	DECLARE_PROPERTY_TYPE(int, SAMPLERCUBESHADOW, SamplerCubeShadow)
	DECLARE_PROPERTY_TYPE(int, SAMPLER2DSHADOW, Sampler2DShadow)
	DECLARE_PROPERTY_TYPE(int, SAMPLER2DARRAY, Sampler2DArray)
	DECLARE_PROPERTY_TYPE(int, SAMPLER2DARRAYSHADOW, Sampler2DArrayShadow)
	DECLARE_PROPERTY_TYPE(int, ISAMPLER2D, ISampler2D)
	DECLARE_PROPERTY_TYPE(int, ISAMPLER3D, ISampler3D)
	DECLARE_PROPERTY_TYPE(int, ISAMPLERCUBE, ISamplerCube)
	DECLARE_PROPERTY_TYPE(int, ISAMPLER2DARRAY, ISampler2DArray)
	DECLARE_PROPERTY_TYPE(int, USAMPLER2D, USampler2D)
	DECLARE_PROPERTY_TYPE(int, USAMPLER3D, USampler3D)
	DECLARE_PROPERTY_TYPE(int, USAMPLERCUBE, USamplerCube)
	DECLARE_PROPERTY_TYPE(int, USAMPLER2DARRAY, USampler2DArray)
	DECLARE_PROPERTY_TYPE(int, ISAMPLER2DMS, ISampler2DMS)
	DECLARE_PROPERTY_TYPE(int, IIMAGE2D, IImage2D)
	DECLARE_PROPERTY_TYPE(int, IIMAGE3D, IImage3D)
	DECLARE_PROPERTY_TYPE(int, IIMAGECUBE, IImageCube)
	DECLARE_PROPERTY_TYPE(int, IIMAGE2DARRAY, IImage2DArray)
	DECLARE_PROPERTY_TYPE(unsigned, ATOMIC_UINT, AtomicUint)
	DECLARE_PROPERTY_TYPE(int, USAMPLER2DMS, USampler2DMS)
	DECLARE_PROPERTY_TYPE(int, UIMAGE2D, UImage2D)
	DECLARE_PROPERTY_TYPE(int, UIMAGE3D, UImage3D)
	DECLARE_PROPERTY_TYPE(int, UIMAGECUBE, UImageCube)
	DECLARE_PROPERTY_TYPE(int, UIMAGE2DARRAY, UImage2DArray)
	DECLARE_PROPERTY_TYPE(int, SAMPLER2DMS, Sampler2DMS)
	DECLARE_PROPERTY_TYPE(int, IMAGE2D, Image2D)
	DECLARE_PROPERTY_TYPE(int, IMAGE3D, Image3D)
	DECLARE_PROPERTY_TYPE(int, IMAGECUBE, ImageCube)
	DECLARE_PROPERTY_TYPE(int, IMAGE2DARRAY, Image2DArray)
	DECLARE_PROPERTY_TYPE(double, DOUBLE, Double)
	DECLARE_PROPERTY_TYPE(int, UNIFORM_BLOCK, UniformBlock)
	DECLARE_PROPERTY_TYPE(int, BUFFER_BLOCK, BufferBlock)
	DECLARE_PROPERTY_TYPE(std::string, STRING, String)

#undef DECLARE_PROPERTY_TYPE

	class SimpleAssociativePropertyList
	{
	private:
		SharedSimplePropertyPtr defaultItem;
		std::string emptyString;
		std::map<std::string, int> nameIndex;
		std::vector<std::pair<std::string, SharedSimplePropertyPtr>> properties;

	public:
		/// <summary>Constructor for SimpleAssociativePropertyList</summary>
		SimpleAssociativePropertyList();

		/// <summary>Destructor for SimpleAssociativePropertyList</summary>
		~SimpleAssociativePropertyList();

		/// <summary>reset() completely clears all pointers and names from this container.</summary>
		void Clear();

		/// <summary>Push(name, valuePtr) inserts a std::shared_ptr to a simple property in the list.</summary>
		void Add(const std::string& name, SharedSimplePropertyPtr& valuePtr);

		/// <summary>Remove(name) deletes the pointer for the property named <code>name</code>.</summary>
		void Remove(const std::string& name);

		/// <summary>IsElement(name) returns true if the pointer for the property named <code>name</code> is valid.</summary>
		bool IsElement(const std::string& name);

		/// <summary>Get(name) returns a std::shared_ptr for the property named <code>name</code>.
		/// It returns an empty pointer if <code>name</code> is not valid.</summary>
		SharedSimplePropertyPtr& Get(const std::string& name);

		/// <summary>GetDynamicCastSharedPtr<type, propertyType>(name) returns a std::shared_ptr for the property
		/// named <code>name</code>. It returns an empty pointer if <code>name</code> is not valid.</summary>
		template <typename T>
		std::shared_ptr<TSimpleProperty<T>>& GetDynamicCastSharedPtr(const std::string& name) {
			SharedSimplePropertyPtr ptr = Get(name);
			if (!ptr)
				return std::shared_ptr<TSimpleProperty<T>>();

			return DynamicSharedSimplePropertyCast<T>(ptr);
		}

		/// <summary>GetValuePtr<type, propertyType>(name) returns a const pointer to a T* variable or nullptr
		/// if the property named <code>name</code> is valid or not valid, respectively.</summary>
		template <typename T>
		const T* GetValuePtr(const std::string& name) {
			SharedSimplePropertyPtr ptr = Get(name);
			if (!ptr)
				return nullptr;

			return CastAsPropertyTypeValuePtr<T>(ptr);
		}

		/// <summary>GetValue<type, propertyType>(name) returns a reference to a T variable for
		/// the property named <code>name</code>. If either the pointer is bad, or the types are
		/// not matching, this method will throw a bad_cast exception.</summary>
		template <typename T>
		T& GetValue(const std::string& name) {
			SharedSimplePropertyPtr ptr = Get(name);
			if (!ptr) {
				HFLOGERROR("T& SimpleAssociatedPropertyContainer::GetValue(name) --> pointer is invalid");
				throw std::bad_cast();
			}

			if (ptr->GetTypeID() != typeid(T)) {
				HFLOGERROR("T& SimpleAssociatedPropertyContainer::GetValue(name) --> types are incompatible");
				throw std::bad_cast();
			}

			return *(CastAsPropertyTypeValuePtr<T>(ptr));
		}

		/// <summary>GetIndexOf(name) returns a non-negative number if the property named <code>name</code>
		/// is valid and a negative number if it is not valid.</summary>
		int GetIndexOf(const std::string& name);

		/// <summary>GetAtIndex(i) returns a std::shared_ptr for the property at index <code>i</code>.
		/// It returns an empty pointer if <code>i</code> is not in the range of the container.</summary>
		SharedSimplePropertyPtr& GetAtIndex(int i);

		/// <summary>GetNameAtIndex(i) returns a string for the name of the property at index <code>i</code>.
		/// It returns an empty string if <code>i</code> is not in the range of the container.</summary>
		const std::string& GetNameAtIndex(int i);

		/// <summary>operator[](name) returns a std::shared_ptr for the property named <code>name</code>.
		/// It is the very pointer stored for that location, so it may be used to add a new property
		/// to the list and have a reference to the smart pointer associated with that name.</summary>
		SharedSimplePropertyPtr& operator[](const std::string& name);

		/// <summary>begin() returns an iterator for the beginning of the properties list.</summary>
		std::vector<std::pair<std::string, SharedSimplePropertyPtr>>::iterator begin() { return properties.begin(); }

		/// <summary>begin() returns an iterator for the end of the properties list.</summary>
		std::vector<std::pair<std::string, SharedSimplePropertyPtr>>::iterator end() { return properties.end(); }

		/// <summary>cbegin() returns a const iterator for the beginning of the properties list.</summary>
		std::vector<std::pair<std::string, SharedSimplePropertyPtr>>::const_iterator cbegin() { return properties.cbegin(); }

		/// <summary>cend() returns a const iterator for the end of the properties list.</summary>
		std::vector<std::pair<std::string, SharedSimplePropertyPtr>>::const_iterator cend() { return properties.cend(); }
	};

	/*
	typedef TSimpleProperty<float, PropertyType::FLOAT> FloatProperty;
	typedef TSimpleProperty<Vector2f, PropertyType::VEC2> Vec2Property;

	template <typename ... Args>
	std::unique_ptr<SimpleProperty> MakeVec2Property(Args&& ... args)
	{
	// return std::unique_ptr<SimpleProperty>(new Vec2Property(args));
	return MakeProperty<Vector2f, PropertyType::VEC2>(args);
	}

	Vec2Property *StaticCastVec2Property(std::unique_ptr<SimpleProperty> &ptr)
	{
	if (!ptr) return nullptr;
	if (*ptr->GetPropertytype() == PropertyType::VEC2)
	return CastAsPropertyType<Vector2f, PropertyType::VEC2>(ptr);
	}



	class SimpleMaterial
	{
	private:
	std::map<std::string, int> nameIndex;
	std::vector<SimpleProperty> properties;
	public:
	SimpleMaterial()
	{

	}
	};
	*/

	int test_fluxions_simple_property();
} // namespace Fluxions

#endif // FLUXIONS_SIMPLE_PROPERTY_HPP
