//
//	Widget.h
//	Good Neighbours
//
//	Created by Adrian Montes on 29/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _WIDGET_H_
#define _WIDGET_H_

namespace Engine
{
	struct MeshRenderer;
	struct Text3D;
	namespace UI
	{
		class ENGINE_API Widget
		{
			friend struct GraphicsPipeline;
		public:
#pragma region //Constructors & Destructors
			Widget() noexcept;
#pragma endregion
#pragma region // Settors and Gettors
			void Display() noexcept;
			void Hide() noexcept;
			void SetHUD(HUD* h) noexcept;
			bool IsDisplayed() const noexcept;
			HUD* GetHUD() const noexcept;
#pragma endregion
			virtual void Update() const noexcept = 0;
			virtual void Render() const noexcept = 0;
#pragma region // Transform functions
			Math::Transform GetTransform() const noexcept;
			void SetTransform(const Math::Transform& transform) noexcept;
			virtual void SetPosition(const Math::Vector3D& pos) noexcept;
			virtual void SetScale(const Math::Vector3D& sca) noexcept;
			void SetRotation(const Math::Vector3D& rot) noexcept;
#pragma endregion
		protected:
			HUD* mHUD;
			bool mDisplayed = true;
			Math::Transform mTransform;
		};

		class ENGINE_API ImageW : public Widget
		{
		public:
			ImageW();
			ImageW(const char* image);
			~ImageW();
			void SetImage(const char* image) noexcept;
			void Update() const noexcept;
			void Render() const noexcept;
		private:
			MeshRenderer* mImage;
			Asset<Graphics::Texture> mTexture;
		};

		class ENGINE_API TextW : public Widget
		{
		public:
			TextW();
			TextW(const char* txt);
			~TextW();
			void SetText(const char* txt) noexcept;
			void SetTextColor(const glm::vec4& color) noexcept;
			void SetTextFont(const char* path) noexcept;
			void Update() const noexcept;
			void Render() const noexcept;
		private:
			Text3D* mText;
		};

		class ENGINE_API ButtonW : public Widget
		{
		public:
			ButtonW();
			~ButtonW();
#pragma region //Settors
			void SetButtonEvent(events::Event* e) noexcept;
			void SetMouseInput(int i) noexcept;
			void SetPosition(const Math::Vector3D& pos) noexcept;
			void SetScale(const Math::Vector3D& sca) noexcept;
			void SetImage(const char* image) noexcept;
			void SetText(const char* txt) noexcept;
#pragma endregion
			void Update() const noexcept;
			void Render() const noexcept;
		private:
			ImageW* mImage;
			TextW* mText;
			int mInput;
			events::Event* mEvent;
		};
	}
}
#endif // !_WIDGET_H_
