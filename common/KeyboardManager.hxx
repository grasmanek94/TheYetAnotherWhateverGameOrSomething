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
	void ResetKeyData();
	std::function<void(bool, unsigned char)> onKeyStateChange;
public:
	KeyboardManager(const std::function<void(bool, unsigned char)>& onKeyStateChange = nullptr);
	void CheckKeys();
	bool Pressed(unsigned char key);
	bool Released(unsigned char key);
	bool Up(unsigned char key);
	bool Down(unsigned char key);
};