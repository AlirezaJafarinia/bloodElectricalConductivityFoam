strainTensor ==  2* 0.5 * (fvc::grad(U) + (fvc::grad(U)().T() )); 

strainTensor.correctBoundaryConditions();






