//
//	Object.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

namespace Engine {
	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Initializes the default type and parent to nullptr
	*/ //----------------------------------------------------------------------
	GameObject::GameObject() :
		mType(__Texts::Engine::GameObject::StaticText::GameObject), mParent(nullptr), mTransform(nullptr) {
		PageAllocator<Math::Transform> transformPoolAlloc;
		mTransform = transformPoolAlloc.allocate();
		transformPoolAlloc.construct(mTransform);
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Removes the Children to ensure no memory leaks
	*/ //----------------------------------------------------------------------
	GameObject::~GameObject() {
		PageAllocator<GameObject> alloc; PageAllocator<Component> allocc;

		FForEach(mChilds.begin(), mChilds.end(), [&alloc](GameObject* const x) {
			alloc.destroy(x); alloc.deallocate(x);
			});
		FForEach(mComponents.begin(), mComponents.end(), [&allocc](Component* const x) {
			allocc.destroy(x); allocc.deallocate(x);
			});
		PageAllocator<Math::Transform> transformPoolAlloc;
		transformPoolAlloc.deallocate(mTransform);
	}

	// ------------------------------------------------------------------------
	/*! Remove Child
	*
	*   Removes a Child from the Actor
	*/ //--------------------------------------------------------------------
	void GameObject::RemoveChild(GameObject* const obj) {
		assertThis(obj, "Can't remove null child");
		GCollector::Instance().AddChildToRemove(obj);
	}

	// ------------------------------------------------------------------------
	/*! Add Component
	*
	*   Adds an already created Component to the Object
	*/ //---------------------------------------------------------------------
	void GameObject::AddComponent(Component* const obj) {
		assertThis(obj, "Can't add null component");
		obj->mOwner = this;
		mComponents.emplace_back(obj);

		//If we are currently playing
#ifdef _EDITOR_
		if (GEditor->IsPlaying())
#endif
			GObjectMgr->beginqueue.insert(obj);
	}

	// ------------------------------------------------------------------------
	/*! Remove Child Parentship
	*
	*   Removes the Parentship and ownership of a Object over a child
	*/ //---------------------------------------------------------------------
	void GameObject::RemoveChildParentship(GameObject* const obj) {
		assertThis(obj, "Cant remove parentship from null object");
		mChilds.remove(obj);
		obj->mParent = nullptr;
	}

	// ------------------------------------------------------------------------
	/*! Remove Component
	*
	*   Removes a Component from our Childs
	*/ //--------------------------------------------------------------------
	void GameObject::RemoveComponent(Component* const obj) {
		assertThis(obj, "Can't remove null component");
		GCollector::Instance().AddComponentToRemove(obj);
	}

	// ------------------------------------------------------------------------
	/*! Set position
	*
	*   Sets the position of the Game Object
	*/ //--------------------------------------------------------------------
	void GameObject::SetPosition(const Math::Vector3D& pos) noexcept {
		//Math::Vector3D _pos = pos;
		//if (mParent) _pos -= mParent->GetWorldPosition();
		GGfxPipeline->lightSys->mUpdatePending[this] = true;
		mTransform->mPos = pos;
		mWorldTransformMtrx = ComputeWorldMatrix();
	}

	// ------------------------------------------------------------------------
	/*! Set Rotation
	*
	*   Sets the Rotation of the Game Object
	*/ //--------------------------------------------------------------------
	void GameObject::SetRotation(const Math::Vector3D& rot) noexcept {
		//Math::Vector3D _rot = rot;
		//if (mParent) _rot -= mParent->GetWorldRotation();
		GGfxPipeline->lightSys->mUpdatePending[this] = (mTransform->mRot != rot);
		mTransform->mRot = rot;
		mWorldTransformMtrx = ComputeWorldMatrix();
	}

	// ------------------------------------------------------------------------
	/*! Set Scale
	*
	*   Sets the Scale of the Game Object
	*/ //--------------------------------------------------------------------
	void GameObject::SetScale(const Math::Vector3D& scal) noexcept {
		GGfxPipeline->lightSys->mUpdatePending[this] = (mTransform->mScale != scal);
		mTransform->mScale = scal;
		mWorldTransformMtrx = ComputeWorldMatrix();
	}

	// ------------------------------------------------------------------------
	/*! To Json
	*
	*   Serializes a Game Object to a Json File
	*/ //----------------------------------------------------------------------
	void GameObject::ToJson(nlohmann::json& val) const {
		using __Texts::Engine::GameObject::StaticText;

		size_t _count = 0;
		nlohmann::json& _refcomponents = val[StaticText::Components];
		nlohmann::json& _refchilds = val[StaticText::Childs];
		nlohmann::json& _reftags = val[StaticText::Tags];

		FForEach(mChilds.begin(), mChilds.end(), [&_refchilds, &_count](GameObject* x) {
			nlohmann::json& ref = _refchilds[std::to_string(_count++)];
			x->ToJson(ref), ref[StaticText::Name] = x->GetName();
			});

		_count = 0;

		FForEach(mTags.begin(), mTags.end(), [&_reftags, &_count](const std::string& x) {
			nlohmann::json& ref = _reftags[std::to_string(_count++)];
			ref[StaticText::Tags] = x;
			});

		_count = 0;

		FForEach(mComponents.begin(), mComponents.end(), [&_refcomponents, &_count](Component* x) {
			nlohmann::json& ref = _refcomponents[std::to_string(_count++)];
			x->ToJson(ref);
			ref[StaticText::Name] = x->GetName();
			});

		mTransform->ToJson(val[StaticText::Transform]), val[StaticText::Name] = GetName();
	}

	// ------------------------------------------------------------------------
	/*! From Json
	*
	*   Retrieves a Game Object from a Json file
	*/ //----------------------------------------------------------------------
	void GameObject::FromJson(const nlohmann::json& val) {
		using __Texts::Engine::GameObject::StaticText;

		mTransform->FromJson(val[StaticText::Transform]);
		SetName(val[StaticText::Name].get<std::string>().c_str());
		
		fetch(mType, val, StaticText::Type);

		auto it = val.find(StaticText::Components);
		if (it != val.end())
		{
			const nlohmann::json& _refcomponents = val[StaticText::Components];

			FForEach(_refcomponents.begin(), _refcomponents.end(),
				[this](const nlohmann::json& a) {
					Component* _a = Cast<Component>(GFactory->CreateTypeByName(a[StaticText::Name].get<std::string>().c_str()));
					if (_a) {
						_a->mOwner = this;
						_a->FromJson(a);
						_a->SetName(a[StaticText::Name].get<std::string>().c_str());
						AddComponent(_a);
					}
				});
		}

		it = val.find(StaticText::Childs);
		if (it != val.end())
		{
			const nlohmann::json& _refchilds = val[StaticText::Childs];

			FForEach(_refchilds.begin(), _refchilds.end(), [this](const nlohmann::json& a) {
				//try to retrieve the component
				GameObject* temp = AddChild(a[StaticText::Name].get<std::string>().c_str());
				temp->FromJson(a);
				});
		}

		const nlohmann::json& _reftags = val[StaticText::Tags];

		mWorldTransformMtrx = ComputeWorldMatrix();
		//FForEach(_reftags.begin(), _reftags.end(), [this](const nlohmann::json& a) {
		//	AddTag(a[StaticText::Name].get<std::string>().c_str());
		//	});
	}

	// ------------------------------------------------------------------------
	/*! Has Tag
	*
	*   Returns wether a Base Class has a tag
	*/ //--------------------------------------------------------------------
	const bool GameObject::HasTag(const char* flg) const noexcept {
		//Traverse through all tags
		for (auto& x : mTags)
			//If we have the same flag
			if (!strcmp(x.c_str(), flg))
				return true;
		return false;
	}

	// ------------------------------------------------------------------------
	/*! Remove tag
	*
	*   Removes a tag from the base class
	*/ //--------------------------------------------------------------------
	void GameObject::RemoveTag(const char* flg) {
		//for every tag
		for (Array<std::string>::iterator it = mTags.begin(), _end = mTags.end(); it != _end; ++it)
			//If we can find an identical flag
			if (!strcmp((*it).c_str(), flg)) {
				mTags.remove(*it);
				return;
			}
	}
	// ------------------------------------------------------------------------
	/*! Tick
	*
	*   Updates parent and children
	*/ //----------------------------------------------------------------------
	void GameObject::Tick()
	{
		//Update parent components
		unsigned compCount = GetComponentCount();
		for (int j = 0; j < compCount; j++) {
			GetComponent(j)->Tick();
		}
		
		//Update children components
		unsigned childCount = GetChildCount();
		for (int j = 0; j < childCount; j++) {
			GetChild(j)->Tick();
		}
	}

	// ------------------------------------------------------------------------
	/*! Destroy
	*
	*   Destroys the Object, it's children and it's components
	*/ //----------------------------------------------------------------------
	void GameObject::Destroy() {
		FForEach(mChilds.begin(), mChilds.end(), [](GameObject* const x) { x->Destroy();  });
		FForEach(mComponents.begin(), mComponents.end(), [](Component* const x) { x->Destroy();  });
	}

	// ------------------------------------------------------------------------
	/*! Spawn From Prefab
	*
	*   Converts an Object onto a Prefab from an asset file
	*/ //----------------------------------------------------------------------
	// ------------------------------------------------------------------------
	/*! Spawn From Prefab
	*
	*   Converts an Object onto a Prefab from an asset file
	*/ //----------------------------------------------------------------------
	void GameObject::LoadFromPrefab(const std::string& mPath) {
		using __Texts::Engine::GameObject::StaticText;

		std::filesystem::path filePath = mPath;
		std::ifstream input(mPath);

		nlohmann::json J;
		input >> J;
		input.close();
		FromJson(J);
	}

	GameObject* GameObject::InstantiateThisObject() {
		//instantiate de prefab
		GameObject* const new_obj = GWorld->GetScene()->SpawnObject(GetName());

		//copy construct transform component
		Math::Vector3D newPos = this->GetPosition();
		Math::Vector3D newSca = this->GetScale();
		Math::Vector3D newRot = this->GetRotation();
		Math::Transform* newTr = new Math::Transform(newPos, newSca, newRot);
		new_obj->mTransform = newTr;

		//point ot the same 3dModel
		new_obj->AddComponent(this->GetComponentByType<Graphics::Model3D>());

		//copy components
		for (auto& x : mComponents) {
			Component* newComp = new Component(*x);
			new_obj->AddComponent(newComp);
		}

		//copy ...?


		return new_obj;
	}

	// ------------------------------------------------------------------------
	/*! Get World Position
	*
	*   Retrieves the world position
	*/ //----------------------------------------------------------------------
	Math::Vector3D GameObject::GetWorldPosition() const noexcept {
		glm::mat4 parent_mtx = GetParentTransformMtx();
		return glm::vec3(parent_mtx * glm::vec4(mTransform->mPos.x, mTransform->mPos.y, mTransform->mPos.z, 1));
	}

	// ------------------------------------------------------------------------
	/*! Get World Rotation
	*
	*   Retrieves the world rotation
	*/ //----------------------------------------------------------------------
	Math::Vector3D GameObject::GetWorldRotation() const noexcept {
		glm::mat4 world_mtx = mWorldTransformMtrx;
		glm::vec3 t, rotation, s;
		Math::fast_decompose_transfom(world_mtx, t, rotation, s);
		return rotation;
	}

	glm::mat4& GameObject::GetWorldMatrix() {
		return mWorldTransformMtrx;
	}
}