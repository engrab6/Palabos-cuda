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
 * Atomic block -- implementation file.
 */
#include "atomicBlock/atomicBlock3D.h"
#include "atomicBlock/atomicBlockSerializer3D.h"

namespace plb {

/* *************** Class StatSubscriber3D *********************************** */

StatSubscriber3D::StatSubscriber3D(AtomicBlock3D& block_)
    : block(block_)
{ }

plint StatSubscriber3D::subscribeAverage() {
    return block.getInternalStatistics().subscribeAverage();
}

plint StatSubscriber3D::subscribeSum() {
    return block.getInternalStatistics().subscribeSum();
}

plint StatSubscriber3D::subscribeMax() {
    return block.getInternalStatistics().subscribeMax();
}

plint StatSubscriber3D::subscribeIntSum() {
    return block.getInternalStatistics().subscribeIntSum();
}


/* *************** Class AtomicBlock3D ************************************** */

AtomicBlock3D::AtomicBlock3D(plint nx_, plint ny_, plint nz_)
    : nx(nx_), ny(ny_), nz(nz_),
      location(0,0,0),
      flag(false),
      internalStatistics(),
      statisticsSubscriber(*this)
{ }

AtomicBlock3D::AtomicBlock3D(AtomicBlock3D const& rhs)
    : nx(rhs.nx), ny(rhs.ny), nz(rhs.nz),
      location(rhs.location),
      flag(rhs.flag),
      internalStatistics(rhs.internalStatistics),
      statisticsSubscriber(*this)
{
    copyDataProcessors (
            rhs.explicitInternalProcessors, explicitInternalProcessors );
    copyDataProcessors (
            rhs.automaticInternalProcessors, automaticInternalProcessors );
}

AtomicBlock3D::~AtomicBlock3D()
{
    clearDataProcessors();
}

void AtomicBlock3D::swap(AtomicBlock3D& rhs) {
    std::swap(nx, rhs.nx);
    std::swap(ny, rhs.ny);
    std::swap(nz, rhs.nz);
    std::swap(location, rhs.location);
    std::swap(flag, rhs.flag);
    std::swap(internalStatistics, rhs.internalStatistics);
    explicitInternalProcessors.swap(rhs.explicitInternalProcessors);
    automaticInternalProcessors.swap(rhs.automaticInternalProcessors);
}

void AtomicBlock3D::initialize() {
    executeInternalProcessors();
}

BlockStatistics& AtomicBlock3D::getInternalStatistics() {
    return internalStatistics;
}

BlockStatistics const& AtomicBlock3D::getInternalStatistics() const {
    return internalStatistics;
}

void AtomicBlock3D::setLocation(Dot3D const& location_) {
    location = location_;
}

Dot3D AtomicBlock3D::getLocation() const {
    return location;
}

void AtomicBlock3D::setFlag(bool value) {
    flag = value;
}

bool AtomicBlock3D::getFlag() const {
    return flag;
}

Box3D AtomicBlock3D::getBoundingBox() const {
    return Box3D(0, getNx()-1, 0, getNy()-1, 0, getNz()-1);
}

void AtomicBlock3D::integrateDataProcessor (
    DataProcessor3D* processor, plint level )
{
    // Negative level numbers account for explicit internal BlockProcessors
    if (level<0) {
        integrateDataProcessor(processor, -level-1, explicitInternalProcessors);
    }
    // Positive-or-zero level numbers account for automatic internal BlockProcessors
    else {
        integrateDataProcessor(processor, level, automaticInternalProcessors);
    }
}

void AtomicBlock3D::integrateDataProcessor (
    DataProcessor3D* processor, plint level, DataProcessorVector& processors )
{
    if (level >= (plint)processors.size()) {
        processors.resize(level+1);
    }
    processors[level].push_back(processor);
}

void AtomicBlock3D::copyDataProcessors(DataProcessorVector const& from, DataProcessorVector& to) {
    clearDataProcessors(to);
    to.resize(from.size());
    for (pluint iLevel=0; iLevel<from.size(); ++iLevel) {
        to[iLevel].resize(from[iLevel].size());
        for (pluint iProc=0; iProc<from[iLevel].size(); ++iProc) {
            to[iLevel][iProc] = from[iLevel][iProc]->clone();
        }
    }
}

void AtomicBlock3D::clearDataProcessors() {
    clearDataProcessors(explicitInternalProcessors);
    clearDataProcessors(automaticInternalProcessors);
}

void AtomicBlock3D::removeDataProcessors(int staticId) {
    for (pluint iLevel=0; iLevel<explicitInternalProcessors.size(); ++iLevel) {
        std::vector<DataProcessor3D*>::iterator it = explicitInternalProcessors[iLevel].begin();
        for (;  it != explicitInternalProcessors[iLevel].end(); ++it) {
            if ((*it)->getStaticId()==staticId) {
                delete *it;
                it = explicitInternalProcessors[iLevel].erase(it);
            }
        }
    }
    for (pluint iLevel=0; iLevel<automaticInternalProcessors.size(); ++iLevel) {
        std::vector<DataProcessor3D*>::iterator it = automaticInternalProcessors[iLevel].begin();
        for (;  it != automaticInternalProcessors[iLevel].end(); ++it) {
            if ((*it)->getStaticId()==staticId) {
                delete *it;
                it = automaticInternalProcessors[iLevel].erase(it);
            }
        }
    }
}

void AtomicBlock3D::clearDataProcessors(DataProcessorVector& processors) {
    for (pluint iLevel=0; iLevel<processors.size(); ++iLevel) {
        for (pluint iProc=0; iProc<processors[iLevel].size(); ++iProc) {
            delete processors[iLevel][iProc];
        }
    }
    processors.clear();
}

void AtomicBlock3D::executeInternalProcessors() {
    for (pluint iLevel=0; iLevel<automaticInternalProcessors.size(); ++iLevel) {
        executeInternalProcessors(iLevel, automaticInternalProcessors);
    }
}

void AtomicBlock3D::executeInternalProcessors(plint level)
{
    // Negative level numbers account for explicit internal BlockProcessors
    if (level<0) {
        executeInternalProcessors(-level-1, explicitInternalProcessors);
    }
    // Positive-or-zero level numbers account for automatic internal BlockProcessors
    else {
        executeInternalProcessors(level, automaticInternalProcessors);
    }
}

void AtomicBlock3D::executeInternalProcessors(plint level, DataProcessorVector& processors)
{
    if (level<(plint)processors.size()) {
        for (pluint iProc=0; iProc<processors[level].size(); ++iProc) {
            processors[level][iProc] -> process();
        }
    }
}

DataSerializer* AtomicBlock3D::getBlockSerializer (
            Box3D const& domain, IndexOrdering::OrderingT ordering ) const
{
    return new AtomicBlockSerializer3D(*this, domain, ordering);
}

DataUnSerializer* AtomicBlock3D::getBlockUnSerializer (
            Box3D const& domain, IndexOrdering::OrderingT ordering )
{
    return new AtomicBlockUnSerializer3D(*this, domain, ordering);
}

StatSubscriber& AtomicBlock3D::internalStatSubscription() {
    return statisticsSubscriber;
}

void AtomicBlock3D::evaluateStatistics() {
    getInternalStatistics().evaluate();
}


Dot3D computeRelativeDisplacement(AtomicBlock3D const& block1, AtomicBlock3D const& block2) {
    return Dot3D(block1.getLocation().x-block2.getLocation().x,
                 block1.getLocation().y-block2.getLocation().y,
                 block1.getLocation().z-block2.getLocation().z);
}

}  // namespace plb
