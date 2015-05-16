#include <Windows.h>
#include "KeyboardManager.hxx"

void KeyboardManager::ResetKeyData()
{
	Keys.fill(KeyData{ false, false, false });
}

KeyboardManager::KeyboardManager(
	const std::function<void(bool, unsigned char)>& onKeyStateChange)
	: onKeyStateChange(onKeyStateChange)
{
	ResetKeyData();
}

void KeyboardManager::CheckKeys()
{
	if (GetConsoleWindow() == GetForegroundWindow())
	{
		for (int key = 0, pressed = GetAsyncKeyState(key); key < 256; pressed = GetAsyncKeyState(++key))
		{
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

bool KeyboardManager::Pressed(unsigned char key)
{
	return Keys[key].PRESSED;
}

bool KeyboardManager::Released(unsigned char key)
{
	return Keys[key].RELEASED;
}

bool KeyboardManager::Up(unsigned char key)
{
	return !Keys[key].DOWN;
}

bool KeyboardManager::Down(unsigned char key)
{
	return Keys[key].DOWN;
}