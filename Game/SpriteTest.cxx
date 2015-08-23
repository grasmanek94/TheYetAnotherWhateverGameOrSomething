#include "pch.hxx"

using namespace DirectX;

SpriteTest::SpriteTest(ID3D11Device* device, ID3D11DeviceContext* context)
	: spriteBatch((std::make_unique<SpriteBatch>(context))), device(device)
{

}

SpriteTest::~SpriteTest()
{

}

void SpriteTest::Draw()
{
	spriteBatch->Begin();
	for (auto& entry : textures)
	{
		spriteBatch->Draw(entry.texture, entry.position, entry.color);
	}
	spriteBatch->End();
}

void SpriteTest::AddTexture(TextureData& texture)
{
	CreateDDSTextureFromFile(device,
		texture.texture_name.c_str(),
		NULL,
		&texture.texture,
		NULL);

	textures.push_back(texture);
}