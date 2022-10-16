/* -----------------------------------------------------------------------------------
    Copyright (C) 2021 Diego Revilla Institute of Technology.
    Reproduction or disclosure of this file or its contents without the prior written
    consent of Diego Revilla Institute of Technology is prohibited.

    File: PostProcess.cpp
    Author: Jon Ander Eceiza
    Creation date: 10/06/2021
    Brief:
        Declaration of functions & methods needed to add PostProcessing into the
        Graphics pipeline.

------------------------------------------------------------------------------------*/

#pragma once


namespace Engine
{
        class PostProcessor : public Serializable
        {
        public:
            // state
            Graphics::ShaderProgram* PostProcessingShader = nullptr;
            Graphics::Texture* mTexture = nullptr;
            unsigned int Width, Height;

            // BLOOM
            bool Bloom_ON = true;
            float threshold = 0.8f;
            int samples = 7;

            // HDR
            bool HDR_ON = true;
            bool ACES_ON = true;
            float exposure = 1.f;
            float gamma = 2.2f;
            // constructor
            PostProcessor(Graphics::ShaderProgram* shader, unsigned int width, unsigned int height);
            ~PostProcessor();
            // prepares the postprocessor's framebuffer operations before rendering the game
            void BeginRender();
            // should be called after rendering the game, so it stores all the rendered data into a texture object
            void EndRender();

            // It is required to call BeginRender() before rendering the game
            // and EndRender() after rendering the game for the class to work.

            void FromJson(const nlohmann::json& val)override;
            void ToJson(nlohmann::json& val) const override;

        private:
            std::string path = "EngineContent/Levels/PostProcessProperties";
            // render state
            unsigned int FBO; // MSFBO = Multisampled FBO. FBO is regular, used for blitting MS color-buffer to texture
            unsigned int RBO; // RBO is used for multisampled color buffer

            unsigned int rboDepth;
            unsigned int colorBuffers[2];

            unsigned int VAO;
            // initialize quad for rendering postprocessing texture
            void initRenderData();
        };
}