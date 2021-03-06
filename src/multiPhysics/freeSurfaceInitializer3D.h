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

#ifndef FREE_SURFACE_INITIALIZER_3D_H
#define FREE_SURFACE_INITIALIZER_3D_H

#include "core/globalDefs.h"
#include "atomicBlock/dataProcessingFunctional3D.h"
#include "multiPhysics/freeSurfaceUtil3D.h"

namespace plb {

template< typename T,template<typename U> class Descriptor>
class DefaultInitializeFreeSurface3D : public BoxProcessingFunctional3D {
public:
    DefaultInitializeFreeSurface3D(Dynamics<T,Descriptor>* dynamicsTemplate_, Array<T,3> g_, T rhoIni_=(T)1.)
        : dynamicsTemplate(dynamicsTemplate_), g(g_), rhoIni(rhoIni_)
    { }
    DefaultInitializeFreeSurface3D(DefaultInitializeFreeSurface3D<T,Descriptor> const& rhs)
        : dynamicsTemplate(rhs.dynamicsTemplate->clone()),
          g(rhs.g), rhoIni(rhs.rhoIni)
    { }
    DefaultInitializeFreeSurface3D<T,Descriptor>* operator=(DefaultInitializeFreeSurface3D<T,Descriptor> const& rhs)
    { 
        DefaultInitializeFreeSurface3D<T,Descriptor>(rhs).swap(*this);
        return *this;
    }
    void swap(DefaultInitializeFreeSurface3D<T,Descriptor>& rhs) {
        std::swap(dynamicsTemplate, rhs.dynamicsTemplate);
        std::swap(g, rhs.g);
        std::swap(rhoIni, rhs.rhoIni);
    }
    virtual ~DefaultInitializeFreeSurface3D() {
        delete dynamicsTemplate;
    }
    virtual void processGenericBlocks(Box3D domain, std::vector<AtomicBlock3D*> atomicBlocks);
    virtual DefaultInitializeFreeSurface3D<T,Descriptor>* clone() const {
        return new DefaultInitializeFreeSurface3D(*this);
    }
    virtual void getTypeOfModification(std::vector<modif::ModifT>& modified) const {
        std::fill(modified.begin(), modified.end(), modif::nothing);
        modified[0] = modif::dataStructure;    // Fluid
        modified[1] = modif::staticVariables;  // rhoBar.
        modified[2] = modif::staticVariables;  // j.
        modified[3] = modif::staticVariables;  // Mass
        modified[4] = modif::staticVariables;  // Volume-fraction
        modified[5] = modif::staticVariables;  // Flag-status
        modified[6] = modif::nothing;          // Normal.
        modified[7] = modif::nothing;          // Interface lists
        modified[8] = modif::nothing;          // Curvature.
        modified[9] = modif::nothing;          // Outside density.
    }
private:
    Dynamics<T,Descriptor>* dynamicsTemplate;
    Array<T,3> g;
    T rhoIni;
};

// Same as DefaultInitializeFreeSurface, but without initializing the Volume-fraction.
template< typename T,template<typename U> class Descriptor>
class PartiallyDefaultInitializeFreeSurface3D : public BoxProcessingFunctional3D {
public:
    PartiallyDefaultInitializeFreeSurface3D(Dynamics<T,Descriptor>* dynamicsTemplate_, Array<T,3> g_, T rhoIni_=(T)1.)
        : dynamicsTemplate(dynamicsTemplate_), g(g_), rhoIni(rhoIni_)
    { }
    PartiallyDefaultInitializeFreeSurface3D(PartiallyDefaultInitializeFreeSurface3D<T,Descriptor> const& rhs)
        : dynamicsTemplate(rhs.dynamicsTemplate->clone()),
          g(rhs.g),
          rhoIni(rhs.rhoIni)
    { }
    PartiallyDefaultInitializeFreeSurface3D<T,Descriptor>* operator= (
            PartiallyDefaultInitializeFreeSurface3D<T,Descriptor> const& rhs )
    { 
        PartiallyDefaultInitializeFreeSurface3D<T,Descriptor>(rhs).swap(*this);
        return *this;
    }
    void swap(PartiallyDefaultInitializeFreeSurface3D<T,Descriptor>& rhs) {
        std::swap(dynamicsTemplate, rhs.dynamicsTemplate);
        std::swap(g, rhs.g);
        std::swap(rhoIni, rhs.rhoIni);
    }
    virtual ~PartiallyDefaultInitializeFreeSurface3D() {
        delete dynamicsTemplate;
    }
    virtual void processGenericBlocks(Box3D domain, std::vector<AtomicBlock3D*> atomicBlocks);
    virtual PartiallyDefaultInitializeFreeSurface3D<T,Descriptor>* clone() const {
        return new PartiallyDefaultInitializeFreeSurface3D(*this);
    }
    virtual void getTypeOfModification(std::vector<modif::ModifT>& modified) const {
        std::fill(modified.begin(), modified.end(), modif::nothing);
        modified[0] = modif::dataStructure;    // Fluid
        modified[1] = modif::staticVariables;  // rhoBar.
        modified[2] = modif::staticVariables;  // j.
        modified[3] = modif::staticVariables;  // Mass
        modified[4] = modif::staticVariables;  // Volume-fraction
        modified[5] = modif::staticVariables;  // Flag-status
        modified[6] = modif::nothing;          // Normal.
        modified[7] = modif::nothing;          // Interface lists
        modified[8] = modif::nothing;          // Curvature.
        modified[9] = modif::nothing;          // Outside density.
    }
private:
    Dynamics<T,Descriptor>* dynamicsTemplate;
    Array<T,3> g;
    T rhoIni;
};

template<typename T,class InsideFunction>
class AnalyticalIniVolumeFraction3D : public BoxProcessingFunctional3D {
public:
    AnalyticalIniVolumeFraction3D(InsideFunction const& insideFunction_, plint subDivision_=5)
        : insideFunction(insideFunction_),
          subDivision(subDivision_)
    { PLB_ASSERT( subDivision > 1 ); }
    virtual void processGenericBlocks(Box3D domain, std::vector<AtomicBlock3D*> atomicBlocks);
    virtual AnalyticalIniVolumeFraction3D<T,InsideFunction>* clone() const {
        return new AnalyticalIniVolumeFraction3D<T,InsideFunction>(*this);
    }
    virtual void getTypeOfModification(std::vector<modif::ModifT>& modified) const {
        modified[0] = modif::staticVariables;  // Volume-fraction
        modified[1] = modif::staticVariables;  // Flag-status
    }
private:
    void subDomainVolumeFraction (
            plint iX, plint iY, plint iZ, int& flag, T& volumeFraction );
private:
    InsideFunction const& insideFunction;
    plint subDivision;
};

template<typename T, class InsideFunction>
void analyticalIniVolumeFraction (
        MultiScalarField3D<T>& volumeFraction, MultiScalarField3D<int>& flagStatus,
        InsideFunction const& insideFunction, plint subDivision = 5 );


template<typename T, template<typename U> class Descriptor>
class ConstantIniVelocityFreeSurface3D : public BoxProcessingFunctional3D {
public:
    ConstantIniVelocityFreeSurface3D(Array<T,3> velocity_, T rhoIni_)
        : velocity(velocity_),
          rhoIni(rhoIni_)
    { }
    virtual void processGenericBlocks(Box3D domain, std::vector<AtomicBlock3D*> atomicBlocks);
    virtual ConstantIniVelocityFreeSurface3D<T,Descriptor>* clone() const {
        return new ConstantIniVelocityFreeSurface3D<T,Descriptor>(*this);
    }
    virtual void getTypeOfModification (std::vector<modif::ModifT>& modified) const {
        std::fill(modified.begin(), modified.end(), modif::nothing);
        modified[0] = modif::staticVariables; // Fluid.
        modified[1] = modif::nothing;         // rhoBar.
        modified[2] = modif::staticVariables; // j.
        modified[3] = modif::nothing;         // Mass.
        modified[4] = modif::nothing;         // Volume-fraction.
        modified[5] = modif::nothing;         // Flag-status.
        modified[6] = modif::nothing;         // Normal.
        modified[7] = modif::nothing;         // Interface-lists.
        modified[8] = modif::nothing;         // Curvature.
        modified[9] = modif::nothing;         // Outside density.
    }
private:
    Array<T,3> velocity;
    T rhoIni;
};

template<typename T, template<typename U> class Descriptor>
class InletConstVolumeFraction3D : public BoxProcessingFunctional3D {
public:
    InletConstVolumeFraction3D(T volumeFraction_)
        : volumeFraction(volumeFraction_)
    { }
    virtual void processGenericBlocks(Box3D domain, std::vector<AtomicBlock3D*> atomicBlocks);
    virtual InletConstVolumeFraction3D<T,Descriptor>* clone() const {
        return new InletConstVolumeFraction3D<T,Descriptor>(*this);
    }
    virtual void getTypeOfModification (std::vector<modif::ModifT>& modified) const {
        std::fill(modified.begin(), modified.end(), modif::nothing);
        modified[0] = modif::nothing;         // Fluid.
        modified[1] = modif::nothing;         // rhoBar.
        modified[2] = modif::nothing;         // j.
        modified[3] = modif::staticVariables; // Mass.
        modified[4] = modif::nothing;         // Volume-fraction.
        modified[5] = modif::nothing;         // Flag-status.
        modified[6] = modif::nothing;         // Normal.
        modified[7] = modif::nothing;         // Interface-lists.
        modified[8] = modif::nothing;         // Curvature.
        modified[9] = modif::nothing;         // Outside density.
    }
private:
    T volumeFraction;
};

template<typename T, template<typename U> class Descriptor>
class OutletMaximumVolumeFraction3D : public BoxProcessingFunctional3D {
public:
    OutletMaximumVolumeFraction3D(T volumeFraction_)
        : volumeFraction(volumeFraction_)
    { }
    virtual void processGenericBlocks(Box3D domain, std::vector<AtomicBlock3D*> atomicBlocks);
    virtual OutletMaximumVolumeFraction3D<T,Descriptor>* clone() const {
        return new OutletMaximumVolumeFraction3D<T,Descriptor>(*this);
    }
    virtual void getTypeOfModification (std::vector<modif::ModifT>& modified) const {
        std::fill(modified.begin(), modified.end(), modif::nothing);
        modified[0] = modif::nothing;         // Fluid.
        modified[1] = modif::nothing;         // rhoBar.
        modified[2] = modif::nothing;         // j.
        modified[3] = modif::staticVariables; // Mass.
        modified[4] = modif::nothing;         // Volume-fraction.
        modified[5] = modif::nothing;         // Flag-status.
        modified[6] = modif::nothing;         // Normal.
        modified[7] = modif::nothing;         // Interface-lists.
        modified[8] = modif::nothing;         // Curvature.
        modified[9] = modif::nothing;         // Outside density.
    }
private:
    T volumeFraction;
};

template<typename T, template<typename U> class Descriptor>
class OutletMaximumVolumeFraction2_3D : public BoxProcessingFunctional3D {
public:
    OutletMaximumVolumeFraction2_3D(T volumeFraction_)
        : volumeFraction(volumeFraction_)
    { }
    virtual void processGenericBlocks(Box3D domain, std::vector<AtomicBlock3D*> atomicBlocks);
    virtual OutletMaximumVolumeFraction2_3D<T,Descriptor>* clone() const {
        return new OutletMaximumVolumeFraction2_3D<T,Descriptor>(*this);
    }
    virtual void getTypeOfModification (std::vector<modif::ModifT>& modified) const {
        std::fill(modified.begin(), modified.end(), modif::nothing);
        modified[0] = modif::staticVariables; // Fluid.
        modified[1] = modif::nothing;         // rhoBar.
        modified[2] = modif::nothing;         // j.
        modified[3] = modif::staticVariables; // Mass.
        modified[4] = modif::nothing;         // Volume-fraction.
        modified[5] = modif::nothing;         // Flag-status.
        modified[6] = modif::nothing;         // Normal.
        modified[7] = modif::nothing;         // Interface-lists.
        modified[8] = modif::nothing;         // Curvature.
        modified[9] = modif::nothing;         // Outside density.
    }
private:
    T volumeFraction;
};

template<typename T, template<typename U> class Descriptor>
class NoSlipMaximumVolumeFraction3D : public BoxProcessingFunctional3D {
public:
    NoSlipMaximumVolumeFraction3D(T volumeFraction_)
        : volumeFraction(volumeFraction_)
    { }
    virtual void processGenericBlocks(Box3D domain, std::vector<AtomicBlock3D*> atomicBlocks);
    virtual NoSlipMaximumVolumeFraction3D<T,Descriptor>* clone() const {
        return new NoSlipMaximumVolumeFraction3D<T,Descriptor>(*this);
    }
    virtual void getTypeOfModification (std::vector<modif::ModifT>& modified) const {
        std::fill(modified.begin(), modified.end(), modif::nothing);
        modified[0] = modif::nothing;         // Fluid.
        modified[1] = modif::staticVariables; // rhoBar.
        modified[2] = modif::nothing;         // j.
        modified[3] = modif::nothing;         // Mass.
        modified[4] = modif::nothing;         // Volume-fraction.
        modified[5] = modif::nothing;         // Flag-status.
        modified[6] = modif::nothing;         // Normal.
        modified[7] = modif::nothing;         // Interface-lists.
        modified[8] = modif::nothing;         // Curvature.
        modified[9] = modif::nothing;         // Outside density.
    }
private:
    T volumeFraction;
};

template<typename T, template<typename U> class Descriptor>
class PunchSphere3D : public BoxProcessingFunctional3D {
public:
    PunchSphere3D(Array<T,3> const& center_, T radius_, T rho0_)
        : center(center_),
          radius(radius_),
          rho0(rho0_)
    { }
    virtual void processGenericBlocks(Box3D domain, std::vector<AtomicBlock3D*> atomicBlocks);
    virtual PunchSphere3D<T,Descriptor>* clone() const {
        return new PunchSphere3D<T,Descriptor>(*this);
    }
    virtual void getTypeOfModification (std::vector<modif::ModifT>& modified) const {
        std::fill(modified.begin(), modified.end(), modif::nothing);
        modified[0] = modif::staticVariables; // Fluid.
        modified[1] = modif::staticVariables; // rhoBar.
        modified[2] = modif::staticVariables; // j.
        modified[3] = modif::staticVariables; // Mass.
        modified[4] = modif::staticVariables; // Volume-fraction.
        modified[5] = modif::staticVariables; // Flag-status.
        modified[6] = modif::nothing;         // Normal.
        modified[7] = modif::nothing;         // Interface-lists.
        modified[8] = modif::staticVariables; // Curvature.
        modified[9] = modif::staticVariables; // Outside density.
    }
private:
    Array<T,3> center;
    T radius;
    T rho0;
};

template<typename T, template<typename U> class Descriptor>
class CalculateAverageSphereDensity3D : public PlainReductiveBoxProcessingFunctional3D {
public:
    CalculateAverageSphereDensity3D(Array<T,3> const& center_, T radius_)
        : center(center_),
          radius(radius_),
          averageDensityId(this->getStatistics().subscribeAverage())
    { }
    virtual void processGenericBlocks(Box3D domain, std::vector<AtomicBlock3D*> atomicBlocks);
    virtual CalculateAverageSphereDensity3D<T,Descriptor>* clone() const {
        return new CalculateAverageSphereDensity3D<T,Descriptor>(*this);
    }
    T getAverageDensity() const {
        return this->getStatistics().getAverage(averageDensityId);
    }
    virtual void getTypeOfModification (std::vector<modif::ModifT>& modified) const {
        std::fill(modified.begin(), modified.end(), modif::nothing);
        modified[0] = modif::staticVariables; // Fluid.
        modified[1] = modif::staticVariables; // rhoBar.
        modified[2] = modif::staticVariables; // j.
        modified[3] = modif::staticVariables; // Mass.
        modified[4] = modif::staticVariables; // Volume-fraction.
        modified[5] = modif::staticVariables; // Flag-status.
        modified[6] = modif::nothing;         // Normal.
        modified[7] = modif::nothing;         // Interface-lists.
        modified[8] = modif::staticVariables; // Curvature.
        modified[9] = modif::staticVariables; // Outside density.
    }
private:
    Array<T,3> center;
    T radius;
    plint averageDensityId;
};


}  // namespace plb

#endif  // FREE_SURFACE_INITIALIZER_3D_H

