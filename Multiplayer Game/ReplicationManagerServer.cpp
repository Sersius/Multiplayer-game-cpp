#include "Networks.h"
#include "ReplicationManagerServer.h"

// TODO(you): World state replication lab session

void ReplicationManagerServer::create(uint32 networkId)
{
	ReplicationCommand replicate;

	replicate.action = ReplicationAction::Create;
	replicate.networkId = networkId;

	bool created = false;

	for (int i = 0; i < replications.size(); i++)
	{
		if (replications[i].action == replicate.action && replications[i].networkId == replicate.networkId)
			created = true;
	}

	if (created == false)
		replications.push_back(replicate);
}

void ReplicationManagerServer::update(uint32 networkId) 
{
	ReplicationCommand replicate;

	replicate.action = ReplicationAction::Update;
	replicate.networkId = networkId;

	bool created = false;

	for (int i = 0; i < replications.size(); i++)
	{
		if (replications[i].action == replicate.action && replications[i].networkId == replicate.networkId)
			created = true;
	}

	if (created == false)
		replications.push_back(replicate);
}

void ReplicationManagerServer::destroy(uint32 networkId) 
{
	ReplicationCommand replicate;

	replicate.action = ReplicationAction::Destroy;
	replicate.networkId = networkId;

	bool created = false;

	for (int i = 0; i < replications.size(); i++)
	{
		if (replications[i].action == replicate.action && replications[i].networkId == replicate.networkId)
			created = true;
	}

	if (created == false)
		replications.push_back(replicate);
}

void ReplicationManagerServer::write(OutputMemoryStream &packet)
{
	for (int i = 0; i < replications.size(); i++)
	{
		ReplicationCommand replicate = replications[i];

		packet << replicate.action;
		packet << replicate.networkId;

		if (replicate.action == ReplicationAction::Create || replicate.action == ReplicationAction::Update)
		{
			GameObject* gameObject = App->modLinkingContext->getNetworkGameObject(replicate.networkId);

			if (gameObject != nullptr)
			{
				packet << gameObject->position.x;
				packet << gameObject->position.y;

				packet << gameObject->angle;

				packet << gameObject->size.x;
				packet << gameObject->size.y;

				if (gameObject->sprite->texture == App->modResources->spacecraft1)
					packet << (uint32)0;
				else if (gameObject->sprite->texture == App->modResources->spacecraft2)
					packet << (uint32)1;
				else if (gameObject->sprite->texture == App->modResources->spacecraft3)
					packet << (uint32)2;
				else
					packet << (uint32)3;
			}
		}
	}

	replications.clear();

}