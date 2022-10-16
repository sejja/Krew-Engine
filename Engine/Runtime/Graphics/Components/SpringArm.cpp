#include <Shared.h>

namespace Engine
{
	SpringArm::SpringArm()
	{
	}

	SpringArm::~SpringArm()
	{
	}

	void SpringArm::BeginPlay()
	{
		mTarget = GWorld->GetScene()->GetObjectByName("Player");
		mRay = Ray(GetOwner()->GetPosition(), mTarget->GetPosition());
		prevpos = GetOwner()->GetPosition();
	}

	void SpringArm::Tick()
	{
		if (mTarget) {
			mRay.set_start(GetOwner()->GetPosition());
			mRay.set_end(mTarget->GetPosition());
			RayToObject();
		}
	}

	void SpringArm::FromJson(const nlohmann::json& val)
	{
		return;
	}

	void SpringArm::ToJson(nlohmann::json& val) const
	{
		return;
	}

	void SpringArm::RayToObject()
	{
		auto all_objects = GWorld->GetScene()->get_all_objects();
		for (auto it = all_objects.begin(); it != all_objects.end(); it++)
		{
			if (*it == mTarget || *it == GetOwner())
				continue;

			auto f = intersection_time_ray_aabb(mRay, *it);

			if (f >= 0.0f && f < 1.0f)
			{
				if (prevpos <= mTarget->GetPosition())
					GetOwner()->SetPosition((*it)->GetPosition() - (*it)->GetScale().x - 1);
				else
					GetOwner()->SetPosition((*it)->GetPosition() - (*it)->GetScale().x + 1);

				prevpos = GetOwner()->GetPosition();
			}
			else
				GetOwner()->SetPosition(prevpos);

		}
	}
}