#include <iostream>
#include <chrono>
#include <thread>
#include <enet/enetpp.hxx>

NetworkClient* client;

int main()
{
	client = new NetworkClient();

	if (client->GetInitCode())
	{
		std::cout << "An error occurred while trying to create an ENet object." << std::endl;
		exit(EXIT_FAILURE);
	}

	client->Create();

	if (!client->Good())
	{
		std::cout << "An error occurred while trying to create an ENet object." << std::endl;
		exit(EXIT_FAILURE);
	}

	ENetPeer *peer = client->Connect("127.0.0.1", 0xBEEF);

	if (peer == NULL)
	{
		std::cout << "No available peers for initiating an ENet connection." << std::endl;
		exit(EXIT_FAILURE);
	}

	while (true)
	{
		if (client->Pull())
		{
			ENetEvent event = client->Event();
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("connected to %x:%u.\n",
					event.peer->address.host,
					event.peer->address.port);
				/* Store any relevant client information here. */
				event.peer->data = "Client information";
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				printf("A packet of length %u containing %s was received from %s on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->data,
					event.channelID);
				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy(event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s disconnected.\n", event.peer->data);
				/* Reset the peer's client information. */
				event.peer->data = NULL;
				break;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	delete client;

	return 0;
}