#pragma once


namespace Engine
{
	struct SpringArm : public Component
	{
		SpringArm();
		~SpringArm();

		void BeginPlay() override;
		void Tick() override;

		void FromJson(const nlohmann::json& val);
		void ToJson(nlohmann::json& val) const;

		void RayToObject();


	private:
		GameObject* mTarget = nullptr;
		Ray mRay;
		Math::Vector3D prevpos;
	};
}
