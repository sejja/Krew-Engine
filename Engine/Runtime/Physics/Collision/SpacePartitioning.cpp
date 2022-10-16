#include "Shared.h"


namespace Engine {
#pragma region phy
    PhysicsPartitioning::PhysicsPartitioning() : m_col_system(nullptr){

    }

    PhysicsPartitioning::~PhysicsPartitioning() {

    }

    void PhysicsPartitioning::Init(CollisionSystem* c_system) {
        m_col_system = c_system;

        //set data
        m_octree.set_root_size(1u << m_size_bit);
        m_octree.set_levels(m_levels);
    }

    void PhysicsPartitioning::Update() {
        // update octree levels
        UpdateOctree();
        for (unsigned i = 0; i < m_col_system->mCollisionPrecision; i++) {
            m_col_system->mCurrentCollisionIteration = i;
            //check collisions
            CheckCollisions();
        }
        m_col_system->BroadcastCollisions();

        //debug
        if (m_debug) {
           for (auto& it : m_octree.m_nodes) {
               if (it.second->first) {
                   AABB b = LocationalCode::compute_bv(it.second->locational_code, m_octree.root_size());
                   GDebug->DebugDrawAABB(b, glm::vec4(0.4 * it.second->locational_code, 1.0, 0, 0));
               }
           }
        }
    }

    void PhysicsPartitioning::UpdateOctree() {
        if (!static_obj_added)
            CreateOctree();
        //traverse thorugh all dynamic objets
        for (auto& it : m_col_system->mBodies) {
            // get or create if not created yet the node that encapsulates the object
            Octree<Collider>::node* new_node = m_octree.create_node(it->m_bounding_vol);
            //check if it already had another octree::node list 
            if (it->m_octree_node) {
                if (it->m_octree_node == new_node)
                    continue;
                else {
                    EraseObjFromPrevNode(it);
                    it->m_octree_node = new_node;
                    AddObjToNode(it, it->m_octree_node);
                }

            }
            else {
                it->m_octree_node = new_node;
                AddObjToNode(it, it->m_octree_node);
            }
        }
    }

    //mainly creates the octree for the static objects that does not make sense to compute
    // it every frame, cause they are static
    void PhysicsPartitioning::CreateOctree() {
        //traverse thorugh all static objets and aadd them to the octree
        for (auto& it : m_col_system->mStaticBodies) {
            //get or create the node that encapsulates the object
            it->m_octree_node = m_octree.create_node(it->m_bounding_vol);
            AddObjToNode(it, it->m_octree_node);
        }
        // dont have to repeat the logic for non-moving objects
        static_obj_added = true;
    }

    void PhysicsPartitioning::CheckCollisions() {
        Octree<Collider>::node* root = m_octree.find_node(0b1);
        //no objects
        if (!root)   return;
        //check node collision in the root
        CheckNodeCollisions(root);
        int level = 1;
        std::vector< Octree<Collider>::node*> parents;
        parents.push_back(root);

        while (level <= m_levels) {
            std::vector<Octree<Collider>::node*> nodes;

            //get all parents children
            for (auto it : parents)
                m_octree.children_nodes(it, nodes, -1);

            // check collision in all children nodes(separately)
            for (auto it : nodes)
                CheckNodeCollisions(it);

            level++;
            //reset parents for next iteration
            parents.clear();
            parents = nodes;
        }
    }

    void PhysicsPartitioning::CheckNodeCollisions(Octree<Collider>::node* node) {
        Collider* pointer = node->first;
        //check root intersection
        while (pointer != nullptr) {
            Collider* pointer2 = pointer->m_octree_next_obj;
            while (pointer2 != nullptr) {
                m_col_system->CollideAndResolve(pointer, pointer2);

                if (pointer2->m_octree_next_obj == nullptr)
                    break;
                pointer2 = pointer2->m_octree_next_obj;
            }
            //check with the parent node object the collision
            Octree<Collider>::node* parent = m_octree.find_node(node->locational_code >> 3);
            if (parent) {
                Collider* parentPointer = parent->first;

                while (parentPointer != nullptr) {
                    m_col_system->CollideAndResolve(pointer, parentPointer);

                    if (parentPointer->m_octree_next_obj == nullptr)
                        break;
                    parentPointer = parentPointer->m_octree_next_obj;
                }
            }


            if (pointer->m_octree_next_obj == nullptr)
                break;
            pointer = pointer->m_octree_next_obj;
        }
    }

    void PhysicsPartitioning::AddObjToNode(Collider* obj, Octree<Collider>::node* node) {
        //Add the object to the node's object linked list
                 //check if octree node first is null
        if (obj->m_octree_node->first == nullptr) {
            obj->m_octree_node->first = obj; //set this object as the first object
        }
        else {
            bool flag = false; //flag to check if the object was already inserted
            Collider* pointer = obj->m_octree_node->first;
            while (pointer->m_octree_next_obj != nullptr) {
                pointer = pointer->m_octree_next_obj;
                if (pointer->m_ID == obj->m_ID) {//same object
                    flag = true;
                    break;
                }
            }
            //set the pointers if the object was not already inside, check also with the first and last objects
            if (!flag && obj->m_octree_node->first->m_ID != obj->m_ID) {
                pointer->m_octree_next_obj = obj;
                obj->m_octree_prev_obj = pointer;
            }
        }
    }

    void PhysicsPartitioning::EraseObjFromPrevNode(Collider* obj) {
        //traverse through the linked list, in order to erase it from the prev linked list
        Collider* pointer = obj->m_octree_node->first;
        if (obj->m_octree_node->first) {
            //if it was in the 1 position
            if (obj->m_octree_node->first->m_ID == obj->m_ID) {
                if (pointer->m_octree_next_obj == nullptr) //alone in the linked list
                {
                    obj->m_octree_node->first = nullptr;
                    if (obj->m_octree_node->children_active == 0) //if not children nodes delete
                        m_octree.delete_node(obj->m_octree_node->locational_code);

                }
                else { //stil more objects in the linked list
                    obj->m_octree_node->first = obj->m_octree_next_obj;
                    obj->m_octree_next_obj = nullptr;
                }
            }
            else {
                while (pointer->m_octree_next_obj != nullptr) {
                    //traverse
                    pointer = pointer->m_octree_next_obj;

                    if (pointer->m_ID == obj->m_ID) {
                        //if it is in the last position
                        if (pointer->m_octree_next_obj == nullptr) {
                            pointer->m_octree_prev_obj->m_octree_next_obj = nullptr;
                        }
                        else {
                            pointer->m_octree_prev_obj->m_octree_next_obj = pointer->m_octree_next_obj;
                            pointer->m_octree_next_obj->m_octree_prev_obj = pointer->m_octree_prev_obj;
                        }

                        //set to null the pointers of the object
                        obj->m_octree_prev_obj = nullptr;
                        obj->m_octree_next_obj = nullptr;
                    }
                }
            }
        }
    }

    // traverse through the octree until the specific level and return nodes in that level
    std::vector<Octree<Collider>::node*> PhysicsPartitioning::get_nodes_from_level(Octree<Collider>::node* root, int level) {
        std::vector<Octree<Collider>::node*> outNodes;
        std::vector<Octree<Collider>::node*> helper;

        helper.push_back(root);
        //root
        if (level == 0)
            return helper;

        int l = 1;
        while (l < level) {
            std::vector<Octree<Collider>::node*> parents = helper;
            helper.clear();

            for (auto it : parents)
                m_octree.children_nodes(it, helper, -1);

            l++;
        }
        //when level == highlight_level
        for (auto it : helper) {
            m_octree.children_nodes(it, outNodes, -1);
        }

        return outNodes;
    }

    void PhysicsPartitioning::ShutDown() {
        m_octree.clear();
    }

#pragma endregion phy
#pragma region gfx
    GraphicsPartitioning::GraphicsPartitioning() : view_frustum(new Frustum()){

    }

    GraphicsPartitioning::~GraphicsPartitioning() {
        delete view_frustum;
    }

    void GraphicsPartitioning::Init() {
        //set data
        m_octree.set_root_size(1u << m_size_bit);
        m_octree.set_levels(m_levels);
    }

    void GraphicsPartitioning::Update() {
        // update octree levels
        UpdateOctree();

        ComputeFrustrum();

        CheckFrustrumNodeCollisions();

        //debug
        if (m_debug) {
            for (int i = 0; i < 6; i++) {
                GDebug->DebugDrawFrustum(view_frustum->vp, glm::vec4(0, 0, 1.f, 0.5f));
                //GDebug->DebugDrawPlane((*view_frustum)[i].pos, -(*view_frustum)[i].normal, 50, glm::vec4(0, 0, 1.f, 0.5f));
            }
            GDebug->DebugDrawFrustum(GGfxPipeline->get_proj_mtx() * GGfxPipeline->get_view_mtx(), glm::vec4(1.f));
            for (auto& it : m_octree.m_nodes) {
                if (it.second->first) {
                    AABB b = LocationalCode::compute_bv(it.second->locational_code, m_octree.root_size());
                    GDebug->DebugDrawAABB(b, glm::vec4(0.4 * it.second->locational_code, 1.0, 0, 0));
                }
            }
        }
    }

    void GraphicsPartitioning::UpdateOctree() {
        auto& renderables = GGfxPipeline->get_renderables();
        //traverse thorugh all renderables
        for (auto& it : renderables) {
            if (it->get_mesh() && it->GetOwner()) {
                const glm::mat4& model = it->GetOwner()->GetWorldMatrix();
                // get or create if not created yet the node that encapsulates the object
                Octree<MeshRenderer>::node* new_node = m_octree.create_node(AABB(model * glm::vec4(it->get_mesh()->bvMin, 1.f)
                    , model * glm::vec4(it->get_mesh()->bvMax, 1.f)));
                //check if it already had another octree::node list 
                if (it->m_octree_node) {
                    if (it->m_octree_node == new_node)
                        continue;
                    else {
                        EraseObjFromPrevNode(it);
                        it->m_octree_node = new_node;
                        AddObjToNode(it, it->m_octree_node);
                    }
                }
                else {
                    it->m_octree_node = new_node;
                    AddObjToNode(it, it->m_octree_node);
                }
            }
        }
    }

    void GraphicsPartitioning::ComputeFrustrum() {
        view_frustum->vp = GGfxPipeline->get_proj_mtx() * GGfxPipeline->get_view_mtx();
        view_frustum->Compute();
    }


    void GraphicsPartitioning::AddObjToNode(MeshRenderer* obj, Octree<MeshRenderer>::node* node) {
        //Add the object to the node's object linked list
                 //check if octree node first is null
        if (obj->m_octree_node->first == nullptr) {
            obj->m_octree_node->first = obj; //set this object as the first object
        }
        else {
            bool flag = false; //flag to check if the object was already inserted
            MeshRenderer* pointer = obj->m_octree_node->first;
            while (pointer->m_octree_next_obj != nullptr) {
                pointer = pointer->m_octree_next_obj;
                if (pointer->m_ID == obj->m_ID) {//same object
                    flag = true;
                    break;
                }
            }
            //set the pointers if the object was not already inside, check also with the first and last objects
            if (!flag && obj->m_octree_node->first->m_ID != obj->m_ID) {
                pointer->m_octree_next_obj = obj;
                obj->m_octree_prev_obj = pointer;
            }
        }
    }

    void GraphicsPartitioning::EraseObjFromPrevNode(MeshRenderer* obj) {
        //traverse through the linked list, in order to erase it from the prev linked list
        MeshRenderer* pointer = obj->m_octree_node->first;
        if (obj->m_octree_node->first) {
            //if it was in the 1 position
            if (obj->m_octree_node->first->m_ID == obj->m_ID) {
                if (pointer->m_octree_next_obj == nullptr) //alone in the linked list
                {
                    obj->m_octree_node->first = nullptr;
                    if (obj->m_octree_node->children_active == 0) //if not children nodes delete
                        m_octree.delete_node(obj->m_octree_node->locational_code);

                }
                else { //stil more objects in the linked list
                    obj->m_octree_node->first = obj->m_octree_next_obj;
                    obj->m_octree_next_obj = nullptr;
                }
            }
            else {
                while (pointer->m_octree_next_obj != nullptr) {
                    //traverse
                    pointer = pointer->m_octree_next_obj;

                    if (pointer->m_ID == obj->m_ID) {
                        //if it is in the last position
                        if (pointer->m_octree_next_obj == nullptr) {
                            pointer->m_octree_prev_obj->m_octree_next_obj = nullptr;
                        }
                        else {
                            pointer->m_octree_prev_obj->m_octree_next_obj = pointer->m_octree_next_obj;
                            pointer->m_octree_next_obj->m_octree_prev_obj = pointer->m_octree_prev_obj;
                        }

                        //set to null the pointers of the object
                        obj->m_octree_prev_obj = nullptr;
                        obj->m_octree_next_obj = nullptr;
                    }
                }
            }
        }
    }

    void GraphicsPartitioning::CheckFrustrumNodeCollisions() {
        for (auto& it : m_octree.m_nodes) {
            if (it.second->first) {//if it has objects inside
                int c = classify_frustum_aabb_naive(*view_frustum, 
                    LocationalCode::compute_bv(it.second->locational_code, m_octree.root_size()));

                if (c == eINSIDE) {
                    MeshRenderer* pointer = it.second->first;
                    while (pointer) {
                        pointer->m_render = true;
                        pointer = pointer->m_octree_next_obj;
                    }
                }
                else if (c == eOUTSIDE) {
                    MeshRenderer* pointer = it.second->first;
                    while (pointer) {
                        pointer->m_render = false;
                        pointer = pointer->m_octree_next_obj;
                    }
                }
                else {// overlaping
                    CheckFrustrumObjectCollisions(it.second);
                }
            }
        }
    }

    void GraphicsPartitioning::CheckFrustrumObjectCollisions(Octree<MeshRenderer>::node* node) {
        MeshRenderer* pointer = node->first;

        while (pointer) {
            const glm::mat4& model = pointer->GetOwner()->GetWorldMatrix();
            glm::vec3 min = model * glm::vec4(pointer->get_mesh()->bvMin, 1.f);
            glm::vec3 max = model * glm::vec4(pointer->get_mesh()->bvMax, 1.f);
            int c = classify_frustum_aabb_naive(*view_frustum, AABB(min, max));

            if (c == eOUTSIDE)  // its is outside, not render it
                pointer->m_render = false;
            else
                pointer->m_render = true;

            pointer = pointer->m_octree_next_obj;
        }
    }

    void GraphicsPartitioning::ShutDown() {
        m_octree.clear();
    }

#pragma endregion gfx
}




