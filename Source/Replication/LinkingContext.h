#pragma once
#include "MemoryPCH.h"

enum EPacketType
{
	PT_Hello,
	PT_ReplicationData,
	PT_Disconnect,
	PT_MAX
};

template<int tValue, int tBits>
struct GetRequiredBitsHelper
{
	enum { Value = GetRequiredBitsHelper <(tValue >> 1), tBits + 1 > ::Value };
};

template<int tBits>
struct GetRequiredBitsHelper<0, tBits>
{
	enum { Value = tBits };
};

template <int tValue>
struct GetRequiredBits
{
	enum { Value = GetRequiredBitsHelper<tValue, 0>::Value };
};

class GameObject;

class LinkingContext
{
public:
	LinkingContext() {}

	uint32_t GetNetworkId(const GameObject* inGameObject, bool inShouldCreateIfNotFound)
	{
		unordered_map<const GameObject*, uint32_t>::const_iterator it = mGameObjectToNetworkIdMap.find(inGameObject);

		if (it != mGameObjectToNetworkIdMap.end())
		{
			return it->second;
		}
		else if (inShouldCreateIfNotFound)
		{
			uint32_t newNetworkId = ++mNextNetworkId;
			AddGameObject(const_cast<GameObject*>(inGameObject), newNetworkId);
			return newNetworkId;
		}
		else
		{
			return 0;
		}
	}

	void AddGameObject(GameObject* inGameObject, uint32_t inNetworkId)
	{
		mNetworkIdToGameObjectMap[inNetworkId] = inGameObject;
		mGameObjectToNetworkIdMap[inGameObject] = inNetworkId;
	}

	void RemoveGameObject(GameObject* inGameObject)
	{
		uint32_t networkId = mGameObjectToNetworkIdMap[inGameObject];
		mGameObjectToNetworkIdMap.erase(inGameObject);
		mNetworkIdToGameObjectMap.erase(networkId);
	}

	GameObject* GetGameObject(uint32_t inNetworkId)
	{
		unordered_map<uint32_t, GameObject*>::const_iterator it = mNetworkIdToGameObjectMap.find(inNetworkId);
		if (it != mNetworkIdToGameObjectMap.cend())
		{
			return it->second;
		}
		else
		{
			return nullptr;
		}
	}


private:
	uint32_t mNextNetworkId = 0;
	unordered_map<uint32_t, GameObject*> mNetworkIdToGameObjectMap;
	unordered_map<const GameObject*, uint32_t> mGameObjectToNetworkIdMap;
};