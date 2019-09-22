#pragma once

using RPCUnwrapFunc = std::function<void(InputMemoryBitStream& inStream)>;

class RPCManager
{
public:
	void RegisterUnwrapFunction(uint32_t inName, RPCUnwrapFunc inFunc)
	{
		assert(mNameToRPCTable.find(inName) == mNameToRPCTable.end());
		mNameToRPCTable[inName] = inFunc;
	}

	void ProcessRPC(InputMemoryBitStream& inStream)
	{
		uint32_t name;
		inStream.Read(name);
		mNameToRPCTable[name](inStream);
	}

	static void UnwrapPlaySound(InputMemoryBitStream& inStream)
	{
		string soundName;
		DirectX::XMFLOAT3 location;
		float volume;

		inStream.Read(soundName);
		inStream.Read(location);
		inStream.Read(volume);
		PlaySound(soundName, location, volume);
	}

	static void PlaySound(const string& inSoundName, const DirectX::XMFLOAT3& inLocation, float inVolume)
	{

	}

	void RegisterRPCs(RPCManager* inRPCManager)
	{
		inRPCManager->RegisterUnwrapFunction('PSND', UnwrapPlaySound);
	}

	void PlaySoundRPC(OutputMemoryBitStream& inStream,
		const string& inName,
		const DirectX::XMFLOAT3& inLocation, float inVolume)
	{
		//ReplicationHeader rh(RA_RPC);
		//rh.Write(inStream);
		//inStream.Write(inName);
		//inStream.Write(inLocation);
		//inStream.Write(inVolume);
	}

private:
	std::unordered_map<uint32_t, RPCUnwrapFunc> mNameToRPCTable;
};