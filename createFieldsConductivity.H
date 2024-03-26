
// ============================================================//
// volScalarFileds
// ============================================================//
// b0;
// membraneShearModulus;
// lambda;
// sigma_pl;
// sigma_posdef;
// SRSMALL;
// small_N_tau;
// dimlessUnityField;
// phiLambda;
// MLambda ;
// C_a;
// C_b;
// C_r;
// fMaxShearRate;
// C_alpha;
// C_beta;
// sigma_alpha;
// sigma_beta;
// sigma_EV_XX;
// sigma_EV_XY;
// sigma_EV_XZ;
// sigma_EV_YY;
// sigma_EV_YZ;
// sigma_EV_ZZ;
// sigma_VV_XX;
// sigma_VV_XY;
// sigma_VV_XZ;
// sigma_VV_YY;
// sigma_VV_YZ;
// sigma_VV_ZZ;
// gammaDot_max;
// tau_max;
// tau_one;
// tau_two;
// tau_three;
// psi_alpha;
// psi_sigma;
// tensorContraction;
// VVSMALL;

// ============================================================//
// volVectorFields
// ============================================================//
// ev_one;
// ev_two;
// ev_three;
// e_alpha_EV;
// vort;
// e_alpha_VV;
// eigenValuesD;
// eigenValuesTau;
// ============================================================//
// volTensorFileds
// ============================================================//
// I;
// sigma_EV;
// sigma_VV;
// eigenVectorsTau;

// ============================================================//
// Reading Transport Properties
// ============================================================//

Info << "Reading transportProperties" << endl;

IOdictionary transportProperties(
    IOobject(
        "transportProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE));

Info << "Reading blood density rho" << endl;
dimensionedScalar rho(transportProperties.lookup("rho"));

Info << "Reading conductivityModelConstants" << endl;

IOdictionary conductivityModelConstants(
    IOobject(
        "conductivityModelConstants",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE));

Info << "Reading b0" << endl;
dimensionedScalar b0(conductivityModelConstants.lookup("b0"));

Info << "Reading membraneShearModulus" << endl;
dimensionedScalar membraneShearModulus(conductivityModelConstants.lookup("membraneShearModulus"));

Info << "Reading lambda" << endl;
dimensionedScalar lambda(conductivityModelConstants.lookup("lambda"));

Info << "Reading sigma_pl" << endl;
dimensionedScalar sigma_pl(conductivityModelConstants.lookup("sigma_pl"));

Info << "Reading hematocrit" << endl;
dimensionedScalar hematocrit(conductivityModelConstants.lookup("hematocrit"));

Info << "Reading Orientation/Disorientation constant" << endl;
dimensionedScalar odConst(conductivityModelConstants.lookup("odConst"));

// Ensure denominators are not zero by adding epsilon
const double epsilon = 1.0e-10; // Choose an appropriate small value

// Define a new dimensionless scalar field and initialize it
dimensionedScalar dimFieldSMALL(
    "dimFieldSMALL",
    dimensionSet(-2, -4, 6, 0, 0, 4, 0),
    epsilon // Initialize with epsilon or any other value
);
dimensionedScalar dimlessFieldSMALL(
    "dimlessFieldSMALL",
    dimensionSet(0, 0, 0, 0, 0, 0, 0),
    epsilon // Initialize with epsilon or any other value
);
dimensionedScalar SRSMALL("SRSMALL",
                          dimensionSet(0, 0, -1, 0, 0, 0, 0), epsilon);

dimensionedScalar VVSMALL("VVSMALL",
                          dimensionSet(0, 1, -2, 0, 0, 0, 0), epsilon);

// ============================================================//
// Reading Fields
// ============================================================//
Info << "Creating nuEff field" << endl;
volScalarField nuEff(
    IOobject(
        "nuEff",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::NO_WRITE),
    mesh,
    dimensionedScalar("nuEff", dimensionSet(0, 2, -1, 0, 0, 0, 0), 0.0));

Info << "Creating field ShearRate" << endl;
volScalarField shearRate(
    IOobject(
        "shearRate",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("shearRate", dimensionSet(0, 0, -1, 0, 0, 0, 0), 0.0));

Info << "Creating field tau" << endl;
volScalarField tau(
    IOobject(
        "tau",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("tau", dimensionSet(1, -1, -2, 0, 0, 0, 0), 0.0));

Info << "Creating field stressTensor" << endl;
volTensorField stressTensor(
    IOobject(
        "stressTensor",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedTensor("stressTensor", dimensionSet(1, -1, -2, 0, 0, 0, 0), tensor::zero));

Info << "Creating field strainTensor" << endl;
volTensorField strainTensor(
    IOobject(
        "strainTensor",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedTensor("strainTensor", dimensionSet(0, 0, -1, 0, 0, 0, 0), tensor::zero));

Info << "Reading field I\n"
     << endl;
volTensorField I(
    IOobject(
        "I",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE),
    mesh,
    dimensionedTensor("I",
                      dimensionSet(0, 0, 0, 0, 0, 0, 0),
                      tensor::identity()));

Info << "Reading field dimlessUnityField\n"
     << endl;
volScalarField dimlessUnityField(
    IOobject(
        "dimlessUnityField",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE),
    mesh,
    dimensionedScalar("dimlessUnityField",
                      dimensionSet(0, 0, 0, 0, 0, 0, 0),
                      1));

Info << "Reading field tau_max\n"
     << endl;
volScalarField tau_max(
    IOobject(
        "tau_max",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE),
    mesh,
    dimensionedScalar("tau_max",
                      dimensionSet(1, -1, -2, 0, 0, 0, 0),
                      0));

Info << "Reading field phiLambda\n"
     << endl;
volScalarField phiLambda(
    IOobject(
        "phiLambda",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE),
    mesh,
    dimensionedScalar("phiLambda",
                      dimensionSet(0, 0, 0, 0, 0, 0, 0),
                      1),
    zeroGradientFvPatchScalarField::typeName);

forAll(phiLambda, celli)
{
    phiLambda[celli] = Foam::acos(lambda.value() * pow((1 + (mag(tau_max[celli]) * b0.value() / (4 * membraneShearModulus.value()))), -3));
}

Info << "Reading field MLambda \n"
     << endl;
volScalarField MLambda(
    IOobject(
        "MLambda ",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE),
    mesh,
    dimensionedScalar("MLambda ",
                      dimensionSet(0, 0, 0, 0, 0, 0, 0),
                      1),
    zeroGradientFvPatchScalarField::typeName);
forAll(MLambda, celli)
{
    MLambda[celli] = Foam::cos(phiLambda[celli]) * (phiLambda[celli] - 0.5 * Foam::sin(2 * phiLambda[celli])) / (pow(Foam::sin(phiLambda[celli]), 3) + (dimlessUnityField[celli] * 1e-20));
}

Info << "Reading field C_a\n"
     << endl;
volScalarField C_a(
    IOobject(
        "C_a",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE),
    1 / (MLambda + (dimlessUnityField * 1e-20)),
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field C_b\n"
     << endl;
volScalarField C_b(
    IOobject(
        "C_b",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE),
    2 / (2 - MLambda),
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field C_r\n"
     << endl;
volScalarField C_r(
    IOobject(
        "C_r",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE),
    (C_a + 2 * C_b) / 3,
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field fMaxShearRate\n"
     << endl;
volScalarField fMaxShearRate(
    IOobject(
        "fMaxShearRate",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE),
    mesh,
    dimensionedScalar("fMaxShearRate",
                      dimensionSet(0, 0, 0, 0, 0, 0, 0),
                      1),
    zeroGradientFvPatchScalarField::typeName);

forAll(fMaxShearRate, celli)
{
    fMaxShearRate[celli] = ((shearRate[celli] + SRSMALL.value())) / (((shearRate[celli] + SRSMALL.value())) + ((Foam::pow((shearRate[celli] + SRSMALL.value()), 0.5))));
}

Info << "Reading field C_alpha\n"
     << endl;
volScalarField C_alpha(
    IOobject(
        "C_alpha",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE),
    fMaxShearRate *C_b + (dimlessUnityField - fMaxShearRate) * C_r,
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field C_beta\n"
     << endl;
volScalarField C_beta(
    IOobject(
        "C_beta",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE),
    fMaxShearRate *C_b + (dimlessUnityField - fMaxShearRate) * C_r,
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field sigma_alpha\n"
     << endl;
volScalarField sigma_alpha(
    IOobject(
        "sigma_alpha",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    sigma_pl * (1 - hematocrit.value()) / (dimlessUnityField + (C_alpha - dimlessUnityField) * hematocrit.value()),
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field sigma_beta\n"
     << endl;
volScalarField sigma_beta(
    IOobject(
        "sigma_beta",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    sigma_pl * (1 - hematocrit.value()) / (dimlessUnityField + (C_beta - dimlessUnityField) * hematocrit.value()),
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field sigma_EV\n"
     << endl;
volTensorField sigma_EV(
    IOobject(
        "sigma_EV",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedTensor("sigma_EV",
                      dimensionSet(-1, -2, 3, 0, 0, 2, 0),
                      tensor::zero));

Info << "Reading field sigma_EV_XX\n"
     << endl;
volScalarField sigma_EV_XX(
    IOobject(
        "sigma_EV_XX",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("sigma_EV_XX",
                      dimensionSet(-1, -2, 3, 0, 0, 2, 0), 0),
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field sigma_EV_XY\n"
     << endl;
volScalarField sigma_EV_XY(
    IOobject(
        "sigma_EV_XY",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("sigma_EV_XY",
                      dimensionSet(-1, -2, 3, 0, 0, 2, 0), 0),
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field sigma_EV_XZ\n"
     << endl;
volScalarField sigma_EV_XZ(
    IOobject(
        "sigma_EV_XZ",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("sigma_EV_XZ",
                      dimensionSet(-1, -2, 3, 0, 0, 2, 0), 0),
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field sigma_EV_YY\n"
     << endl;
volScalarField sigma_EV_YY(
    IOobject(
        "sigma_EV_YY",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("sigma_EV_YY",
                      dimensionSet(-1, -2, 3, 0, 0, 2, 0), 0),
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field sigma_EV_YZ\n"
     << endl;
volScalarField sigma_EV_YZ(
    IOobject(
        "sigma_EV_YZ",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("sigma_EV_YZ",
                      dimensionSet(-1, -2, 3, 0, 0, 2, 0), 0),
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field sigma_EV_ZZ\n"
     << endl;
volScalarField sigma_EV_ZZ(
    IOobject(
        "sigma_EV_ZZ",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("sigma_EV_ZZ",
                      dimensionSet(-1, -2, 3, 0, 0, 2, 0), 0),
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field sigma_VV\n"
     << endl;
volTensorField sigma_VV(
    IOobject(
        "sigma_VV",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedTensor("sigma_VV",
                      dimensionSet(-1, -2, 3, 0, 0, 2, 0),
                      tensor::zero));

Info << "Reading field sigma_VV_XX\n"
     << endl;
volScalarField sigma_VV_XX(
    IOobject(
        "sigma_VV_XX",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("sigma_VV_XX",
                      dimensionSet(-1, -2, 3, 0, 0, 2, 0), 0),
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field sigma_VV_XY\n"
     << endl;
volScalarField sigma_VV_XY(
    IOobject(
        "sigma_VV_XY",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("sigma_VV_XY",
                      dimensionSet(-1, -2, 3, 0, 0, 2, 0), 0),
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field sigma_VV_XZ\n"
     << endl;
volScalarField sigma_VV_XZ(
    IOobject(
        "sigma_VV_XZ",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("sigma_VV_XZ",
                      dimensionSet(-1, -2, 3, 0, 0, 2, 0), 0),
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field sigma_VV_YY\n"
     << endl;
volScalarField sigma_VV_YY(
    IOobject(
        "sigma_VV_YY",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("sigma_VV_YY",
                      dimensionSet(-1, -2, 3, 0, 0, 2, 0), 0),
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field sigma_VV_YZ\n"
     << endl;
volScalarField sigma_VV_YZ(
    IOobject(
        "sigma_VV_YZ",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("sigma_VV_YZ",
                      dimensionSet(-1, -2, 3, 0, 0, 2, 0), 0),
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field sigma_VV_ZZ\n"
     << endl;
volScalarField sigma_VV_ZZ(
    IOobject(
        "sigma_VV_ZZ",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("sigma_VV_ZZ",
                      dimensionSet(-1, -2, 3, 0, 0, 2, 0), 0),
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field gammaDot_max\n"
     << endl;
volScalarField gammaDot_max(
    IOobject(
        "gammaDot_max",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE),
    mesh,
    dimensionedScalar("gammaDot_max",
                      dimensionSet(0, 0, -1, 0, 0, 0, 0),
                      0));

Info << "Reading field ev_one\n"
     << endl;
volVectorField ev_one(
    IOobject(
        "ev_one",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedVector("ev_one",
                      dimensionSet(0, 0, 0, 0, 0, 0, 0),
                      vector::zero));

Info << "Reading field ev_two\n"
     << endl;
volVectorField ev_two(
    IOobject(
        "ev_two",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedVector("ev_two",
                      dimensionSet(0, 0, 0, 0, 0, 0, 0),
                      vector::zero));

Info << "Reading field ev_three\n"
     << endl;
volVectorField ev_three(
    IOobject(
        "ev_three",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedVector("ev_three",
                      dimensionSet(0, 0, 0, 0, 0, 0, 0),
                      vector::zero));

Info << "Reading field tau_one\n"
     << endl;
volScalarField tau_one(
    IOobject(
        "tau_one",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE),
    mesh,
    dimensionedScalar("tau_one",
                      dimensionSet(1, -1, -2, 0, 0, 0, 0),
                      0));

Info << "Reading field tau_two\n"
     << endl;
volScalarField tau_two(
    IOobject(
        "tau_two",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE),
    mesh,
    dimensionedScalar("tau_two",
                      dimensionSet(1, -1, -2, 0, 0, 0, 0),
                      0));

Info << "Reading field tau_three\n"
     << endl;
volScalarField tau_three(
    IOobject(
        "tau_three",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE),
    mesh,
    dimensionedScalar("tau_three",
                      dimensionSet(1, -1, -2, 0, 0, 0, 0),
                      0));

Info << "Reading field e_alpha_EV\n"
     << endl;
volVectorField e_alpha_EV(
    IOobject(
        "e_alpha_EV",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedVector("e_alpha_EV",
                      dimensionSet(0, 0, 0, 0, 0, 0, 0),
                      vector::zero));

Info << "Reading field vort\n"
     << endl;
volVectorField vort(
    IOobject(
        "vort",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedVector("vort",
                      dimensionSet(0, 0, -1, 0, 0, 0, 0),
                      vector::zero));

Info << "Reading field VVCross\n"
     << endl;
volVectorField VVCross(
    IOobject(
        "VVCross",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE),
    mesh,
    dimensionedVector("VVCross",
                      dimensionSet(0, 1, -2, 0, 0, 0, 0),
                      vector::zero));

Info << "Reading field e_alpha_VV\n"
     << endl;
volVectorField e_alpha_VV(
    IOobject(
        "e_alpha_VV",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedVector("e_alpha_VV",
                      dimensionSet(0, 0, 0, 0, 0, 0, 0),
                      vector::zero));

Info << "Reading field psi_alpha\n"
     << endl;
volScalarField psi_alpha(
    IOobject(
        "psi_alpha",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("psi_alpha",
                      dimensionSet(0, 0, 0, 0, 0, 0, 0),
                      0),
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field psi_sigma\n"
     << endl;
volScalarField psi_sigma(
    IOobject(
        "psi_sigma",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("psi_sigma",
                      dimensionSet(0, 0, 0, 0, 0, 0, 0),
                      0),
    zeroGradientFvPatchScalarField::typeName);

Info << "Reading field eigenValuesD\n"
     << endl;
volVectorField eigenValuesD(
    IOobject(
        "eigenValuesD",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE),
    mesh,
    dimensionedVector("eigenValuesD",
                      dimensionSet(0, 0, -1, 0, 0, 0, 0),
                      vector::zero));

Info << "Reading field eigenValuesTau\n"
     << endl;
volVectorField eigenValuesTau(
    IOobject(
        "eigenValuesTau",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE),
    mesh,
    dimensionedVector("eigenValuesTau",
                      dimensionSet(1, -1, -2, 0, 0, 0, 0),
                      vector::zero));

Info << "Reading field eigenVectorsTau\n"
     << endl;
volTensorField eigenVectorsTau(
    IOobject(
        "eigenVectorsTau",
        runTime.timeName(),
        mesh, IOobject::NO_READ,
        IOobject::NO_WRITE),
    mesh,
    dimensionedTensor("eigenVectorsTau",
                      dimensionSet(0, 0, 0, 0, 0, 0, 0),
                      tensor::zero));
