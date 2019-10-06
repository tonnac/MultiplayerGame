#pragma once


class ActorComponent : public Object
{



protected:
	Actor* mOwner = nullptr;
	ActorComponent* mParentComponent = nullptr;
	vector<ActorComponent*> mChildComponent;
};