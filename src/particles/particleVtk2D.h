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

#ifndef PARTICLE_VTK_2D_H
#define PARTICLE_VTK_2D_H

#include "core/globalDefs.h"
#include "particles/multiParticleField2D.h"
#include "offLattice/segmentBoundary2D.h"
#include "algorithm/functions.h"
#include <vector>
#include <string>

namespace plb {

template< typename T,
          template<typename U> class Descriptor,
          class BoundaryType >
void writeSurfaceVTK( SegmentBoundary2D<T> const& boundary,
                      MultiParticleField2D<DenseParticleField2D<T,Descriptor> >& particles,
                      std::vector<std::string> const& scalars,
                      std::vector<std::string> const& vectors,
                      std::string const& fName, bool dynamicMesh, plint tag );


template< typename T,
          template<typename U> class Descriptor,
          class BoundaryType >
void writeSurfaceVTK( SegmentBoundary2D<T> const& boundary,
                      MultiParticleField2D<DenseParticleField2D<T,Descriptor> >& particles,
                      std::vector<std::string> const& scalars,
                      std::vector<std::string> const& vectors,
                      std::string const& fName, bool dynamicMesh, plint tag,
                      std::vector<T> const& scalarFactor, std::vector<T> const& vectorFactor );


template< typename T,
          template<typename U> class Descriptor,
          class BoundaryType >
void vtkForVertices(std::vector<Particle2D<T,Descriptor>*> const& particles,
                    SegmentBoundary2D<T> const& boundary,
                    std::vector<std::string> const& scalars,
                    std::vector<std::string> const& vectors,
                    std::string fName, bool dynamicMesh, plint tag,
                    std::vector<T> const& scalarFactor, std::vector<T> const& vectorFactor );


template< typename T,
          template<typename U> class Descriptor,
          class BoundaryType >
void writeVertexAsciiData (
                      SegmentBoundary2D<T> const& boundary,
                      MultiParticleField2D<DenseParticleField2D<T,Descriptor> >& particles,
                      std::vector<std::string> const& scalars,
                      std::vector<std::string> const& vectors,
                      std::string const& fName, bool dynamicMesh, bool printHeader,
                      std::vector<T> const& scalarFactor, std::vector<T> const& vectorFactor );

template< typename T,
          template<typename U> class Descriptor,
          class BoundaryType >
void vertexAsciiData(std::vector<Particle2D<T,Descriptor>*> const& particles,
                         SegmentBoundary2D<T> const& boundary,
                         std::vector<std::string> const& scalars,
                         std::vector<std::string> const& vectors,
                         std::string fName, bool dynamicMesh, bool printHeader,
                         std::vector<T> const& scalarFactor, std::vector<T> const& vectorFactor );

template<typename T, template<typename U> class Descriptor>
void writeAsciiParticlePos (
        MultiParticleField2D<DenseParticleField2D<T,Descriptor> >& particles,
        std::string const& fName );

template<typename T, template<typename U> class Descriptor>
void writeParticleVtk (
        MultiParticleField2D<DenseParticleField2D<T,Descriptor> >& particles,
        std::string const& fName, pluint maxNumParticlesToWrite = 0 );

template<typename T, template<typename U> class Descriptor>
void writeParticleVtk (
        MultiParticleField2D<DenseParticleField2D<T,Descriptor> >& particles,
        std::string const& fName,
        std::map<plint,std::string> const& additionalScalars,
        std::map<plint,std::string> const& additionalVectors,
        pluint maxNumParticlesToWrite = 0 );

template<typename T, template<typename U> class Descriptor>
void writeSelectedParticleVtk (
        MultiParticleField2D<DenseParticleField2D<T,Descriptor> >& particles,
        std::string const& fName, Box2D const& domain, plint tag )
{
    writeSelectedParticleVtk(particles, fName, domain, util::SelectConstInt(tag));
}

template<typename T, template<typename U> class Descriptor>
void writeSelectedParticleVtk (
        MultiParticleField2D<DenseParticleField2D<T,Descriptor> >& particles,
        std::string const& fName, Box2D const& domain, plint tag,
        std::map<plint,std::string> const& additionalScalars,
        std::map<plint,std::string> const& additionalVectors )
{
    writeSelectedParticleVtk(particles, fName, domain, util::SelectConstInt(tag),
                             additionalScalars, additionalVectors);
}

template<typename T, template<typename U> class Descriptor>
void writeSelectedParticleVtk (
        MultiParticleField2D<DenseParticleField2D<T,Descriptor> >& particles,
        std::string const& fName, Box2D const& domain, util::SelectInt const& tags );

template<typename T, template<typename U> class Descriptor>
void writeSelectedParticleVtk (
        MultiParticleField2D<DenseParticleField2D<T,Descriptor> >& particles,
        std::string const& fName, Box2D const& domain, util::SelectInt const& tags,
        std::map<plint,std::string> const& additionalScalars,
        std::map<plint,std::string> const& additionalVectors );

}  // namespace plb

#endif  // PARTICLE_VTK_2D_H
