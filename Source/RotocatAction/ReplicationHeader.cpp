#include "RobocatPCH.h"

void ReplicationHeader::Write(OutputMemoryBitStream& inStream)
{
	inStream.WriteBits(mReplicationAction, mLog(RA_MAX));
	inStream.Write(mNetworkId);

	if (mReplicationAction != RA_Destroy)
	{
		inStream.Write(mClassId);
	}
}

void ReplicationHeader::Read(InputMemoryBitStream& inStream)
{
	inStream.Read(mReplicationAction, mLog(RA_MAX));
	inStream.Read(mNetworkId);

	if (mReplicationAction != RA_Destroy)
	{
		inStream.Read(mClassId);
	}
}
