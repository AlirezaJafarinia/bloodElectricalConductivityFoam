shearRate ==  pow(2,0.5) * mag(symm(fvc::grad(U))); //
shearRate.correctBoundaryConditions();
