#ifdef _GAME_
#undef _EDITOR_
#endif

#include <cmath>
#include <stdexcept>
#include <map>
#include <filesystem>
#include <algorithm>
#include <set>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <queue>
#include <stack>
#include <mutex>
#include <future>
#include <shlobj.h>
#include <random>

#include <Windows.h>
#include <tlhelp32.h>
#include <xinput.h>
#ifdef _ENGINE_
#include <ft2build.h>
#include FT_FREETYPE_H
#include <assimp/importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#endif

#include "ThirdParty/GLM/glm.hpp"
#include "ThirdParty/GLM/gtc/matrix_transform.hpp"
#include "ThirdParty/GLM/gtc/type_ptr.hpp"
#include "ThirdParty/GLM/gtc/epsilon.hpp"
#include "ThirdParty/GLM/gtx/matrix_decompose.hpp"
#include "ThirdParty/GLM/gtx/norm.hpp"
#include "ThirdParty/GLM/gtx/integer.hpp"
#include "ThirdParty/GLM/gtx/quaternion.hpp"
#include "ThirdParty/GLM/gtx/intersect.hpp"


#define GLEW_STATIC
#include "ThirdParty/GLEW/glew.h"
#include "ThirdParty/GLEW/wglew.h"

#ifdef _EDITOR_
#include "ThirdParty/ImGui/imgui_impl_win32.h"
#include "ThirdParty/ImGui/imgui_impl_opengl3.h"
#include "ThirdParty/ImGui/imgui_internal.h"
#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/ImGuizmo.h"
#include "ThirdParty/ImGui/misc/cpp/imgui_stdlib.h"
#include "ThirdParty/ImGui/tahoma.h"
#include "ThirdParty/ImGui/imgui_notify.h"
#endif


#include "ThirdParty/Json/single_include/json.hpp"

#include "ThirdParty/FMOD/Public/fmod.hpp"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "ThirdParty/STB/stb_image.h"

#include "Developer/StringTable/EditorStringTable.h"
#include "Developer/StringTable/EngineStringTable.h"

#include "Runtime/Shared/CommonDefines.h"
#include "Runtime/Shared/Runtime.h"
#include "Runtime/Shared/Singleton.h"

#include "Developer/Debug/Assert.h"

#include "Runtime/Containers/Algorithm.h"
#include "Runtime/Containers/Array.h"

#include "Runtime/Memory/Tracker/CodeTracker.h"
#include "Runtime/Memory/Tracker/MemoryTracker.h"

#include "Runtime/Memory/ObjectAllocator.h"
#include "Runtime/Memory/Allocator.h"
#include "Runtime/Memory/PageAllocator.h"

#include "Runtime/Memory/Pointers/UniquePointer.h"

#include "Runtime/RTTI/RTTI.h"

#include "Developer/Serialization/Serializable.h"

#include "Developer/Console/Console.h"

#include "Runtime/Core/Thread/ThreadPool.h"

#include "Developer/Content/Public/SharedResources.h"
#include "Developer/Content/Private/Importers/CommonImporters.h"

#include "Runtime/Core/Events/type_info.hpp"
#include "Runtime/Core/Events/event.hpp"
#include "Runtime/Core/Events/dispatcher.hpp"

#include "Runtime/Core/Mathematics/Geometry.h"
#include "Runtime/Core/Mathematics/LookupTables/LookupTable.h"
#include "Runtime/Core/Mathematics/Constants.h"
#include "Runtime/Core/Mathematics/Math.h"
#include "Runtime/Core/Mathematics/Vector.h"
#include "Runtime/Core/Mathematics/Interpolation.h"
#include "Runtime/Core/Mathematics/Transform.h"
#include "Runtime/Core/Mathematics/Conversions.h"
#include "Runtime/Core/Mathematics/Spline.h"

#include "Runtime/Graphics/Primitives/Color.h"
#include "Runtime/Graphics/GraphicsManager.h"
#include "Runtime/Graphics/Types/FrameBuffer.h"
#include "Runtime/Graphics/Primitives/Texture.h"
#include "Runtime/Graphics/Primitives/Shader.h"
#include "Runtime/Graphics/Primitives/ShaderProgram.h"
#include "Runtime/Graphics/Primitives/FogMachine.h"

#include "Developer/Serialization/ColorSerializer.h"
#include "Developer/Serialization/MathSerializer.h"

#include "Runtime/Core/FrameController/FrameController.h"
#include "Runtime/Core/UI/UI.h"
#include "Runtime/Core/UI/Widget.h"

#ifdef _EDITOR_
#include "Editor/Archetypes/IWindow.h"
#include "Editor/Archetypes/IEditorState.h"
#include "Editor/Archetypes/DrawingFunctions.h"
#include "Editor/Archetypes/FileExplorer.h"
#include "Editor/Archetypes/Gizmo.h"
#include "Editor/Archetypes/Creator.h"

#include "Editor/EditorManager.h"

#include "Editor/States/MainState.h"

#include "Editor/GUI/ConsoleGUI.h"
#include "Editor/GUI/PropertiesGUI.h"
#include "Editor/GUI/Viewport.h"
#include "Editor/GUI/ImporterWindowModal.h"
#include "Editor/GUI/ResourceWindow.h"
#include "Editor/GUI/MemoryTrackerWindow.h"
#include "Editor/GUI/ContentBrowser.h"
#include "Editor/GUI/FPSProfiler.h"
#include "Editor/GUI/PaletteWindow.h"
#include "Editor/GUI/PrefabEditor.h"
#endif

#include "Runtime/Composition/IBase.h"
#include "Runtime/Composition/GameObject.h"
#include "Runtime/Composition/Component.h"
#include "Runtime/Composition/Scene.h"
#include "Runtime/Composition/World.h"
#include "Runtime/Graphics/Types/Skybox.h"
#include "Runtime/Graphics/Components/RenderComp.h"
#include "Runtime/Graphics/Primitives/Text.h"
#include "Runtime/Graphics/Types/3DModel.h"
#include "Runtime/Graphics/Components/Fog.h"
#include "Runtime/Graphics/Animation/StateMachines.h"

#include "Runtime/Core/ObjectManager/ObjectManager.h"

#include "Runtime/Core/States/StateManager.h"

#include "Runtime/Core/Windows/Window/Window.h"

#include "Runtime/Core/Input/InputManager.h"

#include "Developer/Factory/Factory.h"

#include "Developer/EngineBuild/DLLBuilder.h"

#include "Runtime/Memory/GarbageCollector/GarbageCollector.h"

#include "Runtime/Core/Windows/FileSystem/FilePath.h"

#include "Runtime/Audio/MasterAudio.h"
#include "Runtime/Audio/Audio.h"

#include "Runtime/Audio/Components/AudioComponent.h"

#include "Runtime/AI/Blackboard.h"
#include "Runtime/AI/BehaviorTree.h"
#include "Runtime/AI/Nodes/Leaf.h"
#include "Runtime/AI/Nodes/ChangeScene.h"
#include "Runtime/AI/Nodes/Composite.h"
#include "Runtime/AI/Nodes/Decorator.h"
#include "Runtime/AI/Nodes/RepeatNTimes.h"
#include "Runtime/AI/Nodes/Sequencer.h"

#include "Runtime/Physics/Collision/SpacePartitioning.h"

#include "Runtime/Graphics/Debug/DebugShapes.h"
#include "Runtime/Graphics/Primitives/Camera.h"
#include "Runtime/Graphics/GraphicsPipeline.h"
#include "Runtime/Graphics/Types/PostProcess.h"
#include "Runtime/Graphics/Shapes/PrimitiveShapes.h"
#include "Runtime/Graphics/Components/MeshRenderer.h"
#include "Runtime/Graphics/Components/ParticleEmitter.h"
#include "Runtime/Graphics/Primitives/Lighting.h"
#include "Runtime/Graphics/Primitives/Particle.h"
#include "Runtime/Graphics/Types/PostProcess.h"
#include "Runtime/Graphics/GraphicsPipeline.h"
#include "Runtime/Graphics/Primitives/Text.h"
#include "Runtime/Graphics/Types/3DModel.h"

#include "Runtime/Core/Random/Random.hpp"

#include "Runtime/Physics/Collision/Types/TriggerBox.h"
#include "Runtime/Physics/RigidBody/RayCasting.h"

#include "Runtime/Physics/Collision/Octree.h"
#include "Runtime/Physics/Collision/Collider.h"
#include "Runtime/Physics/Collision/Collisions.h"
#include "Runtime/Physics/Collision/ContactCollisions.h"
#include "Runtime/Physics/Collision/CollisionSystem.h"
#include "Runtime/Physics/Collision/Types/TriggerBox.h"

#include "Runtime/Physics/RigidBody/PhysicsManager.h"
#include "Runtime/Physics/RigidBody/RigidBody.h"

#include "Runtime/Core/States/StateManager.h"

#include "Runtime/Graphics/Components/SpringArm.h"
#include "Developer/Debug/Debug.h"


#include "Runtime/Audio/Components/SoundEmitter3D.h"
#include "Runtime/Audio/Components/SoundListener3D.h"