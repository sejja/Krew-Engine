/* -----------------------------------------------------------------------------------
    Copyright (C) 2021 Diego Revilla Institute of Technology.
    Reproduction or disclosure of this file or its contents without the prior written
    consent of Diego Revilla Institute of Technology is prohibited.

    File: PostProcess.cpp
    Author: Jon Ander Eceiza
    Creation date: 10/06/2021
    Brief:
        Definition of functions & methods declared in class PostProcess.

------------------------------------------------------------------------------------*/

#include <Shared.h>


namespace Engine
{

        PostProcessor::PostProcessor(Graphics::ShaderProgram* shaderlist, unsigned int width, unsigned int height)
            : PostProcessingShader(shaderlist), Width(width), Height(height)
        {
#ifdef _EDITOR_
            glm::vec2 win_dim = GEditor->GetViewportDimensions();
#else
            glm::vec2 win_dim = GWindow->GetDimensions();
#endif
            // initialize renderbuffer/framebuffer object
            glGenFramebuffers(1, &FBO);
            glGenRenderbuffers(1, &rboDepth);
            glBindFramebuffer(GL_FRAMEBUFFER, FBO);
            glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, win_dim.x, win_dim.y);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

            glGenTextures(2, colorBuffers);
            for (unsigned int i = 0; i < 2; i++)
            {
                glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
                glTexImage2D(
                    GL_TEXTURE_2D, 0, GL_RGBA16F, win_dim.x, win_dim.y, 0, GL_RGBA, GL_FLOAT, NULL
                );
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
                // attach texture to framebuffer
                glFramebufferTexture2D(
                    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0
                );
            }

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                abort();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // initialize render data and uniforms
            initRenderData();
        }

        PostProcessor::~PostProcessor()
        {
            if (mTexture)
                delete mTexture;
        }

        void PostProcessor::BeginRender()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, FBO);
            unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
            glDrawBuffers(2, attachments);
        }
        void PostProcessor::EndRender()
        {
#ifdef _EDITOR_
            glBindFramebuffer(GL_FRAMEBUFFER, GEditor->GetBuffer());
#else
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // set uniforms/options
            PostProcessingShader->Bind();
            PostProcessingShader->SetShaderUniform("bBloom", &Bloom_ON);
            PostProcessingShader->SetShaderUniform("bHDR", &HDR_ON);
            PostProcessingShader->SetShaderUniform("bAces", &ACES_ON);
            PostProcessingShader->SetShaderUniform("exposure", &exposure);
            PostProcessingShader->SetShaderUniform("gamma", &gamma);
            PostProcessingShader->SetShaderUniform("samples", &samples);

            glActiveTexture(GL_TEXTURE0);
            int value = 0;
            glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
            PostProcessingShader->SetShaderUniform("sceneTex", &value);

            glActiveTexture(GL_TEXTURE1);
            int value2 = 1;
            glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            PostProcessingShader->SetShaderUniform("bloomTex", &value2);

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

        void PostProcessor::initRenderData()
        {
            // configure VAO/VBO
            unsigned int VBO;
            float vertices[] = {
                // pos        // tex
                -1.0f, -1.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, 1.0f, 1.0f,
                -1.0f,  1.0f, 0.0f, 1.0f,

                -1.0f, -1.0f, 0.0f, 0.0f,
                 1.0f, -1.0f, 1.0f, 0.0f,
                 1.0f,  1.0f, 1.0f, 1.0f
            };
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindVertexArray(VAO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        void PostProcessor::FromJson(const nlohmann::json& val)
        {

            nlohmann::json j_;
            std::ifstream input(path + ".prop");

            input >> j_;

            fetch(gamma, j_, "Gamma");
            fetch(exposure, j_, "Exposure");
            fetch(threshold, j_, "threshold");
            fetch(samples, j_, "samples");

            input.close();
        }
        void PostProcessor::ToJson(nlohmann::json& val) const
        {
            nlohmann::json _outjson;

            _outjson["threshold"] = threshold;
            _outjson["samples"] = samples;
            _outjson["Gamma"] = gamma;
            _outjson["Exposure"] = exposure;
            std::ofstream _output(path + ".prop");
            _output << std::setw(4) << _outjson;
            _output.close();
        }

}