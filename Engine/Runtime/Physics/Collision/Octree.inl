#ifndef _OCTR__INL_
#define _OCTR__INL_
namespace Engine
{
    namespace LocationalCode
    {
        template<unsigned dimension>
        int compute_locational_code(glm::vec<dimension, long long> pos, const unsigned root_size, const unsigned levels)
        {
            pos += root_size / 2; // offset

            // check boundaries
            for (unsigned i = 0; i < dimension; ++i)
                if (pos[i] < 0 || pos[i] >= root_size)
                    return 0b1;

            // compute bits
            unsigned bits = 0;
            unsigned maxPower = glm::log2(root_size);
            for (unsigned i = 0; i < maxPower; ++i)
            {
                unsigned pow2 = 1 << i;
                for (unsigned j = 0; j < dimension; ++j)
                    if (pos[j] & pow2)
                        bits += 1 << (i * dimension + j);
            }

            // shifting
            unsigned shift = maxPower - levels;
            bits >>= (shift * dimension);

            // sentinel bit
            bits += 1 << (levels * dimension);

            return bits;
        }
        
        template<unsigned dimension>
        int common_locational_code(unsigned loc1, unsigned loc2)
        {
            if (loc1 == loc2) // Same
                return loc1;

            // loc1 == smallest
            unsigned smallest = loc1;
            // loc2 == biggest
            unsigned biggest = loc2;

            if (smallest > biggest)
                std::swap(smallest, biggest);

            unsigned startingBit = maxBits - 1;
            startingBit -= startingBit % dimension;
            unsigned bigSentinel = 0;
            unsigned smallSentinel = 0;

            // get biggest sentinel bit number
            for (long int i = startingBit; i >= 0; i -= dimension)
                if (biggest & (1 << i))
                {
                    bigSentinel = i;
                    break;
                }

            // get smallest sentinel bit number
            for (long int i = bigSentinel; i >= 0; i -= dimension)
                if (smallest & (1 << i))
                {
                    smallSentinel = i;
                    break;
                }

            // shift to discard last bits of biggest (if needed)
            biggest >>= (bigSentinel - smallSentinel);

            unsigned commonBits = 1;
            for (long int i = smallSentinel - dimension; i >= 0; i -= dimension)
            {
                unsigned smallShift = smallest >> i;
                unsigned bigShift = biggest >> i;
                if (smallShift == bigShift)
                    commonBits = smallShift;
                else
                    break;
            }
            return commonBits;
        }
    }

    template<typename T>
    Octree<T>::~Octree()
    {
        clear();
    }

    template<typename T>
    void Octree<T>::clear()
    {
        for (auto& it : m_nodes)
            delete it.second;
        m_nodes.clear();
    }

    template<typename T>
    typename Octree<T>::node* Octree<T>::create_node(const AABB& bv)
    {
        return create_node(LocationalCode::compute_locational_code(bv, m_root_size, m_levels));
    }

    template<typename T>
    typename Octree<T>::node* Octree<T>::create_node(unsigned int loc)
    {
        if (loc == 0u)
            return nullptr;

        node* n = find_node(loc);
        if (n)
            return n;

        m_nodes[loc] = n = new node;
        n->locational_code = loc;
        n->children_active = 0;
        n->first = nullptr;

        if (loc == 0b1)
            return n;

        node* parent = create_node(loc >> 3);
        if (parent)
        {
            int childLoc = (loc & 0b111);
            parent->children_active |= 1u << childLoc;
        }

        return n;
    }

    template<typename T>
    typename Octree<T>::node* Octree<T>::find_node(const AABB& bv)const
    {
        return find_node(LocationalCode::compute_locational_code(bv, m_root_size, m_levels));
    }

    template<typename T>
    typename Octree<T>::node* Octree<T>::find_node(unsigned int loc)const
    {
        auto found = m_nodes.find(loc);
        if (found != m_nodes.end())
            return found->second;
        return nullptr;
    }

    template<typename T>
    void Octree<T>::delete_node(unsigned int loc)
    {
        auto found = m_nodes.find(loc);
        if (found != m_nodes.end())
        {
            delete found->second;
            m_nodes.erase(found);
        }
    }

    template<typename T>
    void Octree<T>::children_nodes(node* n, std::vector<node*>& childrens, int level)const
    {
        if (level == 0)
        {
            childrens.push_back(n);
            return;
        }
        if (level < 0)
        {
            childrens.push_back(n);
            if (n->children_active == 0)
                return;
        }

        for (int i = 0; i < 8; i++)
            if (n->children_active & (1 << i))
            {
                int lc = (n->locational_code << 3) + i;

                node* found = find_node(lc);
                if (found)
                    children_nodes(found, childrens, level - 1);
            }
    }

    template<typename T>
    void Octree<T>::set_root_size(unsigned s)
    {
        m_root_size = s;
    }

    template<typename T>
    void Octree<T>::set_levels(unsigned l)
    {
        m_levels = l;
    }
}
#endif
