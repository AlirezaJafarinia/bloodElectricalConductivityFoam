// Calculate values for phiLambda

phiLambda = Foam::acos(lambda * pow((1 + (mag(tau_max) * b0 / (4 * membraneShearModulus))), -3));

// Calculate values for MLambda

MLambda = Foam::cos(phiLambda) *
          (phiLambda - 0.5 * Foam::sin(2 * phiLambda)) / (pow(Foam::sin(phiLambda), 3) + (dimlessFieldSMALL));

// Calculate values   C_a, C_b, and C_r
C_a == 1 / (MLambda + (dimlessUnityField * 1e-20));
C_b == 2 / ((2 - MLambda) + (dimlessUnityField * 1e-20));
C_r == (C_a + 2 * C_b) / 3;

// Calculate values for fMaxShearRate

fMaxShearRate = ((mag(gammaDot_max) + SRSMALL)) /
                (((mag(gammaDot_max) + SRSMALL)) + (odConst * Foam::pow((mag(gammaDot_max) + SRSMALL), 0.5)));

// Calculate conductivities for C_a, C_b, and C_c
C_alpha == fMaxShearRate *C_a + (dimlessUnityField - fMaxShearRate) * C_r;
C_beta == fMaxShearRate *C_b + (dimlessUnityField - fMaxShearRate) * C_r;

// Calculate conductivities for blood cv
sigma_alpha = sigma_pl * (1 - hematocrit.value()) / ((dimlessUnityField + (C_alpha - dimlessUnityField) * hematocrit.value()) + (dimlessUnityField * 1e-20));
sigma_beta = sigma_pl * (1 - hematocrit.value()) / ((dimlessUnityField + (C_beta - dimlessUnityField) * hematocrit.value()) + (dimlessUnityField * 1e-20));
sigma_beta.correctBoundaryConditions();
sigma_alpha.correctBoundaryConditions();

sigma_EV = sigma_alpha * (e_alpha_EV * e_alpha_EV) + sigma_beta * (I - e_alpha_EV * e_alpha_EV);
sigma_VV = sigma_alpha * (e_alpha_VV * e_alpha_VV) + sigma_beta * (I - e_alpha_VV * e_alpha_VV);

// comparing the models

// Calculate the psi_alpha between vectors
psi_alpha = acos(mag(e_alpha_VV & e_alpha_EV) / ((mag(e_alpha_VV) * mag(e_alpha_EV)) + dimlessFieldSMALL));

// Calculate the psi_alpha between tensors
psi_sigma = acos(mag(sigma_VV && sigma_EV) / ((mag(sigma_VV) * mag(sigma_EV)) + dimFieldSMALL));

// Save the tensor components
sigma_EV_XX = sigma_EV.component(0);
sigma_EV_XY = sigma_EV.component(1);
sigma_EV_XZ = sigma_EV.component(2);
sigma_EV_YY = sigma_EV.component(4);
sigma_EV_YZ = sigma_EV.component(5);
sigma_EV_ZZ = sigma_EV.component(8);

sigma_VV_XX = sigma_VV.component(0);
sigma_VV_XY = sigma_VV.component(1);
sigma_VV_XZ = sigma_VV.component(2);
sigma_VV_YY = sigma_VV.component(4);
sigma_VV_YZ = sigma_VV.component(5);
sigma_VV_ZZ = sigma_VV.component(8);
