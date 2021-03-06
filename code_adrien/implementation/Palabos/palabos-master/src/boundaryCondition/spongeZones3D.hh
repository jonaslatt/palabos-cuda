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

/** \file
 * Sponge (absorbing) zones, to be mainly used in addition to outflow boundary conditions -- generic implementation.
 */

#ifndef SPONGE_ZONES_3D_HH
#define SPONGE_ZONES_3D_HH

#include "boundaryCondition/spongeZones3D.h"
#include "atomicBlock/atomicBlock3D.h"
#include "atomicBlock/blockLattice3D.h"
#include "atomicBlock/dataField3D.h"
#include <cmath>

namespace plb {

/* ******************** class ViscositySpongeZone3D *********************************** */

template<typename T, template<typename U> class Descriptor>
ViscositySpongeZone3D<T,Descriptor>::ViscositySpongeZone3D(plint nx_, plint ny_, plint nz_, T bulkOmega_,
        Array<plint,6> const& numSpongeCells_, Array<T,6> const& translationParameters_,
        Array<T,6> const& scaleParameters_)
        : nx(nx_),
          ny(ny_),
          nz(nz_),
          bulkOmega(bulkOmega_),
          numSpongeCells(numSpongeCells_),
          translationParameters(translationParameters_),
          scaleParameters(scaleParameters_),
          useTanhSpongeFunction(true)
{
    for (int i = 0; i < 6; i++) {
        PLB_ASSERT(translationParameters[i] > (T) 0 && translationParameters[i] < (T) 1);
    }
}

template<typename T, template<typename U> class Descriptor>
ViscositySpongeZone3D<T,Descriptor>::ViscositySpongeZone3D(plint nx_, plint ny_, plint nz_, T bulkOmega_,
        Array<plint,6> const& numSpongeCells_)
        : nx(nx_),
          ny(ny_),
          nz(nz_),
          bulkOmega(bulkOmega_),
          numSpongeCells(numSpongeCells_),
          useTanhSpongeFunction(false)
{ }

template<typename T, template<typename U> class Descriptor>
void ViscositySpongeZone3D<T,Descriptor>::processGenericBlocks(Box3D domain, std::vector<AtomicBlock3D*> blocks)
{
    PLB_ASSERT(blocks.size() == 1);
    BlockLattice3D<T,Descriptor> *lattice = dynamic_cast<BlockLattice3D<T,Descriptor>*>(blocks[0]);
    PLB_ASSERT(lattice);

    Dot3D offset = lattice->getLocation();

    T targetOmega = 1.0; // Here we hardcode the value of the relaxation parameter to 1.0 for stability purposes.

    T alpha = targetOmega / bulkOmega;

    plint spongePositions[6]; // Sponge positions.
    spongePositions[0] =          numSpongeCells[0];
    spongePositions[1] = nx - 1 - numSpongeCells[1];
    spongePositions[2] =          numSpongeCells[2];
    spongePositions[3] = ny - 1 - numSpongeCells[3];
    spongePositions[4] =          numSpongeCells[4];
    spongePositions[5] = nz - 1 - numSpongeCells[5];

    if (useTanhSpongeFunction) {
        plint spongeCenters[6]; // Sponge centers.
        spongeCenters[0] = util::roundToInt(((T) 1             - translationParameters[0]) * numSpongeCells[0]);
        spongeCenters[1] = util::roundToInt(spongePositions[1] + translationParameters[1]  * numSpongeCells[1]);
        spongeCenters[2] = util::roundToInt(((T) 1             - translationParameters[2]) * numSpongeCells[2]);
        spongeCenters[3] = util::roundToInt(spongePositions[3] + translationParameters[3]  * numSpongeCells[3]);
        spongeCenters[4] = util::roundToInt(((T) 1             - translationParameters[4]) * numSpongeCells[4]);
        spongeCenters[5] = util::roundToInt(spongePositions[5] + translationParameters[5]  * numSpongeCells[5]);

        T sigma[6]; // Sponge parameters.
        for (int i = 0; i < 6; i++) {
            sigma[i] = scaleParameters[i] * numSpongeCells[i];
        }

        for (plint iX=domain.x0; iX<=domain.x1; ++iX) {
            plint i = iX + offset.x;
            for (plint iY=domain.y0; iY<=domain.y1; ++iY) {
                plint j = iY + offset.y;
                for (plint iZ=domain.z0; iZ<=domain.z1; ++iZ) {
                    plint k = iZ + offset.z;

                    T spongeFunction = 1.0;
                    bool modifyOmega = false;
                    if (numSpongeCells[0] > 0 && i <= spongePositions[0]) {
                        spongeFunction *= 0.5 * (1.0 + std::tanh((i - spongeCenters[0]) / sigma[0]));
                        modifyOmega = true;
                    }
                    if (numSpongeCells[1] > 0 && i >= spongePositions[1]) {
                        spongeFunction *= 0.5 * (1.0 - std::tanh((i - spongeCenters[1]) / sigma[1]));
                        modifyOmega = true;
                    }

                    if (numSpongeCells[2] > 0 && j <= spongePositions[2]) {
                        spongeFunction *= 0.5 * (1.0 + std::tanh((j - spongeCenters[2]) / sigma[2]));
                        modifyOmega = true;
                    }
                    if (numSpongeCells[3] > 0 && j >= spongePositions[3]) {
                        spongeFunction *= 0.5 * (1.0 - std::tanh((j - spongeCenters[3]) / sigma[3]));
                        modifyOmega = true;
                    }

                    if (numSpongeCells[4] > 0 && k <= spongePositions[4]) {
                        spongeFunction *= 0.5 * (1.0 + std::tanh((k - spongeCenters[4]) / sigma[4]));
                        modifyOmega = true;
                    }
                    if (numSpongeCells[5] > 0 && k >= spongePositions[5]) {
                        spongeFunction *= 0.5 * (1.0 - std::tanh((k - spongeCenters[5]) / sigma[5]));
                        modifyOmega = true;
                    }

                    if (modifyOmega) {
                        spongeFunction = (1.0 - alpha) * spongeFunction + alpha;
                        T localOmega = bulkOmega * spongeFunction;
                        lattice->get(iX, iY, iZ).getDynamics().setOmega(localOmega);
                    }
                }
            }
        }
    } else {
        T pi = std::acos((T) -1.0);

        for (plint iX=domain.x0; iX<=domain.x1; ++iX) {
            plint i = iX + offset.x;
            for (plint iY=domain.y0; iY<=domain.y1; ++iY) {
                plint j = iY + offset.y;
                for (plint iZ=domain.z0; iZ<=domain.z1; ++iZ) {
                    plint k = iZ + offset.z;

                    T spongeFunction = 1.0;
                    bool modifyOmega = false;
                    if (numSpongeCells[0] > 0 && i <= spongePositions[0]) {
                        spongeFunction *= 0.5 * (1.0 - std::cos(pi * (T) i /                        (T) numSpongeCells[0]));
                        modifyOmega = true;
                    }
                    if (numSpongeCells[1] > 0 && i >= spongePositions[1]) {
                        spongeFunction *= 0.5 * (1.0 + std::cos(pi * (T) (i - spongePositions[1]) / (T) numSpongeCells[1]));
                        modifyOmega = true;
                    }

                    if (numSpongeCells[2] > 0 && j <= spongePositions[2]) {
                        spongeFunction *= 0.5 * (1.0 - std::cos(pi * (T) j /                        (T) numSpongeCells[2]));
                        modifyOmega = true;
                    }
                    if (numSpongeCells[3] > 0 && j >= spongePositions[3]) {
                        spongeFunction *= 0.5 * (1.0 + std::cos(pi * (T) (j - spongePositions[3]) / (T) numSpongeCells[3]));
                        modifyOmega = true;
                    }

                    if (numSpongeCells[4] > 0 && k <= spongePositions[4]) {
                        spongeFunction *= 0.5 * (1.0 - std::cos(pi * (T) k /                        (T) numSpongeCells[4]));
                        modifyOmega = true;
                    }
                    if (numSpongeCells[5] > 0 && k >= spongePositions[5]) {
                        spongeFunction *= 0.5 * (1.0 + std::cos(pi * (T) (k - spongePositions[5]) / (T) numSpongeCells[5]));
                        modifyOmega = true;
                    }

                    if (modifyOmega) {
                        spongeFunction = (1.0 - alpha) * spongeFunction + alpha;
                        T localOmega = bulkOmega * spongeFunction;
                        lattice->get(iX, iY, iZ).getDynamics().setOmega(localOmega);
                    }
                }
            }
        }
    }
}


/* ******************** class MaskedViscositySpongeZone3D *********************************** */

template<typename T, template<typename U> class Descriptor>
MaskedViscositySpongeZone3D<T,Descriptor>::MaskedViscositySpongeZone3D(plint nx_, plint ny_, plint nz_, T bulkOmega_,
        int flag_, Array<plint,6> const& numSpongeCells_, Array<T,6> const& translationParameters_,
        Array<T,6> const& scaleParameters_)
        : nx(nx_),
          ny(ny_),
          nz(nz_),
          bulkOmega(bulkOmega_),
          numSpongeCells(numSpongeCells_),
          translationParameters(translationParameters_),
          scaleParameters(scaleParameters_),
          useTanhSpongeFunction(true),
          flag(flag_)
{
    for (int i = 0; i < 6; i++) {
        PLB_ASSERT(translationParameters[i] > (T) 0 && translationParameters[i] < (T) 1);
    }
}

template<typename T, template<typename U> class Descriptor>
MaskedViscositySpongeZone3D<T,Descriptor>::MaskedViscositySpongeZone3D(plint nx_, plint ny_, plint nz_, T bulkOmega_,
        int flag_, Array<plint,6> const& numSpongeCells_)
        : nx(nx_),
          ny(ny_),
          nz(nz_),
          bulkOmega(bulkOmega_),
          numSpongeCells(numSpongeCells_),
          useTanhSpongeFunction(false),
          flag(flag_)
{ }

template<typename T, template<typename U> class Descriptor>
void MaskedViscositySpongeZone3D<T,Descriptor>::processGenericBlocks(Box3D domain, std::vector<AtomicBlock3D*> blocks)
{
    PLB_ASSERT(blocks.size() == 2);
    BlockLattice3D<T,Descriptor> *lattice = dynamic_cast<BlockLattice3D<T,Descriptor>*>(blocks[0]);
    ScalarField3D<int> *flagMatrix = dynamic_cast<ScalarField3D<int>*>(blocks[1]);
    PLB_ASSERT(lattice);
    PLB_ASSERT(flagMatrix);

    Dot3D ofsFM = computeRelativeDisplacement(*lattice, *flagMatrix);
    Dot3D offset = lattice->getLocation();

    T targetOmega = 1.0; // Here we hardcode the value of the relaxation parameter to 1.0 for stability purposes.

    T alpha = targetOmega / bulkOmega;

    plint spongePositions[6]; // Sponge positions.
    spongePositions[0] =          numSpongeCells[0];
    spongePositions[1] = nx - 1 - numSpongeCells[1];
    spongePositions[2] =          numSpongeCells[2];
    spongePositions[3] = ny - 1 - numSpongeCells[3];
    spongePositions[4] =          numSpongeCells[4];
    spongePositions[5] = nz - 1 - numSpongeCells[5];

    if (useTanhSpongeFunction) {
        plint spongeCenters[6]; // Sponge centers.
        spongeCenters[0] = util::roundToInt(((T) 1             - translationParameters[0]) * numSpongeCells[0]);
        spongeCenters[1] = util::roundToInt(spongePositions[1] + translationParameters[1]  * numSpongeCells[1]);
        spongeCenters[2] = util::roundToInt(((T) 1             - translationParameters[2]) * numSpongeCells[2]);
        spongeCenters[3] = util::roundToInt(spongePositions[3] + translationParameters[3]  * numSpongeCells[3]);
        spongeCenters[4] = util::roundToInt(((T) 1             - translationParameters[4]) * numSpongeCells[4]);
        spongeCenters[5] = util::roundToInt(spongePositions[5] + translationParameters[5]  * numSpongeCells[5]);

        T sigma[6]; // Sponge parameters.
        for (int i = 0; i < 6; i++) {
            sigma[i] = scaleParameters[i] * numSpongeCells[i];
        }

        for (plint iX=domain.x0; iX<=domain.x1; ++iX) {
            plint i = iX + offset.x;
            for (plint iY=domain.y0; iY<=domain.y1; ++iY) {
                plint j = iY + offset.y;
                for (plint iZ=domain.z0; iZ<=domain.z1; ++iZ) {
                    plint k = iZ + offset.z;

                    if (flagMatrix->get(iX+ofsFM.x, iY+ofsFM.y, iZ+ofsFM.z) != flag) {
                        continue;
                    }

                    T spongeFunction = 1.0;
                    bool modifyOmega = false;
                    if (numSpongeCells[0] > 0 && i <= spongePositions[0]) {
                        spongeFunction *= 0.5 * (1.0 + std::tanh((i - spongeCenters[0]) / sigma[0]));
                        modifyOmega = true;
                    }
                    if (numSpongeCells[1] > 0 && i >= spongePositions[1]) {
                        spongeFunction *= 0.5 * (1.0 - std::tanh((i - spongeCenters[1]) / sigma[1]));
                        modifyOmega = true;
                    }

                    if (numSpongeCells[2] > 0 && j <= spongePositions[2]) {
                        spongeFunction *= 0.5 * (1.0 + std::tanh((j - spongeCenters[2]) / sigma[2]));
                        modifyOmega = true;
                    }
                    if (numSpongeCells[3] > 0 && j >= spongePositions[3]) {
                        spongeFunction *= 0.5 * (1.0 - std::tanh((j - spongeCenters[3]) / sigma[3]));
                        modifyOmega = true;
                    }

                    if (numSpongeCells[4] > 0 && k <= spongePositions[4]) {
                        spongeFunction *= 0.5 * (1.0 + std::tanh((k - spongeCenters[4]) / sigma[4]));
                        modifyOmega = true;
                    }
                    if (numSpongeCells[5] > 0 && k >= spongePositions[5]) {
                        spongeFunction *= 0.5 * (1.0 - std::tanh((k - spongeCenters[5]) / sigma[5]));
                        modifyOmega = true;
                    }

                    if (modifyOmega) {
                        spongeFunction = (1.0 - alpha) * spongeFunction + alpha;
                        T localOmega = bulkOmega * spongeFunction;
                        lattice->get(iX, iY, iZ).getDynamics().setOmega(localOmega);
                    }
                }
            }
        }
    } else {
        T pi = std::acos((T) -1.0);

        for (plint iX=domain.x0; iX<=domain.x1; ++iX) {
            plint i = iX + offset.x;
            for (plint iY=domain.y0; iY<=domain.y1; ++iY) {
                plint j = iY + offset.y;
                for (plint iZ=domain.z0; iZ<=domain.z1; ++iZ) {
                    plint k = iZ + offset.z;

                    if (flagMatrix->get(iX+ofsFM.x, iY+ofsFM.y, iZ+ofsFM.z) != flag) {
                        continue;
                    }

                    T spongeFunction = 1.0;
                    bool modifyOmega = false;
                    if (numSpongeCells[0] > 0 && i <= spongePositions[0]) {
                        spongeFunction *= 0.5 * (1.0 - std::cos(pi * (T) i /                        (T) numSpongeCells[0]));
                        modifyOmega = true;
                    }
                    if (numSpongeCells[1] > 0 && i >= spongePositions[1]) {
                        spongeFunction *= 0.5 * (1.0 + std::cos(pi * (T) (i - spongePositions[1]) / (T) numSpongeCells[1]));
                        modifyOmega = true;
                    }

                    if (numSpongeCells[2] > 0 && j <= spongePositions[2]) {
                        spongeFunction *= 0.5 * (1.0 - std::cos(pi * (T) j /                        (T) numSpongeCells[2]));
                        modifyOmega = true;
                    }
                    if (numSpongeCells[3] > 0 && j >= spongePositions[3]) {
                        spongeFunction *= 0.5 * (1.0 + std::cos(pi * (T) (j - spongePositions[3]) / (T) numSpongeCells[3]));
                        modifyOmega = true;
                    }

                    if (numSpongeCells[4] > 0 && k <= spongePositions[4]) {
                        spongeFunction *= 0.5 * (1.0 - std::cos(pi * (T) k /                        (T) numSpongeCells[4]));
                        modifyOmega = true;
                    }
                    if (numSpongeCells[5] > 0 && k >= spongePositions[5]) {
                        spongeFunction *= 0.5 * (1.0 + std::cos(pi * (T) (k - spongePositions[5]) / (T) numSpongeCells[5]));
                        modifyOmega = true;
                    }

                    if (modifyOmega) {
                        spongeFunction = (1.0 - alpha) * spongeFunction + alpha;
                        T localOmega = bulkOmega * spongeFunction;
                        lattice->get(iX, iY, iZ).getDynamics().setOmega(localOmega);
                    }
                }
            }
        }
    }
}


/* ******************** class SmagorinskySpongeZone3D *********************************** */

template<typename T, template<typename U> class Descriptor>
SmagorinskySpongeZone3D<T,Descriptor>::SmagorinskySpongeZone3D(plint nx_, plint ny_, plint nz_, T bulkCSmago_, T targetCSmago_,
        Array<plint,6> const& numSpongeCells_, Array<T,6> const& translationParameters_,
        Array<T,6> const& scaleParameters_)
        : nx(nx_),
          ny(ny_),
          nz(nz_),
          bulkCSmago(bulkCSmago_),
          targetCSmago(targetCSmago_),
          numSpongeCells(numSpongeCells_),
          translationParameters(translationParameters_),
          scaleParameters(scaleParameters_),
          useTanhSpongeFunction(true)
{
    for (int i = 0; i < 6; i++) {
        PLB_ASSERT(translationParameters[i] > (T) 0 && translationParameters[i] < (T) 1);
    }
}

template<typename T, template<typename U> class Descriptor>
SmagorinskySpongeZone3D<T,Descriptor>::SmagorinskySpongeZone3D(plint nx_, plint ny_, plint nz_, T bulkCSmago_, T targetCSmago_,
        Array<plint,6> const& numSpongeCells_)
        : nx(nx_),
          ny(ny_),
          nz(nz_),
          bulkCSmago(bulkCSmago_),
          targetCSmago(targetCSmago_),
          numSpongeCells(numSpongeCells_),
          useTanhSpongeFunction(false)
{
    for (int i = 0; i < 6; i++) {
        numSpongeCells[i] = numSpongeCells_[i];
    }
}

template<typename T, template<typename U> class Descriptor>
void SmagorinskySpongeZone3D<T,Descriptor>::processGenericBlocks(Box3D domain, std::vector<AtomicBlock3D*> blocks)
{
    PLB_ASSERT(blocks.size() == 1);
    BlockLattice3D<T,Descriptor> *lattice = dynamic_cast<BlockLattice3D<T,Descriptor>*>(blocks[0]);
    PLB_ASSERT(lattice);

    Dot3D offset = lattice->getLocation();
    plint whichParameter = dynamicParams::smagorinskyConstant;

    T alpha = targetCSmago / bulkCSmago;

    plint spongePositions[6]; // Sponge positions.
    spongePositions[0] =          numSpongeCells[0];
    spongePositions[1] = nx - 1 - numSpongeCells[1];
    spongePositions[2] =          numSpongeCells[2];
    spongePositions[3] = ny - 1 - numSpongeCells[3];
    spongePositions[4] =          numSpongeCells[4];
    spongePositions[5] = nz - 1 - numSpongeCells[5];

    if (useTanhSpongeFunction) {
        plint spongeCenters[6]; // Sponge centers.
        spongeCenters[0] = util::roundToInt(((T) 1             - translationParameters[0]) * numSpongeCells[0]);
        spongeCenters[1] = util::roundToInt(spongePositions[1] + translationParameters[1]  * numSpongeCells[1]);
        spongeCenters[2] = util::roundToInt(((T) 1             - translationParameters[2]) * numSpongeCells[2]);
        spongeCenters[3] = util::roundToInt(spongePositions[3] + translationParameters[3]  * numSpongeCells[3]);
        spongeCenters[4] = util::roundToInt(((T) 1             - translationParameters[4]) * numSpongeCells[4]);
        spongeCenters[5] = util::roundToInt(spongePositions[5] + translationParameters[5]  * numSpongeCells[5]);

        T sigma[6]; // Sponge parameters.
        for (int i = 0; i < 6; i++) {
            sigma[i] = scaleParameters[i] * numSpongeCells[i];
        }

        for (plint iX=domain.x0; iX<=domain.x1; ++iX) {
            plint i = iX + offset.x;
            for (plint iY=domain.y0; iY<=domain.y1; ++iY) {
                plint j = iY + offset.y;
                for (plint iZ=domain.z0; iZ<=domain.z1; ++iZ) {
                    plint k = iZ + offset.z;

                    T spongeFunction = 1.0;
                    bool modifyCSmago = false;
                    if (numSpongeCells[0] > 0 && i <= spongePositions[0]) {
                        spongeFunction *= 0.5 * (1.0 + std::tanh((i - spongeCenters[0]) / sigma[0]));
                        modifyCSmago = true;
                    }
                    if (numSpongeCells[1] > 0 && i >= spongePositions[1]) {
                        spongeFunction *= 0.5 * (1.0 - std::tanh((i - spongeCenters[1]) / sigma[1]));
                        modifyCSmago = true;
                    }

                    if (numSpongeCells[2] > 0 && j <= spongePositions[2]) {
                        spongeFunction *= 0.5 * (1.0 + std::tanh((j - spongeCenters[2]) / sigma[2]));
                        modifyCSmago = true;
                    }
                    if (numSpongeCells[3] > 0 && j >= spongePositions[3]) {
                        spongeFunction *= 0.5 * (1.0 - std::tanh((j - spongeCenters[3]) / sigma[3]));
                        modifyCSmago = true;
                    }

                    if (numSpongeCells[4] > 0 && k <= spongePositions[4]) {
                        spongeFunction *= 0.5 * (1.0 + std::tanh((k - spongeCenters[4]) / sigma[4]));
                        modifyCSmago = true;
                    }
                    if (numSpongeCells[5] > 0 && k >= spongePositions[5]) {
                        spongeFunction *= 0.5 * (1.0 - std::tanh((k - spongeCenters[5]) / sigma[5]));
                        modifyCSmago = true;
                    }

                    if (modifyCSmago) {
                        spongeFunction = (1.0 - alpha) * spongeFunction + alpha;
                        T localCSmago = bulkCSmago * spongeFunction;
                        lattice->get(iX, iY, iZ).getDynamics().setParameter(whichParameter, localCSmago);
                    }
                }
            }
        }
    } else {
        T pi = std::acos((T) -1.0);

        for (plint iX=domain.x0; iX<=domain.x1; ++iX) {
            plint i = iX + offset.x;
            for (plint iY=domain.y0; iY<=domain.y1; ++iY) {
                plint j = iY + offset.y;
                for (plint iZ=domain.z0; iZ<=domain.z1; ++iZ) {
                    plint k = iZ + offset.z;

                    T spongeFunction = 1.0;
                    bool modifyCSmago = false;
                    if (numSpongeCells[0] > 0 && i <= spongePositions[0]) {
                        spongeFunction *= 0.5 * (1.0 - std::cos(pi * (T) i /                        (T) numSpongeCells[0]));
                        modifyCSmago = true;
                    }
                    if (numSpongeCells[1] > 0 && i >= spongePositions[1]) {
                        spongeFunction *= 0.5 * (1.0 + std::cos(pi * (T) (i - spongePositions[1]) / (T) numSpongeCells[1]));
                        modifyCSmago = true;
                    }

                    if (numSpongeCells[2] > 0 && j <= spongePositions[2]) {
                        spongeFunction *= 0.5 * (1.0 - std::cos(pi * (T) j /                        (T) numSpongeCells[2]));
                        modifyCSmago = true;
                    }
                    if (numSpongeCells[3] > 0 && j >= spongePositions[3]) {
                        spongeFunction *= 0.5 * (1.0 + std::cos(pi * (T) (j - spongePositions[3]) / (T) numSpongeCells[3]));
                        modifyCSmago = true;
                    }

                    if (numSpongeCells[4] > 0 && k <= spongePositions[4]) {
                        spongeFunction *= 0.5 * (1.0 - std::cos(pi * (T) k /                        (T) numSpongeCells[4]));
                        modifyCSmago = true;
                    }
                    if (numSpongeCells[5] > 0 && k >= spongePositions[5]) {
                        spongeFunction *= 0.5 * (1.0 + std::cos(pi * (T) (k - spongePositions[5]) / (T) numSpongeCells[5]));
                        modifyCSmago = true;
                    }

                    // Scale and translate the sponge function.
                    if (modifyCSmago) {
                        spongeFunction = (1.0 - alpha) * spongeFunction + alpha;
                        T localCSmago = bulkCSmago * spongeFunction;
                        lattice->get(iX, iY, iZ).getDynamics().setParameter(whichParameter, localCSmago);
                    }
                }
            }
        }
    }
}


/* ******************** class MaskedSmagorinskySpongeZone3D *********************************** */

template<typename T, template<typename U> class Descriptor>
MaskedSmagorinskySpongeZone3D<T,Descriptor>::MaskedSmagorinskySpongeZone3D(plint nx_, plint ny_, plint nz_, T bulkCSmago_,
        T targetCSmago_, int flag_, Array<plint,6> const& numSpongeCells_, Array<T,6> const& translationParameters_,
        Array<T,6> const& scaleParameters_)
        : nx(nx_),
          ny(ny_),
          nz(nz_),
          bulkCSmago(bulkCSmago_),
          targetCSmago(targetCSmago_),
          numSpongeCells(numSpongeCells_),
          translationParameters(translationParameters_),
          scaleParameters(scaleParameters_),
          useTanhSpongeFunction(true),
          flag(flag_)
{
    for (int i = 0; i < 6; i++) {
        PLB_ASSERT(translationParameters[i] > (T) 0 && translationParameters[i] < (T) 1);
    }
}

template<typename T, template<typename U> class Descriptor>
MaskedSmagorinskySpongeZone3D<T,Descriptor>::MaskedSmagorinskySpongeZone3D(plint nx_, plint ny_, plint nz_, T bulkCSmago_,
        T targetCSmago_, int flag_, Array<plint,6> const& numSpongeCells_)
        : nx(nx_),
          ny(ny_),
          nz(nz_),
          bulkCSmago(bulkCSmago_),
          targetCSmago(targetCSmago_),
          numSpongeCells(numSpongeCells_),
          useTanhSpongeFunction(false),
          flag(flag_)
{
    for (int i = 0; i < 6; i++) {
        numSpongeCells[i] = numSpongeCells_[i];
    }
}

template<typename T, template<typename U> class Descriptor>
void MaskedSmagorinskySpongeZone3D<T,Descriptor>::processGenericBlocks(Box3D domain, std::vector<AtomicBlock3D*> blocks)
{
    PLB_ASSERT(blocks.size() == 2);
    BlockLattice3D<T,Descriptor> *lattice = dynamic_cast<BlockLattice3D<T,Descriptor>*>(blocks[0]);
    ScalarField3D<int> *flagMatrix = dynamic_cast<ScalarField3D<int>*>(blocks[1]);
    PLB_ASSERT(lattice);
    PLB_ASSERT(flagMatrix);

    Dot3D ofsFM = computeRelativeDisplacement(*lattice, *flagMatrix);
    Dot3D offset = lattice->getLocation();
    plint whichParameter = dynamicParams::smagorinskyConstant;

    T alpha = targetCSmago / bulkCSmago;

    plint spongePositions[6]; // Sponge positions.
    spongePositions[0] =          numSpongeCells[0];
    spongePositions[1] = nx - 1 - numSpongeCells[1];
    spongePositions[2] =          numSpongeCells[2];
    spongePositions[3] = ny - 1 - numSpongeCells[3];
    spongePositions[4] =          numSpongeCells[4];
    spongePositions[5] = nz - 1 - numSpongeCells[5];

    if (useTanhSpongeFunction) {
        plint spongeCenters[6]; // Sponge centers.
        spongeCenters[0] = util::roundToInt(((T) 1             - translationParameters[0]) * numSpongeCells[0]);
        spongeCenters[1] = util::roundToInt(spongePositions[1] + translationParameters[1]  * numSpongeCells[1]);
        spongeCenters[2] = util::roundToInt(((T) 1             - translationParameters[2]) * numSpongeCells[2]);
        spongeCenters[3] = util::roundToInt(spongePositions[3] + translationParameters[3]  * numSpongeCells[3]);
        spongeCenters[4] = util::roundToInt(((T) 1             - translationParameters[4]) * numSpongeCells[4]);
        spongeCenters[5] = util::roundToInt(spongePositions[5] + translationParameters[5]  * numSpongeCells[5]);

        T sigma[6]; // Sponge parameters.
        for (int i = 0; i < 6; i++) {
            sigma[i] = scaleParameters[i] * numSpongeCells[i];
        }

        for (plint iX=domain.x0; iX<=domain.x1; ++iX) {
            plint i = iX + offset.x;
            for (plint iY=domain.y0; iY<=domain.y1; ++iY) {
                plint j = iY + offset.y;
                for (plint iZ=domain.z0; iZ<=domain.z1; ++iZ) {
                    plint k = iZ + offset.z;

                    if (flagMatrix->get(iX+ofsFM.x, iY+ofsFM.y, iZ+ofsFM.z) != flag) {
                        continue;
                    }

                    T spongeFunction = 1.0;
                    bool modifyCSmago = false;
                    if (numSpongeCells[0] > 0 && i <= spongePositions[0]) {
                        spongeFunction *= 0.5 * (1.0 + std::tanh((i - spongeCenters[0]) / sigma[0]));
                        modifyCSmago = true;
                    }
                    if (numSpongeCells[1] > 0 && i >= spongePositions[1]) {
                        spongeFunction *= 0.5 * (1.0 - std::tanh((i - spongeCenters[1]) / sigma[1]));
                        modifyCSmago = true;
                    }

                    if (numSpongeCells[2] > 0 && j <= spongePositions[2]) {
                        spongeFunction *= 0.5 * (1.0 + std::tanh((j - spongeCenters[2]) / sigma[2]));
                        modifyCSmago = true;
                    }
                    if (numSpongeCells[3] > 0 && j >= spongePositions[3]) {
                        spongeFunction *= 0.5 * (1.0 - std::tanh((j - spongeCenters[3]) / sigma[3]));
                        modifyCSmago = true;
                    }

                    if (numSpongeCells[4] > 0 && k <= spongePositions[4]) {
                        spongeFunction *= 0.5 * (1.0 + std::tanh((k - spongeCenters[4]) / sigma[4]));
                        modifyCSmago = true;
                    }
                    if (numSpongeCells[5] > 0 && k >= spongePositions[5]) {
                        spongeFunction *= 0.5 * (1.0 - std::tanh((k - spongeCenters[5]) / sigma[5]));
                        modifyCSmago = true;
                    }

                    if (modifyCSmago) {
                        spongeFunction = (1.0 - alpha) * spongeFunction + alpha;
                        T localCSmago = bulkCSmago * spongeFunction;
                        lattice->get(iX, iY, iZ).getDynamics().setParameter(whichParameter, localCSmago);
                    }
                }
            }
        }
    } else {
        T pi = std::acos((T) -1.0);

        for (plint iX=domain.x0; iX<=domain.x1; ++iX) {
            plint i = iX + offset.x;
            for (plint iY=domain.y0; iY<=domain.y1; ++iY) {
                plint j = iY + offset.y;
                for (plint iZ=domain.z0; iZ<=domain.z1; ++iZ) {
                    plint k = iZ + offset.z;

                    if (flagMatrix->get(iX+ofsFM.x, iY+ofsFM.y, iZ+ofsFM.z) != flag) {
                        continue;
                    }

                    T spongeFunction = 1.0;
                    bool modifyCSmago = false;
                    if (numSpongeCells[0] > 0 && i <= spongePositions[0]) {
                        spongeFunction *= 0.5 * (1.0 - std::cos(pi * (T) i /                        (T) numSpongeCells[0]));
                        modifyCSmago = true;
                    }
                    if (numSpongeCells[1] > 0 && i >= spongePositions[1]) {
                        spongeFunction *= 0.5 * (1.0 + std::cos(pi * (T) (i - spongePositions[1]) / (T) numSpongeCells[1]));
                        modifyCSmago = true;
                    }

                    if (numSpongeCells[2] > 0 && j <= spongePositions[2]) {
                        spongeFunction *= 0.5 * (1.0 - std::cos(pi * (T) j /                        (T) numSpongeCells[2]));
                        modifyCSmago = true;
                    }
                    if (numSpongeCells[3] > 0 && j >= spongePositions[3]) {
                        spongeFunction *= 0.5 * (1.0 + std::cos(pi * (T) (j - spongePositions[3]) / (T) numSpongeCells[3]));
                        modifyCSmago = true;
                    }

                    if (numSpongeCells[4] > 0 && k <= spongePositions[4]) {
                        spongeFunction *= 0.5 * (1.0 - std::cos(pi * (T) k /                        (T) numSpongeCells[4]));
                        modifyCSmago = true;
                    }
                    if (numSpongeCells[5] > 0 && k >= spongePositions[5]) {
                        spongeFunction *= 0.5 * (1.0 + std::cos(pi * (T) (k - spongePositions[5]) / (T) numSpongeCells[5]));
                        modifyCSmago = true;
                    }

                    // Scale and translate the sponge function.
                    if (modifyCSmago) {
                        spongeFunction = (1.0 - alpha) * spongeFunction + alpha;
                        T localCSmago = bulkCSmago * spongeFunction;
                        lattice->get(iX, iY, iZ).getDynamics().setParameter(whichParameter, localCSmago);
                    }
                }
            }
        }
    }
}

}

#endif  // SPONGE_ZONES_3D_HH

