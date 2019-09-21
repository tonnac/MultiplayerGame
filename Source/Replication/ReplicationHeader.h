#pragma once
#include "MemoryPCH.h"

enum ReplicationAction
{
	RA_Create,
	RA_Update,
	RA_Destroy,
	RA_RPC,
	RA_MAX
};

class ReplicationHeader
{
public:
	ReplicationHeader() :
		mNetworkId(0), mClassId(0), mReplicationAction(RA_Create)
	{};
	ReplicationHeader(ReplicationAction inRA, uint32_t inNetworkId, uint32_t inClassId = 0) :
		mReplicationAction(inRA), mNetworkId(inNetworkId), mClassId(inClassId)
	{}

	void Write(OutputMemoryBitStream& inStream);
	void Read(InputMemoryBitStream& inStream);


	ReplicationAction mReplicationAction;
	uint32_t mNetworkId;
	uint32_t mClassId;
};