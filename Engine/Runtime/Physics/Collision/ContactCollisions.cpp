#include <Shared.h>

//
//  ConactCollisions.cpp
//  Astro Engine
//
//  Created by Asier Azpiri on 22/9/21.
//  Copyright � 2021 Diego Revilla. All rights reserved.
//

namespace Engine
{
	using namespace Math;

	bool StaticCircleToStaticCircleEx(const glm::vec3& Center1, float Radius1, const glm::vec3& Center2, float Radius2, Contact* pResult)
	{
		//No overlap
		if (!StaticCircleToStaticCircle(Center1, Radius1, Center2, Radius2))
			return false;
		//Overlap
		else
		{
			if (pResult) //Sanity check
			{
				float distance = glm::distance(Center1, Center2); //Distance
				if (distance == 0) pResult->mNormal = glm::vec3(0, 0, 0); //When distance is zero
				else pResult->mNormal = glm::vec3((Center2.x - Center1.x) / distance, //Normal x
					(Center2.y - Center1.y) / distance, (Center2.z - Center1.z) / distance); //Normal y
				pResult->mPenetration = Radius1 + Radius2 - distance; //Penetration
				pResult->mPi = Center1 + pResult->mNormal * Radius1; //Point of intersection
			}
			return true;
		}
	}

	bool StaticRectToStaticCircleEx(const glm::vec3& Rect, float Width, float Height, float Depth, const glm::vec3& Center, float Radius, Contact* pResult)
	{
		if (Width < 0.0f || Height < 0.0f || Depth < 0.0f)
			return false;


		float RectRight = Rect.x + Width / 2; //Rectangle right size
		float RectLeft = Rect.x - Width / 2; //Rectangle left size
		float RectTop = Rect.y + Height / 2; //Rectangle top size
		float RectBot = Rect.y - Height / 2; //Rectangle bottom size
		float RectSurf = Rect.z - Depth / 2; 
		float RectDeep = Rect.z + Depth / 2;

		//No overlap
		if (!StaticRectToStaticCirlce(Rect, Width, Height, Depth, Center, Radius))
			return false;
		//Overlap and inside
		else if (Center.x <= RectRight && Center.x >= RectLeft && Center.y <= RectTop && Center.y >= RectBot && Center.z <= RectDeep && Center.z >= RectSurf)
		{
			if (pResult) //Sanity check
			{
				glm::vec3 distance = Center - Rect; //Distance
				float penetrationX = Width / 2 - sqrt(distance.x * distance.x); //Penetration x
				float penetrationY = Height / 2 - sqrt(distance.y * distance.y); //Penetration y
				float penetrationZ = Depth / 2 - sqrt(distance.z * distance.z); //Penetration z
				pResult->mPenetration = fmin(penetrationX,(fmin(penetrationY, penetrationZ))) + Radius; //Penetration
				if (penetrationX < penetrationY && penetrationX < penetrationZ) 
					pResult->mNormal = glm::vec3(1, 0, 0); //Normal on x axis
				else if(penetrationY < penetrationZ && penetrationY < penetrationX)
					pResult->mNormal = glm::vec3(0, 1, 0); //Normal on y axis
				else
					pResult->mNormal = glm::vec3(0, 0, 1); //Normal on y axis

				if (glm::dot(distance, pResult->mNormal) < 0) pResult->mNormal *= -1; //Normal negation
				pResult->mPi = Center; //Point of intersection
			}
			return true;
		}
		//Overlap and not inside
		else
		{
			if (pResult) //Sanity check
			{
				glm::vec3 clamp(std::clamp(Center.x, Rect.x - Width / 2, Rect.x + Width / 2), //Clamp x
								std::clamp(Center.y, Rect.y - Height / 2, Rect.y + Height / 2), //Clamp y
								std::clamp(Center.z, Rect.z - Depth / 2, Rect.z + Depth / 2)); //Clamp z
				float distance = glm::distance(clamp, Center); //Distance
				pResult->mNormal = glm::vec3((Center.x - clamp.x) / distance, //Normal x
								   (Center.y - clamp.y) / distance, // Normal y
								   (Center.z - clamp.z) / distance); //Normal z
				pResult->mPenetration = Radius - distance; //Penetration
				pResult->mPi = clamp; //Point of intersection
			}
			return true;
		}
	}

	glm::vec3 MultiplyPoint(const glm::mat4 & mat, const glm::vec3& rhs)
	{
		return std::move(glm::vec3(mat[0][0] * rhs.x + mat[0][1] * rhs.y + mat[0][2] * rhs.z + mat[0][3],
								   mat[1][0] * rhs.x + mat[1][1] * rhs.y + mat[1][2] * rhs.z + mat[1][3],
								   mat[2][0] * rhs.x + mat[2][1] * rhs.y + mat[2][2] * rhs.z + mat[2][3]));
	}

	glm::vec3 MultiplyDirection(const glm::mat4& mat, const glm::vec3& rhs)
	{
		return std::move(glm::vec3(mat[0][0] * rhs.x + mat[0][1] * rhs.y + mat[0][2] * rhs.z,
								   mat[1][0] * rhs.x + mat[1][1] * rhs.y + mat[1][2] * rhs.z,
								   mat[2][0] * rhs.x + mat[2][1] * rhs.y + mat[2][2] * rhs.z));
	}

	bool StaticOBBToStaticCircleEx(const Math::Transform& OBB, const glm::vec3& Center, float Radius, Contact* pResult)
	{
		glm::mat4 invMatrix = glm::rotate(glm::mat4(1.f), -glm::radians(OBB.mRot.z), { 0.f, 0.f, 1.f })
							  * glm::rotate(glm::mat4(1.f), -glm::radians(OBB.mRot.y), { 0.f, 1.f, 0.f })
							  * glm::rotate(glm::mat4(1.f), -glm::radians(OBB.mRot.x), { 1.f, 0.f, 0.f })
							  * glm::translate(glm::mat4(1.f), { -OBB.mPos.x, -OBB.mPos.y, -OBB.mPos.z });

		glm::vec3 temp1 = glm::vec3(invMatrix * glm::vec4(OBB.mPos.x, OBB.mPos.y, OBB.mPos.z,1.f));
		glm::vec3 temp2 = glm::vec3(invMatrix * glm::vec4(Center.x, Center.y, Center.z, 1.f));

		if (StaticRectToStaticCircleEx(temp1, OBB.mScale.x, OBB.mScale.y, OBB.mScale.z, temp2, Radius, pResult))
		{
			if (pResult) //Sanity check
			{
				glm::mat4 matrix = glm::rotate(glm::mat4(1.f), glm::radians(OBB.mRot.x), { 1.f, 0.f, 0.f })
								* glm::rotate(glm::mat4(1.f), glm::radians(OBB.mRot.y), { 0.f, 1.f, 0.f })
								* glm::rotate(glm::mat4(1.f), glm::radians(OBB.mRot.z), { 0.f, 0.f, 1.f });

				pResult->mNormal = glm::vec3(matrix * glm::vec4(pResult->mNormal,1));	//Normal
				matrix *= glm::translate(glm::mat4(1.f), { OBB.mPos.x, OBB.mPos.y, OBB.mPos.z });
				pResult->mPi = glm::vec3(matrix * glm::vec4(pResult->mPi, 1)); //Point of intersection
			}
			return true;
		}
		//No overlap
		else
			return false;
	}

	bool StaticRectToStaticRectEx(const glm::vec3& pos1, const glm::vec3& size1, const glm::vec3& pos2, const glm::vec3& size2, Contact* pResult)
	{
		if (size1.x < 0.0f || size1.y < 0.0f || size1.z < 0.0f || size2.x < 0.0f || size2.y < 0.0f || size2.z < 0.0f)
			return false;

		//No overlap
		if (!StaticRectToStaticRect(pos1, size1.x, size1.y, size1.z, pos2, size2.x, size2.y, size2.z))
			return false;
		//Overlap
		else
		{
			if (pResult) //Sanity check
			{
				pResult->mPenetration = 0; //Set to zero
				pResult->mNormal = glm::vec3(0, 0, 0); //Set to zero
				pResult->mPi = glm::vec3(0, 0, 0); //Set to zero
				glm::vec3 distance = pos2 - pos1;												//Distance
				float penetrationX = ((size1.x + size2.x) / 2) - sqrt(distance.x * distance.x);	//Penetration x
				float penetrationY = ((size1.y + size2.y) / 2) - sqrt(distance.y * distance.y);	//Penetration y
				float penetrationZ = ((size1.z + size2.z) / 2) - sqrt(distance.z * distance.z);	//Penetration z
				if (penetrationX < penetrationY && penetrationX < penetrationZ) 
					pResult->mPenetration = penetrationX; //Penetration on the x axis
				else if (penetrationY < penetrationZ && penetrationY < penetrationX)
					pResult->mPenetration = penetrationY; //Penetration on the y axis
				else
					pResult->mPenetration = penetrationZ; //Penetration on the z axis

				if (penetrationX < penetrationY && penetrationX < penetrationZ) 
					pResult->mNormal = glm::vec3(1, 0, 0); //Normal on the x axis
				else if (penetrationY < penetrationZ && penetrationY < penetrationX)
					pResult->mNormal = glm::vec3(0, 1, 0); //Normal on the y axis
				else
					pResult->mNormal = glm::vec3(0, 0, 1); //Normal on the y axis


				if (glm::dot(distance, pResult->mNormal) < 0) pResult->mNormal *= -1; //Normal negation

				if (StaticPointToStaticRect(glm::vec3(pos2.x + size2.x / 2, pos2.y + size2.y / 2, pos2.z + size2.z / 2), pos1, size1.x, size1.y, size1.z))
					pResult->mPi = glm::vec3(pos2.x + size2.x / 2, pos2.y + size2.y / 2, pos2.z + size2.z / 2);

				else if (StaticPointToStaticRect(glm::vec3(pos2.x + size2.x / 2, pos2.y + size2.y / 2, pos2.z - size2.z / 2), pos1, size1.x, size1.y, size1.z))
					pResult->mPi = glm::vec3(pos2.x + size2.x / 2, pos2.y + size2.y / 2, pos2.z - size2.z / 2);

				else if (StaticPointToStaticRect(glm::vec3(pos2.x + size2.x / 2, pos2.y - size2.y / 2, pos2.z + size2.z / 2), pos1, size1.x, size1.y, size1.z))
					pResult->mPi = glm::vec3(pos2.x + size2.x / 2, pos2.y - size2.y / 2, pos2.z + size2.z / 2);

				else if (StaticPointToStaticRect(glm::vec3(pos2.x - size2.x / 2, pos2.y + size2.y / 2, pos2.z + size2.z / 2), pos1, size1.x, size1.y, size1.z))
					pResult->mPi = glm::vec3(pos2.x - size2.x / 2, pos2.y + size2.y / 2, pos2.z + size2.z / 2);

				else if (StaticPointToStaticRect(glm::vec3(pos2.x - size2.x / 2, pos2.y - size2.y / 2, pos2.z + size2.z / 2), pos1, size1.x, size1.y, size1.z))
					pResult->mPi = glm::vec3(pos2.x - size2.x / 2, pos2.y - size2.y / 2, pos2.z + size2.z / 2);

				else if (StaticPointToStaticRect(glm::vec3(pos2.x - size2.x / 2, pos2.y + size2.y / 2, pos2.z - size2.z / 2), pos1, size1.x, size1.y, size1.z))
					pResult->mPi = glm::vec3(pos2.x - size2.x / 2, pos2.y + size2.y / 2, pos2.z - size2.z / 2);

				else if (StaticPointToStaticRect(glm::vec3(pos2.x + size2.x / 2, pos2.y - size2.y / 2, pos2.z - size2.z / 2), pos1, size1.x, size1.y, size1.z))
					pResult->mPi = glm::vec3(pos2.x + size2.x / 2, pos2.y - size2.y / 2, pos2.z - size2.z / 2);

				else if (StaticPointToStaticRect(glm::vec3(pos2.x - size2.x / 2, pos2.y - size2.y / 2, pos2.z - size2.z / 2), pos1, size1.x, size1.y, size1.z))
					pResult->mPi = glm::vec3(pos2.x - size2.x / 2, pos2.y - size2.y / 2, pos2.z - size2.z / 2);


				else if (StaticPointToStaticRect(glm::vec3(pos1.x + size1.x / 2, pos1.y + size1.y / 2, pos1.z + size1.z / 2), pos1, size2.x, size2.y, size2.z))
					pResult->mPi = glm::vec3(pos1.x + size1.x / 2, pos1.y + size1.y / 2, pos1.z + size1.z / 2);

				else if (StaticPointToStaticRect(glm::vec3(pos1.x + size1.x / 2, pos1.y + size1.y / 2, pos1.z - size1.z / 2), pos1, size2.x, size2.y, size2.z))
					pResult->mPi = glm::vec3(pos1.x + size1.x / 2, pos1.y + size1.y / 2, pos1.z - size1.z / 2);

				else if (StaticPointToStaticRect(glm::vec3(pos1.x + size1.x / 2, pos1.y - size1.y / 2, pos1.z + size1.z / 2), pos1, size2.x, size2.y, size2.z))
					pResult->mPi = glm::vec3(pos1.x + size1.x / 2, pos1.y - size1.y / 2, pos1.z + size1.z / 2);

				else if (StaticPointToStaticRect(glm::vec3(pos1.x - size1.x / 2, pos1.y + size1.y / 2, pos1.z + size1.z / 2), pos1, size2.x, size2.y, size2.z))
					pResult->mPi = glm::vec3(pos1.x - size1.x / 2, pos1.y + size1.y / 2, pos1.z + size1.z / 2);

				else if (StaticPointToStaticRect(glm::vec3(pos1.x - size1.x / 2, pos1.y - size1.y / 2, pos1.z + size1.z / 2), pos1, size2.x, size2.y, size2.z))
					pResult->mPi = glm::vec3(pos1.x - size1.x / 2, pos1.y - size1.y / 2, pos1.z + size1.z / 2);

				else if (StaticPointToStaticRect(glm::vec3(pos1.x - size1.x / 2, pos1.y + size1.y / 2, pos1.z - size1.z / 2), pos1, size2.x, size2.y, size2.z))
					pResult->mPi = glm::vec3(pos1.x - size1.x / 2, pos1.y + size1.y / 2, pos1.z - size1.z / 2);

				else if (StaticPointToStaticRect(glm::vec3(pos1.x + size1.x / 2, pos1.y - size1.y / 2, pos1.z - size1.z / 2), pos1, size2.x, size2.y, size2.z))
					pResult->mPi = glm::vec3(pos1.x + size1.x / 2, pos1.y - size1.y / 2, pos1.z - size1.z / 2);

				else if (StaticPointToStaticRect(glm::vec3(pos1.x - size1.x / 2, pos1.y - size1.y / 2, pos1.z - size1.z / 2), pos1, size2.x, size2.y, size2.z))
					pResult->mPi = glm::vec3(pos1.x - size1.x / 2, pos1.y - size1.y / 2, pos1.z - size1.z / 2);
			}
			return true;
		}
	}

	bool OrientedRectToOrientedRectEx(const Math::Transform& OBB1, const Math::Transform& OBB2, Contact* pResult)
	{

		//If contact is valid

		//Compute distance between OBB

		glm::vec3 Dist(OBB2.mPos - OBB1.mPos);

		//Compute half-extents of both OBB
		glm::vec3 Axis[6] = { Rotate(OBB1.mRot.x) * glm::vec3(OBB1.mScale.x / 2, 0.0f, 0.0f),
							Rotate(OBB1.mRot.y) * glm::vec3(0, OBB1.mScale.y / 2, 0.0f),
							Rotate(OBB1.mRot.z)* glm::vec3(0, 0, OBB1.mScale.z / 2),
							Rotate(OBB2.mRot.x) * glm::vec3(OBB2.mScale.x / 2, 0, 0.0f),
							Rotate(OBB2.mRot.y) * glm::vec3(0, OBB2.mScale.y / 2, 0.0f),
							Rotate(OBB2.mRot.z)* glm::vec3(0, 0, OBB2.mScale.z / 2)};

		//Array of projections
		float Proj[6] = { 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };

		//Array of lengths
		float Length[6] = { 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };

		//Array of lengths
		float penetration[6] = { 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };

		//Loop to compute projection
		for (int i = 0; i < 6; i++)
		{
			//Compute length of projection
			Proj[i] = glm::length(Project(Dist, Axis[i]));
			//Compute total length
			Length[i] = (glm::length(Project(Axis[0], Axis[i])) + glm::length(Project(Axis[1], Axis[i])) + glm::length(Project(Axis[2], Axis[i])) 
						+ glm::length(Project(Axis[3], Axis[i])) + glm::length(Project(Axis[4], Axis[i])) + glm::length(Project(Axis[5], Axis[i])));
			//Compute penetration
			penetration[i] = Proj[i] - Length[i];
		}

		//If local contact is false
		if (pResult == NULL)
		{
			//Return false
			return false;
		}

		//Minimum penetration
		float MinPen = penetration[0];
		//Minimun axis
		glm::vec3 MinAxis = Axis[0];

		//Loop to compute penetration
		for (int i = 0; i < 6; i++)
		{
			if (penetration[i] > 0.0f)
			{
				return false;
			}
			//If we have found a minor penetration
			else if (penetration[i] > MinPen)
			{
				//It is the minimum penetration
				MinPen = penetration[i];
				//And its corresponding axis
				MinAxis = Axis[i];
			}
		}

		//Set minimum penetration
		pResult->mPenetration = MinPen;
		//Normalize the minimum axis to get the normal
		pResult->mNormal = glm::normalize(MinAxis);
		//Check for normal negation
		if (glm::dot(pResult->mNormal, Dist) > 0)
		{
			//Negate it
			pResult->mNormal *= -1;
		}

		//OBB2 corners
		glm::vec3 vertex1[16] = {
							 glm::vec3(OBB2.mPos + Axis[3] + Axis[4]), //Top-Right corner
							 glm::vec3(OBB2.mPos - Axis[3] + Axis[4]), //Top-Left corner
							 glm::vec3(OBB2.mPos + Axis[3] - Axis[4]), //Bot-Right corner
							 glm::vec3(OBB2.mPos - Axis[3] - Axis[4]), //Bot-Left corner
							 glm::vec3(OBB2.mPos + Axis[4] + Axis[5]), 
							 glm::vec3(OBB2.mPos - Axis[4] + Axis[5]), 
							 glm::vec3(OBB2.mPos + Axis[4] - Axis[5]), 
							 glm::vec3(OBB2.mPos - Axis[4] - Axis[5]), 

							 glm::vec3(OBB1.mPos + Axis[0] + Axis[1]), //Top-Right corner
							 glm::vec3(OBB1.mPos - Axis[0] + Axis[1]), //Top-Left corner
							 glm::vec3(OBB1.mPos + Axis[0] - Axis[1]), //Bot-Right corner
							 glm::vec3(OBB1.mPos - Axis[0] - Axis[1]), //Bot-Left corner
							 glm::vec3(OBB1.mPos + Axis[1] + Axis[2]), 
							 glm::vec3(OBB1.mPos - Axis[1] + Axis[2]),
							 glm::vec3(OBB1.mPos + Axis[1] - Axis[2]), 
							 glm::vec3(OBB1.mPos - Axis[1] - Axis[2])
		};



		//Loop to get point of intersection
		for (int i = 0; i < 16; i++)
		{
			//If corner is in OBB
			if (i < 8 && StaticPointToOrientedRect(vertex1[i], OBB1.mPos, OBB1.mScale.x, OBB1.mScale.y, OBB1.mScale.z, OBB1.mRot.x))
			{
				//Set it as point of intersection
				pResult->mPi = vertex1[i];
				//Return true
				return true;
			}
			//If corner is in OBB
			if (i >= 8 && StaticPointToOrientedRect(vertex1[i], OBB2.mPos, OBB2.mScale.x, OBB2.mScale.y, OBB2.mScale.z, OBB2.mRot.x))
			{
				//Set it as point of intersection
				pResult->mPi = vertex1[i];
				//Return true
				return true;
			}
		}

		//Return false otherwise
		return false;
	}

}