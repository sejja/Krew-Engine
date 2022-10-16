#ifndef GCollisions
#define GCollisions

//
//  Collisions.h
//  Astro Engine
//
//  Created by Asier Azpiri on 17/9/21.
//  Copyright � 2021 Diego Revilla. All rights reserved.
//

	bool StaticPointToStaticCircle(const glm::vec3& P, const  glm::vec3& Center,
	float Radius) noexcept;

	bool StaticPointToStaticRect(const  glm::vec3& Pos, const glm::vec3& Rect,
	float Width, float Height, float Depth) noexcept;

	 bool StaticPointToOrientedRect(const  glm::vec3& Pos, const  glm::vec3& Rect,
	float Width, float Height, float Depth, float AngleRad) noexcept;

	bool StaticCircleToStaticCircle(const  glm::vec3& Center0, float Radius0,
	const  glm::vec3& Center1, float Radius1) noexcept;

	bool StaticRectToStaticRect(const  glm::vec3& Rect0, float Width0, float Height0, float Depth0,
	const  glm::vec3& Rect1, float Width1, float Height1, float Depth1) noexcept;

	bool StaticRectToStaticCirlce(const  glm::vec3& Rect, float Width, float Height, float Depth,
	const  glm::vec3& Center, float Radius) noexcept;

	bool OrientedRectToStaticCirlce(const glm::vec3& Rect, float Width, float Height, float Depth, 
	float AngleRad, const  glm::vec3& Center, float Radius) noexcept;


#endif