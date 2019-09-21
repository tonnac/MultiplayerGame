#include "ObjectCreationRegistry.h"
#include "GameObj.h"

ObjectCreationRegistry& ObjectCreationRegistry::Get()
{
	static ObjectCreationRegistry sInstance;
	return sInstance;
}

void ObjectCreationRegistry::RegisterObjectCreation()
{
	ObjectCreationRegistry::Get().RegisterCreationFunction<GameObject>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<Robocat>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<RoboMouse>();

	mDataTypeMap[GameObject::kClassId] = GameObject::GetDataType();
	mDataTypeMap[Robocat::kClassId] = Robocat::GetDataType();
	mDataTypeMap[RoboMouse::kClassId] = RoboMouse::GetDataType();
}

GameObject* ObjectCreationRegistry::CreateGameObject(uint32_t inClassId)
{
	GameObjectCreationFunc creationFunc = mObjectCreationFunc[inClassId];
	GameObject* gameObject = creationFunc();

	//GameObject* gameObj = mDataTypeMap[inClassId]->GetCreationFunc()();

	return gameObject;
}
