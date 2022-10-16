//
//  CommonImporters.h
//  Good Neighbours
//
//  Created by Diego Revilla on 4/11/20.
//  Copyright � 2020 Diego Revilla. All rights reserved.
//

#ifndef _COMMON_IMPORTERS__H_
#define _COMMON_IMPORTERS__H_

namespace Engine {
	namespace Assets {
		namespace Importers {
			class LevelImporter : public IResourceImporter {
				IResource* ImportFromFile(const char* filename, eImportMode mode) const override;
			};

			class ShaderImporter : public IResourceImporter {
				IResource* ImportFromFile(const char* filename, eImportMode mode) const override;
			};

			class ShaderProgramImporter : public IResourceImporter {
				IResource* ImportFromFile(const char* filename, eImportMode mode) const override;
			};

			class AudioImporter : public IResourceImporter {
				IResource* ImportFromFile(const char* filename, eImportMode mode) const override;
			};

			class TextureImporter : public IResourceImporter {
				IResource* ImportFromFile(const char* filename, eImportMode mode) const override;
				void AsyncLoad(const char* filename, IResource*& asset) override;
				void AsyncIntegrate(IResource*& asset) override;
			};
		}
	}
}

#endif
