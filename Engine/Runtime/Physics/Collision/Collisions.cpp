#include <Shared.h>


bool StaticPointToStaticCircle(const  glm::vec3& P, const  glm::vec3& Center, float Radius) noexcept
{
	if (glm::distance(P, Center) <= Radius)
		return true;

	return false;
}

bool StaticPointToStaticRect(const  glm::vec3& Pos, const  glm::vec3& Rect, float Width, float Height, float Depth) noexcept
{
	if ((Rect.x - Width / 2 <= Pos.x && Pos.x <= Rect.x + Width / 2) &&
		(Rect.y - Height / 2 <= Pos.y && Pos.y <= Rect.y + Height / 2) &&
		(Rect.z - Depth / 2 <= Pos.z && Pos.z <= Rect.z + Depth / 2))
		return true;

	return false;
}

bool StaticPointToOrientedRect(const  glm::vec3& Pos, const  glm::vec3& Rect, float Width, float Height, float Depth, float AngleRad) noexcept
{
	const glm::mat4 invRect = glm::rotate(glm::mat4(1.0f), -AngleRad, glm::vec3(0, 0, 1)) * glm::translate(glm::mat4(1.0f), glm::vec3(-Rect.x, -Rect.y, -Rect.z));

	if (StaticPointToStaticRect(Pos, invRect * glm::vec4(Rect.x, Rect.y, Rect.z, 1), Width, Height, Depth))
		return true;

	return false;
}

bool StaticCircleToStaticCircle(const  glm::vec3& Center0, float Radius0, const  glm::vec3& Center1, float Radius1) noexcept
{
	if(glm::distance(Center0,Center1) <= Radius0 + Radius1)
		return true;

	return false;
}

bool StaticRectToStaticRect(const  glm::vec3& Rect0, float Width0, float Height0, float Depth0, const  glm::vec3& Rect1, float Width1, float Height1, float Depth1) noexcept
{
	if ( ((Rect0.x - Width0 / 2) > (Rect1.x + Width1 / 2) || (Rect1.x - Width1 / 2) > (Rect0.x + Width0 / 2)) || 
		 ((Rect0.y + Height0 / 2) < (Rect1.y - Height1 / 2) || (Rect1.y + Height1 / 2) < (Rect0.y - Height0 / 2)) ||
		 ((Rect0.z - Depth0 / 2) > (Rect1.z + Depth1 / 2) || (Rect1.z - Depth1 / 2) > (Rect0.z + Depth0 / 2)) )
			return false;

	return true;
}

bool StaticRectToStaticCirlce(const  glm::vec3& Rect, float Width, float Height, float Depth, const  glm::vec3& Center, float Radius) noexcept
{
	if (StaticPointToStaticCircle({ glm::clamp(Center.x, Rect.x - Width / 2, Rect.x + Width / 2), glm::clamp(Center.y, Rect.y - Height / 2, Rect.y + Height / 2), glm::clamp(Center.z, Rect.z - Depth / 2, Rect.z + Depth / 2)}, Center, Radius))
		return true;
	
	return false;
}

bool OrientedRectToStaticCirlce(const glm::vec3& Rect, float Width, float Height, float Depth, float AngleRad, const  glm::vec3& Center, float Radius) noexcept
{
	const glm::mat4 invRect = glm::rotate(glm::mat4(1.0f), -AngleRad, glm::vec3(0, 0, 1)) * glm::translate(glm::mat4(1.0f), glm::vec3(-Rect.x,-Rect.y,-Rect.z));

	if (StaticRectToStaticCirlce(invRect * glm::vec4(Rect.x, Rect.y, Rect.z, 1), Width, Height, Depth, invRect * glm::vec4(Center.x, Center.y, Center.z, 1), Radius))
		return true;

	return false;
}