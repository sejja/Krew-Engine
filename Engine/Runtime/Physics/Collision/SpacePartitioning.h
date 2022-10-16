#ifndef _SPA_PART__H_
#define _SPA_PART__H_

#include "Octree.h"

namespace Engine {
	struct Collider;
	struct MeshRenderer;
	struct CollisionSystem;

	class PhysicsPartitioning {
		Octree <Collider> m_octree;
		//octree variables
		unsigned short m_size_bit = 7;
		unsigned short m_levels = 3;

		void CreateOctree();
		void UpdateOctree();
		void AddObjToNode(Collider* obj, Octree<Collider>::node* node);
		void EraseObjFromPrevNode(Collider* obj);
		std::vector<Octree<Collider>::node*> get_nodes_from_level(Octree<Collider>::node* root, int level);

		void CheckCollisions();
		void CheckNodeCollisions(Octree<Collider>::node* node);

		bool static_obj_added = false;
		bool m_debug{ false };
	public:
		PhysicsPartitioning();
		~PhysicsPartitioning();
		void Init(CollisionSystem* c_system);
		void Update();
		void ShutDown();
		int idGenerator = 0;
		// a pointer to the collision system that intializez space partitioning
		CollisionSystem* m_col_system;
	};


	class GraphicsPartitioning {
		Octree <MeshRenderer> m_octree;
		//octree variables
		unsigned short m_size_bit = 10;
		unsigned short m_levels = 6;

		void ComputeFrustrum();
		void UpdateOctree();
		void AddObjToNode(MeshRenderer* obj, Octree<MeshRenderer>::node* node);
		void EraseObjFromPrevNode(MeshRenderer* obj);

		void CheckFrustrumNodeCollisions();
		void CheckFrustrumObjectCollisions(Octree<MeshRenderer>::node* node);

		Frustum* view_frustum; //in order to check frstrum culling
		bool m_debug{ false };
	public:
		GraphicsPartitioning();
		~GraphicsPartitioning();
		void Init();
		void Update();
		void ShutDown();
		int idGenerator = 0;
	};
}
#endif