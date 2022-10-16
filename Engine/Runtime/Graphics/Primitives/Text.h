#pragma once

namespace Engine
{
	struct Text3D : public RenderComp
	{
		friend class GraphicsPipeline;
		struct Character
		{
			GLuint mID;				  // Character texture
			glm::ivec2   mSize;       // Size of glyph
			glm::ivec2   mBearing;    // Offset from baseline to left/top of glyph
			unsigned mAdvance;        // Offset
		};
		std::map<char, Character> mCharacters; //Each char is mapped to a texture

		//Set-up functions
		void LoadCharacterMap();
		void InitBuffers();

		void SetFontPath(const std::string& path);
		void Render();
		void RenderHUDs(glm::mat4 world);

		//Serialization
		void FromJson(const nlohmann::json& val);
		void ToJson(nlohmann::json& val) const;

		Text3D(bool isHUD = false);
		~Text3D();

		void OnEditor() override;

		bool isHUD = false;
		bool mVisible = true;
		glm::vec4 mTextColor = glm::vec4(1.f);
		std::string mText = "Text";
		std::string mFontFilePath = "EngineContent/Fonts/CourierPrime-Regular.ttf";

		Graphics::Texture* mTexture = nullptr;
		Graphics::ShaderProgram* mProgram = nullptr;

		//Buffers / vertex arrays
		GLuint mVAO = 0;
		GLuint mVBO = 0;

		glm::vec2 mTextPos = glm::vec2(0.f);
		float mTextScale = 10.0f;

	};
}

