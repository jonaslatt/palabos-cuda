/* This file is part of the Palabos library.
 *
 * Copyright (C) 2011-2015 FlowKit Sarl
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

#ifndef DYNAMIC_SMAGORINSKY_DYNAMICS_H
#define DYNAMIC_SMAGORINSKY_DYNAMICS_H

#include "core/globalDefs.h"
#include "core/dynamics.h"

namespace plb {

/// A dynamics which reads the relaxation parameter from external scalar before collision.
template<typename T, template<typename U> class Descriptor>
class ExternalOmegaDynamics : public CompositeDynamics<T,Descriptor> {
public:
    ExternalOmegaDynamics(Dynamics<T,Descriptor>* baseDynamics_)
        : CompositeDynamics<T,Descriptor>(baseDynamics_)
    { }
    virtual void prepareCollision(Cell<T,Descriptor>& cell) {
        // Copy relaxation parameter from external scalar.
        this->setOmega(*cell.getExternal(Descriptor<T>::ExternalField::omegaBeginsAt));
    }
    ExternalOmegaDynamics<T,Descriptor>* clone() const {
        return new ExternalOmegaDynamics(*this);
    }
};

} // namespace plb

#endif  // DYNAMIC_SMAGORINSKY_DYNAMICS_H
