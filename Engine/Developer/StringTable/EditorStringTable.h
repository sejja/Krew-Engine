//
//	EditorStringTable.h
//	Good Neighbours
//
//	Created by Diego Revilla on 04/10/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _EDITOR_STRING_TABLE__H_
#define _EDITOR_STRING_TABLE__H_
#ifdef _EDITOR_

namespace __Texts {
	namespace Editor {
		namespace PaletteWindow {
			struct StaticText {
				constexpr static const char Plane[] = "Plane";  constexpr static const char Cube[] = "Cube";
				constexpr static const char Cone[] = "Cone"; constexpr static const char Cylinder[] = "Cylinder";
				constexpr static const char Sphere[] = "Sphere";
				constexpr static const char PlanePath[] = "EngineContent/Texture/PlanePreview.png";
				constexpr static const char CubePath[] = "EngineContent/Texture/CubePreview.png";
				constexpr static const char ConePath[] = "EngineContent/Texture/ConePreview.png";
				constexpr static const char CylinderPath[] = "EngineContent/Texture/CylinderPreview.png";
				constexpr static const char SpherePath[] = "EngineContent/Texture/SpherePreview.png";
				constexpr static const char GOSpawnerID[] = "VIEWPORT_GO_CLASS";
				constexpr static const char ComponentSpawnerID[] = "VIEWPORT_COMPONENT_CLASS";
				constexpr static const char MeshRenderer[] = "Mesh Renderer";
				constexpr static const char NoName[] = "No Name";
				constexpr static const char CreateNew[] = "Create New";
				constexpr static const char Tools[] = "Tools"; constexpr static const char Objects[] = "Objects";
				constexpr static const char Basic[] = "Basic";
				constexpr static const char EmptyObject[] = "Empty Object";
				constexpr static const char Geometry[] = "Geometry";
				constexpr static const char Emitters[] = "Emitters";
				constexpr static const char SoundEmitter[] = "Sound Emitter";
				constexpr static const char SoundEmittterPath[] = "EngineContent/Texture/AudioSource.png";
				constexpr static const char Components[] = "Components";
				constexpr static const char EngineClasses[] = "Engine Classes";
				constexpr static const char CClasses[] = "C++ Classes";
			};
		}

		namespace PropertiesGUI {
			struct StaticText {
				constexpr static const char Transform[] = "Transform";
				constexpr static const char Tags[] = "Tags";
				constexpr static const char Properties[] = "Properties";
				constexpr static const char Options[] = "Options";
				constexpr static const char RemoveObject[] = "Remove Object";
				constexpr static const char MakePrefab[] = "Make prefab";
				constexpr static const char RemoveComponent[] = "Remove Component";
				constexpr static const char Position[] = "Position";
				constexpr static const char Scale[] = "Scale";
				constexpr static const char Rotation[] = "Rotation";
				constexpr static const char Add[] = "Add";
				constexpr static const char Remove[] = "Remove";
				constexpr static const char CurrentTags[] = "Current Tags: ";
				constexpr static const char add[] = "+";
				constexpr static const char CallBack[] = "To view properties, please select an object from the scene";
			};
		}

		namespace SceneWindow {
			struct StaticText {
				constexpr static const char Scene[] = "Scene";
				constexpr static const char ID[] = "ID";
				constexpr static const char Name[] = "Name";
				constexpr static const char Type[] = "Type";
				constexpr static const char ChildPayload[] = "SCENE_CHILD_CLASS";
				constexpr static const char ChildrenOf[] = "Children Of";
				constexpr static const char IDFormating[] = "%03llu";
				constexpr static const char PayloadText[] = "Moving \"%s\"";
			};
		}

		namespace PostProcessorGUI {
			struct StaticText {
				constexpr static const char PostProcessorGUI[] = "PostProcessorGUI";
				constexpr static const char Tags[] = "Tags";
			};
		}

		namespace ResourceWindow {
			struct StaticText {
				constexpr static const char Title[] = "Streamed Resources";
				constexpr static const char Name[] = "Name";
				constexpr static const char FullPath[] = "Full Path";
				constexpr static const char Type[] = "Type";
				constexpr static const char Size[] = "Size";
			};
		}

		namespace Viewport {
			struct StaticText {
				constexpr static const char PlayTexture[] = "EngineContent/Texture/Play.png";
				constexpr static const char PauseTexture[] = "EngineContent/Texture/Pause.png";
				constexpr static const char SkipTexture[] = "EngineContent/Texture/Skip.png";
				constexpr static const char Toolset[] = "Toolset";
				constexpr static const char Viewport[] = "Viewport";
				constexpr static const char Window[] = "Window";
				constexpr static const char Combo[] = "##combo";
				constexpr static const char ViewportPayloadID[] = "VIEWPORT_GO_CLASS";
				constexpr static const char ViewportPrefabID[] = "CONTENT_PREFAB_FILE";
				constexpr static const char Layers[] = "Layers";
				constexpr static const char LayerView[] = "Layer View";
				constexpr static const char LitScene[] = "LitScene";
				constexpr static const char ShadowMap[] = "Shadow Map ";
				constexpr static const char RenderInvisibleObjects[] = "Render Invisible Objects";
				constexpr static const char Wireframe[] = "WireFrame";
				constexpr static const char BoundingBoxes[] = "Bounding Boxes";
			};
		}

		namespace MainState {
			struct StaticText {
				constexpr static const char Title[] = "Main Window";
				constexpr static const char BarTitle[] = "Engine";
				constexpr static const char File[] = "File";
				constexpr static const char NewScene[] = "New Scene";
				constexpr static const char Save[] = "Save,,,";
				constexpr static const char SaveAs[] = "Save As,,,";
				constexpr static const char View[] = "View";
				constexpr static const char ShowConsole[] = "Show Console";
				constexpr static const char ShowProjectExplorer[] = "Show Project Explorer";
				constexpr static const char ShowFPSProfiler[] = "Show FPS Profiler";
				constexpr static const char ShowStreamedResources[] = "Show Streamed Resources";
				constexpr static const char Source[] = "Source";
				constexpr static const char HotReload[] = "Hot Reload";
				constexpr static const char HotReloadAndBuild[] = "Build + Hot Reloadr";
				constexpr static const char ShowMemoryTracker[] = "Show Memory Tracker";
				constexpr static const char ProgressBar[] = "Progress Bar";
				constexpr static const char SpinnerID[] = "##spinner";
				constexpr static const char BufferBarID[] = "##buffer_bar";
				constexpr static const char nulltext[] = "";
				constexpr static const char SaveShortcut[] = "Ctrl+S";
			};
		}

		namespace EditorManager {
			struct StaticText {
				constexpr static const char FontPath[] = "EngineContent/Fonts/Calamity-Regular.ttf";
				constexpr static const char DebugFontPath[] = "EngineContent/Fonts/Calamity-Regular.ttf";
				constexpr static const char OpenGLVersion[] = "#version 460 core";
				constexpr static const char DockspaceID[] = "_dockspace_";
				constexpr static const char DebugTextWindowID[] = "_logger_";
			};
		}
	}
}
#else
namespace __Texts {
	namespace Editor {
		namespace MainState {
			struct StaticText {
				constexpr static const char File[] = "File";
			};
		}
	}
}
#endif
#endif