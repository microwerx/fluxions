// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/KASL/GLUT Extensions
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
#include "stdafx.h"
#include <fluxions_simple_property.hpp>

namespace Fluxions
{
SimpleAssociativePropertyList::SimpleAssociativePropertyList()
{
}

SimpleAssociativePropertyList::~SimpleAssociativePropertyList()
{
}

void SimpleAssociativePropertyList::Clear()
{
	nameIndex.clear();
	properties.clear();
}

void SimpleAssociativePropertyList::Add(const string &name, SharedSimplePropertyPtr &valuePtr)
{
	// check if index already exists...
	int usedIndex = GetIndexOf(name);
	if (usedIndex < 0)
	{
		// nope, add new element.
		properties.push_back(pair<string, SharedSimplePropertyPtr>(name, valuePtr));
		usedIndex = (int)properties.size() - 1;
		nameIndex[name] = usedIndex;
	}
	if (usedIndex >= 0)
	{
		properties[usedIndex].second = valuePtr;
	}
}

void SimpleAssociativePropertyList::Remove(const string &name)
{
	int usedIndex = GetIndexOf(name);
	if (usedIndex >= 0)
	{
		properties[usedIndex].first.clear();
		properties[usedIndex].second.reset();
		// Names are never removed from the map to preserve the array order.
	}
}

bool SimpleAssociativePropertyList::IsElement(const string &name)
{
	// If it's in the index and it's not a reset shared_ptr, return true.
	int usedIndex = GetIndexOf(name);
	if (usedIndex >= 0)
	{
		if (properties[usedIndex].second.use_count() > 0)
			return true;
	}
	return false;
}

SharedSimplePropertyPtr &SimpleAssociativePropertyList::Get(const string &name)
{
	int usedIndex = GetIndexOf(name);
	if (usedIndex >= 0)
	{
		return properties[usedIndex].second;
	}
	return defaultItem;
}

int SimpleAssociativePropertyList::GetIndexOf(const string &name)
{
	auto it = nameIndex.find(name);
	if (it != nameIndex.end())
	{
		return it->second;
	}
	return -1;
}

const string &SimpleAssociativePropertyList::GetNameAtIndex(int i)
{
	if (i < 0 || i >= (int)properties.size())
	{
		return properties[i].first;
	}
	return emptyString;
}

SharedSimplePropertyPtr &SimpleAssociativePropertyList::GetAtIndex(int i)
{
	if (i < 0 || i >= (int)properties.size())
	{
		return properties[i].second;
	}
	return defaultItem;
}

SharedSimplePropertyPtr &SimpleAssociativePropertyList::operator[](const string &name)
{
	int index = GetIndexOf(name);
	if (index < 0)
	{
		properties.push_back(pair<string, SharedSimplePropertyPtr>(name, SharedSimplePropertyPtr()));
		index = (int)properties.size() - 1;
		nameIndex[name] = index;
	}

	return properties[index].second;
}

const string PropertyTypeToString(PropertyType propertyType)
{
	switch (propertyType)
	{
	case PropertyType::NOTHING:
		return string("NOTHING");
	case PropertyType::STRUCT:
		return string("STRUCT");
	case PropertyType::ARRAY:
		return string("ARRAY");
	case PropertyType::BOOL:
		return string("BOOL");
	case PropertyType::INT:
		return string("INT");
	case PropertyType::FLOAT:
		return string("FLOAT");
	case PropertyType::DOUBLE:
		return string("DOUBLE");
	case PropertyType::VEC2:
		return string("VEC2");
	case PropertyType::VEC3:
		return string("VEC3");
	case PropertyType::VEC4:
		return string("VEC4");
	case PropertyType::BVEC2:
		return string("BVEC2");
	case PropertyType::BVEC3:
		return string("BVEC3");
	case PropertyType::BVEC4:
		return string("BVEC4");
	case PropertyType::IVEC2:
		return string("IVEC2");
	case PropertyType::IVEC3:
		return string("IVEC3");
	case PropertyType::IVEC4:
		return string("IVEC4");
	case PropertyType::MAT2:
		return string("MAT2");
	case PropertyType::MAT3:
		return string("MAT3");
	case PropertyType::MAT4:
		return string("MAT4");
	case PropertyType::SAMPLER2D:
		return string("SAMPLER2D");
	case PropertyType::SAMPLERCUBE:
		return string("SAMPLERCUBE");
	case PropertyType::UINT:
		return string("UINT");
	case PropertyType::UVEC2:
		return string("UVEC2");
	case PropertyType::UVEC3:
		return string("UVEC3");
	case PropertyType::UVEC4:
		return string("UVEC4");
	case PropertyType::MAT2X2:
		return string("MAT2X2");
	case PropertyType::MAT2X3:
		return string("MAT2X3");
	case PropertyType::MAT2X4:
		return string("MAT2X4");
	case PropertyType::MAT3X2:
		return string("MAT3X2");
	case PropertyType::MAT3X3:
		return string("MAT3X3");
	case PropertyType::MAT3X4:
		return string("MAT3X4");
	case PropertyType::MAT4X2:
		return string("MAT4X2");
	case PropertyType::MAT4X3:
		return string("MAT4X3");
	case PropertyType::MAT4X4:
		return string("MAT4X4");
	case PropertyType::SAMPLER3D:
		return string("SAMPLER3D");
	case PropertyType::SAMPLERCUBESHADOW:
		return string("SAMPLERCUBESHADOW");
	case PropertyType::SAMPLER2DSHADOW:
		return string("SAMPLER2DSHADOW");
	case PropertyType::SAMPLER2DARRAY:
		return string("SAMPLER2DARRAY");
	case PropertyType::SAMPLER2DARRAYSHADOW:
		return string("SAMPLER2DARRAYSHADOW");
	case PropertyType::ISAMPLER2D:
		return string("ISAMPLER2D");
	case PropertyType::ISAMPLER3D:
		return string("ISAMPLER3D");
	case PropertyType::ISAMPLERCUBE:
		return string("ISAMPLERCUBE");
	case PropertyType::ISAMPLER2DARRAY:
		return string("ISAMPLER2DARRAY");
	case PropertyType::USAMPLER2D:
		return string("USAMPLER2D");
	case PropertyType::USAMPLER3D:
		return string("USAMPLER3D");
	case PropertyType::USAMPLERCUBE:
		return string("USAMPLERCUBE");
	case PropertyType::USAMPLER2DARRAY:
		return string("USAMPLER2DARRAY");
	case PropertyType::ISAMPLER2DMS:
		return string("ISAMPLER2DMS");
	case PropertyType::IIMAGE2D:
		return string("IIMAGE2D");
	case PropertyType::IIMAGE3D:
		return string("IIMAGE3D");
	case PropertyType::IIMAGECUBE:
		return string("IIMAGECUBE");
	case PropertyType::IIMAGE2DARRAY:
		return string("IIMAGE2DARRAY");
	case PropertyType::ATOMIC_UINT:
		return string("ATOMIC_UINT");
	case PropertyType::USAMPLER2DMS:
		return string("USAMPLER2DMS");
	case PropertyType::UIMAGE2D:
		return string("UIMAGE2D");
	case PropertyType::UIMAGE3D:
		return string("UIMAGE3D");
	case PropertyType::UIMAGECUBE:
		return string("UIMAGECUBE");
	case PropertyType::UIMAGE2DARRAY:
		return string("UIMAGE2DARRAY");
	case PropertyType::SAMPLER2DMS:
		return string("SAMPLER2DMS");
	case PropertyType::IMAGE2D:
		return string("IMAGE2D");
	case PropertyType::IMAGE3D:
		return string("IMAGE3D");
	case PropertyType::IMAGECUBE:
		return string("IMAGECUBE");
	case PropertyType::IMAGE2DARRAY:
		return string("IMAGE2DARRAY");
	case PropertyType::MAXVALUETYPE:
		return string("MAXVALUETYPE");
	case PropertyType::UNIFORM_BLOCK:
		return string("UNIFORM_BLOCK");
	case PropertyType::BUFFER_BLOCK:
		return string("BUFFER_BLOCK");
	case PropertyType::STRING:
		return string("STRING");
	}
	return string("UNKNOWN VALUE TYPE");
}

int test_fluxions_simple_property(int argc, char **argv)
{
	using namespace std;
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
	cout << PropertyTypeToString(PropertyType::VEC2) << " is VEC2\n";

	TSimpleProperty<int> myProperty(PropertyType::INT, 0);
	cout << "TSimpleProperty myProperty; --> GetTypeAsString() = " << myProperty.GetTypeAsString() << endl;
	cout << "TSimpleProperty myProperty; --> GetValue() = " << myProperty.GetValue() << endl;
	cout << "TSimpleProperty myProperty; --> GetValuePointer() = " << myProperty.GetValuePointer() << endl;
	myProperty.SetValue(5);
	cout << "TSimpleProperty myProperty; --> SetValue(5) = " << myProperty.GetValue() << endl;

	auto pmyProperty = MakeSimpleProperty<int>(PropertyType::INT, 5);
	cout << "pmyProperty = " << pmyProperty.get() << endl;
	cout << "pmyProperty->GetTypeAsString() = " << pmyProperty->GetTypeAsString() << endl;
	auto pmyIntProperty = CastAsPropertyType<int>(pmyProperty);
	cout << "pmyIntProperty->GetValue() = " << pmyIntProperty->GetValue() << endl;

	auto iptr1 = CastAsPropertyTypeValuePtr<int>(pmyProperty);
	// this is the same as:
	iptr1 = GetIntValuePtr(pmyProperty);
	cout << "CastAsPropertyTypeValue<int, PropertyType::INT>(pmyProperty) = " << iptr1 << endl;
	if (iptr1 != nullptr)
		cout << "value = " << *iptr1 << endl;

	auto iptr2 = CastAsPropertyTypeValuePtr<int>(pmyIntProperty);
	cout << "CastAsPropertyTypeValue<int, PropertyType::INT>(pmyIntProperty) = " << iptr2 << endl;
	if (iptr2 != nullptr)
		cout << "value = " << *iptr2 << endl;

	IntProperty intProperty(PropertyType::INT, 7);
	cout << "intProperty.value = " << intProperty.value << endl;

	UniqueIntPropertyPtr pIntProperty = MakeUniqueIntProperty(8);
	cout << "pIntProperty.value = " << pIntProperty->value << endl;

	UniqueFloatPropertyPtr pFloatProperty = MakeUniqueFloatProperty(3.14159f);
	cout << "pFloatProperty->value = " << pFloatProperty->value << endl;

	unique_ptr<SimpleProperty> pmyProperty2 = std::move(pFloatProperty);
	cout << "pFloatProperty->GetAsSimplePropertyPtr() = " << pmyProperty2.get() << endl;

	auto fptr1 = CastAsPropertyTypeValuePtr<float>(pmyProperty2);
	if (fptr1)
		cout << "fptr1 = " << *fptr1 << endl;

	//pFloatProperty = StaticUniqueSimplePropertyCast<float, PropertyType::FLOAT>(pmyProperty2);
	pFloatProperty = CastToUniqueFloatPropertyPtr(pmyProperty2);
	if (pFloatProperty)
		pFloatProperty->Value() = 5.0;
	if (fptr1)
		cout << "fptr1 = " << *fptr1 << endl;

	auto fptr2 = GetFloatValuePtr(pmyProperty2);
	if (fptr2)
		cout << "fptr2 = " << *fptr2 << endl;

	return 0;
}

// template specializations
#define DECLARE_SPECIALIZATION(ctype) template class TSimpleProperty<ctype>;

DECLARE_SPECIALIZATION(int)
DECLARE_SPECIALIZATION(unsigned)
DECLARE_SPECIALIZATION(float)
DECLARE_SPECIALIZATION(bool)
DECLARE_SPECIALIZATION(double)
DECLARE_SPECIALIZATION(string)
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
