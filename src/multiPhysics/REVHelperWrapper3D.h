#ifndef REV_HELPER_WRAPPER_3D_H
#define REV_HELPER_WRAPPER_3D_H
#include "multiPhysics/REVHelperFunctional3D.h"
#include "atomicBlock/dataProcessorWrapper3D.h"
#include "atomicBlock/dataField3D.h"
#include "multiBlock/multiDataField3D.h"
#include "multiBlock/multiDataProcessorWrapper3D.h"
namespace plb
{
template <typename T1,typename T2>
void computeLocalInvK3D ( MultiTensorField3D< T1, 3  >& orient,
                          MultiTensorField3D< T2, 9  >& invK,
                          const Array<T1,9> &rawInvK,
                          const Array<T1,3> &rawDir );

template <typename T1,typename T2>
std::auto_ptr<TensorField3D<T2,9> > computeLocalInvK3D (
    MultiTensorField3D<T1,3>& orient,
    const Array<T1,9> & rawInvK,
    const Array<T1,3> & rawDir );

}//namespace plb
#endif //REV_HELPER_WRAPPER_3D_H
