/* -----------------------------------------------------------------------------------
    Copyright (C) 2021 Diego Revilla Institute of Technology.
    Reproduction or disclosure of this file or its contents without the prior written
    consent of Diego Revilla Institute of Technology is prohibited.

    File: Skybox.cpp
    Author: Javier Madariaga (javier.madariaga)
    Creation date: 09/29/2021
    Brief:
        Definition of functions & methods declared in class Skybox.

------------------------------------------------------------------------------------*/

#include <Shared.h>

Engine::Skybox::SkyboxComp::SkyboxComp()
{
    //Start shader program
    mProgram = GGfxPipeline->shaders.SkyboxShader;

    //Load mesh to draw onto
    OBJLoader load;
    mMesh = load.CreateCube();
    mMesh->SetupMesh();

    SetCubeMapPath("EngineContent/Texture/SkyBox/Stars/");
    CreateCubeMap();

    mCamMtx = &GGfxPipeline->mtxCam;
    mProjMtx = &GGfxPipeline->mtxProj;
    GGfxPipeline->SetSkybox(this);
}

Engine::Skybox::SkyboxComp::~SkyboxComp()
{

}
//
///**********************************************
//* fn	Construct
//* brief
//*   Sets up the Skybox object
//**********************************************/
//void Engine::Skybox::Construct()
//{
//    
//}
//
///**********************************************
//* fn	BeginPlay
//* brief
//*   Initializes shape's internal data
//**********************************************/
//void Engine::Skybox::BeginPlay()
//{
//
//}


/**********************************************
* fn	FromJson
* brief
*   Recovers the values from a .json file
* param val : .json file to read from
**********************************************/
void Engine::Skybox::SkyboxComp::FromJson(const nlohmann::json& val) noexcept
{
    fetch(mVisible, val, "mSamplingType");
    fetch(mMoving, val, "mMoving");
    fetch(speed, val, "speed");
    if (val.find("axis") != val.end())
        for (int i = 0; i < 3; ++i)
            axis[i] = val["axis"][i];
    fetch(mCubeMapPath, val, "CubeMapPath");
    CreateCubeMap();
}

/**********************************************
* fn	ToJson
* brief
*   Stored a Skybox obj.'s data into a .json file
* param val : .json file to write onto
**********************************************/
void Engine::Skybox::SkyboxComp::ToJson(nlohmann::json& val) const noexcept
{
    val["Visible"] = mVisible;
    val["mMoving"] = mMoving;
    val["speed"] = speed;
    for (int i = 0; i < 3; ++i)
        val["axis"][i] = axis[i];
    val["CubeMapPath"] = mCubeMapPath;
}

/**********************************************
* fn	SetCubeMapPath
* brief
*   Sets a new path to search for cube map textures
* param path : file path to the folder with the required
        textures
**********************************************/
void Engine::Skybox::SkyboxComp::SetCubeMapPath(std::string path)
{
    mCubeMapPath = path;
}

void Engine::Skybox::SkyboxComp::CreateCubeMap()
{ 
    unsigned MapSize = 512;

    std::string Faces[6] = 
    {
        "Right.png",
        "Left.png",
        "Top.png",
        "Bottom.png",
        "Front.png",
        "Back.png"
    };

    glGenTextures(1, &mMapHandle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mMapHandle);

    int Width = 0;
    int Height = 0;
    int Format = 0;

    //Load 6 images for the map
    for (unsigned int i = 0; i < 6; i++)
    {
        stbi_uc* Surface = stbi_load((mCubeMapPath + Faces[i]).c_str(), &Width, &Height, &Format, 4);
        unsigned char* Tex = static_cast<unsigned char*>(Surface);
        if (Tex)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Tex);
            stbi_image_free(Tex);
        }
        else
        {
            GConsole->Log(Console::eMessageType::e_Error, "Cubemap tex failed to load at path: %s", Faces[i].c_str());
            stbi_image_free(Tex);
        }
    }

    //Texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

//void Engine::Skybox::Tick()
//{
//
//}

/**********************************************
* fn	Destroy
* brief
*   Destructor. Frees all memory used
**********************************************/
//void Engine::Skybox::Destroy()
//{
//    //Free memory used by program
//    if (mProgram)
//        delete mProgram;
//}

/**********************************************
* fn	RenderSkybox
* brief
*   Special render function to draw a Skybox
**********************************************/
void Engine::Skybox::SkyboxComp::RenderSkybox()
{
    //Disable depth mask, enable it later
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);

    // Bind the program and this object's VAO
    mProgram->Bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mMapHandle);

    glm::mat4 CamM = glm::mat3(*mCamMtx);
    mProgram->SetShaderUniform("V", &CamM);
    mProgram->SetShaderUniform("P", mProjMtx);
    if(mMoving)
    {
        rotation += GFrames->GetFrameTime() * speed;
        if (rotation >= aLoop)
            rotation -= aLoop;
        uvRot = glm::rotate(glm::mat4(1), rotation, axis);
    }
    mProgram->SetShaderUniform("uvRot", &uvRot);

    mMesh->Render();

    //Enable depth mask & culling again
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);

    // Unbind
    glUseProgram(0);
}

bool Engine::Skybox::SkyboxComp::IsVisible()
{
    return mVisible;
}

/**********************************************
* fn	LoadShader
* brief
*   Loads a specific shader for a Skybox
* param vtxPath : path to the vertex shader
* param fragPath : path to the fragment shader
**********************************************/
void Engine::Skybox::SkyboxComp::LoadShader(std::string vtxPath, std::string fragPath)
{
    //Load vertex & fragment shaders
    auto ShaderV = mProgram->GetShader(Graphics::Shader::EType::Vertex);
    auto ShaderF = mProgram->GetShader(Graphics::Shader::EType::Fragment);

    if (ShaderV)
        ShaderV->LoadSource(vtxPath.c_str());
    else
        mProgram->SetShader(new Graphics::Shader(vtxPath.c_str(), Graphics::Shader::EType::Vertex), false);

    if (ShaderF)
        ShaderF->LoadSource(fragPath.c_str());
    else
        mProgram->SetShader(new Graphics::Shader(fragPath.c_str(), Graphics::Shader::EType::Fragment), true);
}

/**********************************************
* fn	OnEditor
* brief
*   Displays all required data to edit the 
    Skybox component
**********************************************/
void Engine::Skybox::SkyboxComp::OnEditor()
{
#ifdef _EDITOR_
    //Toggle visibility
    ImGui::Checkbox("Visible", &mVisible);
    ImGui::Checkbox("Moving", &mMoving);
    ImGui::DragFloat("Speed", &speed, 0.0001f, 0.0f, 1.0f, "%.04f");
    if (ImGui::DragFloat3("Axis", &axis.x, 0.001f))
        axis = glm::normalize(axis);

    if (ImGui::InputText("Cube map folder", &mCubeMapPath))
    {
        auto Sky = dynamic_cast<Skybox*>(GetOwner());
        if (Sky)
            CreateCubeMap();
    }
#endif
}
