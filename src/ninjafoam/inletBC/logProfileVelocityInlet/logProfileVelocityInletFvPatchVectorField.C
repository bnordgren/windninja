/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

\*---------------------------------------------------------------------------*/

#include "logProfileVelocityInletFvPatchVectorField.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "wallDist.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

logProfileVelocityInletFvPatchVectorField::logProfileVelocityInletFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchVectorField(p, iF),
    UfreeStream_(0.0),
    uDirection_(0.0,0.0,0.0),
    inputWindHeight_Veg_(0),
    z0_(0),
    Rd_(0)
{

}


logProfileVelocityInletFvPatchVectorField::logProfileVelocityInletFvPatchVectorField
(
    const logProfileVelocityInletFvPatchVectorField& ptf,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchVectorField(ptf, p, iF, mapper),
    UfreeStream_(ptf.UfreeStream_),
    uDirection_(ptf.uDirection_),
    inputWindHeight_Veg_(ptf.inputWindHeight_Veg_),
    z0_(ptf.z0_),
    Rd_(ptf.Rd_)
{

}

logProfileVelocityInletFvPatchVectorField::logProfileVelocityInletFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchVectorField(p, iF),
    UfreeStream_(readScalar(dict.lookup("UfreeStream"))),
    uDirection_(dict.lookup("uDirection")),
    inputWindHeight_Veg_(readScalar(dict.lookup("inputWindHeight_Veg"))),
    z0_(readScalar(dict.lookup("z0"))),
    Rd_(readScalar(dict.lookup("Rd")))
{

    scalar ustar = UfreeStream_*0.41/Foam::log((inputWindHeight_Veg_)/z0_);
    scalar ucalc(0.0);
    vectorField Up(patch().Cf().size(), vector(0.0, 0.0, 0.0));
    
    const fvMesh& mesh = patch().boundaryMesh().mesh();
    const volScalarField& y = wallDist(mesh).y(); //distance from cell centers
    const scalarField yp(patch().patchInternalField(y)); //go from cell center --> patch
  
    // Loop over all the faces in that patch
    forAll(Up, faceI )
    {
        Info<<"yp[faceI] = "<<yp[faceI]<<endl;
        ucalc = ustar/0.41*Foam::log((yp[faceI])/z0_);
        Up[faceI] = ucalc*uDirection_;
    }

    operator==(Up);

}


logProfileVelocityInletFvPatchVectorField::logProfileVelocityInletFvPatchVectorField
(
    const logProfileVelocityInletFvPatchVectorField& fcvpvf,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchVectorField(fcvpvf, iF),
    UfreeStream_(fcvpvf.UfreeStream_),
    uDirection_(fcvpvf.uDirection_),
    inputWindHeight_Veg_(fcvpvf.inputWindHeight_Veg_),
    z0_(fcvpvf.z0_),
    Rd_(fcvpvf.Rd_)
{

}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//called by simpleFoam
void logProfileVelocityInletFvPatchVectorField::updateCoeffs()
{
    scalar ustar = UfreeStream_*0.41/Foam::log((inputWindHeight_Veg_)/z0_);
    scalar ucalc(0.0);
    vectorField Up(patch().Cf().size(), vector(0.0, 0.0, 0.0));
    
    const fvMesh& mesh = patch().boundaryMesh().mesh();
    const volScalarField& y = wallDist(mesh).y(); //distance from cell centers
    const scalarField yp(patch().patchInternalField(y)); //go from cell center --> patch
  
    // Loop over all the faces in that patch
    forAll(Up, faceI )
    {
        Info<<"yp[faceI] = "<<yp[faceI]<<endl;
        ucalc = ustar/0.41*Foam::log((yp[faceI])/z0_);
        Up[faceI] = ucalc*uDirection_;
    }

    operator==(Up);

}

// Write
void logProfileVelocityInletFvPatchVectorField::write(Ostream& os) const
{
    fvPatchVectorField::write(os);
    os.writeKeyword("UfreeStream")
        << UfreeStream_ << token::END_STATEMENT << nl;
    os.writeKeyword("uDirection")
        << uDirection_ << token::END_STATEMENT << nl;
    os.writeKeyword("inputWindHeight_Veg")
        << inputWindHeight_Veg_ << token::END_STATEMENT << nl;
    os.writeKeyword("z0")
        << z0_ << token::END_STATEMENT << nl;
    os.writeKeyword("Rd")
        << Rd_ << token::END_STATEMENT << nl;
    os.writeKeyword("firstCellHeight")
	<< firstCellHeight_ << token::END_STATEMENT << nl;
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField(fvPatchVectorField, logProfileVelocityInletFvPatchVectorField);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
