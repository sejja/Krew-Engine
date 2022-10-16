//
//	Widget.cpp
//	Good Neighbours
//
//	Created by Adrian Montes on 29/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

namespace Engine
{
	namespace UI
	{
#pragma region General Widget Class
		Widget::Widget() noexcept :
			mHUD(nullptr) {}

		// ----------------------------------------------------------------------
		/*! Display
		*
		*   Sets widget to be displayed on hud
		*/
		//--------------------------------------------------------------------
		void Widget::Display() noexcept
		{
			mDisplayed = true;
		}
		// ----------------------------------------------------------------------
		/*! Hide
		*
		*   Sets widget to be hidden on hud
		*/ //--------------------------------------------------------------------
		void Widget::Hide() noexcept
		{
			mDisplayed = false;
		}
		// ----------------------------------------------------------------------
		/*! GetHUD
		*
		*   Gets widget hud
		*/ //--------------------------------------------------------------------
		HUD* Widget::GetHUD() const noexcept
		{
			return mHUD;
		}
		// ----------------------------------------------------------------------
		/*! IsDisplayed
		*
		*   Checks if is displayed on HUD
		*/ //--------------------------------------------------------------------
		bool Widget::IsDisplayed() const noexcept
		{
			return mDisplayed;
		}
		// ----------------------------------------------------------------------
		/*! SetParent
		*
		*   Sets hud of widget
		*/ //--------------------------------------------------------------------
		void Widget::SetHUD(HUD* h) noexcept
		{
			mHUD = h;
		}
#pragma region Widget Transform functions
		// ----------------------------------------------------------------------
		/*! GetTransform
		*
		*   Gets widget transform
		*/ //--------------------------------------------------------------------
		Math::Transform Widget::GetTransform() const noexcept
		{
			return mTransform;
		}
		// ----------------------------------------------------------------------
		/*! SetTransform
		*
		*   Sets widget transform
		*/ //--------------------------------------------------------------------
		void Widget::SetTransform(const Math::Transform& transform) noexcept
		{
			mTransform = transform;
		}
		// ----------------------------------------------------------------------
		/*! SetPosition
		*
		*   Sets widget position
		*/ //--------------------------------------------------------------------
		void Widget::SetPosition(const Math::Vector3D& pos) noexcept
		{
			mTransform.mPos = pos;
		}
		// ----------------------------------------------------------------------
		/*! SetScale
		*
		*   Sets widget scale
		*/ //--------------------------------------------------------------------
		void Widget::SetScale(const Math::Vector3D& sca) noexcept
		{
			mTransform.mScale = sca;
		}
		// ----------------------------------------------------------------------
		/*! SetRotation
		*
		*   Sets widget rotation
		*/ //--------------------------------------------------------------------
		void Widget::SetRotation(const Math::Vector3D& rot) noexcept
		{
			mTransform.mRot = rot;
		}
#pragma endregion
#pragma endregion

#pragma region Image Widget Class
		// ----------------------------------------------------------------------
		/*! ImageW
		*
		*   Default image constructor (set default image later)
		*/ //--------------------------------------------------------------------
		ImageW::ImageW() : mImage(new MeshRenderer(true)), mTexture(nullptr)
		{
			mImage->SetBlendingType(BlendingType::eOpaque);
			mImage->SetMeshByName("Plane");
			Display();
		}
		// ----------------------------------------------------------------------
		/*! ImageW
		*
		*   Custom image constructor
		*/ //--------------------------------------------------------------------
		ImageW::ImageW(const char* image) : mImage(new MeshRenderer(true))
		{
			mImage->SetBlendingType(BlendingType::eOpaque);
			mImage->SetMeshByName("Plane");
			SetImage(image);
			Display();
		}
		// ----------------------------------------------------------------------
		/*! ~ImageW
		*
		*   Default destructor
		*/ //--------------------------------------------------------------------
		ImageW::~ImageW()
		{
		}
		// ----------------------------------------------------------------------
		/*! SetImage
		*
		*   Changes widget image
		*/ //--------------------------------------------------------------------
		void ImageW::SetImage(const char* image) noexcept
		{
			if (!mTexture)
				mTexture = GContent->GetResource<Graphics::Texture>(image);
			else
				mTexture->Get()->LoadFromFile(image);
			mTexture->Get()->UploadToGPU();
			mImage->SetTexture(mTexture);
		}
		// ----------------------------------------------------------------------
		/*! Update
		*
		*   On image, does nothing
		*/ //--------------------------------------------------------------------
		void ImageW::Update() const noexcept
		{
		}
		// ----------------------------------------------------------------------
		/*! Render
		*
		*   If displayed, renders image on screen
		*/ //--------------------------------------------------------------------
		void ImageW::Render() const noexcept
		{
			//If not displayed dont draw
			if (!mDisplayed)
				return;

			//Set texture and draw
			mImage->RenderHUDs(mTransform.GetModelMtx());
		}
#pragma endregion

#pragma region Text Widget Class
		// ----------------------------------------------------------------------
		/*! TextW
		*
		*   Default constructor
		*/ //--------------------------------------------------------------------
		TextW::TextW() : mText(new Engine::Text3D(true))
		{
			mText->mTextColor = { 0,0,0,1.0f };
			Display();
		}
		// ----------------------------------------------------------------------
		/*! TextW
		*
		*   Custom constructor with text
		*/ //--------------------------------------------------------------------
		TextW::TextW(const char* txt) : mText(new Engine::Text3D(true))
		{
			mText->mTextColor = { 0,0,0,1.0f };
			mText->mText = txt;
			Display();
		}
		// ----------------------------------------------------------------------
		/*! TextW
		*
		*   Default destructor
		*/ //--------------------------------------------------------------------
		TextW::~TextW()
		{
			delete mText;
		}
		// ----------------------------------------------------------------------
		/*! SetText
		*
		*   Changes actual text
		*/ //--------------------------------------------------------------------
		void TextW::SetText(const char* txt) noexcept
		{
			//mText->mText = txt;
		}
		// ----------------------------------------------------------------------
		/*! SetTextColor
		*
		*   Set new text color
		*/ //--------------------------------------------------------------------
		void TextW::SetTextColor(const glm::vec4& color) noexcept
		{
			mText->mTextColor = color;
		}
		// ----------------------------------------------------------------------
		/*! SetTextFont
		*
		*   Set new text font
		*/ //--------------------------------------------------------------------
		void TextW::SetTextFont(const char* path) noexcept
		{
			mText->mFontFilePath = path;
		}
		// ----------------------------------------------------------------------
		/*! Update
		*
		*   On text, does nothing
		*/ //--------------------------------------------------------------------
		void TextW::Update() const noexcept
		{
		}
		// ----------------------------------------------------------------------
		/*! Render
		*
		*   If displayed, renders text on screen
		*/ //--------------------------------------------------------------------
		void TextW::Render() const noexcept
		{
			mText->RenderHUDs(mTransform.GetModelMtx());
		}
#pragma endregion
#pragma region Button Widget Class
		// ----------------------------------------------------------------------
		/*! ButtonW
		*
		*   Default button widget constructor
		*/ //--------------------------------------------------------------------
		ButtonW::ButtonW() : mImage(new ImageW()), mText(nullptr), mInput(VK_LBUTTON)
		{
			Display();
		}
		// ----------------------------------------------------------------------
		/*! ~ButtonW
		*
		*   Default button widget destructor
		*/ //--------------------------------------------------------------------
		ButtonW::~ButtonW()
		{
			if (mImage)
				delete mImage;
			if (mText)
				delete mText;
		}
		// ----------------------------------------------------------------------
		/*! SetButtonEvent
		*
		*   Sets event to do if button is pressed
		*/ //--------------------------------------------------------------------
		void ButtonW::SetButtonEvent(events::Event* e) noexcept
		{
			mEvent = e;
		}
		// ----------------------------------------------------------------------
		/*! SetMouseInput
		*
		*   Sets mouse button to use on button
		*/ //--------------------------------------------------------------------
		void ButtonW::SetMouseInput(int i) noexcept
		{
			if (i != VK_LBUTTON || i != VK_RBUTTON)
				return;
			mInput = i;
		}
		// ----------------------------------------------------------------------
		/*! SetPosition
		*
		*   Button position settor
		*/ //--------------------------------------------------------------------
		void ButtonW::SetPosition(const Math::Vector3D& pos) noexcept
		{
			mTransform.mPos = pos;
			if (mImage)
				mImage->SetPosition(pos);
			if (mText)
				mText->SetPosition(pos - glm::vec3(mTransform.mScale.x * 0.4f, 0.0f,0.0f));
		}
		// ----------------------------------------------------------------------
		/*! SetScale
		*
		*   Sets button scale, setting also image and text scales
		*/ //--------------------------------------------------------------------
		void ButtonW::SetScale(const Math::Vector3D& sca) noexcept
		{
			mTransform.mScale = sca;
			if (mImage)
				mImage->SetScale(sca);
			if (mText)
				mText->SetScale(sca / 40.f);
		}
		// ----------------------------------------------------------------------
		/*! SetImage
		*
		*   Sets button image
		*/ //--------------------------------------------------------------------
		void ButtonW::SetImage(const char* image) noexcept
		{
			mImage->SetImage(image);
		}
		// ----------------------------------------------------------------------
		/*! SetText
		*
		*   Sets button text
		*/ //--------------------------------------------------------------------
		void ButtonW::SetText(const char* txt) noexcept
		{
			if (!mText)
				mText = new TextW();
			mText->SetText(txt);
		}
		// ----------------------------------------------------------------------
		/*! Update
		*
		*   Updates button
		*/ //--------------------------------------------------------------------
		void ButtonW::Update() const noexcept
		{
			if (!mDisplayed)
				return;

			if (GInput->IsKeyPressed(mInput))				
			{
				//Get mouse position and window and frame buffer sizes
				glm::vec2 screen_m(GInput->GetMousePosition().x, GInput->GetMousePosition().y);
				glm::vec2 s_size(GWindow->GetDimensions().x, GWindow->GetDimensions().y);

				//Compute mouse position on OpenGL pixels
				glm::vec2 pixel_m = screen_m / s_size;
				glm::vec2 offset(pixel_m.x / 7.5f, (1 - pixel_m.y) / 7.5f);
				glm::vec3 win((pixel_m.x * 2.f - 1.f) - offset.x, (1.f - 2.f * pixel_m.y) - offset.y, 0.0f);

				//Convert position to [0,1] of screen
				glm::vec3 world = glm::vec3(mTransform.mPos.x, mTransform.mPos.y, 0.0f);
				if (StaticPointToStaticRect(win, world, mTransform.mScale.x, mTransform.mScale.y, mTransform.mScale.z))
					//	//Start event
					mText->SetText("clicked!!");
			}
		}
		// ----------------------------------------------------------------------
		/*! Render
		*
		*   Renders image and text
		*/ //--------------------------------------------------------------------
		void ButtonW::Render() const noexcept
		{
			if (!mDisplayed)
				return;

			//Render both things
			if (mText)
				mText->Render();
			if (mImage)
				mImage->Render();
		}
#pragma endregion
	}
}