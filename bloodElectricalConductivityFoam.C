/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
Application

    bloodConductivityFoam
\*---------------------------------------------------------------------------*/


#include "fvCFD.H"
#include "singlePhaseTransportModel.H"
#include "turbulentTransportModel.H"
#include "pimpleControl.H"
#include "fvOptions.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "postProcess.H"
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "createControl.H"
    #include "createTimeControls.H"
    #include "createFields.H"
    #include "createFvOptions.H"
    #include "initContinuityErrs.H"

   // Include files for creating fields for calculation of electrical conductivity of blood.
 
    #include "createFieldsConductivity.H"

    turbulence->validate();

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    //			SOLVING FOR FLUID FLOW AND BLOOD ELECTRICAL CONDUCTIVITY
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Info<< "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
        #include "readTimeControls.H"
        #include "CourantNo.H"
        #include "setDeltaT.H"
	Info<< "Time = " << runTime.timeName() << nl << endl;
	Info<< "deltaT = " <<  runTime.deltaTValue() << endl;
        runTime++;
        // --- Pressure-velocity PIMPLE corrector loop
        while (pimple.loop())
        {
            #include "UEqn.H"

            // --- Pressure corrector loop
            while (pimple.correct())
            {
                #include "pEqn.H"
            }

            if (pimple.turbCorr())
            {
                laminarTransport.correct();
                turbulence->correct();
            }
        }
	

	# include "calcRateofDeformationTensor.H"
	# include "calcShearRate.H"
	# include "calcExtraStressTensor.H"
	# include "calcShearStress.H"
   	# include "computeOreintationRBC.H" 
  	# include "computeConductivityTensor.H"


	
	runTime.write();
	Info<< "Time = " << runTime.timeName() << nl << endl;
	Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s" << "  ClockTime = " << runTime.elapsedClockTime() << " s" << nl << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}



// ************************************************************************* //
