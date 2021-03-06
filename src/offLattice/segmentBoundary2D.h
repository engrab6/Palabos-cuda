/* This file is part of the Palabos library.
 *
 * Copyright (C) 2011-2013 FlowKit Sarl
 * Route d'Oron 2
 * 1010 Lausanne, Switzerland
 * E-mail contact: contact@flowkit.com
 *
 * The most recent release of Palabos can be downloaded at 
 * <http://www.palabos.org/>
 *
 * The library Palabos is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * The library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INNER_FLOW_BOUNDARY_2D_H
#define INNER_FLOW_BOUNDARY_2D_H

#include "core/globalDefs.h"
#include "offLattice/boundaryShapes2D.h"
#include "offLattice/segmentSet.h"
#include "offLattice/segmentPolygonMesh2D.h"
#include "offLattice/offLatticeBoundaryProfiles2D.h"
#include "particles/multiParticleField2D.h"
#include "multiBlock/redistribution2D.h"
#include "multiBlock/multiDataField2D.h"
#include "atomicBlock/atomicContainerBlock2D.h"
#include "multiBlock/multiContainerBlock2D.h"
#include <stack>

namespace plb {

template<typename T>
class DEFscaledMesh2D {
public:
    DEFscaledMesh2D(SegmentSet<T> const& segmentSet_);
    DEFscaledMesh2D (
        SegmentSet<T> const& segmentSet_,
        plint resolution_, plint referenceDirection_,
        plint margin_, plint extraLayer );
    DEFscaledMesh2D (
        SegmentSet<T> const& segmentSet_,
        plint resolution_, plint referenceDirection_,
        plint margin_, Dot2D location );
    DEFscaledMesh2D(DEFscaledMesh2D<T> const& rhs);
    ~DEFscaledMesh2D();
    DEFscaledMesh2D<T>& operator=(DEFscaledMesh2D<T> const& rhs);
    void swap(DEFscaledMesh2D<T>& rhs);
public: // Mesh usage interface.
    /// Get a reference to the currently active mesh.
    SegmentPolygonMesh2D<T>& getMesh();
    /// Get a const reference to the currently active mesh.
    SegmentPolygonMesh2D<T> const& getMesh() const;
    plint getMargin() const;
    std::vector<Array<T,2> > const& getVertexList() const {
        return vertexList;
    }
    std::vector<plint> const& getEmanatingEdgeList() const {
        return emanatingEdgeList;
    }
    std::vector<Edge2D> const& getEdgeList() const {
            return edgeList;
        }
    Array<T,2> getPhysicalLocation() const {
        return physicalLocation;
    }
    T getDx() const {
        return dx;
    }
    void setPhysicalLocation(Array<T,2> physicalLocation_) {
        physicalLocation = physicalLocation_;
    }
    void setDx(T dx_) {
        dx = dx_;
    }
private:
    void initialize (
        SegmentSet<T> const& segmentSet_, plint resolution_,
        plint referenceDirection_, Dot2D location );
private:
    std::vector<Array<T,2> > vertexList;
    /// Each vertex has exactly one emanating edge. This is a structural
    ///   information.
    std::vector<plint> emanatingEdgeList;
    /// Edges are a structural information.
    std::vector<Edge2D> edgeList;
    SegmentPolygonMesh2D<T>* mesh;
    plint margin;
    Array<T,2> physicalLocation;
    T dx;
};

template<typename T>
struct VertexProperty2D {
    virtual ~VertexProperty2D() { }
    // Wall portions which should not follow the laws of elasticity are rigid.
    virtual bool isRigid() const =0;
    // Inlet/outlet nodes are for example not part of the wall.
    virtual bool isWall() const =0;
    virtual VertexProperty2D<T>* clone() const =0;
};

template<typename T>
struct RigidWallProperty2D : public VertexProperty2D<T> {
    virtual bool isRigid() const { return true; }
    virtual bool isWall() const { return true; }
    virtual RigidWallProperty2D<T>* clone() const {
        return new RigidWallProperty2D<T>(*this);
    }
};

template<typename T>
struct InletOutletProperty2D : public VertexProperty2D<T> {
    virtual bool isRigid() const { return true; }
    virtual bool isWall() const { return false; }
    virtual InletOutletProperty2D<T>* clone() const {
        return new InletOutletProperty2D<T>(*this);
    }
};

template<typename T>
bool isRigid(VertexProperty2D<T> const* property) {
    if (property) {
        return property->isRigid();
    }
    else {
        return false;
    }
}

template<typename T>
bool isWall(VertexProperty2D<T> const* property) {
    if (property) {
        return property->isWall();
    }
    else {
        return true;
    }
}

template<typename T> class SegmentBoundary2D;

template<typename T, class SurfaceData>
class BoundaryProfiles2D {
public:    
    BoundaryProfiles2D();
    ~BoundaryProfiles2D();
    BoundaryProfiles2D(BoundaryProfiles2D<T,SurfaceData> const& rhs);
    BoundaryProfiles2D<T,SurfaceData>& operator=(BoundaryProfiles2D<T,SurfaceData> const& rhs);
    void swap(BoundaryProfiles2D<T,SurfaceData>& rhs);
    void defineProfile(plint tag, BoundaryProfile2D<T,SurfaceData>* profile);
    void resetProfiles(std::map<plint,BoundaryProfile2D<T,SurfaceData>*> profiles_);
    void defineInletOutletTags(SegmentBoundary2D<T> const& boundary, plint sortDirection);
    void setWallProfile(BoundaryProfile2D<T,SurfaceData>* wallProfile_);
    void setInletOutlet( std::vector<BoundaryProfile2D<T,SurfaceData>*> inletOutlets );
    void setInletOutlet( BoundaryProfile2D<T,SurfaceData>* profile1, BoundaryProfile2D<T,SurfaceData>* profile2 );
    void setInletOutlet( BoundaryProfile2D<T,SurfaceData>* profile1, BoundaryProfile2D<T,SurfaceData>* profile2,
                         BoundaryProfile2D<T,SurfaceData>* profile3 );
    void setInletOutlet( BoundaryProfile2D<T,SurfaceData>* profile1, BoundaryProfile2D<T,SurfaceData>* profile2,
                         BoundaryProfile2D<T,SurfaceData>* profile3, BoundaryProfile2D<T,SurfaceData>* profile4 );
    void adjustInletOutlet(SegmentBoundary2D<T> const& boundary, plint sortDirection);
    BoundaryProfile2D<T,SurfaceData> const& getProfile (
            SegmentBoundary2D<T> const& boundary, plint iSegment ) const;
private:
    void replaceProfile(plint id, BoundaryProfile2D<T,SurfaceData>* newProfile);
    void clearProfiles();
private:
    BoundaryProfile2D<T,SurfaceData>* wallProfile;
    std::map<plint,BoundaryProfile2D<T,SurfaceData>*> profiles;
    std::vector<plint> inletOutletIds;
    std::vector<Array<T,2> > lidNormal;
    std::vector<Array<T,2> > lidCenter;
    std::vector<T> lidRadius;
};

template<typename T>
class SegmentBoundary2D {
public:
    template<typename TMesh>
    SegmentBoundary2D(DEFscaledMesh2D<TMesh> const& defMesh, bool automaticCloseHoles=true);
    SegmentBoundary2D(DEFscaledMesh2D<T> const& defMesh, bool automaticCloseHoles=true);
    ~SegmentBoundary2D();
    SegmentBoundary2D(SegmentBoundary2D<T> const& rhs);
    SegmentBoundary2D<T>& operator=(SegmentBoundary2D<T> const& rhs);
    void swap(SegmentBoundary2D<T>& rhs);
public: // Mesh usage interface.
    /// Select the mesh which is subsequently being referred to in calls to
    ///   the methods of this class.
    SegmentBoundary2D<T> const& select (
            plint whichTopology, plint whichVertices ) const;
    /// Select the mesh which is subsequently being referred to in calls to
    ///   the methods of this class. Save the previous selection, which
    ///   can be recovered through a call to popSelect.
    SegmentBoundary2D<T> const& pushSelect (
            plint whichTopology, plint whichVertices ) const;
    /// Recover the previous selection of the mesh, stored through a
    ///   call to pushSelect.
    SegmentBoundary2D<T> const& popSelect() const;
    void getSelection(plint& whichTopology, plint& whichVertices) const;
    /// Get a reference to the currently active mesh.
    SegmentPolygonMesh2D<T>& getMesh();
    /// Get a const reference to the currently active mesh.
    SegmentPolygonMesh2D<T> const& getMesh() const;
    /// Get the material property (for example the elasticity constants)
    ///   implemented on a given vertex (the answer is independent of the
    ///   currently active mesh).
    VertexProperty2D<T> const* getVertexProperty(plint iVertex) const;
    /// Get intersection between a line segment (fromPoint,fromPoint+direction)
    ///   and a given segment in the currently active mesh; return true incase
    ///   of success.
    bool intersectSegment (
            plint iSegment, AtomicBlock2D* boundaryArg,
            Array<T,2> const& fromPoint, Array<T,2> const& direction,
            Array<T,2>& locatedPoint, T& distance, Array<T,2>& wallNormal ) const;
    /// Given a point p on the surface of the shape, determine its "continuous normal".
    ///   If the shape is for example piecewise linear, the normal is adjusted to vary
    ///   continuously over the surface.
    Array<T,2> computeContinuousNormal (
            Array<T,2> const& p, plint iSegment, bool isAreaWeighted = false ) const;
    /// Create a new set of vertices, and an associated open and closed mesh.
    void cloneVertexSet(plint whichVertexSet);
    /// Coordinates of the lower-left corner in physical units.
    Array<T,2> getPhysicalLocation() const {
        return physicalLocation;
    }
    /// Size of a grid spacing.
    T getDx() const {
        return dx;
    }
public: // Mesh preparation interface.
    /// Get a list of all inlets and outlets (no specific sorting order).
    std::vector<Curve2D> const& getInletOutlet() const;
    /// Get a list of all inlets and outlets, sorted along a given direction.
    /** This information can be used as a hint to select the boundary condition
     *    associated to each inlet/outlet. Access the variable lid.baryCenter 
     *    to know the location of a given inlet/outlet.
     **/
    std::vector<Curve2D> getInletOutlet(plint sortDirection) const;
    template<typename DomainFunctional> plint tagDomain(DomainFunctional functional);
    template<typename DomainFunctional> plint tagDomain(DomainFunctional functional, Array<T,2> normal, T angleTolerance, plint previousTag=-1);
    std::vector<plint> getInletOutletIds(plint sortDirection) const;
    void getLidProperties (
        plint sortDirection, std::vector<Array<T,2> >& normal,
        std::vector<Array<T,2> >& center, std::vector<T>& radius ) const;
    /// Define the material property to be used on all vertices contained in
    ///   the domain specified by the domain functional.
    /** Returns the tag which was assigned to the corresponding vertices. **/
    template<typename DomainFunctional>
    plint setVertexProperty (
            VertexProperty2D<T> const& property, DomainFunctional functional );
    plint getMargin() const;
    /// Return the tag (id of boundary-portion with specific boundary condition)
    ///   of a segment.
    plint getTag(plint iSegment) const;
    std::vector<plint> const& getSegmentTags() const { return segmentTagList; }
    std::vector<plint> const& getVertexTags() const { return vertexTagList; }
private:
    plint currentMesh() const;
    void defineMeshes();
    /// Detect holes, close them and register them as potential inlets/outlets.
    ///   They are default initialized to no-slip. Use setInletOutlet to assign
    ///   different conditions.
    void closeHoles();
    void assignLidVertexProperty();
private:
    /// Assign a new tag to all segments corresponding to one of the provided inlets/outlets.
    ///   The tag is taken in increasing integer value according to a sorting
    ///   or the inlets/outlets along the given space direction.
    void tagInletOutlet (
        std::vector<Curve2D> const& newLids );
    /// There may exist more than one set of vertices, for example in
    ///   case of a moving wall which has current vertex positions and
    ///   equilibrium vertex positions.
    std::vector< std::vector<Array<T,2> > > vertexLists;
    /// Each vertex has exactly one emanating edge. This is a structural
    ///   information which is identical for all sets of vertices.
    std::vector<plint> emanatingEdgeLists[2];
    /// Edges are a structural information which is shared by all sets
    ///   of vertices.
    std::vector<Edge2D> edgeLists[2];
    /// For each set of vertices there is a mesh, with a reference to the
    ///   given vertices (individual to each mesh) and to the
    ///   emanatingEdgeList and edgeList (same for all meshes).
    std::vector<SegmentPolygonMesh2D<T> > meshes;
    /// The segment type is an indirect index which links to the boundary
    ///   condition implemented by each segment and defined in boundaryProfiles.
    std::vector<plint> segmentTagList;
    plint currentTagNum;
    /// The vertex type is an indirect index which links to generic material
    ///   properties implemented at that vertex and defined in vertexProperties.
    std::vector<plint> vertexTagList;
    /// Vertex properties, indexed by the vertex type in vertexTagList.
    std::vector<VertexProperty2D<T>*> vertexProperties;
    /// Inlets and outlets, saved as a collection of segments.
    std::vector<Curve2D> lids;
    plint margin;
    Array<T,2> physicalLocation;
    T dx;
    mutable std::stack<plint> topology;
    mutable std::stack<plint> vertexSet;
};

template< typename T, class SurfaceData >
class SegmentFlowShape2D : public BoundaryShape2D<T,SurfaceData> {
public:
    SegmentFlowShape2D (
            SegmentBoundary2D<T> const& boundary_,
            BoundaryProfiles2D<T,SurfaceData> const& profiles_ );
    virtual bool isInside(Dot2D const& location) const;
    virtual bool pointOnSurface (
            Array<T,2> const& fromPoint, Array<T,2> const& direction,
            Array<T,2>& locatedPoint, T& distance,
            Array<T,2>& wallNormal, SurfaceData& surfaceData,
            OffBoundary::Type& bdType, plint& id ) const;
    virtual Array<T,2> computeContinuousNormal (
            Array<T,2> const& p, plint id, bool isAreaWeighted = false ) const;
    virtual bool intersectsSurface (
            Array<T,2> const& p1, Array<T,2> const& p2, plint& id ) const;
    virtual plint getTag(plint id) const;
    virtual bool distanceToSurface( Array<T,2> const& point,
                                    T& distance, bool& isBehind ) const;
    virtual SegmentFlowShape2D<T,SurfaceData>* clone() const;
    /// Use this clone function to provide the meshed data to this object.
    /** The arguments are:
     *  0: The voxel flags (ScalarField2D<T>),
     *  1: The hash container (AtomicContainerBlock2D),
     *  2: The boundary argument: an additional argument needed by BoundaryProfiles
     *     in order to compute the boundary condition. In dynamic walls this is for
     *     example often a MultiParticleField, used to determined the wall velocity.
     **/
    virtual SegmentFlowShape2D<T,SurfaceData>*
                clone(std::vector<AtomicBlock2D*> args) const;
private:
    SegmentBoundary2D<T> const& boundary;
    BoundaryProfiles2D<T,SurfaceData> const& profiles;
    /// Data from previous voxelization.
    ScalarField2D<int>* voxelFlags;
    /// Needed for fast access to the mesh.
    AtomicContainerBlock2D* hashContainer;
    AtomicBlock2D* boundaryArg;
};


template<typename T>
class VoxelizedDomain2D {
public:
    VoxelizedDomain2D(SegmentBoundary2D<T> const& boundary_,
                      int flowType_, plint extraLayer_, plint borderWidth_,
                      plint envelopeWidth_, plint blockSize_,
                      plint gridLevel_=0, bool dynamicMesh_ = false);
    VoxelizedDomain2D(SegmentBoundary2D<T> const& boundary_,
                      int flowType_, Box2D const& boundingBox, plint borderWidth_,
                      plint envelopeWidth_, plint blockSize_,
                      plint gridLevel_=0, bool dynamicMesh_ = false);
    VoxelizedDomain2D(SegmentBoundary2D<T> const& boundary_,
                      int flowType_, Box2D const& boundingBox, plint borderWidth_,
                      plint envelopeWidth_, plint blockSize_,
                      Box2D const& seed,
                      plint gridLevel_=0, bool dynamicMesh_ = false);
    VoxelizedDomain2D(VoxelizedDomain2D<T> const& rhs);
    ~VoxelizedDomain2D();
    MultiScalarField2D<int>& getVoxelMatrix();
    MultiScalarField2D<int> const& getVoxelMatrix() const;
    MultiContainerBlock2D& getSegmentHash();
    MultiBlockManagement2D const& getMultiBlockManagement() const;
    template<class ParticleFieldT>
    void adjustVoxelization(MultiParticleField2D<ParticleFieldT>& particles, bool dynamicMesh);
    void reparallelize(MultiBlockRedistribute2D const& redistribute);
    SegmentBoundary2D<T> const& getBoundary() const { return boundary; }
    int getFlowType() const { return flowType; }
private:
    VoxelizedDomain2D<T>& operator=(VoxelizedDomain2D<T> const& rhs) { }
    void createSparseVoxelMatrix (
        MultiScalarField2D<int>& fullVoxelMatrix,
        plint blockSize_, plint envelopeWidth_ );
    void computeSparseVoxelMatrix (
            MultiScalarField2D<int>& fullVoxelMatrix,
            plint blockSize, plint envelopeWidth );
    void extendEnvelopeWidth (
            MultiScalarField2D<int>& fullVoxelMatrix, plint envelopeWidth );
    void createSegmentHash();
    template<class ParticleFieldT>
    void reCreateSegmentHash(MultiParticleField2D<ParticleFieldT>& particles);
    void computeOuterMask();
private:
    int flowType;
    plint borderWidth;
    SegmentBoundary2D<T> const& boundary;
    MultiScalarField2D<int>* voxelMatrix;
    MultiContainerBlock2D* segmentHash;
};

template<typename T>
void addLayer(MultiScalarField2D<T>& matrix, Box2D const& domain,
              T previousLayer);

template<typename T>
class AddLayerFunctional2D : public BoxProcessingFunctional2D_S<T> {
public:
    AddLayerFunctional2D(T previousLayer_);
    virtual void process(Box2D domain, ScalarField2D<T>& voxels);
    virtual AddLayerFunctional2D<T>* clone() const;
    virtual void getTypeOfModification(std::vector<modif::ModifT>& modified) const;
    virtual BlockDomain::DomainT appliesTo() const;
private:
    T previousLayer;
};

}  // namespace plb

#endif  // INNER_FLOW_BOUNDARY_2D_H
