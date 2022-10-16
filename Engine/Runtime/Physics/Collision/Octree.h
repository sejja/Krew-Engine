#ifndef _OCTR__H_
#define _OCTR__H_

//#include "../RigidBody/Geometry.h"

namespace Engine {
    struct AABB;
    namespace LocationalCode {
        static const unsigned maxBits = sizeof(unsigned) * 8;

        template<unsigned dimension = 3>
        int compute_locational_code(glm::vec<dimension, long long> pos, const unsigned root_size, const unsigned levels);

        template<unsigned dimension = 3>
        int common_locational_code(unsigned loc1, unsigned loc2);

        int compute_locational_code(const AABB& bv, const unsigned root_size, const unsigned levels);

        AABB compute_bv(unsigned loc, float size);
    }

    /**
     * @brief
     * 	Linear octree, each node stores a head for a linked list of T
     * @tparam T
     */
    template <typename T>
    class Octree
    {
    public:
        struct node
        {
            unsigned int locational_code = 0;
            unsigned char  children_active = 0;
            T* first = nullptr;
        };

        std::unordered_map<unsigned int, node*> m_nodes;
    private:
        unsigned int m_root_size;
        unsigned int m_levels;

    public:
        ~Octree();

        void clear();
        node* create_node(const AABB& bv);
        node* create_node(unsigned int loc);
        node* find_node(const AABB& bv)const;
        node* find_node(unsigned int loc)const;
        void delete_node(unsigned int loc);
        void children_nodes(node* n, std::vector<node*>& childrens, int level)const;
        void set_root_size(unsigned s);
        void set_levels(unsigned l);
        [[nodiscard]] unsigned root_size() const { return m_root_size; }
    };
}

#include "Octree.inl"

#endif

