#include "ReplicationManager.h"
#include "GameObj.h"
#include "ObjectCreationRegistry.h"

void ReplicationManager::ReplicateWorldState(OutputMemoryBitStream& inStream, const std::vector<GameObject*>& inAllObjects)
{
	inStream.WriteBits(PT_ReplicationData, mLog(PT_MAX));

	for (GameObject* go : inAllObjects)
	{
		ReplicateIntoStream(inStream, go);
	}
}

void ReplicationManager::ReceiveWorld(InputMemoryBitStream& inStream)
{
	std::unordered_set<GameObject*> receivedObjects;
	while (inStream.GetRemainingBitCount() > 0)
	{
		GameObject* receivedGameObject = ReceiveReplicatedObject(inStream);
		receivedObjects.insert(receivedGameObject);
	}

	for (GameObject* go : mObjectReplicatedToMe)
	{
		if (receivedObjects.find(go) == mObjectReplicatedToMe.end())
		{
			mLinkingContext->RemoveGameObject(go);
			//go->Destroy();
		}
	}
	mObjectReplicatedToMe = receivedObjects;
}

void ReplicationManager::ReplicateCreate(OutputMemoryBitStream& inStream, GameObject* inGameObject)
{
	ReplicationHeader rh(RA_Create, mLinkingContext->GetNetworkId(inGameObject, true),
		inGameObject->GetClassId());

	rh.Write(inStream);
//	inGameObject->Write(inStream);
}

void ReplicationManager::ReplicateUpdate(OutputMemoryBitStream& inStream, GameObject* inGameObject)
{
	ReplicationHeader rh(RA_Update, mLinkingContext->GetNetworkId(inGameObject, false),
		inGameObject->GetClassId());

	rh.Write(inStream);
//	inGameObject->Write(inStream);
}

void ReplicationManager::ReplicateDestroy(OutputMemoryBitStream& inStream, GameObject* inGameObject)
{
	ReplicationHeader rh(RA_Destroy, mLinkingContext->GetNetworkId(inGameObject, false));

	rh.Write(inStream);
}

void ReplicationManager::ProcessReplicationAction(InputMemoryBitStream& inStream)
{
	ReplicationHeader rh;
	rh.Read(inStream);
	switch (rh.mReplicationAction)
	{
	case RA_Create:
	{
		GameObject* go = ObjectCreationRegistry::Get().CreateGameObject(rh.mClassId);
		mLinkingContext->AddGameObject(go, rh.mNetworkId);
	}break;
	case RA_Update:
	{
		GameObject* go = mLinkingContext->GetGameObject(rh.mNetworkId);
		if (go)
		{
			//go->Read(inStream);
		}
		else
		{
			uint32_t classId = rh.mClassId;
			go = ObjectCreationRegistry::Get().CreateGameObject(classId);
			//go->Read(inStream);
			delete go;
		}
	}break;
	case RA_Destroy:
	{
		GameObject* go = mLinkingContext->GetGameObject(rh.mNetworkId);
		mLinkingContext->RemoveGameObject(go);
		//go->Destroy();
	}break;
	default:
		break;
	}
}

void ReplicationManager::ReplicateIntoStream(OutputMemoryBitStream& inStream, GameObject* inGameObject)
{
	inStream.Write(mLinkingContext->GetNetworkId(inGameObject, true));
	inStream.Write(inGameObject->GetClassId());

//	inGameObject->Write(inStream);
}

GameObject* ReplicationManager::ReceiveReplicatedObject(InputMemoryBitStream& inStream)
{
	uint32_t networkId;
	uint32_t classId;
	inStream.Read(networkId);
	inStream.Read(classId);

	GameObject* go = mLinkingContext->GetGameObject(networkId);
	if (!go)
	{
		go = ObjectCreationRegistry::Get().CreateGameObject(classId);
		mLinkingContext->AddGameObject(go, networkId);
	}

//	go->Read(inStream);

	return go;
}
