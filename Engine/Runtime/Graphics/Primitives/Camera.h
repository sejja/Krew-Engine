//
//	Camera.h
//	Good Neighbours
//
//	Created by Fernando Uribe on 09/24/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _CAMERA__H_
#define _CAMERA__H_

namespace Engine {

	struct Camera : public Component
	{
		Camera();
		
		void BeginPlay() override;
		void Destroy() override;
		void Update();
		void OnEditor() override;

		void SetFov(const float fov);
		void SetAspect(const float asp);
		void SetNear(const float n);
		void SetFar(const float f);
		void SetPitch(const float p);
		void SetYaw(const float y);

		void SetFirstMouse(const bool f);
		void SetLocked(const bool l);

		void SetPos(const glm::vec3& pos);
		//void SetOffset(const glm::vec3& offset);
		void SetViewVec(const glm::vec3& viewvec);
		void SetUpVec(const glm::vec3& upvec);
		void SetViewMtx(const glm::mat4& mtx);
		void SetProjMtx(const glm::mat4& mtx);
		void SetSensitivity(const float f);

		void AddSpline(const Math::Spline& sp);

		float GetFov() const;
		float GetAspect() const;
		float GetNear() const;
		float GetFar() const;
		float GetPitch() const;
		float GetYaw() const;
		float GetSensitivity() const;

		bool GetFirstMouse() const;
		bool GetLocked() const;

		glm::vec3 GetPos() const;
		//glm::vec3 GetOffset() const;
		glm::vec3 GetViewVec() const;
		glm::vec3 GetUpVec() const;
		glm::mat4 GetViewMtx() const;
		glm::mat4 GetProjMtx() const;

		void FromJson(const nlohmann::json& val);
		void ToJson(nlohmann::json& val) const;

	private:

		float mFoV;
		float mAspect;
		float mNear;
		float mFar;
		float mSensitivity;
		float mPitch;
		float mYaw;
		float mLastMousePosX;
		float mLastMousePosY;

		bool mFirstMouse;
		bool mCamLocked;

		glm::vec3 mPos;
		//glm::vec3 mOffset;
		glm::vec3 mViewVec;
		glm::vec3 mUpVec;

		glm::mat4 mtxView;
		glm::mat4 mtxProj;

		Math::Spline mSpline;
	};

	struct EditorCam
	{
		EditorCam();
		~EditorCam();
		void UpdateEditCam();
		void ReadInputCam();

		Camera* GetCam() const;

		private:
			Camera* mEditCam;
	};

}
#endif
