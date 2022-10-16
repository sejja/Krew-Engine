#include <Shared.h>

namespace Engine
{
	Camera::Camera() : mFoV(45.0f), mAspect(GWindow->GetDimensions().x / GWindow->GetDimensions().y), mNear(0.1f), mFar(1000.0f), mPitch(0.0f), mYaw(-90.0f), mSensitivity(3.f),
					   mLastMousePosX(GWindow->GetDimensions().x), mLastMousePosY(GWindow->GetDimensions().y), mFirstMouse(true), mCamLocked(false), mPos{ 0,10,30 },
					   /*mOffset{ 0,0,0 },*/ mViewVec{ 0,0,-1 }, mUpVec{ 0,1,0 }, mtxView{}, mtxProj{}
	{}

	void Camera::BeginPlay()
	{
		GGfxPipeline->AddCamera(this);
	}

	void Camera::Update()
	{
		GameObject* parent = nullptr;

		if (!mSpline.SplineIsEmpty())
		{
			if (!mSpline.GetStart())
			{
				mSpline.ResetSpline();
				mSpline.StartInterpolate();
				mSpline.SetStart(true);
			}

			mSpline.UpdateInterpolate();
			mPos = mSpline.GetPos();
			GetOwner()->SetPosition(mPos);
			mYaw = GetOwner()->GetRotation().y - 90.0f;
			mPitch = GetOwner()->GetRotation().z;
		}
		else
		{
			if (GetOwner()) 
			{
				mPos = GetOwner()->GetWorldPosition();
				mYaw = GetOwner()->GetRotation().y - 90.0f;
				mPitch = GetOwner()->GetRotation().z;
				parent = GetOwner()->GetParent();
			}
			else 
			{
				glm::vec2 mousePosition = GInput->GetMousePosition();
				glm::vec2 windowDim = GWindow->GetDimensions();

				if (!mCamLocked)
				{
					if (mFirstMouse)
					{
						mLastMousePosX = mousePosition.x;
						mLastMousePosY = mousePosition.y;
						mFirstMouse = false;
					}

					float xOffset = mousePosition.x - mLastMousePosX;
					float yOffset = mLastMousePosY - mousePosition.y;
					mLastMousePosX = mousePosition.x;
					mLastMousePosY = mousePosition.y;

					xOffset *= mSensitivity / 15;
					yOffset *= mSensitivity / 15;

					mYaw += xOffset;
					mPitch += yOffset;

					if (mPitch > 89.0f)
						mPitch = 89.0f;
					if (mPitch < -89.0f)
						mPitch = -89.0f;
				}
			}
		}

		glm::vec3 dir;

		if (parent)
		{
			dir.x = parent->GetWorldPosition().x - GetOwner()->GetWorldPosition().x;
			dir.y = parent->GetWorldPosition().y - GetOwner()->GetWorldPosition().y;
			dir.z = parent->GetWorldPosition().z - GetOwner()->GetWorldPosition().z;
		}
		else
		{
			dir.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
			dir.y = sin(glm::radians(mPitch));
			dir.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		}
		
		mViewVec = glm::normalize(dir);

		mtxView = glm::lookAt(mPos, mPos + mViewVec , mUpVec);
		mtxProj = glm::perspective(glm::radians(mFoV), mAspect, mNear, mFar);
	}		

	void Camera::Destroy(){
		GGfxPipeline->DelCamera(this);
	}
	
	void Camera::OnEditor()
	{
#ifdef _EDITOR_
		ImGui::DragFloat("Sensitivity", &mSensitivity, 0.1f, 0.2f, 1.0f);

		ImGui::DragFloat("Aspect Ratio", &mAspect, 1.0f, 4.0f, 16.0f);

		ImGui::DragFloat("Zoom", &mFoV, 10.0f, 45.0f, 100.0f);

		//ImGui::DragFloat3("Offset", reinterpret_cast<float*>(&mOffset), 0.0f, 2000.0f);
#endif
	}


	void Camera::SetFov(const float fov){
		mFoV = fov;
	}

	void Camera::SetAspect(const float asp){
		mAspect = asp;
	}

	void Camera::SetNear(const float n) {
		mNear = n;
	}

	void Camera::SetFar(const float f) {
		mFar = f;
	}

	void Camera::SetPitch(const float p) {
		mPitch = p;
	}

	void Camera::SetYaw(const float y) {
		mYaw = y;
	}

	void Camera::SetFirstMouse(const bool f) {
		mFirstMouse = f;
	}

	void Camera::SetLocked(const bool l)
	{
		mCamLocked = l;
	}

	void Camera::SetPos(const glm::vec3& pos) {
		mPos = pos;
	}
	/*
	void Camera::SetOffset(const glm::vec3& offset) {
		mOffset = offset;
	}*/

	void Camera::SetViewVec(const glm::vec3& viewvec) {
		mViewVec = viewvec;
	}

	void Camera::SetUpVec(const glm::vec3& upvec){
		mUpVec = upvec;
	}

	void Camera::SetViewMtx(const glm::mat4& mtx) {
		mtxView = mtx;
	}

	void Camera::SetProjMtx(const glm::mat4& mtx)
	{
		mtxProj = mtx;
	}

	void Camera::SetSensitivity(const float f) {
		mSensitivity = f;
	}

	void Camera::AddSpline(const Math::Spline& sp)
	{
		mSpline = sp;
	}

	float Camera::GetFov() const {
		return mFoV;
	}

	float Camera::GetAspect() const {
		return mAspect;
	}

	float Camera::GetNear() const {
		return mNear;
	}

	float Camera::GetFar() const {
		return mFar;
	}

	float Camera::GetPitch() const {
		return mPitch;
	}

	float Camera::GetYaw() const {
		return mYaw;
	}

	float Camera::GetSensitivity() const
	{
		return mSensitivity;
	}

	bool Camera::GetFirstMouse() const {
		return mFirstMouse;
	}

	bool Camera::GetLocked() const
	{
		return mCamLocked;
	}

	glm::vec3 Camera::GetPos() const {
		return mPos;
	}
	/*
	glm::vec3 Camera::GetOffset() const{
		return mOffset;
	}*/

	glm::vec3 Camera::GetViewVec() const {
		return mViewVec;
	}

	glm::vec3 Camera::GetUpVec() const {
		return mUpVec;
	}

	glm::mat4 Camera::GetViewMtx() const
	{
		return mtxView;
	}
	glm::mat4 Camera::GetProjMtx() const
	{
		return mtxProj;
	}

	namespace {
		constexpr const char* sCommonStr[] = { "Position", "Offset", "ViewVec", "UpVec" };
		constexpr const size_t sPosition = 0, sOffset = 1, sView = 2, sUp = 3;
	}

	void Camera::FromJson(const nlohmann::json& val)
	{
		fetch(mFoV, val, "Fov");
		fetch(mAspect, val, "Aspect");
		fetch(mNear, val, "Near");
		fetch(mFar, val, "Far");
		fetch(mSensitivity, val, "Sensitivity");
		fetch(mYaw, val, "Yaw");
		fetch(mPitch, val, "Pitch");
		fetch(mLastMousePosX, val, "MouseX");
		fetch(mLastMousePosY, val, "MouseY");
		fetch(mFirstMouse, val, "FirstMouse");

		
		mPos = Serializers::MathSerializer::Vector3DFromJson(val[sCommonStr[sPosition]]);
		//mOffset = Serializers::MathSerializer::Vector3DFromJson(val[sCommonStr[sOffset]]);
		mViewVec = Serializers::MathSerializer::Vector3DFromJson(val[sCommonStr[sView]]);
		mUpVec = Serializers::MathSerializer::Vector3DFromJson(val[sCommonStr[sUp]]);

	}

	void Camera::ToJson(nlohmann::json& val) const
	{
		val["Fov"] = mFoV;
		val["Aspect"] = mAspect;
		val["Near"] = mNear;
		val["Far"] = mFar;
		val["Sensitivity"] = mSensitivity;
		val["Yaw"] = mYaw;
		val["Pitch"] = mPitch;
		val["MouseX"] = mLastMousePosX;
		val["MouseY"] = mLastMousePosY;
		val["FirstMouse"] = mFirstMouse;

		
		val[sCommonStr[sPosition]] = Serializers::MathSerializer::Vector3DToJson(mPos);
		//val[sCommonStr[sOffset]] = Serializers::MathSerializer::Vector3DToJson(mOffset);
		val[sCommonStr[sView]] = Serializers::MathSerializer::Vector3DToJson(mViewVec);
		val[sCommonStr[sUp]] = Serializers::MathSerializer::Vector3DToJson(mUpVec);
	}

	EditorCam::EditorCam()
	{
		mEditCam = new Camera;
		mEditCam->SetLocked(true);
	}

	EditorCam::~EditorCam()
	{
		if (mEditCam)
			delete mEditCam;
	}

	void EditorCam::ReadInputCam()
	{
		glm::vec3 pos = mEditCam->GetPos();
		glm::vec3 dir = mEditCam->GetViewVec();
		glm::vec3 right = glm::normalize(glm::cross(dir, mEditCam->GetUpVec()));

		glm::vec3 Pos1(1.5 * dir.x, 1.5 * dir.y, 1.5 * dir.z);
		glm::vec3 Pos2(1.5 * right.x, 1.5 * right.y, 1.5 * right.z);
		

		if (GInput->IsKeyDown(VK_RBUTTON) && mEditCam->GetLocked() == true)
		{
			mEditCam->SetLocked(false);
			mEditCam->SetFirstMouse(true);
		}
		else if (!GInput->IsKeyDown(VK_RBUTTON))
			mEditCam->SetLocked(true);

		if (GInput->IsKeyDown('W'))
			pos += Pos1 * (GetCam()->GetSensitivity() / 3);
		if (GInput->IsKeyDown('S'))
			pos -= Pos1 * (GetCam()->GetSensitivity() / 3);
		if (GInput->IsKeyDown('A'))
			pos -= Pos2 * (GetCam()->GetSensitivity() / 3);
		if (GInput->IsKeyDown('D'))
			pos += Pos2 * (GetCam()->GetSensitivity() / 3);
		if (GInput->IsKeyDown(VK_RCONTROL))
			pos -= glm::vec3(0.f, 1.f, 0.f) * (GetCam()->GetSensitivity() / 3);
		if (GInput->IsKeyDown(VK_SPACE))
			pos += glm::vec3(0.f,1.f,0.f) * (GetCam()->GetSensitivity() / 3);

		mEditCam->SetPos(pos);
	}

	void EditorCam::UpdateEditCam()
	{
		ReadInputCam();
		mEditCam->Update();
	}

	Camera* EditorCam::GetCam() const
	{
		return mEditCam;
	}
}