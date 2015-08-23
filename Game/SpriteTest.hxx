#pragma once

#include "pch.hxx"

struct TextureData
{
	std::wstring texture_name;
	ID3D11ShaderResourceView* texture;
	XMFLOAT2 position;
	FXMVECTOR color;

	TextureData(const std::wstring& name, const XMFLOAT2& position, const FXMVECTOR& color)
		: texture_name(name), position(position), color(color)
	{

	}
};

class SpriteTest
{
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::vector<TextureData> textures;
	ID3D11Device* device;
public:
	SpriteTest(ID3D11Device* device, ID3D11DeviceContext* context);
	~SpriteTest();
	void Draw();
	void AddTexture(TextureData& texture);
};

