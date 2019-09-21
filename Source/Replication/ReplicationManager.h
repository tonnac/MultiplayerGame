#pragma once
#include "MemoryPCH.h"
#include "LinkingContext.h"
#include "ReplicationHeader.h"

class ReplicationManager
{
public:
	void ReplicateWorldState(OutputMemoryBitStream& inStream,
		const std::vector<GameObject*>& inAllObjects);

	void ReceiveWorld(InputMemoryBitStream& inStream);

	void ReplicateCreate(OutputMemoryBitStream& inStream, GameObject* inGameObject);
	void ReplicateUpdate(OutputMemoryBitStream& inStream, GameObject* inGameObject);
	void ReplicateDestroy(OutputMemoryBitStream& inStream, GameObject* inGameObject);

	void ProcessReplicationAction(InputMemoryBitStream& inStream);

private:
	void ReplicateIntoStream(OutputMemoryBitStream& inStream,
		GameObject* inGameObject);

	GameObject* ReceiveReplicatedObject(InputMemoryBitStream& inStream);

	std::unordered_set<GameObject*> mObjectReplicatedToMe;

	LinkingContext* mLinkingContext;
};