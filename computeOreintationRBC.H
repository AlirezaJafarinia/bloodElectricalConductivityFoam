
eigenValuesTau = eigenValues(stressTensor);
eigenVectorsTau = eigenVectors(stressTensor);
eigenValuesD = eigenValues(strainTensor);

tau_one = eigenValuesTau.component(0);
tau_two = eigenValuesTau.component(1);
tau_three = eigenValuesTau.component(2);

double sq = Foam::sqrt(2.0);

// Velocity-Vorticity model
vort = fvc::curl(U);
VVCross = U ^ vort;
e_alpha_VV = VVCross / (mag(VVCross) + VVSMALL);

// Eignevector model

forAll(eigenValuesTau, celli)
{
	ev_one[celli] = vector(eigenVectorsTau[celli].component(0), eigenVectorsTau[celli].component(1), eigenVectorsTau[celli].component(2));
	ev_two[celli] = vector(eigenVectorsTau[celli].component(3), eigenVectorsTau[celli].component(4), eigenVectorsTau[celli].component(5));
	ev_three[celli] = vector(eigenVectorsTau[celli].component(6), eigenVectorsTau[celli].component(7), eigenVectorsTau[celli].component(8));
	tau_max[celli] = mag(eigenValuesTau[celli].component(0) - eigenValuesTau[celli].component(2)) * 0.5;
	gammaDot_max[celli] = mag(eigenValuesD[celli].component(0) - eigenValuesD[celli].component(2)) * 0.5;

	e_alpha_EV[celli] = 1 / sq * (ev_three[celli] + ev_one[celli]);

	if ( ( mag(U[celli] & (1 / sq * (ev_three[celli] - ev_one[celli]))) ) < ( mag( U[celli] & (1 / sq * (ev_three[celli] + ev_one[celli])) ) ) )
	{
		e_alpha_EV[celli] = (1 / sq * (ev_three[celli] - ev_one[celli]));
	}
}
