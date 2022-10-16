#include <Shared.h>

namespace Engine
{

	// ------------------------------------------------------------------------
	/*! Constructor
	*/ //----------------------------------------------------------------------
	Collider::Collider() : mColliderShape(1u), mColliderGroup(1u), mSelectShape(0), mSelectGroup(0), mHasCollider(false)
	{
		if (gCollisionSystem && gCollisionSystem->mPhySpacePart)
			m_ID = ++gCollisionSystem->mPhySpacePart->idGenerator;
		else
			m_ID = -1;
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*/ //----------------------------------------------------------------------
	Collider::~Collider() {
	}

	// ------------------------------------------------------------------------
	/*! Construct
	*
	*   Collider initializer for the editor
	*/ //----------------------------------------------------------------------
	void Collider::Construct()
	{
		//Add the collider to the CollisionSystem
		if (!mHasCollider)
		{
			mRigidBody = GetOwner()->GetComponentByType<RigidBody>();
			gCollisionSystem->AddCollider(this, 1);
		}

		mHasCollider = true;
	}

	// ------------------------------------------------------------------------
	/*! BeginPlay
	*
	*   Collider initializer for the game
	*/ //----------------------------------------------------------------------
	void Collider::BeginPlay()
	{
		if (!mHasCollider)
		{
			//Add the collider to the CollisionSystem
			mRigidBody = GetOwner()->GetComponentByType<RigidBody>();
			gCollisionSystem->AddCollider(this, 1);


			//Set AABB Min and Max
			m_bounding_vol.min.x = (GetOwner()->GetWorldPosition().x + mTransform.mPos.x) - (GetOwner()->GetScale().x * mTransform.mScale.x / 2);
			m_bounding_vol.max.x = (GetOwner()->GetWorldPosition().x + mTransform.mPos.x) + (GetOwner()->GetScale().x * mTransform.mScale.x / 2);

			m_bounding_vol.min.y = (GetOwner()->GetWorldPosition().y + mTransform.mPos.y) - (GetOwner()->GetScale().y * mTransform.mScale.y / 2);
			m_bounding_vol.max.y = (GetOwner()->GetWorldPosition().y + mTransform.mPos.y) + (GetOwner()->GetScale().y * mTransform.mScale.y / 2);

			m_bounding_vol.min.z = (GetOwner()->GetWorldPosition().z + mTransform.mPos.z) - (GetOwner()->GetScale().z * mTransform.mScale.z / 2);
			m_bounding_vol.max.z = (GetOwner()->GetWorldPosition().z + mTransform.mPos.z) + (GetOwner()->GetScale().z * mTransform.mScale.z / 2);
		}

		mHasCollider = true;
	}

	// ------------------------------------------------------------------------
	/*! Destroy
	*
	*   Remove a collider from the CollisionSystem
	*/ //----------------------------------------------------------------------
	void Collider::Destroy()
	{
		gCollisionSystem->RemoveCollider(this);
	}

	// ------------------------------------------------------------------------
	/*! OnEditor
	*
	*   Change parameters through the editor
	*/ //----------------------------------------------------------------------
	void Collider::OnEditor()
	{
#ifdef _EDITOR_
		//Shape list
		const char* shapes[] = { "Point", "Cube", "Sphere", "OBB" };

		ImGui::Combo("Shape", &mSelectShape, shapes, IM_ARRAYSIZE(shapes));

		switch (mSelectShape)
		{
			case 0: //Point
				ImGui::DragFloat2("Position", reinterpret_cast<float*>(&mTransform.mPos), -2000, 2000);

				mColliderShape = 1u;
				break;
			case 1: //Cube
				ImGui::DragFloat3("Position", reinterpret_cast<float*>(&mTransform.mPos), -2000, 2000);
				ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&mTransform.mScale.x), 0, 1000);

				mColliderShape = 2u;
				break;
			case 2: //Sphere
				ImGui::DragFloat2("Position", reinterpret_cast<float*>(&mTransform.mPos), -2000, 2000);
				ImGui::DragFloat("Radius", reinterpret_cast<float*>(&mTransform.mScale.x), 0, 1000);

				mColliderShape = 4u;
				break;
			default: //OBB
				ImGui::DragFloat3("Position", reinterpret_cast<float*>(&mTransform.mPos), -2000, 2000);
				ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&mTransform.mScale.x), 0, 1000);
				ImGui::DragFloat3("Rotation", reinterpret_cast<float*>(&mTransform.mRot), 0, 360);

				mColliderShape = 8u;
		}


		//Group list
		const char* groups[] = { "Environment", "Zone", "Player", "Enemy", "Player Bullet" , "Enemy Bullet"};

		ImGui::Combo("Collider Group", &mSelectGroup, groups, IM_ARRAYSIZE(groups));

		switch (mSelectGroup)
		{
			case 0: //Environment
				mColliderGroup = 1u;
				break;
			case 1: //Zone
				mColliderGroup = 2u;
				break;
			case 2: //Player
				mColliderGroup = 4u;
				break;
			case 3: //Enemy
				mColliderGroup = 8u;
				break;
			case 4://Player Bullet
				mColliderGroup = 16u;
				break;
			default: //Enemy Bullet
				mColliderGroup = 32u;
		}
#endif
	}

	// ------------------------------------------------------------------------
	/*! Tick
	*
	*   Updates the collider
	*/ //----------------------------------------------------------------------
	void Collider::Tick()
	{
		//Set AABB Min and Max
		m_bounding_vol.min.x = (GetOwner()->GetWorldPosition().x + mTransform.mPos.x) - (GetOwner()->GetScale().x * mTransform.mScale.x / 2);
		m_bounding_vol.max.x = (GetOwner()->GetWorldPosition().x + mTransform.mPos.x) + (GetOwner()->GetScale().x * mTransform.mScale.x / 2);

		m_bounding_vol.min.y = (GetOwner()->GetWorldPosition().y + mTransform.mPos.y) - (GetOwner()->GetScale().y * mTransform.mScale.y / 2);
		m_bounding_vol.max.y = (GetOwner()->GetWorldPosition().y + mTransform.mPos.y) + (GetOwner()->GetScale().y * mTransform.mScale.y / 2);

		m_bounding_vol.min.z = (GetOwner()->GetWorldPosition().z + mTransform.mPos.z) - (GetOwner()->GetScale().z * mTransform.mScale.z / 2);
		m_bounding_vol.max.z = (GetOwner()->GetWorldPosition().z + mTransform.mPos.z) + (GetOwner()->GetScale().z * mTransform.mScale.z / 2);
	}

	// ------------------------------------------------------------------------
	/*! EditorTick
	*
	*   Updates the editor
	*/ //----------------------------------------------------------------------
	void Collider::EditorTick() {
#ifdef _EDITOR_
		if (GEditor->RenderBoundingBoxes()) 
		{
			glm::vec3 scale(GetOwner()->GetScale().x * mTransform.mScale.x, GetOwner()->GetScale().y * mTransform.mScale.y, GetOwner()->GetScale().z * mTransform.mScale.z);

			for (auto& x : gCollisionSystem->mNewContacts) 
			{
				if (x.objects[0] == this || x.objects[1] == this) 
				{
					GDebug->DebugDrawAABB(GetOwner()->GetWorldPosition() + mTransform.mPos + x.contactData.mPi, 
						x.contactData.mNormal * (x.contactData.mPenetration * 100.f), { 0, 1, 0, 1 });
					GDebug->DebugDrawAABB(GetOwner()->GetWorldPosition() + mTransform.mPos, scale, { 1, 0, 0, 1 });
					return;
				}
			}

			switch (mColliderShape)
			{
				case 2: //AABB
					GDebug->DebugDrawAABB(GetOwner()->GetWorldPosition() + mTransform.mPos, scale, { 1, 1, 1, 1 });
					break;
				case 4: //Sphere
					GDebug->DebugDrawSphere(GetOwner()->GetWorldPosition() + mTransform.mPos, scale.x, { 1,1,1,1 });
					break;
				default:
					GDebug->DebugDrawAABB(GetOwner()->GetWorldPosition() + mTransform.mPos, scale, { 1, 1, 1, 1 });
			}
		}
#endif
	}

	// ------------------------------------------------------------------------
	/*! FromJson
	*
	*   Gets data from a .json
	*/ //----------------------------------------------------------------------
	void Collider::FromJson(const nlohmann::json& val)
	{
		mTransform.FromJson(val["Transform"]);
		fetch(mColliderShape, val, "ColliderShape");
		fetch(mSelectShape, val, "SelectedShape");

		auto it = val.find("SelectedGroup");
		if (it != val.end())
			mSelectGroup = it->get<int>();

		it = val.find("ColliderGroup");
		if (it != val.end())
			mColliderGroup = it->get<int>();
	}

	// ------------------------------------------------------------------------
	/*! ToJson
	*
	*   Gets data to a .json
	*/ //----------------------------------------------------------------------
	void Collider::ToJson(nlohmann::json& val) const
	{
		mTransform.ToJson(val["Transform"]);
		val["ColliderShape"] = mColliderShape;
		val["SelectedShape"] = mSelectShape;
		val["SelectedGroup"] = mSelectGroup;
		val["ColliderGroup"] = mColliderGroup;
	}
}