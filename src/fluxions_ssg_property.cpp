#include "fluxions_ssg_pch.hpp"
#include <fluxions_ssg_property.hpp>

template class std::shared_ptr<Fluxions::SimpleProperty>;
template class std::unique_ptr<Fluxions::SimpleProperty>;
template class std::map<std::string, int>;

/*
// #define DECLARE_PROPERTY_TYPE(ctype, type, TypeName)                         \
// 	using TypeName##Property = Fluxions::TSimpleProperty<ctype>;             \
// 	template class Fluxions::TSimpleProperty<ctype>;                         \
// 	template class std::unique_ptr<TypeName##Property>;                      \
// 	template class std::shared_ptr<TypeName##Property>;                      \
// 	template std::unique_ptr<Fluxions::TSimpleProperty<T>> Fluxions::MakeProperty<ctype>(PropertyType::type, value); \
// 	template std::unique_ptr<Fluxions::TSimpleProperty<T>> Fluxions::MakeUniqueProperty<ctype>(PropertyType::type, value); \
// 	template std::shared_ptr<Fluxions::TSimpleProperty<T>> Fluxions::MakeSharedProperty<ctype>(PropertyType::type, value);\
// 	template Fluxions::UniqueSimplePropertyPtr Fluxions::MakeSimpleProperty<ctype>(PropertyType::type, value); \
// 	template Fluxions::CastAsPropertyTypeValuePtr<ctype>(ptr);               \
// 	template Fluxions::DynamicSharedSimplePropertyCast<ctype>(ptr);          \
// 	template Fluxions::StaticUniqueSimplePropertyCast<ctype>(ptr);           \
*/
// DECLARE_PROPERTY_TYPE(bool, BOOL, Bool)
// DECLARE_PROPERTY_TYPE(int, INT, Int)
// DECLARE_PROPERTY_TYPE(float, FLOAT, Float)
// DECLARE_PROPERTY_TYPE(Fluxions::Vector2f, VEC2, Vec2)
// DECLARE_PROPERTY_TYPE(Fluxions::Vector3f, VEC3, Vec3)
// DECLARE_PROPERTY_TYPE(Fluxions::Vector4f, VEC4, Vec4)
// DECLARE_PROPERTY_TYPE(Fluxions::Vector2i, BVEC2, BVec2)
// DECLARE_PROPERTY_TYPE(Fluxions::Vector3i, BVEC3, BVec3)
// DECLARE_PROPERTY_TYPE(Fluxions::Vector4i, BVEC4, BVec4)
// DECLARE_PROPERTY_TYPE(Fluxions::Vector2i, IVEC2, IVec2)
// DECLARE_PROPERTY_TYPE(Fluxions::Vector3i, IVEC3, IVec3)
// DECLARE_PROPERTY_TYPE(Fluxions::Vector4i, IVEC4, IVec4)
// DECLARE_PROPERTY_TYPE(Fluxions::Color3f, VEC3, Col3)
// DECLARE_PROPERTY_TYPE(Fluxions::Color4f, VEC4, Col4)
// DECLARE_PROPERTY_TYPE(Fluxions::Matrix2f, MAT2, Mat2)
// DECLARE_PROPERTY_TYPE(Fluxions::Matrix3f, MAT3, Mat3)
// DECLARE_PROPERTY_TYPE(Fluxions::Matrix4f, MAT4, Mat4)
// DECLARE_PROPERTY_TYPE(int, SAMPLER2D, Sampler2D)
// DECLARE_PROPERTY_TYPE(int, SAMPLERCUBE, SamplerCube)
// DECLARE_PROPERTY_TYPE(unsigned, UINT, Uint)
// DECLARE_PROPERTY_TYPE(Fluxions::Vector2ui, UVEC2, UVec2)
// DECLARE_PROPERTY_TYPE(Fluxions::Vector3ui, UVEC3, UVec3)
// DECLARE_PROPERTY_TYPE(Fluxions::Vector4ui, UVEC4, UVec4)
// DECLARE_PROPERTY_TYPE(Fluxions::Matrix2f, MAT2X2, Mat2x2)
// //DECLARE_PROPERTY_TYPE(Fluxions::Matrix2x3f, MAT2X3, Mat2x3)
// //DECLARE_PROPERTY_TYPE(Fluxions::Matrix2x4f, MAT2X4, Mat2x4)
// //DECLARE_PROPERTY_TYPE(Fluxions::Matrix3x2f, MAT3X2, Mat3x2)
// DECLARE_PROPERTY_TYPE(Fluxions::Matrix3f, MAT3X3, Mat3x3)
// //DECLARE_PROPERTY_TYPE(Fluxions::Matrix3x4f, MAT3X4, Mat3x4)
// //DECLARE_PROPERTY_TYPE(Fluxions::Matrix4x2f, MAT4X2, Mat4x2)
// //DECLARE_PROPERTY_TYPE(Fluxions::Matrix4x3f, MAT4X3, Mat4x3)
// DECLARE_PROPERTY_TYPE(Fluxions::Matrix4f, MAT4X4, Mat4x4)
// DECLARE_PROPERTY_TYPE(int, SAMPLER3D, Sampler3D)
// DECLARE_PROPERTY_TYPE(int, SAMPLERCUBESHADOW, SamplerCubeShadow)
// DECLARE_PROPERTY_TYPE(int, SAMPLER2DSHADOW, Sampler2DShadow)
// DECLARE_PROPERTY_TYPE(int, SAMPLER2DARRAY, Sampler2DArray)
// DECLARE_PROPERTY_TYPE(int, SAMPLER2DARRAYSHADOW, Sampler2DArrayShadow)
// DECLARE_PROPERTY_TYPE(int, ISAMPLER2D, ISampler2D)
// DECLARE_PROPERTY_TYPE(int, ISAMPLER3D, ISampler3D)
// DECLARE_PROPERTY_TYPE(int, ISAMPLERCUBE, ISamplerCube)
// DECLARE_PROPERTY_TYPE(int, ISAMPLER2DARRAY, ISampler2DArray)
// DECLARE_PROPERTY_TYPE(int, USAMPLER2D, USampler2D)
// DECLARE_PROPERTY_TYPE(int, USAMPLER3D, USampler3D)
// DECLARE_PROPERTY_TYPE(int, USAMPLERCUBE, USamplerCube)
// DECLARE_PROPERTY_TYPE(int, USAMPLER2DARRAY, USampler2DArray)
// DECLARE_PROPERTY_TYPE(int, ISAMPLER2DMS, ISampler2DMS)
// DECLARE_PROPERTY_TYPE(int, IIMAGE2D, IImage2D)
// DECLARE_PROPERTY_TYPE(int, IIMAGE3D, IImage3D)
// DECLARE_PROPERTY_TYPE(int, IIMAGECUBE, IImageCube)
// DECLARE_PROPERTY_TYPE(int, IIMAGE2DARRAY, IImage2DArray)
// DECLARE_PROPERTY_TYPE(unsigned, ATOMIC_UINT, AtomicUint)
// DECLARE_PROPERTY_TYPE(int, USAMPLER2DMS, USampler2DMS)
// DECLARE_PROPERTY_TYPE(int, UIMAGE2D, UImage2D)
// DECLARE_PROPERTY_TYPE(int, UIMAGE3D, UImage3D)
// DECLARE_PROPERTY_TYPE(int, UIMAGECUBE, UImageCube)
// DECLARE_PROPERTY_TYPE(int, UIMAGE2DARRAY, UImage2DArray)
// DECLARE_PROPERTY_TYPE(int, SAMPLER2DMS, Sampler2DMS)
// DECLARE_PROPERTY_TYPE(int, IMAGE2D, Image2D)
// DECLARE_PROPERTY_TYPE(int, IMAGE3D, Image3D)
// DECLARE_PROPERTY_TYPE(int, IMAGECUBE, ImageCube)
// DECLARE_PROPERTY_TYPE(int, IMAGE2DARRAY, Image2DArray)
// DECLARE_PROPERTY_TYPE(double, DOUBLE, Double)
// DECLARE_PROPERTY_TYPE(int, UNIFORM_BLOCK, UniformBlock)
// DECLARE_PROPERTY_TYPE(int, BUFFER_BLOCK, BufferBlock)
// DECLARE_PROPERTY_TYPE(std::string, STRING, String)

// #undef DECLARE_PROPERTY_TYPE

namespace Fluxions {
	SimpleAssociativePropertyList::SimpleAssociativePropertyList() {}

	SimpleAssociativePropertyList::~SimpleAssociativePropertyList() {}

	void SimpleAssociativePropertyList::Clear() {
		nameIndex.clear();
		properties.clear();
	}

	void SimpleAssociativePropertyList::Add(const std::string& name, SharedSimplePropertyPtr& valuePtr) {
		// check if index already exists...
		int usedIndex = GetIndexOf(name);
		if (usedIndex < 0) {
			// nope, add new element.
			properties.push_back(std::pair<std::string, SharedSimplePropertyPtr>(name, valuePtr));
			usedIndex = (int)properties.size() - 1;
			nameIndex[name] = usedIndex;
		}
		if (usedIndex >= 0) {
			properties[usedIndex].second = valuePtr;
		}
	}

	void SimpleAssociativePropertyList::Remove(const std::string& name) {
		int usedIndex = GetIndexOf(name);
		if (usedIndex >= 0) {
			properties[usedIndex].first.clear();
			properties[usedIndex].second.reset();
			// Names are never removed from the map to preserve the array order.
		}
	}

	bool SimpleAssociativePropertyList::IsElement(const std::string& name) {
		// If it's in the index and it's not a reset shared_ptr, return true.
		int usedIndex = GetIndexOf(name);
		if (usedIndex >= 0) {
			if (properties[usedIndex].second.use_count() > 0)
				return true;
		}
		return false;
	}

	SharedSimplePropertyPtr& SimpleAssociativePropertyList::Get(const std::string& name) {
		int usedIndex = GetIndexOf(name);
		if (usedIndex >= 0) {
			return properties[usedIndex].second;
		}
		return defaultItem;
	}

	int SimpleAssociativePropertyList::GetIndexOf(const std::string& name) {
		auto it = nameIndex.find(name);
		if (it != nameIndex.end()) {
			return it->second;
		}
		return -1;
	}

	const std::string& SimpleAssociativePropertyList::GetNameAtIndex(int i) {
		if (i < 0 || i >= (int)properties.size()) {
			return properties[i].first;
		}
		return emptyString;
	}

	SharedSimplePropertyPtr& SimpleAssociativePropertyList::GetAtIndex(int i) {
		if (i < 0 || i >= (int)properties.size()) {
			return properties[i].second;
		}
		return defaultItem;
	}

	SharedSimplePropertyPtr& SimpleAssociativePropertyList::operator[](const std::string& name) {
		int index = GetIndexOf(name);
		if (index < 0) {
			properties.push_back(std::pair<std::string, SharedSimplePropertyPtr>(name, SharedSimplePropertyPtr()));
			index = (int)properties.size() - 1;
			nameIndex[name] = index;
		}

		return properties[index].second;
	}

	const std::string PropertyTypeToString(PropertyType propertyType) {
		switch (propertyType) {
		case PropertyType::NOTHING:
			return std::string("NOTHING");
		case PropertyType::STRUCT:
			return std::string("STRUCT");
		case PropertyType::ARRAY:
			return std::string("ARRAY");
		case PropertyType::BOOL:
			return std::string("BOOL");
		case PropertyType::INT:
			return std::string("INT");
		case PropertyType::FLOAT:
			return std::string("FLOAT");
		case PropertyType::DOUBLE:
			return std::string("DOUBLE");
		case PropertyType::VEC2:
			return std::string("VEC2");
		case PropertyType::VEC3:
			return std::string("VEC3");
		case PropertyType::VEC4:
			return std::string("VEC4");
		case PropertyType::BVEC2:
			return std::string("BVEC2");
		case PropertyType::BVEC3:
			return std::string("BVEC3");
		case PropertyType::BVEC4:
			return std::string("BVEC4");
		case PropertyType::IVEC2:
			return std::string("IVEC2");
		case PropertyType::IVEC3:
			return std::string("IVEC3");
		case PropertyType::IVEC4:
			return std::string("IVEC4");
		case PropertyType::MAT2:
			return std::string("MAT2");
		case PropertyType::MAT3:
			return std::string("MAT3");
		case PropertyType::MAT4:
			return std::string("MAT4");
		case PropertyType::SAMPLER2D:
			return std::string("SAMPLER2D");
		case PropertyType::SAMPLERCUBE:
			return std::string("SAMPLERCUBE");
		case PropertyType::UINT:
			return std::string("UINT");
		case PropertyType::UVEC2:
			return std::string("UVEC2");
		case PropertyType::UVEC3:
			return std::string("UVEC3");
		case PropertyType::UVEC4:
			return std::string("UVEC4");
		case PropertyType::MAT2X2:
			return std::string("MAT2X2");
		case PropertyType::MAT2X3:
			return std::string("MAT2X3");
		case PropertyType::MAT2X4:
			return std::string("MAT2X4");
		case PropertyType::MAT3X2:
			return std::string("MAT3X2");
		case PropertyType::MAT3X3:
			return std::string("MAT3X3");
		case PropertyType::MAT3X4:
			return std::string("MAT3X4");
		case PropertyType::MAT4X2:
			return std::string("MAT4X2");
		case PropertyType::MAT4X3:
			return std::string("MAT4X3");
		case PropertyType::MAT4X4:
			return std::string("MAT4X4");
		case PropertyType::SAMPLER3D:
			return std::string("SAMPLER3D");
		case PropertyType::SAMPLERCUBESHADOW:
			return std::string("SAMPLERCUBESHADOW");
		case PropertyType::SAMPLER2DSHADOW:
			return std::string("SAMPLER2DSHADOW");
		case PropertyType::SAMPLER2DARRAY:
			return std::string("SAMPLER2DARRAY");
		case PropertyType::SAMPLER2DARRAYSHADOW:
			return std::string("SAMPLER2DARRAYSHADOW");
		case PropertyType::ISAMPLER2D:
			return std::string("ISAMPLER2D");
		case PropertyType::ISAMPLER3D:
			return std::string("ISAMPLER3D");
		case PropertyType::ISAMPLERCUBE:
			return std::string("ISAMPLERCUBE");
		case PropertyType::ISAMPLER2DARRAY:
			return std::string("ISAMPLER2DARRAY");
		case PropertyType::USAMPLER2D:
			return std::string("USAMPLER2D");
		case PropertyType::USAMPLER3D:
			return std::string("USAMPLER3D");
		case PropertyType::USAMPLERCUBE:
			return std::string("USAMPLERCUBE");
		case PropertyType::USAMPLER2DARRAY:
			return std::string("USAMPLER2DARRAY");
		case PropertyType::ISAMPLER2DMS:
			return std::string("ISAMPLER2DMS");
		case PropertyType::IIMAGE2D:
			return std::string("IIMAGE2D");
		case PropertyType::IIMAGE3D:
			return std::string("IIMAGE3D");
		case PropertyType::IIMAGECUBE:
			return std::string("IIMAGECUBE");
		case PropertyType::IIMAGE2DARRAY:
			return std::string("IIMAGE2DARRAY");
		case PropertyType::ATOMIC_UINT:
			return std::string("ATOMIC_UINT");
		case PropertyType::USAMPLER2DMS:
			return std::string("USAMPLER2DMS");
		case PropertyType::UIMAGE2D:
			return std::string("UIMAGE2D");
		case PropertyType::UIMAGE3D:
			return std::string("UIMAGE3D");
		case PropertyType::UIMAGECUBE:
			return std::string("UIMAGECUBE");
		case PropertyType::UIMAGE2DARRAY:
			return std::string("UIMAGE2DARRAY");
		case PropertyType::SAMPLER2DMS:
			return std::string("SAMPLER2DMS");
		case PropertyType::IMAGE2D:
			return std::string("IMAGE2D");
		case PropertyType::IMAGE3D:
			return std::string("IMAGE3D");
		case PropertyType::IMAGECUBE:
			return std::string("IMAGECUBE");
		case PropertyType::IMAGE2DARRAY:
			return std::string("IMAGE2DARRAY");
		case PropertyType::MAXVALUETYPE:
			return std::string("MAXVALUETYPE");
		case PropertyType::UNIFORM_BLOCK:
			return std::string("UNIFORM_BLOCK");
		case PropertyType::BUFFER_BLOCK:
			return std::string("BUFFER_BLOCK");
		case PropertyType::STRING:
			return std::string("STRING");
		}
		return std::string("UNKNOWN VALUE TYPE");
	}

	int test_fluxions_simple_property() {
		using namespace Fluxions;

		/*
				mtl.Push("Kd", MakeVec4Property(1.0, 0.0, 0.0, 1.0));
				mtl.GetType("Kd");
				mtl.LoadVec4("Kd", myKd);
				mtl.StoreVec4("Kd", myKd);
				myKdPtr = mtl.GetVec4Ptr("Kd");
				// check if nullptr
				*/

				// Test PropertyTypeToString
		std::cout << PropertyTypeToString(PropertyType::VEC2) << " is VEC2\n";

		TSimpleProperty<int> myProperty(PropertyType::INT, 0);
		std::cout << "TSimpleProperty myProperty; --> GetTypeAsString() = " << myProperty.GetTypeAsString() << std::endl;
		std::cout << "TSimpleProperty myProperty; --> GetValue() = " << myProperty.GetValue() << std::endl;
		std::cout << "TSimpleProperty myProperty; --> GetValuePointer() = " << myProperty.GetValuePointer() << std::endl;
		myProperty.SetValue(5);
		std::cout << "TSimpleProperty myProperty; --> SetValue(5) = " << myProperty.GetValue() << std::endl;

		auto pmyProperty = MakeSimpleProperty<int>(PropertyType::INT, 5);
		std::cout << "pmyProperty = " << pmyProperty.get() << std::endl;
		std::cout << "pmyProperty->GetTypeAsString() = " << pmyProperty->GetTypeAsString() << std::endl;
		auto pmyIntProperty = CastAsPropertyType<int>(pmyProperty);
		std::cout << "pmyIntProperty->GetValue() = " << pmyIntProperty->GetValue() << std::endl;

		auto iptr1 = CastAsPropertyTypeValuePtr<int>(pmyProperty);
		// this is the same as:
		iptr1 = GetIntValuePtr(pmyProperty);
		std::cout << "CastAsPropertyTypeValue<int, PropertyType::INT>(pmyProperty) = " << iptr1 << std::endl;
		if (iptr1 != nullptr)
			std::cout << "value = " << *iptr1 << std::endl;

		auto iptr2 = CastAsPropertyTypeValuePtr<int>(pmyIntProperty);
		std::cout << "CastAsPropertyTypeValue<int, PropertyType::INT>(pmyIntProperty) = " << iptr2 << std::endl;
		if (iptr2 != nullptr)
			std::cout << "value = " << *iptr2 << std::endl;

		IntProperty intProperty(PropertyType::INT, 7);
		std::cout << "intProperty.value = " << intProperty.value << std::endl;

		UniqueIntPropertyPtr pIntProperty = MakeUniqueIntProperty(8);
		std::cout << "pIntProperty.value = " << pIntProperty->value << std::endl;

		UniqueFloatPropertyPtr pFloatProperty = MakeUniqueFloatProperty(3.14159f);
		std::cout << "pFloatProperty->value = " << pFloatProperty->value << std::endl;

		std::unique_ptr<SimpleProperty> pmyProperty2 = std::move(pFloatProperty);
		std::cout << "pFloatProperty->GetAsSimplePropertyPtr() = " << pmyProperty2.get() << std::endl;

		auto fptr1 = CastAsPropertyTypeValuePtr<float>(pmyProperty2);
		if (fptr1)
			std::cout << "fptr1 = " << *fptr1 << std::endl;

		//pFloatProperty = StaticUniqueSimplePropertyCast<float, PropertyType::FLOAT>(pmyProperty2);
		pFloatProperty = CastToUniqueFloatPropertyPtr(pmyProperty2);
		if (pFloatProperty)
			pFloatProperty->Value() = 5.0;
		if (fptr1)
			std::cout << "fptr1 = " << *fptr1 << std::endl;

		auto fptr2 = GetFloatValuePtr(pmyProperty2);
		if (fptr2)
			std::cout << "fptr2 = " << *fptr2 << std::endl;

		return 0;
	}

	// template specializations
#define DECLARE_SPECIALIZATION(ctype) template class TSimpleProperty<ctype>;

	DECLARE_SPECIALIZATION(int)
	DECLARE_SPECIALIZATION(unsigned)
	DECLARE_SPECIALIZATION(float)
	DECLARE_SPECIALIZATION(bool)
	DECLARE_SPECIALIZATION(double)
	DECLARE_SPECIALIZATION(std::string)
	DECLARE_SPECIALIZATION(Vector2f)
	DECLARE_SPECIALIZATION(Vector3f)
	DECLARE_SPECIALIZATION(Vector4f)
	DECLARE_SPECIALIZATION(Vector2i)
	DECLARE_SPECIALIZATION(Vector3i)
	DECLARE_SPECIALIZATION(Vector4i)
	DECLARE_SPECIALIZATION(Vector2ui)
	DECLARE_SPECIALIZATION(Vector3ui)
	DECLARE_SPECIALIZATION(Vector4ui)
	DECLARE_SPECIALIZATION(Matrix2f)
	DECLARE_SPECIALIZATION(Matrix3f)
	DECLARE_SPECIALIZATION(Matrix4f)
	//DECLARE_SPECIALIZATION(Matrix2x3f)
	//DECLARE_SPECIALIZATION(Matrix2x4f)
	//DECLARE_SPECIALIZATION(Matrix3x2f)
	//DECLARE_SPECIALIZATION(Matrix3x4f)
	//DECLARE_SPECIALIZATION(Matrix4x2f)
	//DECLARE_SPECIALIZATION(Matrix4x3f)

#undef DECLARE_SPECIALIZATION
} // namespace Fluxions
