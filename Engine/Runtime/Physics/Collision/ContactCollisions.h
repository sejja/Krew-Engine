
//
//  ConactCollisions.h
//  Astro Engine
//
//  Created by Asier Azpiri on 22/9/21.
//  Copyright � 2021 Diego Revilla. All rights reserved.
//

#ifndef _CONTACT_COLLISIONS__H_
#define _CONTACT_COLLISIONS__H_

namespace Engine
{

	struct Contact
	{
		glm::vec3 mPi;
		glm::vec3 mNormal;
		float mPenetration;
	};

	bool StaticCircleToStaticCircleEx(const glm::vec3& Center1, float Radius1, const glm::vec3& Center2, float Radius2, Contact* pResult);

	bool StaticRectToStaticCircleEx(const glm::vec3& Rect, float Width, float Height, float Depth, const glm::vec3& Center, float Radius, Contact* pResult);

	bool StaticOBBToStaticCircleEx(const Math::Transform& OBB, const glm::vec3& Center, float Radius, Contact* pResult);

	bool StaticRectToStaticRectEx(const glm::vec3& pos1, const glm::vec3& size1, const glm::vec3& pos2, const glm::vec3& size2, Contact* pResult);

	bool OrientedRectToOrientedRectEx(const Math::Transform& OBB1, const Math::Transform& OBB2, Contact* pResult);

}

#endif