//
//	Random.hpp
//	Good Neighbours
//
//	Created by Jon Ander Eceiza 17/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#pragma once

 class ENGINE_API Random 
{
public:
	static void HeavyRandomize();
	static int RandomInt(const int min = 0, const int max = INT_MAX, bool heavy_rand = false);
	static bool RandomBool(bool heavy_rand = false);
	static float RandomFloat(const float min = 0.0f, const float max = FLT_MAX, bool heavy_rand = false);
	static Engine::Math::Vector2D RandomVector2D(const Engine::Math::Vector2D& min, const Engine::Math::Vector2D& max, bool heavy_rand = false);
	static Engine::Math::Vector3D RandomVector3D(const Engine::Math::Vector3D& min, const Engine::Math::Vector3D& max, bool heavy_rand = false);
	static Engine::Graphics::Color RandomColor(const Engine::Graphics::Color& min, const Engine::Graphics::Color& max, bool heavy_rand = false);

private:
	Random() {};
};

