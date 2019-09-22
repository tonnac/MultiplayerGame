#pragma once

class GameObject;

#define CLASS_IDENTIFICATION(inCode, inClass)					\
enum {kClassId = inCode};										\
virtual uint32_t GetClassId() const { return kClassId; }		\
static GameObject* CreateInstance() { return new inClass(); }

class GameObject
{
public:
	CLASS_IDENTIFICATION('GOBJ', GameObject)

	static const DataType* GetDataType()
	{
		static DataType* sDatatype = nullptr;
		if (sDatatype == nullptr)
		{
			InitDataType(&sDatatype);
		}
		return sDatatype;
	}

private:
	static void InitDataType(DataType** inDataType)
	{
		*inDataType = new DataType
		(
			{
				MemberVariable("mName", EPrimitiveType::EPT_String, OffsetOf(GameObject, mName))
			},
			CreateInstance
		);
	}

protected:
	DirectX::XMFLOAT3 mLocation;
	std::string mName;
};

class Robocat : public GameObject
{
public:
	CLASS_IDENTIFICATION('RBCT', Robocat)

	static const DataType* GetDataType()
	{
		static DataType* sDatatype = nullptr;
		if (sDatatype == nullptr)
		{
			InitDataType(&sDatatype);
		}
		return sDatatype;
	}
private:
	static void InitDataType(DataType** inDataType)
	{
		*inDataType = new DataType
		(
			{
				MemberVariable("mName", EPrimitiveType::EPT_String, OffsetOf(Robocat, mName)),
				MemberVariable("mHealth", EPrimitiveType::EPT_Int, OffsetOf(Robocat, mHealth))
			},
			CreateInstance
		);
	}

protected:
	int32_t mHealth;
};

class RoboMouse : public GameObject
{
public:
	CLASS_IDENTIFICATION('RBMS', RoboMouse)

	static const DataType* GetDataType()
	{
		static DataType* sDatatype = nullptr;
		if (sDatatype == nullptr)
		{
			InitDataType(&sDatatype);
		}
		return sDatatype;
	}

private:
	static void InitDataType(DataType** inDataType)
	{
		*inDataType = new DataType
		(
			{
				MemberVariable("mName", EPrimitiveType::EPT_String, OffsetOf(RoboMouse, mName)),
				MemberVariable("mElel", EPrimitiveType::EPT_Float, OffsetOf(RoboMouse, elel)),
			},
			CreateInstance
		);
	}
private:
	float elel;
};