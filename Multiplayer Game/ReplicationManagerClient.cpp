#include "Networks.h"
#include "ReplicationManagerClient.h"

// TODO(you): World state replication lab session

void ReplicationManagerClient::read(const InputMemoryStream& packet)
{
	while (packet.RemainingByteCount() > sizeof(uint32))
	{		
		uint32 networkId;
		ReplicationAction action;

		packet >> networkId;
		packet >> action;

		switch (action)
		{
		case ReplicationAction::Create:
		{
			if (networkId != -1)
			{
				GameObject* gameObject = Instantiate();

				if (gameObject)
				{
					uint32 gameObjectType;
				
					packet >> gameObject->position.x;
					packet >> gameObject->position.y;

					packet >> gameObject->angle;

					packet >> gameObject->size.x;
					packet >> gameObject->size.y;

					packet >> gameObjectType;

					gameObject->sprite = App->modRender->addSprite(gameObject);
					gameObject->sprite->order = 5;

					switch (gameObjectType)
					{
					case 0:
						gameObject->sprite->texture = App->modResources->spacecraft1;
						break;
					case 1:
						gameObject->sprite->texture = App->modResources->spacecraft2;
						break;
					case 2:
						gameObject->sprite->texture = App->modResources->spacecraft3;
						break;
					case 3:
						gameObject->sprite->texture = App->modResources->laser;
						break;
					}

					if (gameObjectType != 3)
					{
						gameObject->collider = App->modCollision->addCollider(ColliderType::Player, gameObject);
						gameObject->collider->isTrigger = true;
						gameObject->behaviour = new Spaceship;
						gameObject->behaviour->gameObject = gameObject;
					}
					else
					{
						gameObject->collider = App->modCollision->addCollider(ColliderType::Laser, gameObject);
						gameObject->collider->isTrigger = true;
						gameObject->sprite->order = 1;
					}

					App->modLinkingContext->registerNetworkGameObjectWithNetworkId(gameObject, networkId);

				}
			}
		}
			break;
		case ReplicationAction::Update:
		{
			if (networkId != -1)
			{
				vec2 position;
				float angle;
				vec2 size;
				uint8 gameObjectType;

				packet >> position.x;
				packet >> position.y;

				packet >> angle;

				packet >> size.x;
				packet >> size.y;

				packet >> gameObjectType;

				GameObject* gameObject = App->modLinkingContext->getNetworkGameObject(networkId);

				if (gameObject != nullptr)
				{
					gameObject->position = position;
					gameObject->angle = angle;
				}
				else
				{
					gameObject = Instantiate();

					gameObject->position = position;
					gameObject->angle = angle;
					gameObject->size = size;

					gameObject->sprite = App->modRender->addSprite(gameObject);
					gameObject->sprite->order = 5;

					switch (gameObjectType)
					{
					case 0:
						gameObject->sprite->texture = App->modResources->spacecraft1;
						break;
					case 1:
						gameObject->sprite->texture = App->modResources->spacecraft2;
						break;
					case 2:
						gameObject->sprite->texture = App->modResources->spacecraft3;
						break;
					case 3:
						gameObject->sprite->texture = App->modResources->laser;
						break;
					}
				}
				App->modLinkingContext->registerNetworkGameObjectWithNetworkId(gameObject, networkId);
			}
		}
			break;
		case ReplicationAction::Destroy:
		{
			GameObject* gameObject = App->modLinkingContext->getNetworkGameObject(networkId);

			if (gameObject)
			{
				App->modLinkingContext->unregisterNetworkGameObject(gameObject);
				Destroy(gameObject);
			}
		}
			break;
		}
	}
}