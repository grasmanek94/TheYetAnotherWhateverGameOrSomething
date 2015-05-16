#include <iostream>
#include <chrono>
#include <thread>
#include <enet/enetpp.hxx>
#include <KeyboardManager.hxx>

NetworkClient* client;


void OnKeyStateChange(bool pressed, unsigned char key)
{
	char dest[16];
	std::string toSend(std::string(pressed ? "DOWN" : "UP") + ": 0x" + itoa(key, dest, 16));
	client->Send(toSend.c_str(), toSend.length()+1);
}

KeyboardManager kbmgr(OnKeyStateChange);

int main()
{
	client = new NetworkClient();

	if (	client->GetInitCode() 
		||	!client->Create()
		||	!client->Good()
		||	!client->Connect("127.0.0.1", 0xBEEF))
	{
		std::cout << "An error occurred while trying to create an ENet object." << std::endl;
		exit(EXIT_FAILURE);
	}

	while (true)
	{
		kbmgr.CheckKeys();
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
				printf("%s\n", event.packet->data);
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