#include <iostream>
#include <chrono>
#include <thread>
#include <enet/enetpp.hxx>

NetworkServer* server;

int main()
{
	server = new NetworkServer();

	if (	server->GetInitCode() 
		||	!server->Create(1024) 
		||	!server->Good())
	{
		std::cout << "An error occurred while trying to create an ENet object." << std::endl;
		exit(EXIT_FAILURE);
	}

	while (true)
	{
		if (server->Pull())
		{
			ENetEvent event = server->Event();
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("A new client connected from %x:%u.\n",
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

	delete server;

	return 0;
}

/*
#include <array>
#include <functional>
class KeyboardManager
{
private:
	struct KeyData
	{
		bool PRESSED;
		bool RELEASED;
		bool DOWN;
	};

	std::array<KeyData, 0x100> Keys;

	void ResetKeyData()
	{
		Keys.fill(KeyData{ false, false, false });
	}

	std::function<void(bool, unsigned char)> onKeyStateChange;
public:

	KeyboardManager(
		const std::function<void(bool, unsigned char)>& onKeyStateChange = nullptr)
		: onKeyStateChange(onKeyStateChange)
	{
		ResetKeyData();
	}

	void CheckKeys()
	{
		if (GetConsoleWindow() == GetForegroundWindow())
		{
			for (int key = 0; key < 256; ++key)
			{
				bool pressed = GetAsyncKeyState(key);
				KeyData *data = &Keys[key];
				if (pressed)
				{
					if (!data->DOWN)
					{
						data->DOWN = true;
						data->PRESSED = true;
						data->RELEASED = false;

						if (onKeyStateChange != nullptr)
						{
							onKeyStateChange(true, key);
						}
					}
					else if (data->PRESSED)
					{
						data->PRESSED = false;
					}
				}
				else
				{
					if (data->DOWN)
					{
						data->DOWN = false;
						data->PRESSED = false;
						data->RELEASED = true;

						if (onKeyStateChange != nullptr)
						{
							onKeyStateChange(false, key);
						}
					}
					else if (data->RELEASED)
					{
						data->RELEASED = false;
					}
				}
			}
		}
	}

	bool Pressed(unsigned char key)
	{
		return Keys[key].PRESSED;
	}

	bool Released(unsigned char key)
	{
		return Keys[key].RELEASED;
	}

	bool Up(unsigned char key)
	{
		return !Keys[key].DOWN;
	}

	bool Down(unsigned char key)
	{
		return Keys[key].DOWN;
	}
};

void OnKeyStateChange(bool pressed, unsigned char key)
{
	char dest[16];
	std::string toSend(std::string(pressed ? "DOWN" : "UP") + ": 0x" + itoa(key, dest, 16));
	server->Broadcast(toSend.c_str(), toSend.length() + 1);
}

KeyboardManager *kbmgr = new KeyboardManager(OnKeyStateChange);

//then in while loop:
kbmgr->CheckKeys();
*/