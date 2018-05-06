/*
 * Copyright (c) 2016, Fabian Langguth
 * TU Darmstadt - Graphics, Capture and Massively Parallel Computing
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the LICENSE.txt file for details.
 */

#ifndef SMVS_QUAD_EDGE_HEADER
#define SMVS_QUAD_EDGE_HEADER

#include <memory>

#include "math/vector.h"

#include "defines.h"

SMVS_NAMESPACE_BEGIN

/*
 * Edge and QuadEdge datastructure based on:
 *   Leonidas J. Guibas and Jorge Stolfi,
 *   Primitives for the Manipulation of General Subdivisions and the
 *   Computation of Voronoi Diagrams, ACM ToG 4(2):74–123, April 1985
 */
class QuadEdge;
class Edge
{
public:
    friend QuadEdge;
    typedef Edge * Ptr;
    typedef std::size_t VertexID;
    typedef std::size_t FaceID;

public:
    static void splice (Edge::Ptr a, Edge::Ptr b);

    /// inverse edge
    Ptr inv (void);
    /// dual left-to-right edge
    Ptr dual (void);
    /// dual right-to-left edge
    Ptr inv_dual (void);

    Ptr o_next (void);
    Ptr o_prev (void);
    Ptr d_next (void);
    Ptr d_prev (void);

    Ptr l_next (void);
    Ptr l_prev (void);
    Ptr r_next (void);
    Ptr r_prev (void);

    VertexID & dest (void);
    VertexID & orig (void);
    VertexID const& dest (void) const;
    VertexID const& orig (void) const;
    void set_vertex_ids (VertexID orig, VertexID dest);

    FaceID & left (void);
    FaceID & right (void);
    void set_left_face (FaceID left);
    void set_right_face (FaceID right);

private:
    Edge (void) {next = nullptr;}

    int quad_id;
    Ptr next;
    VertexID vert_id;
    FaceID face_id;
};

class QuadEdge
{
public:
    QuadEdge (void);
    static Edge::Ptr create_edge (std::vector<std::unique_ptr<QuadEdge>> * qs);

private:
    Edge edges[4];
};

/* ------------------------ Implementation ------------------------ */
inline
QuadEdge::QuadEdge (void)
{
    edges[0].quad_id = 0;
    edges[1].quad_id = 1;
    edges[2].quad_id = 2;
    edges[3].quad_id = 3;
    edges[0].next = &(edges[0]);
    edges[1].next = &(edges[3]);
    edges[2].next = &(edges[2]);
    edges[3].next = &(edges[1]);
}

inline Edge::Ptr
QuadEdge::create_edge (std::vector<std::unique_ptr<QuadEdge>> * q_edges)
{
    q_edges->emplace_back(new QuadEdge());
    return q_edges->back()->edges;
}

inline Edge::Ptr
Edge::inv (void)
{
    return this->quad_id < 2 ? this + 2 : this - 2;
}

inline Edge::Ptr
Edge::dual (void)
{
    return this->quad_id < 3 ? this + 1 : this - 3;
}

inline Edge::Ptr
Edge::inv_dual (void)
{
    return this->quad_id > 0 ? this - 1 : this + 3;
}

/* Origin */
inline Edge::Ptr
Edge::o_next (void)
{
    return next;
}

inline Edge::Ptr
Edge::o_prev (void)
{
    return this->dual()->o_next()->dual();
}

/* Destination */
inline Edge::Ptr
Edge::d_next (void)
{
    return this->inv()->o_next()->inv();
}

inline Edge::Ptr
Edge::d_prev (void)
{
    return this->inv_dual()->o_next()->inv_dual();
}

/* Left */
inline Edge::Ptr
Edge::l_next (void)
{
    return this->inv_dual()->o_next()->dual();
}

inline Edge::Ptr
Edge::l_prev (void)
{
    return this->o_next()->inv();
}

/* Right */
inline Edge::Ptr
Edge::r_next (void)
{
    return this->dual()->o_next()->inv_dual();
}

inline Edge::Ptr
Edge::r_prev (void)
{
    return this->inv()->o_next();
}

/* Vertices */
inline Edge::VertexID &
Edge::orig (void)
{
    return vert_id;
}

inline Edge::VertexID &
Edge::dest (void)
{
    return this->inv()->orig();
}

inline Edge::VertexID const&
Edge::orig (void) const
{
    return vert_id;
}

inline Edge::VertexID const&
Edge::dest (void) const
{
    return this->quad_id < 2 ? (this + 2)->vert_id : (this - 2)->vert_id;
}

inline void
Edge::set_vertex_ids(Edge::VertexID orig, Edge::VertexID dest)
{
    this->vert_id = orig;
    this->dest() = dest;
}

/* Faces */
inline Edge::FaceID &
Edge::left (void)
{
    return this->dual()->face_id;
}

inline Edge::FaceID &
Edge::right (void)
{
    return this->inv_dual()->face_id;
}

inline void
Edge::set_left_face (FaceID left)
{
    this->dual()->face_id = left;
}

inline void
Edge::set_right_face (FaceID right)
{
    this->inv_dual()->face_id = right;
}

inline void
Edge::splice(Edge::Ptr a, Edge::Ptr b)
{
    /* Get duals of next ptr */
    Edge::Ptr alpha = a->o_next()->dual();
    Edge::Ptr beta = b->o_next()->dual();

    /* tmp values for swap */
    Edge::Ptr a_o_next = a->o_next();
    Edge::Ptr b_o_next = b->o_next();
    Edge::Ptr alpha_o_next = alpha->o_next();
    Edge::Ptr beta_o_next = beta->o_next();

    /* Swap next of a,b and their duals alpha,beta */
    a->next = b_o_next;
    b->next = a_o_next;
    alpha->next = beta_o_next;
    beta->next = alpha_o_next;
}

SMVS_NAMESPACE_END

#endif /* SMVS_QUAD_EDGE_HEADER */
