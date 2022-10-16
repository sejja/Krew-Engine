//
//	Object.h
//	Good Neighbours
//
//	Created by Diego Revilla on 11/03/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#ifndef _OBJECT__H_
#define _OBJECT__H_

namespace Editor {
	class PrefabWindowModal;
}

namespace Engine {
	class Component;

	class ENGINE_API GameObject : public IBase, public events::Listener, public Serializable {
	public:
#pragma region // Definitions
		friend class GCollector;
		friend class Editor::PrefabWindowModal;
#pragma endregion

#pragma region // Constructors & Destructors
		GameObject();
		virtual ~GameObject();
		GameObject& operator=(const GameObject&) = delete;
#pragma endregion

#pragma region // Functions
		template<typename Ty_>
		DONTDISCARD Ty_* GetChild(const char* name) const noexcept;

		template<typename Ty_>
		DONTDISCARD Ty_ inline* GetChild(const size_t idx) const noexcept;

		template<typename Ty_>
		DONTDISCARD Ty_* GetComponent(const char* name) const noexcept;

		template<typename Ty_>
		DONTDISCARD Ty_* GetComponentByType() const noexcept;

		template<typename Ty_>
		DONTDISCARD const Ty_ inline* GetComponent(const size_t idx) const noexcept;

		template<typename... Args>
		DONTDISCARD GameObject* AddChild(const char* name = nullptr, Args... args);

		template<class K>
		DONTDISCARD bool HasComponentOfType() const noexcept;

		template<typename Ty_, typename... Args>
		DONTDISCARD Ty_* AddComponent(const char* name = nullptr, Args... args);

		void Tick() override;
		void Destroy() override;
		DONTDISCARD size_t inline GetChildCount() const noexcept;
		DONTDISCARD size_t inline GetComponentCount() const noexcept;
		DONTDISCARD GameObject inline* GetParent() const noexcept;
		DONTDISCARD Math::Transform inline GetTransform() const noexcept;
		void SetPosition(const Math::Vector3D& pos) noexcept;
		void SetRotation(const Math::Vector3D& rot) noexcept;
		void SetScale(const Math::Vector3D& scal) noexcept;
		DONTDISCARD Math::Vector3D inline GetPosition() const noexcept;
		DONTDISCARD Math::Vector3D inline GetRotation() const noexcept;
		DONTDISCARD Math::Vector3D inline GetScale() const noexcept;
		DONTDISCARD Math::Vector3D GetWorldPosition() const noexcept;
		DONTDISCARD Math::Vector3D GetWorldRotation() const noexcept;
		DONTDISCARD Math::Vector3D inline GetWorldScale() const noexcept;
		DONTDISCARD Math::Transform inline GetWorldTransform() const noexcept;
		DONTDISCARD glm::mat4 inline ComputeWorldMatrix() noexcept;
		DONTDISCARD GameObject inline* GetChild(const size_t i) const noexcept;
		DONTDISCARD Component inline* GetComponent(const size_t i) const;
		DONTDISCARD GameObject inline* AddChild(GameObject* const child);
		void inline AddTag(const char* flg) noexcept;
		void RemoveChild(GameObject* const obj);
		void AddComponent(Component* const obj);
		void RemoveComponent(Component* const obj);
		DONTDISCARD const bool HasTag(const char* flg) const noexcept;
		DONTDISCARD const Array<std::string> inline& GetTagsRef() const noexcept;
		void RemoveTag(const char* flg);
		void ToJson(nlohmann::json& val) const;
		void FromJson(const nlohmann::json& val);
		void LoadFromPrefab(const std::string& mPath);
		void RemoveChildParentship(GameObject* const obj);
		DONTDISCARD const char inline* GetType() const noexcept;
		glm::mat4& GetWorldMatrix();
		GameObject* InstantiateThisObject();
#pragma endregion

#pragma region // Members
	private:
		Math::Transform* mTransform;
		Array<GameObject*> mChilds;
		Array<Component*> mComponents;
		Array<std::string> mTags;
		std::string mType;
		GameObject* mParent;
		glm::mat4 mWorldTransformMtrx;
		void inline handle_event(const events::Event&) noexcept;
		DONTDISCARD glm::mat4 inline GetParentTransformMtx() const;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Get Transform
	*
	*   Removes a Component from our Childs
	*/ //--------------------------------------------------------------------
	Math::Transform inline GameObject::GetTransform() const noexcept {
		return *mTransform;
	}

	// ------------------------------------------------------------------------
	/*! Get Parent
	*
	*   Gets the Parent of the Actor
	*/ //--------------------------------------------------------------------
	GameObject inline* GameObject::GetParent() const noexcept {
		return mParent;
	}

	// ------------------------------------------------------------------------
	/*! Get Component
	*
	*   Gets the Components of a Game Object given the index
	*/ //--------------------------------------------------------------------
	Component inline* GameObject::GetComponent(const size_t i) const {
		return mComponents.at(i);
	}

	// ------------------------------------------------------------------------
	/*! Get Parent Transform Matrix
	*
	*   Returns the parent's transformation matrix
	*/ //----------------------------------------------------------------------
	glm::mat4 inline GameObject::GetParentTransformMtx() const {
		glm::mat4 t(1.f);
		if (mParent) {
			t = mParent->GetTransform().GetModelMtx();
			for (GameObject* temp = mParent->mParent; temp; temp = temp->GetParent())
				t = temp->GetTransform().GetModelMtx() * t;
		}
		return t;
	}

	// ------------------------------------------------------------------------
	/*! Get World Scale
	*
	*   Retrieves the world scale
	*/ //----------------------------------------------------------------------
	DONTDISCARD Math::Vector3D inline GameObject::GetWorldScale() const noexcept {
		return GetScale();
	}

	// ------------------------------------------------------------------------
	/*! Get World Scale
	*
	*   Retrieves the world scale
	*/ //----------------------------------------------------------------------
	DONTDISCARD Math::Transform inline GameObject::GetWorldTransform() const noexcept {
		glm::mat4 world_matrix = GetParentTransformMtx() * mTransform->GetModelMtx();
		glm::vec3 translation, rotation, scale;
		Math::fast_decompose_transfom(world_matrix, translation, rotation, scale);
		return { translation, rotation, scale };
	}
	// ------------------------------------------------------------------------
	/*! ComputeWorldMatrix
	*
	*   Computes matrix
	*/ //----------------------------------------------------------------------
	DONTDISCARD glm::mat4 inline GameObject::ComputeWorldMatrix() noexcept
	{
		mWorldTransformMtrx = GetParentTransformMtx() * mTransform->GetModelMtx();
		for (auto& it : mChilds)
			it->ComputeWorldMatrix();

		return mWorldTransformMtrx;
	}

	// ------------------------------------------------------------------------
	/*! Get Child
	*
	*   Gets the Child of a Game Object given the index
	*/ //--------------------------------------------------------------------
	GameObject inline* GameObject::GetChild(const size_t i) const noexcept {
		return mChilds[i];
	}

	// ------------------------------------------------------------------------
	/*! Get position
	*
	*   Returns the position of the Game Object
	*/ //--------------------------------------------------------------------
	Math::Vector3D inline GameObject::GetPosition() const noexcept {
		return mTransform->mPos;
	}

	// ------------------------------------------------------------------------
	/*! Get Rotation
	*
	*   Returns the Rotation of a Game Object
	*/ //--------------------------------------------------------------------
	Math::Vector3D inline GameObject::GetRotation() const noexcept {
		return mTransform->mRot;
	}

	// ------------------------------------------------------------------------
	/*! Get Scale
	*
	*   Gets the Scale of a Game Object
	*/ //--------------------------------------------------------------------
	Math::Vector3D inline GameObject::GetScale() const noexcept {
		return mTransform->mScale;
	}

	// ------------------------------------------------------------------------
	/*! Get Child
	*
	*   Gets a Child contained within the Actor given it's name
	*/ //--------------------------------------------------------------------
	template<typename Ty_>
	Ty_* GameObject::GetChild(const char* name) const noexcept {
		//Seek through the childs
		for (auto& x : mChilds)
			//If we find an object with the same name
			if (!strcmp(x->GetName(), name))
				return Cast<Ty_>(x);
		return nullptr;
	}

	// ------------------------------------------------------------------------
	/*! Get Child
	*
	*   Gets a child from the Actor given it's index
	*/ //--------------------------------------------------------------------
	template<typename Ty_>
	Ty_ inline* GameObject::GetChild(const size_t idx) const noexcept {
		return Cast<Ty_>(mChilds.at(idx));
	}

	// ------------------------------------------------------------------------
	/*! Get Component
	*
	*   Gets a Child contained within the Actor given it's name
	*/ //--------------------------------------------------------------------
	template<typename Ty_>
	Ty_* GameObject::GetComponent(const char* name) const noexcept {
		//Seek through the childs
		for (auto& x : mComponents)

			//If we find an object with the same name
			if (!strcmp(x->GetName(), name))
				return Cast<Ty_>(x);
		return nullptr;
	}

	// ------------------------------------------------------------------------
	/*! Get Component By Type
	*
	*   Gets a Component by a given type
	*/ //--------------------------------------------------------------------
	template<typename Ty_>
	Ty_* GameObject::GetComponentByType() const noexcept {
		const auto& rtti = *GRTTI;

		//For each component
		for (auto& x : mComponents)
			//If it's of type Ty_
			if (rtti.IsA<Ty_>(x))
				return Cast<Ty_>(x, cast::opt);
		return nullptr;
	}

	// ------------------------------------------------------------------------
	/*! Get Child
	*
	*   Gets a child from the Actor given it's index
	*/ //--------------------------------------------------------------------
	template<typename Ty_>
	const Ty_ inline* GameObject::GetComponent(const size_t idx) const noexcept {
		return Cast<Ty_>(mComponents.at(idx));
	}

	// ------------------------------------------------------------------------
	/*! Get Child Count
	*
	*   Gets the count of how many children are there
	*/ //--------------------------------------------------------------------
	size_t inline GameObject::GetChildCount() const noexcept {
		return mChilds.size();
	}

	// ------------------------------------------------------------------------
	/*! Get Component Count
	*
	*   Gets the count of how many components are there
	*/ //--------------------------------------------------------------------
	size_t inline GameObject::GetComponentCount() const noexcept {
		return mComponents.size();
	}

	// ------------------------------------------------------------------------
	/*! Add Sub Object
	*
	*   Adds a SubObject to this actor
	*/ //--------------------------------------------------------------------
	template<typename... Args>
	GameObject* GameObject::AddChild(const char* name, Args... args) {
		PageAllocator<GameObject> alloc;
		GameObject* temp_ = alloc.allocate();
		alloc.construct(temp_, args...);

		temp_->SetName(name ? name : "No Name");
		temp_->mParent = this;
		temp_->Construct();

#ifdef _EDITOR_
		if (GEditor->IsPlaying())
#endif
			temp_->BeginPlay();
		mChilds.emplace_back(temp_);

		return temp_;
	}

	// ------------------------------------------------------------------------
	/*! Add Sub Object
	*
	*   Adds a SubObject to this actor
	*/ //--------------------------------------------------------------------
	GameObject inline* GameObject::AddChild(GameObject* const child) {
		
		child->mParent = this;

#ifdef _EDITOR_
		//if (GEditor->IsPlaying())
#endif
		mChilds.emplace_back(child);

		return child;
	}

	// ------------------------------------------------------------------------
	/*! Has Component Of Type
	*
	*   Retusn wether a Object has a component of type or not
	*/ //--------------------------------------------------------------------
	template<class K>
	bool GameObject::HasComponentOfType() const noexcept {
		const auto& rtti = *GRTTI;

		for (auto& x : mComponents)
			if (rtti.IsA<K>(x)) return true;
		return false;
	}

	// ------------------------------------------------------------------------
	/*! Add Sub Object
	*
	*   Adds a SubObject to this actor
	*/ //--------------------------------------------------------------------
	template<typename Ty_, typename... Args>
	Ty_* GameObject::AddComponent(const char* name, Args... args) {
		static PageAllocator<Ty_> alloc;
		Ty_* temp_ = alloc.allocate();
		alloc.construct(temp_, args...);

		temp_->SetName(name ? name : "No Name");
		temp_->mOwner = this;
		temp_->Construct();

#ifdef _EDITOR_
		if (GEditor->IsPlaying())
#endif
			temp_->BeginPlay();
		mComponents.emplace_back(temp_);

		return temp_;
	}

	// ------------------------------------------------------------------------
	/*! Add Tag
	*
	*   Adds a Tag to the base class
	*/ //--------------------------------------------------------------------
	void inline GameObject::AddTag(const char* flg) noexcept {
		mTags.emplace_back(flg);
	}

	// ------------------------------------------------------------------------
	/*! GetTags
	*
	*   Returns array of tags
	*/ //----------------------------------------------------------------------
	const Array<std::string> inline& GameObject::GetTagsRef() const noexcept {
		return mTags;
	}

	// ------------------------------------------------------------------------
	/*! Get Type
	*
	*   Gets Type of Object
	*/ //----------------------------------------------------------------------
	const char inline* GameObject::GetType() const noexcept {
		return mType.c_str();
	}

	// ------------------------------------------------------------------------
	/*! handle_event
	*
	*   EMPTY FUNCTION
	*/ //----------------------------------------------------------------------
	void inline GameObject::handle_event(const events::Event&) noexcept {}

	using Object = GameObject;
}

#endif