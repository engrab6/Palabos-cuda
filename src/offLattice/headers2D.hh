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

/** \file
 * Groups all the include files for the 2D off-lattice directory.
 */

#include "offLattice/marchingCube.hh"
#include "offLattice/segmentSet.hh"
#include "offLattice/nextNeighbors2D.hh"
#include "offLattice/segmentToDef.hh"
#include "offLattice/segmentPolygonMesh2D.hh"
#include "offLattice/voxelizer2D.hh"
#include "offLattice/makeSparse2D.hh"
#include "offLattice/segmentHash.hh"
#include "offLattice/offLatticeBoundaryProcessor2D.hh"
#include "offLattice/offLatticeBoundaryProfiles2D.hh"
#include "offLattice/offLatticeBoundaryCondition2D.hh"
#include "offLattice/boundaryShapes2D.hh"
#include "offLattice/segmentBoundary2D.hh"
#include "offLattice/offLatticeModel2D.hh"
#include "offLattice/guoOffLatticeModel2D.hh"
#include "offLattice/bouzidiOffLatticeModel2D.hh"
// #include "offLattice/guoAdvDiffOffLatticeModel2D.hh"
#include "offLattice/segmentSetGenerator.hh"
#include "offLattice/immersedWalls2D.hh"
#include "offLattice/filippovaHaenel2D.hh"

#ifndef PLB_BGP
#include "offLattice/generalizedOffLatticeModel2D.hh"
#endif