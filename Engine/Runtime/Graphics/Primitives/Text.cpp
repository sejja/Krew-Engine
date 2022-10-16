#include <Shared.h>

namespace Engine
{
    void Text3D::LoadCharacterMap()
    {
        //Init FT context
        FT_Library Lib;

#ifdef _DEBUG
        assertThis(!FT_Init_FreeType(&Lib), "Could not initialize font framework");

        //Load font / face from requested path
        FT_Face Face;
        assertThis(!FT_New_Face(Lib, mFontFilePath.c_str(), 0, &Face), "Could not create font faces");
#else
        FT_Init_FreeType(&Lib);

        //Load font / face from requested path
        FT_Face Face;
        FT_New_Face(Lib, mFontFilePath.c_str(), 0, &Face);
#endif

        //Set char size
        FT_Set_Pixel_Sizes(Face, 0, 48);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

        // Load characters starting from 
        for (unsigned char c = 0; c < 128; c++)
        {
            if (FT_Load_Char(Face, c, FT_LOAD_RENDER))
            {
                GConsole->Log(Console::eMessageType::e_Warning, "ERROR::FREETYTPE: Failed to load Glyph");
                continue;
            }
            // Generate character texture  
            GLuint Tex;
            glGenTextures(1, &Tex);
            glBindTexture(GL_TEXTURE_2D, Tex);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                Face->glyph->bitmap.width,
                Face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                Face->glyph->bitmap.buffer
            );

            // Texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Store the characters on the map to use later
            Character Char =
            {
                Tex,
                glm::ivec2(Face->glyph->bitmap.width, Face->glyph->bitmap.rows),
                glm::ivec2(Face->glyph->bitmap_left, Face->glyph->bitmap_top),
                Face->glyph->advance.x
            };
            mCharacters.insert(std::pair<char, Character>(c, Char));
        }

        //Free face & library data once we are done
        FT_Done_Face(Face);
        FT_Done_FreeType(Lib);
    }
    void Text3D::SetFontPath(const std::string& path)
    {
        mFontFilePath = path;
    }
    void Text3D::Render()
    {        
        if (!GetOwner())
            return;

        mProgram->SetShaderUniform("TextColor", &mTextColor);

        //Apply reduction factor to the scale
        const float Scale = mTextScale * 0.01f;

        glm::mat4 Model = GetOwner()->GetWorldMatrix();
        mProgram->SetShaderUniform("M", &Model);
        mProgram->SetShaderUniform("isHUD", 0);

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(mVAO);

        float CurrX = mTextPos.x;
        float CurrY = mTextPos.y;

        // Parse all the string, draw one quad per char
        std::string::const_iterator c;
        for (c = mText.begin(); c != mText.end(); c++)
        {
            Character ch = mCharacters[*c];

            float xpos = CurrX + ch.mBearing.x * Scale;
            float ypos = CurrY - (ch.mSize.y - ch.mBearing.y) * Scale;

            float w = ch.mSize.x * Scale;
            float h = ch.mSize.y * Scale;
            
            //Update VBO per char
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            // Render glyph
            glBindTexture(GL_TEXTURE_2D, ch.mID);

            // Refresh VBO for this char
            glBindBuffer(GL_ARRAY_BUFFER, mVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            //Draw char
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            //Leave some space to draw the next char
            CurrX += (ch.mAdvance >> 6)* Scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glBindVertexArray(0);
    }

    void Text3D::RenderHUDs(glm::mat4 world)
    {
        //Change blend function for rendering characters
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        mProgram->Bind();
        mProgram->SetShaderUniform("M", &world);
        glm::vec2 size = GWindow->GetDimensions() / 2.f;
        glm::mat4 ortho = glm::ortho(-size.x, size.x, -size.y, size.y);
        mProgram->SetShaderUniform("P", &ortho);
        mProgram->SetShaderUniform("TextColor", &mTextColor);
        mProgram->SetShaderUniform("isHUD", 1);

        //Apply reduction factor to the scale
        const float Scale = mTextScale * 0.01f;

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(mVAO);

        float CurrX = mTextPos.x;
        float CurrY = mTextPos.y;

        // Parse all the string, draw one quad per char
        std::string::const_iterator c;
        for (c = mText.begin(); c != mText.end(); c++)
        {
            Character ch = mCharacters[*c];

            float xpos = CurrX + ch.mBearing.x * Scale;
            float ypos = CurrY - (ch.mSize.y - ch.mBearing.y) * Scale;

            float w = ch.mSize.x * Scale;
            float h = ch.mSize.y * Scale;

            //Update VBO per char
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            // Render glyph
            glBindTexture(GL_TEXTURE_2D, ch.mID);
            // Refresh VBO for this char
            glBindBuffer(GL_ARRAY_BUFFER, mVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            //Draw char
            glDrawArrays(GL_TRIANGLES, 0, 6);

            //Leave some space to draw the next char
            CurrX += (ch.mAdvance >> 6)* Scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glBindVertexArray(0);
    }

    void Text3D::FromJson(const nlohmann::json& val)
    {
        //Relative position & scale
        fetch(mTextPos.x, val, "textPos.x");
        fetch(mTextPos.y, val, "textPos.y");
        fetch(mTextScale, val, "textScale");

        //Text & font path
        fetch(mFontFilePath, val, "fontPath");
        fetch(mText, val, "text");

        //Color
        fetch(mTextColor.r, val, "textColor.r");
        fetch(mTextColor.g, val, "textColor.g");
        fetch(mTextColor.b, val, "textColor.b");
        fetch(mTextColor.a, val, "textColor.a");
    }

    void Text3D::ToJson(nlohmann::json& val) const
    {
        val["textPos.x"] = mTextPos.x;
        val["textPos.y"] = mTextPos.y;
        val["textScale"] = mTextScale;

        val["textColor.r"] = mTextColor.r;
        val["textColor.g"] = mTextColor.g;
        val["textColor.b"] = mTextColor.b;
        val["textColor.a"] = mTextColor.a;

        val["text"] = mText;
        val["fontPath"] = mFontFilePath;
    }

    void Text3D::InitBuffers()
    {
        //Gen arrays / buffers
        glGenVertexArrays(1, &mVAO);
        glGenBuffers(1, &mVBO);

        glBindVertexArray(mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);

        //Quad: 4 vertices in 2D, two texture coordinates (UV) each
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    Text3D::Text3D(bool hud)
    {
        mProgram = GGfxPipeline->shaders.TextShader;
        LoadCharacterMap();
        InitBuffers();

        isHUD = hud;
        if(!isHUD)
            GGfxPipeline->AddText(this);
    }

    Text3D::~Text3D()
    {
        glDeleteBuffers(1, &mVBO);
        glDeleteVertexArrays(1, &mVAO);

        if (!isHUD)
            GGfxPipeline->DelText(this);
    }

    void Text3D::OnEditor()
    {
#ifdef _EDITOR_
        //Toggle visibility
        ImGui::Checkbox("Visible", &mVisible);

        //Change displayed text
        ImGui::InputText("Text", &mText);

        if (ImGui::InputText("Font", &mFontFilePath))
        {
            //Reload char map based on fonts
            mCharacters.clear();
            LoadCharacterMap();
        }

        ImGui::InputFloat2("Position respect to obj", &mTextPos[0]);
        ImGui::InputFloat("Text scale: ", &mTextScale);

        //Change text color
        ImGui::ColorPicker4("Text color", &mTextColor[0]);
#endif
    }
}

