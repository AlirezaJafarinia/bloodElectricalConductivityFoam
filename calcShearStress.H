
nuEff = ( turbulence->nut() + turbulence-> nu());
turbulence->correct();

tau ==  ( nuEff) * rho * pow(2,0.5) * mag(symm(fvc::grad(U))); //

tau.correctBoundaryConditions();
