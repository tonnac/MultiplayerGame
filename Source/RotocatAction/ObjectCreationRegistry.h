#pragma once

class DataType;

class ObjectCreationRegistry
{
public:
	static ObjectCreationRegistry& Get();

	void RegisterObjectCreation();

	template <class T>
	void RegisterCreationFunction()
	{
		assert(mObjectCreationFunc.find(T::kClassId) == mObjectCreationFunc.end());
		mObjectCreationFunc[T::kClassId] = T::CreateInstance;
	}

	GameObject* CreateGameObject(uint32_t inClassId);

private:
	ObjectCreationRegistry() {}
	std::unordered_map<uint32_t, GameObjectCreationFunc> mObjectCreationFunc;
	std::unordered_map<uint32_t, const DataType*> mDataTypeMap;
};