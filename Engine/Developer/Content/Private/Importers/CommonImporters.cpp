//
//  CommonImporters.cpp
//  Good Neighbours
//
//  Created by Diego Revilla on 4/11/20.
//  Copyright � 2020 Diego Revilla. All rights reserved.
//

#include <Shared.h>

namespace Engine {
	// ------------------------------------------------------------------------
	/*! File Size
	*
	*   Retusns the size of a string (in bytes)
	*/ // --------------------------------------------------------------------
	DONTDISCARD static int _file_size(const char* mPath) noexcept {
		struct stat results;

		//If stat returned succesfully
		if (!stat(mPath, &results))
			return results.st_size;
		else
			return NULL;
	}

	// ------------------------------------------------------------------------
	/*! File Size
	*
	*   Retusns the size of a string (in bytes)
	*/ // --------------------------------------------------------------------
	DONTDISCARD std::string _get_file_size(const std::string& mPath) noexcept {
		return Engine::Math::Conversions::ConvertSize(_file_size(mPath.c_str()));
	}

	namespace Assets {
		namespace Importers {
			void _fetch_file_properties(IResource* res,
				const char* filename, const char* name) noexcept {
				res->type = name;
				res->size = _get_file_size(filename);
			}

			// ------------------------------------------------------------------------
			/*! Import From File
			*
			*   Imports a Level from file
			*/ //--------------------------------------------------------------------
			IResource* LevelImporter::ImportFromFile(const char* filename, eImportMode) const {
				GWorld->SetScene(Allocator<Scene>::allocate());
				nlohmann::json j_;
				std::fstream sf_(filename);

				//if we could open the file
				if (sf_.good()) {
					sf_ >> j_;
					GWorld->GetScene()->FromJson(j_);
				}

				return nullptr;
			}

			// ------------------------------------------------------------------------
			/*! Import From File
			*
			*   Imports a Texture from file
			*/ //--------------------------------------------------------------------
			IResource* TextureImporter::ImportFromFile(const char* filename, eImportMode mode) const {
				using Graphics::Texture;

				TResource<Texture>* rawResource = Allocator<TResource<Texture>>::New();

				if (mode == eImportMode::eInmediate) {

					Graphics::Texture* _tex = Allocator<Texture>::New();
					_tex->LoadFromFile(filename);
					rawResource->rawData = _tex;
					auto toup = reinterpret_cast<Texture*>(rawResource->rawData);
					toup->UploadToGPU();
					_fetch_file_properties(rawResource, filename, "Texture");
				} else {
					rawResource->rawData = GContent->GetResource<Graphics::Texture>("EngineContent/Texture/DefaultTex.png", eInmediate)->Get();
				}

				return rawResource;
			}

			void TextureImporter::AsyncLoad(const char* filename, IResource*& asset) {
				using Graphics::Texture;
				
				Graphics::Texture* _tex = Allocator<Texture>::New();
				_tex->LoadFromFile(filename);
				reinterpret_cast<TResource<Texture>*>(asset)->rawData = _tex;
				_fetch_file_properties(asset, filename, "Texture");
			}

			void TextureImporter::AsyncIntegrate(IResource*& asset) {
				using Graphics::Texture;
				auto toup = reinterpret_cast<Texture*>(reinterpret_cast<TResource<Texture>*>(asset)->rawData);
				toup->UploadToGPU();
			}

			Graphics::Shader* LoadShader(const char* filename) {
				Platform::FileSystem::FilePath fp_(filename);
				Graphics::Shader* pShader_ = Allocator<Graphics::Shader>::New();

				//If we created a valid shader
				if (pShader_) {
					pShader_->SetShaderType(fp_.extension == ".frag" ? Graphics::Shader::EType::Fragment :
						Graphics::Shader::EType::Vertex, true);
					pShader_->LoadSource(filename);
					pShader_->Compile(filename);
				}

				return pShader_;
			}

			// ------------------------------------------------------------------------
			/*! Import From File
			*
			*   Imports a Shader from file
			*/ //--------------------------------------------------------------------
			IResource* ShaderImporter::ImportFromFile(const char* filename, eImportMode) const {
				using Graphics::Shader;

				TResource<Shader>* rawResource = Allocator<TResource<Shader>>::New();
				rawResource->rawData = LoadShader(filename);
				_fetch_file_properties(rawResource, filename, "Shader");

				return rawResource;
			}

			// ------------------------------------------------------------------------
			/*! Import From File
			*
			*   Imports a Shader Program from file
			*/ //--------------------------------------------------------------------
			IResource* ShaderProgramImporter::ImportFromFile(const char* filename, eImportMode) const {
				using Graphics::ShaderProgram;

				TResource<ShaderProgram>* rawResource = Allocator<TResource<ShaderProgram>>::New();
				ShaderProgram* shaderProg_ = Allocator<ShaderProgram>::New();
				nlohmann::json j_;
				std::fstream sf_(filename);

				//if we could open the file
				if (sf_.good())
					sf_ >> j_;
				else
					return nullptr;

				std::string mPath = filename;

				const size_t last_slash_idx = mPath.rfind('\\');
				if (std::string::npos != last_slash_idx)
					mPath = mPath.substr(0, last_slash_idx);

				mPath += '\\';
				shaderProg_->SetShader(LoadShader((mPath + j_["Fragment Shader"].get<std::string>()).c_str()));
				shaderProg_->SetShader(LoadShader((mPath + j_["Vertex Shader"].get<std::string>()).c_str()), true);
				sf_.close();

				rawResource->rawData = shaderProg_;
				_fetch_file_properties(rawResource, filename, "Shader Program");

				return rawResource;

				return nullptr;
			}

			// ------------------------------------------------------------------------
			/*! Import From File
			*
			*   Imports an Audio track from file
			*/ //--------------------------------------------------------------------
			IResource* AudioImporter::ImportFromFile(const char* filename, eImportMode) const {
				using Audio::Sound;

				TResource<Sound>* rawResource = Allocator<TResource<Sound>>::New();
				rawResource->rawData = GAudio->CreateSound(filename);
				_fetch_file_properties(rawResource, filename, "Audio");

				return rawResource;

				return nullptr;
			}
		}
	}
}