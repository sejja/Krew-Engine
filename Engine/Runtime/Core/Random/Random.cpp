//
//	Random.cpp
//	Good Neighbours
//
//	Created by Jon Ander Eceiza 17/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>
using namespace Engine::Math;
using namespace Engine::Graphics;


// ------------------------------------------------------------------------
/*! Random Int
*
*   Srand with heavier operations than just fetching UNIX time
*/ // ---------------------------------------------------------------------
void Random::HeavyRandomize()
{
	//gfet the 
	const auto time_seed = static_cast<size_t>(std::time(0));
	const auto clock_seed = static_cast<size_t>(std::clock());
	const size_t pid_seed =
		std::hash<std::thread::id>()(std::this_thread::get_id());

	std::seed_seq seed_value{ time_seed, clock_seed, pid_seed };

	// E.g seeding an engine with the above seed.
	std::mt19937 random;
	random.seed(seed_value);
	srand(random());
}

// ------------------------------------------------------------------------
/*! Random Int
*
*   Name says everything
*/ // ---------------------------------------------------------------------
int Random::RandomInt(const int min, const int max, bool heavy_rand)
{
	//seed the randomizer
	if (heavy_rand)
		HeavyRandomize();
	else
		std::srand(std::time(nullptr));
	//confine the random number between the minimum and maximum
	return min + (std::rand() % (max - min + 1));
}
// ------------------------------------------------------------------------
/*! Random Bool
*
*   Name says everything
*/ // ---------------------------------------------------------------------
bool Random::RandomBool(bool heavy_rand)
{
	return RandomInt(0, 1) == 1 ? true : false;
}
// ------------------------------------------------------------------------
/*! Random Float
*
*   Name says everything
*/ // ---------------------------------------------------------------------
float Random::RandomFloat(const float min, const float max, bool heavy_rand)
{
	//seed the randomizer
	if (heavy_rand)
		HeavyRandomize();
	//confine the random number between the minimum and maximum
	return min + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX / (max - min));
}
// ------------------------------------------------------------------------
/*! Random Vector 2D
*
*   Name says everything
*/ // ---------------------------------------------------------------------
Vector2D Random::RandomVector2D(const Vector2D& min, const Vector2D& max, bool heavy_rand)
{
	return Vector2D(RandomFloat(min.x, max.x), RandomFloat(min.y, max.y));
}
// ------------------------------------------------------------------------
/*! Random Vector 3D
*
*   Name says everything
*/ // ---------------------------------------------------------------------
Vector3D Random::RandomVector3D(const Vector3D& min, const Vector3D& max, bool heavy_rand)
{
	return Vector3D(RandomFloat(min.x, max.x), RandomFloat(min.y, max.y), RandomFloat(min.z, max.z));
}
// ------------------------------------------------------------------------
/*! Random Color
*
*   Name says everything
*/ // ---------------------------------------------------------------------
Engine::Graphics::Color Random::RandomColor(const Engine::Graphics::Color& min, const Engine::Graphics::Color& max, bool heavy_rand)
{
	return Engine::Graphics::Color(RandomFloat(min.r, max.r), RandomFloat(min.g, max.g), RandomFloat(min.b, max.b), RandomFloat(min.a, max.a));
}


