
stressTensor ==   nuEff *rho * (fvc::grad(U) + (fvc::grad(U)().T() )); 

stressTensor.correctBoundaryConditions();





