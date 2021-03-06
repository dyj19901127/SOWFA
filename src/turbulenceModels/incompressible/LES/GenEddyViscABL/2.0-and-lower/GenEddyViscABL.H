/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::incompressible::LESModels::GenEddyViscABL

Description
    General base class for all incompressible models that can be implemented
    as an eddy viscosity, i.e. algebraic and one-equation models. Modified 
    from original version to better handle the atmospheric SGS models.

    Contains fields for k (SGS turbulent kinetic energy), gamma
    (modelled viscosity) and epsilon (SGS dissipation).

SourceFiles
    GenEddyViscABL.C

\*---------------------------------------------------------------------------*/

#ifndef GenEddyViscABL_H
#define GenEddyViscABL_H

#include "LESModel.H"
#include "uniformDimensionedFields.H"
#include "IOdictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace incompressible
{
namespace LESModels
{

/*---------------------------------------------------------------------------*\
                           Class GenEddyViscABL Declaration
\*---------------------------------------------------------------------------*/

class GenEddyViscABL
:
    virtual public LESModel
{
    // Private Member Functions

        // Disallow default bitwise copy construct and assignment
        GenEddyViscABL(const GenEddyViscABL&);
        GenEddyViscABL& operator=(const GenEddyViscABL&);


protected:

    // Model coefficients
   
        dimensionedScalar ce_;

    // Fields

        volScalarField nuSgs_;

        volScalarField l_;

    // Other model parameters or needed information.

        word TName_;

        word kappatName_;

        const volScalarField& T_;

        const uniformDimensionedVectorField& g_;

        IOdictionary transportDict_;

        dimensionedScalar TRef_;


public:

    //- Partial Runtime type information
    static const word typeName;

    // Constructors

        //- Construct from components
        GenEddyViscABL
        (
            const volVectorField& U,
            const surfaceScalarField& phi,
            transportModel& transport,
            const word& turbulenceModelName = turbulenceModel::typeName,
            const word& modelName = typeName
        );


    //- Destructor
    virtual ~GenEddyViscABL()
    {}


    // Member Functions

        //- Return sub-grid disipation rate
        virtual tmp<volScalarField> epsilon() const
        {
            return ce_*k()*sqrt(k())/delta();
        }

        //- Return the SGS viscosity.
        virtual tmp<volScalarField> nuSgs() const
        {
            return nuSgs_;
        }

        //- Return the sub-grid stress tensor.
        virtual tmp<volSymmTensorField> B() const;

        //- Return the deviatoric part of the effective sub-grid 
        //  turbulence stress tensor including the laminar stress
        virtual tmp<volSymmTensorField> devBeff() const;

        //- Return the divergence of the effective deviatoric sub-grid
        //  turbulence stress tensor including the laminar stress
        virtual tmp<fvVectorMatrix> divDevBeff(volVectorField& U) const;

        //- Correct the stability-dependent length scale
        virtual void computeLengthScale();

        //- Correct Eddy-Viscosity and related properties
        virtual void correct(const tmp<volTensorField>& gradU);

        //- Read LESProperties dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace LESModels
} // End namespace incompressible
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
