/*-----------------------------------------------------------------------------------*/
/* FILE			: FUNCTION.C      											         */
/* MODULE		: FUNCTION         											         */
/* AUTHOR		: HGU SASLAB 												         */
/* DATE			: 2014.12.27.												         */
/*-----------------------------------------------------------------------------------*/

#ifndef SOURCE_FUNCTION
#define SOURCE_FUNCTION

/*-----------------------------------------------------------------------------------*/

#include "Function.h"
#include "Declaration.h"

/*-----------------------------------------------------------------------------------*/
/* Inner Product                                                                     */
/*-----------------------------------------------------------------------------------*/
/* input  : vector X and Y, dimension of the vector                                  */
/* output : inner producted value                                                    */
/*-----------------------------------------------------------------------------------*/

REAL    InnerProduct(REAL vecX[], REAL vecY[], NSIZE Dim)
{
	register NSIZE i;
	REAL XdotY = 0.0;

	LOOP(i, Dim) XdotY += vecX[i] * vecY[i];

	return (XdotY);
};

/*-----------------------------------------------------------------------------------*/
/* Cross Product                                                                     */
/*-----------------------------------------------------------------------------------*/
/* input  : three dimensional vector X and Y                                         */
/* output : vector Z                                                                 */
/*-----------------------------------------------------------------------------------*/

void    CrossProduct(REAL vecX[], REAL vecY[], REAL vecZ[])
{
	vecZ[X] = vecX[Y] * vecY[Z] - vecX[Z] * vecY[Y];
	vecZ[Y] = vecX[Z] * vecY[X] - vecX[X] * vecY[Z];
	vecZ[Z] = vecX[X] * vecY[Y] - vecX[Y] * vecY[X];
};

/*-----------------------------------------------------------------------------------*/
/* Set Array                                                                         */
/*-----------------------------------------------------------------------------------*/
/* input  : vector X, value used for initialization, dimension of the vector         */
/* output : vector X                                                                 */
/*-----------------------------------------------------------------------------------*/

void    SetArray(REAL vecX[], REAL value, NSIZE Dim)
{
	register NSIZE i;

	LOOP(i, Dim)  vecX[i] = value;
};

/*-----------------------------------------------------------------------------------*/
/* Set 3*3 Matrix                                                                    */
/*-----------------------------------------------------------------------------------*/
/* input  : 3*3 matrix A, value used for initialization                              */
/* output : 3*3 matrix A                                                             */
/*-----------------------------------------------------------------------------------*/

void    SetMat33(REAL matA[][3], REAL value)
{
	register NSIZE i, j;

	LOOP(i, XYZ)
	{
		LOOP(j, XYZ)  matA[i][j] = value;
	}
};

/*-----------------------------------------------------------------------------------*/
/* Set DCM                                                                           */
/*-----------------------------------------------------------------------------------*/
/* input  : DCM cA_B                                                                 */
/* output : DCM cA_B = eye(3,3)                                                      */
/*-----------------------------------------------------------------------------------*/

void    SetDCM(REAL cA_B[][3])
{
	cA_B[X][X] = 1.0;  cA_B[X][Y] = 0.0;  cA_B[X][Z] = 0.0;
	cA_B[Y][X] = 0.0;  cA_B[Y][Y] = 1.0;  cA_B[Y][Z] = 0.0;
	cA_B[Z][X] = 0.0;  cA_B[Z][Y] = 0.0;  cA_B[Z][Z] = 1.0;
};

/*-----------------------------------------------------------------------------------*/
/* Get DCM                                                                           */
/*-----------------------------------------------------------------------------------*/
/* input  : Euler Angle(yaw, pitch, roll)                                            */
/* output : DCM cA_B = Rx(roll)*Ry(pitch)*Rz(yaw)                                    */
/*-----------------------------------------------------------------------------------*/

void    GetDCM(REAL psi, REAL the, REAL phi, REAL cA_B[][3])
{
	REAL    Cpsi = cos(psi);
	REAL    Spsi = sin(psi);
	REAL    Cthe = cos(the);
	REAL    Sthe = sin(the);
	REAL    Cphi = cos(phi);
	REAL    Sphi = sin(phi);

	cA_B[0][0] = Cthe * Cpsi;
	cA_B[0][1] = Cthe * Spsi;
	cA_B[0][2] = -Sthe;
	cA_B[1][0] = -Cphi * Spsi + Sphi * Sthe * Cpsi;
	cA_B[1][1] = Cphi * Cpsi + Sphi * Sthe * Spsi;
	cA_B[1][2] = Sphi * Cthe;
	cA_B[2][0] = Sphi * Spsi + Cphi * Sthe * Cpsi;
	cA_B[2][1] = -Sphi * Cpsi + Cphi * Sthe * Spsi;
	cA_B[2][2] = Cphi * Cthe;
};

/*-----------------------------------------------------------------------------------*/
/* Get DCM Transpose                                                                 */
/*-----------------------------------------------------------------------------------*/
/* input  : Euler Angle(yaw, pitch, roll)                                            */
/* output : DCM cB_A = Rz(yaw)^T*Ry(pitch)^T*Rx(roll)^T                              */
/*-----------------------------------------------------------------------------------*/

void    GetDCMtr(REAL psi, REAL the, REAL phi, REAL cB_A[][3])
{
	DCM     cA_B[3][3];

	GetDCM(psi, the, phi, cA_B);
	Trans33(cA_B, cB_A);
};

/*-----------------------------------------------------------------------------------*/
/* Transpose N*M Array                                                               */
/*-----------------------------------------------------------------------------------*/
/* input  : N*M array A                                                              */
/* output : N*M array B                                                              */
/*-----------------------------------------------------------------------------------*/

void    Trans(REAL A[], REAL B[], NSIZE N, NSIZE M)
{
	register NSIZE i, j;

	LOOP(i, M)
		LOOP(j, N)  B[i * N + j] = A[i + j * M];
};

/*-----------------------------------------------------------------------------------*/
/* Transpose 3*3 Matrix                                                              */
/*-----------------------------------------------------------------------------------*/
/* input  : 3*3 matrix A                                                             */
/* output : 3*3 matrix B                                                             */
/*-----------------------------------------------------------------------------------*/

void    Trans33(REAL A[][3], REAL B[][3])
{
	Trans(A[0], B[0], 3, 3);
};

/*-----------------------------------------------------------------------------------*/
/* Determinant of 2*2 Matrix										                 */
/*-----------------------------------------------------------------------------------*/
/* input  : 2*2 matrix A                                                             */
/* output : determinant of matrix A                                                  */
/*-----------------------------------------------------------------------------------*/

REAL    DetMat22(REAL A[][2])
{
	return (A[0][0] * A[1][1] - A[0][1] * A[1][0]);
};

/*-----------------------------------------------------------------------------------*/
/* Determinant of 3*3 Matrix										                 */
/*-----------------------------------------------------------------------------------*/
/* input  : 3*3 matrix A                                                             */
/* output : determinant of matrix A                                                  */
/*-----------------------------------------------------------------------------------*/

REAL    DetMat33(REAL A[][3])
{
	return (A[0][0] * A[1][1] * A[2][2] - A[0][0] * A[1][2] * A[2][1] -
		A[0][1] * A[1][0] * A[2][2] + A[0][1] * A[1][2] * A[2][0] +
		A[0][2] * A[1][0] * A[2][1] - A[0][2] * A[1][1] * A[2][0]);
};

/*-----------------------------------------------------------------------------------*/
/* Inverse of 2*2 Matrix											                 */
/*-----------------------------------------------------------------------------------*/
/* input  : 2*2 matrix A                                                             */
/* output : inverse of matrix A                                                      */
/*-----------------------------------------------------------------------------------*/

void    InvMat22(REAL A[][2], REAL invA[][2])
{
	REAL detA = DetMat22(A);

	invA[0][0] = A[1][1] / detA;
	invA[0][1] = -A[0][1] / detA;
	invA[1][0] = -A[1][0] / detA;
	invA[1][1] = A[0][0] / detA;
};

/*-----------------------------------------------------------------------------------*/
/* Inverse of 3*3 Matrix											                 */
/*-----------------------------------------------------------------------------------*/
/* input  : 3*3 matrix A                                                             */
/* output : inverse of matrix A                                                      */
/*-----------------------------------------------------------------------------------*/

void   InvMat33(REAL A[][3], REAL invA[][3])
{
	REAL detA = DetMat33(A);

	invA[0][0] = (A[1][1] * A[2][2] - A[1][2] * A[2][1]) / detA;
	invA[0][1] = -(A[0][1] * A[2][2] - A[0][2] * A[2][1]) / detA;
	invA[0][2] = (A[0][1] * A[1][2] - A[0][2] * A[1][1]) / detA;
	invA[1][0] = -(A[1][0] * A[2][2] - A[1][2] * A[2][0]) / detA;
	invA[1][1] = (A[0][0] * A[2][2] - A[0][2] * A[2][0]) / detA;
	invA[1][2] = -(A[0][0] * A[1][2] - A[0][2] * A[1][0]) / detA;
	invA[2][0] = (A[1][0] * A[2][1] - A[1][1] * A[2][0]) / detA;
	invA[2][1] = -(A[0][0] * A[2][1] - A[0][1] * A[2][0]) / detA;
	invA[2][2] = (A[0][0] * A[1][1] - A[0][1] * A[1][0]) / detA;
};

/*-----------------------------------------------------------------------------------*/
/* Inverse of symmetric matrix M                                                     */
/*-----------------------------------------------------------------------------------*/
/* input  : M    = (Nm by Nm) matrix                                                 */
/*          Nm   = number of row for matrix M <= 3                                   */
/*-----------------------------------------------------------------------------------*/
/* output : invM = (Nm by Nm) symmetric matrix                                       */
/*-----------------------------------------------------------------------------------*/

void InvMat(REAL M[], NSIZE Nm, REAL invM[])
{
	REAL detM = 0.0;

	if (Nm == 1)
	{
		invM[0] = 1.0 / M[0];
	}
	else if (Nm == 2)
	{
		detM = M[0] * M[3] - M[1] * M[2];

		if (detM != 0.0)
		{
			invM[0] = M[3] / detM;  invM[1] = -M[1] / detM;
			invM[2] = -M[2] / detM;  invM[3] = M[0] / detM;
		}
		else
		{
			printf("Singularity error occured in InvMat function....\n");
		}
	}
	else if (Nm == 3)
	{
		detM = M[0] * (M[4] * M[8] - M[5] * M[7]) - M[1] * (M[3] * M[8] - M[5] * M[6]) + M[2] * (M[3] * M[7] - M[4] * M[6]);

		if (detM != 0.0)
		{
			invM[0] = (M[4] * M[8] - M[5] * M[7]) / detM;
			invM[1] = (M[2] * M[7] - M[1] * M[8]) / detM;
			invM[2] = (M[1] * M[5] - M[2] * M[4]) / detM;

			invM[3] = (M[5] * M[6] - M[3] * M[8]) / detM;
			invM[4] = (M[0] * M[8] - M[2] * M[6]) / detM;
			invM[5] = (M[2] * M[3] - M[0] * M[5]) / detM;

			invM[6] = (M[3] * M[7] - M[4] * M[6]) / detM;
			invM[7] = (M[1] * M[6] - M[0] * M[7]) / detM;
			invM[8] = (M[0] * M[4] - M[1] * M[3]) / detM;
		}
		else
		{
			printf("Singularity error occured in InvMat function....\n");
		}
	}
	else
	{
		printf("Dimension error occured in InvMat function....\n");
	}
};

/*-----------------------------------------------------------------------------------*/
/* L*M Matrix and M*N Vector Multiplication                                          */
/*-----------------------------------------------------------------------------------*/
/* input  : L*M vector A and M*N vector B                                            */
/* output : L*N vector Y                                                             */
/*-----------------------------------------------------------------------------------*/

void    MatMult(REAL A[], REAL B[], REAL C[], NSIZE L, NSIZE M, NSIZE N)
{
	register NSIZE i, j, k;

	LOOP(i, L)
	{
		LOOP(k, N)
		{
			C[i * N + k] = 0.0;

			LOOP(j, M)  C[i * N + k] += A[i * M + j] * B[j * N + k];
		}
	}
};

/*-----------------------------------------------------------------------------------*/
/* 3*3 Matrix and 3*1 Vector Multiplication                                          */
/*-----------------------------------------------------------------------------------*/
/* input  : 3*3 matrix A and 3*1 vector X                                            */
/* output : 3*1 vector Y                                                             */
/*-----------------------------------------------------------------------------------*/

void    MatMult31(REAL matA[][3], REAL vecX[], REAL vecY[])
{
	MatMult(matA[0], vecX, vecY, 3, 3, 1);
};

/*-----------------------------------------------------------------------------------*/
/* 2*2 Matrix Multiplication                                                         */
/*-----------------------------------------------------------------------------------*/
/* input  : 2*2 Matrix A and 2*2 Matrix B                                            */
/* output : 2*2 Matrix C                                                             */
/*-----------------------------------------------------------------------------------*/

void    MatMult22(REAL matA[][2], REAL matB[][2], REAL matC[][2])
{
	MatMult(matA[0], matB[0], matC[0], 2, 2, 2);
};

/*-----------------------------------------------------------------------------------*/
/* 3*3 Matrix Multiplication                                                         */
/*-----------------------------------------------------------------------------------*/
/* input  : 3*3 Matrix A and 3*3 Matrix B                                            */
/* output : 3*3 Matrix C                                                             */
/*-----------------------------------------------------------------------------------*/

void    MatMult33(REAL matA[][3], REAL matB[][3], REAL matC[][3])
{
	MatMult(matA[0], matB[0], matC[0], 3, 3, 3);
};

/*-----------------------------------------------------------------------------------*/
/* Multiplication of Three Matrices                                                  */
/*-----------------------------------------------------------------------------------*/
/* input  : A   = (Nk by Nl) matrix                                                  */
/*          B   = (Nl by Nm) matrix                                                  */
/*          C   = (Nm by Nn) matrix                                                  */
/*-----------------------------------------------------------------------------------*/
/* output : D   = (Nk by Nn) matrix                                                  */
/*-----------------------------------------------------------------------------------*/

void  Mat3Mult(REAL A[], REAL B[], REAL C[], NSIZE Nk, NSIZE Nl, NSIZE Nm, NSIZE Nn, REAL D[])
{
	register NSIZE i, j, p, q;

	LOOP(i, Nk)
	{
		LOOP(j, Nn)
		{
			D[i * Nn + j] = 0.0;

			LOOP(p, Nm)
			{
				LOOP(q, Nl)    D[i * Nn + j] += (A[i * Nl + q] * B[q * Nm + p] * C[p * Nn + j]);
			}
		}
	}
};

/*-----------------------------------------------------------------------------------*/
/* 2*1 Vector Summation                                                              */
/*-----------------------------------------------------------------------------------*/
/* input  : 2*1 Vector X and 2*1 Vector Y                                            */
/* output : 2*1 Vector Z                                                             */
/*-----------------------------------------------------------------------------------*/

void    VecSum21(REAL vecX[], REAL vecY[], REAL vecZ[])
{
	register NSIZE i;

	LOOP(i, 2)  vecZ[i] = vecX[i] + vecY[i];
};

/*-----------------------------------------------------------------------------------*/
/* 3*1 Vector Summation                                                              */
/*-----------------------------------------------------------------------------------*/
/* input  : 3*1 Vector X and 3*1 Vector Y                                            */
/* output : 3*1 Vector Z                                                             */
/*-----------------------------------------------------------------------------------*/

void    VecSum31(REAL vecX[], REAL vecY[], REAL vecZ[])
{
	register NSIZE i;

	LOOP(i, 3)  vecZ[i] = vecX[i] + vecY[i];
};

/*-----------------------------------------------------------------------------------*/
/* 2*1 Vector Substitution                                                           */
/*-----------------------------------------------------------------------------------*/
/* input  : 2*1 Vector X and 2*1 Vector Y                                            */
/* output : 2*1 Vector Z                                                             */
/*-----------------------------------------------------------------------------------*/

void    VecSub21(REAL vecX[], REAL vecY[], REAL vecZ[])
{
	register NSIZE i;

	LOOP(i, 2)  vecZ[i] = vecX[i] - vecY[i];
};

/*-----------------------------------------------------------------------------------*/
/* 3*1 Vector Substitution                                                           */
/*-----------------------------------------------------------------------------------*/
/* input  : 3*1 Vector X and 3*1 Vector Y                                            */
/* output : 3*1 Vector Z                                                             */
/*-----------------------------------------------------------------------------------*/

void    VecSub31(REAL vecX[], REAL vecY[], REAL vecZ[])
{
	register NSIZE i;

	LOOP(i, 3)  vecZ[i] = vecX[i] - vecY[i];
};

/*-----------------------------------------------------------------------------------*/
/* N*1 Vector Magnitude                                                              */
/*-----------------------------------------------------------------------------------*/
/* input  : N*1 vector X, dimension of vector Nx                                     */
/* output : magnitude of vector X                                                    */
/*-----------------------------------------------------------------------------------*/

double  Magnitude(REAL vecX[], NSIZE Nx)
{
	register NSIZE i;

	REAL squared_sum = 0.0;

	LOOP(i, Nx)  squared_sum += SQUARE(vecX[i]);

	return (sqrt(squared_sum));
};

/*-----------------------------------------------------------------------------------*/
/* Get Bearing and Elevation Angle                                                   */
/*-----------------------------------------------------------------------------------*/
/* input  : 3*1 vector X                                                             */
/* output : 3*1 Eular angle(yaw,pitch)                                               */
/*-----------------------------------------------------------------------------------*/

void    GetAngle(REAL vecX[], REAL angX[])
{
	REAL  DownRange;

	DownRange = sqrt(SQUARE(vecX[X]) + SQUARE(vecX[Y]));

	angX[PSI] = atan2(vecX[Y], vecX[X]);
	angX[THE] = atan2(-vecX[Z], DownRange);
};

/*-----------------------------------------------------------------------------------*/
/* Generate White Gaussian Noise                                                     */
/*-----------------------------------------------------------------------------------*/
/* input  : mean, sigma                                                              */
/* output : Noise                                                                    */
/*-----------------------------------------------------------------------------------*/
/*
REAL    gauss(REAL mean, REAL sigma)
{
	REAL Noise;

	switch (DataMission.NOISE)
	{
	case USER_DEFINED:    Noise = USER_GAUSS(mean, sigma);    break;
	case COMPILER_RAND:    Noise = RAND_GAUSS(mean, sigma);    break;
	default:    Noise = mean;    break;
	}
	return Noise;
};
*/
/*-----------------------------------------------------------------------------------*/
/*
REAL USER_GAUSS(REAL m, REAL s)
{
	register NSIZE i;

	static int seed;

	int h15, h31, ovf;
	int Lp, L15;

	NSIZE BIT15 = 32768;
	NSIZE BIT16 = 65536;
	NSIZE BIT32_1 = 2147483647;
	NSIZE M1 = 24112;
	NSIZE M2 = 26143;

	REAL n = 0.0;

	if (Manage.flagRandom == NOK)
	{
		Manage.flagRandom = YOK;
		seed = Manage.seed;
	}

	h15 = seed / BIT16;
	Lp = (seed % BIT16) * M1;
	L15 = Lp / BIT16;
	h31 = h15 * M1 + L15;
	ovf = h31 / BIT15;

	seed = ((Lp % BIT16 - BIT32_1) + (h31 % BIT15) * BIT16) + ovf;

	LOOP(i, 12)
	{
		if (seed < 0) seed += BIT32_1;
		h15 = seed / BIT16;
		Lp = (seed % BIT16) * M2;
		L15 = Lp / BIT16;
		h31 = h15 * M2 + L15;
		ovf = h31 / BIT15;

		seed = ((Lp % BIT16 - BIT32_1) + (h31 % BIT15) * BIT16) + ovf;

		if (seed < 0) seed += BIT32_1;
		n += ((2 * (seed / 256) + 1) / 16777216.0);
	}

	return (REAL)((n - 6.0) * s + m);
};
*/
/*-----------------------------------------------------------------------------------*/

REAL RAND_GAUSS(REAL m, REAL s)
{
	register NSIZE i;

	REAL n = 0.0;

	LOOP(i, 12)   n += (REAL)rand() / (REAL)RAND_MAX;

	return (REAL)((n - 6.0) * s + m);
}

/*-----------------------------------------------------------------------------------*/
/* 1st Order Continuous-Time System                                                  */
/*-----------------------------------------------------------------------------------*/
/* input  :  vecRef                                                                  */
/* output :  vecdotx                                                                 */
/*-----------------------------------------------------------------------------------*/
/*     x           1                                                                 */
/*   ----- = --------------                                                          */
/*    Ref      tau*s + 1                                                             */
/*-----------------------------------------------------------------------------------*/
void    FirstOrderSystem(REAL vecX[], REAL vecdotX[], REAL Tau, NSIZE Dim,
	REAL vecRef[])
{
	register NSIZE i;

	LOOP(i, Dim)
	{
		vecdotX[i] = (-1.0 / Tau) * vecX[i] + (1.0 / Tau) * vecRef[i];
	}
};

/*-----------------------------------------------------------------------------------*/
/* 2nd Order Continuous-Time System                                                  */
/*-----------------------------------------------------------------------------------*/
/* input  : vecRef, Zeta, Wn, Dimension                                              */
/* output : auxdotx                                                                  */
/*-----------------------------------------------------------------------------------*/
/*    X(s)                 Wn^2                                                      */
/*   ------ = -----------------------------                                          */
/*    R(s)      s^2 + 2*Zeta*Wn*s + Wn^2                                             */
/*-----------------------------------------------------------------------------------*/

void    SecondOrderSystem(REAL vecX[], REAL vecdotX[], REAL auxX[], REAL auxdotX[],
	REAL Zeta, REAL Wn, NSIZE Dimension, REAL vecRef[])
{
	register NSIZE i;

	LOOP(i, Dimension)
	{
		vecdotX[i] = auxX[i];
		auxdotX[i] = -2.0 * Zeta * Wn * auxX[i] - Wn * Wn * (vecX[i] - vecRef[i]);
	}
};

/*-----------------------------------------------------------------------------------*/
/* 1st Order Discrete-Time Systems                                                   */
/*-----------------------------------------------------------------------------------*/
/* input  : vecRef, vecX, vecXb, Ca, Cb, Dim                                         */
/* output : vecY                                                                     */
/*-----------------------------------------------------------------------------------*/
/*   Y(z)   Cb[1] + Cb[0] * z^{-1}                                                   */
/*   ---- = ----------------------                                                   */
/*   R(z)     1   + Ca[0] * z^{-1}                                                   */
/*-----------------------------------------------------------------------------------*/
/* Using Direct Method                                                               */
/*-----------------------------------------------------------------------------------*/
/*  [ x0[k] ]  = [ -Ca[0] ][ x0[k-1] ]  +  [ 1 ] * ref                               */
/*    vecX                    vecXb                                                  */
/*-----------------------------------------------------------------------------------*/
/*    y[k] = (Cb[0] - Ca[0]Cb[1]) * x0[k-1]                                          */
/*         +  Cb[1] * Ref                                                            */
/*-----------------------------------------------------------------------------------*/

void    DiscreteFirstOrderSystem(REAL vecX[], REAL vecXb[],
	const REAL Ca, const REAL Cb[], const NSIZE Dim,
	REAL vecRef[], REAL vecY[])
{
	register NSIZE i;

	LOOP(i, Dim)
	{
		vecX[i] = -Ca * vecXb[i] + vecRef[i];

		vecY[i] = (Cb[0] - Ca * Cb[1]) * vecXb[i]
			+ Cb[1] * vecRef[i];

		vecXb[i] = vecX[i];
	}
};

/*-----------------------------------------------------------------------------------*/
/* 2nd Order Discrete-Time Systems                                                   */
/*-----------------------------------------------------------------------------------*/
/* input  : vecRef, vecX, vecXb, Ca, Cb, Dim                                         */
/* output : vecY                                                                     */
/*-----------------------------------------------------------------------------------*/
/*   Y(z)   Cb[2] + Cb[1] * z^{-1} + Cb[0] * z^{-2}                                  */
/*   ---- = -----------------------------------------                                */
/*   R(z)     1   + Ca[1] * z^{-1} + Ca[0] * z^{-2}                                  */
/*-----------------------------------------------------------------------------------*/
/* Using Direct Method                                                               */
/*-----------------------------------------------------------------------------------*/
/*  [ x0[k] ]   [   0        1     ][ x0[k-1] ]     [ 0 ]                            */
/*  [ x1[k] ] = [ -Ca[0]   -Ca[1]  ][ x1[k-1] ]  +  [ 1 ] * ref                      */
/*    vecX                              vecXb                                        */
/*-----------------------------------------------------------------------------------*/
/*    y[k] = (Cb[0] - Ca[0]Cb[2]) * x0[k-1]                                          */
/*         + (Cb[1] - Ca[1]Cb[2]) * x1[k-1]                                          */
/*         +  Cb[2] * Ref                                                            */
/*-----------------------------------------------------------------------------------*/

void    DiscreteSecondOrderSystem(REAL vecX[][2], REAL vecXb[][2],
	const REAL Ca[], const REAL Cb[], const NSIZE Dim,
	REAL vecRef[], REAL vecY[])
{
	register NSIZE i;

	LOOP(i, Dim)
	{
		vecX[i][0] = vecXb[i][1];
		vecX[i][1] = -Ca[0] * vecXb[i][0] - Ca[1] * vecXb[i][1] + vecRef[i];

		vecY[i] = (Cb[0] - Ca[0] * Cb[2]) * vecXb[i][0]
			+ (Cb[1] - Ca[1] * Cb[2]) * vecXb[i][1]
			+ Cb[2] * vecRef[i];

		vecXb[i][0] = vecX[i][0];
		vecXb[i][1] = vecX[i][1];
	}
};

/*-----------------------------------------------------------------------------------*/
/* 3rd Order Discrete-Time Systems                                                   */
/*-----------------------------------------------------------------------------------*/
/* input  : vecRef, vecX, vecXb, Ca, Cb, Dim                                         */
/* output : vecY                                                                     */
/*-----------------------------------------------------------------------------------*/
/*   Y(z)    Cb[3] + Cb[2] * z^{-1} + Cb[1] * z^{-2} + Cb[0] * z^{-3}                */
/*   ---- = ----------------------------------------------------------               */
/*   R(z)      1   + Ca[2] * z^{-1} + Ca[1] * z^{-2} + Ca[0] * z^{-3}                */
/*-----------------------------------------------------------------------------------*/
/* Using Direct Method                                                               */
/*-----------------------------------------------------------------------------------*/
/*  [ x0[k] ]   [   0       1       0     ][ x0[k-1] ]     [ 0 ]                     */
/*  [ x1[k] ] = [   0       0       1     ][ x1[k-1] ]  +  [ 0 ] * ref               */
/*  [ x2[k] ]   [ -Ca[0]  -Ca[1]  -Ca[2]  ][ x2[k-1] ]     [ 1 ]                     */
/*    vecX                                     vecXb                                 */
/*-----------------------------------------------------------------------------------*/
/*    y[k] = (Cb[0] - Ca[0]Cb[3]) * x0[k-1]                                          */
/*         + (Cb[1] - Ca[1]Cb[3]) * x1[k-1]                                          */
/*         + (Cb[2] - Ca[2]Cb[3]) * x2[k-1]                                          */
/*         +  Cb[3] * Ref                                                            */
/*-----------------------------------------------------------------------------------*/

void    DiscreteThirdOrderSystem(REAL vecX[][3], REAL vecXb[][3],
	const REAL Ca[], const REAL Cb[], const NSIZE Dim,
	REAL vecRef[], REAL vecY[])
{
	register NSIZE i;

	LOOP(i, Dim)
	{
		vecX[i][0] = vecXb[i][1];
		vecX[i][1] = vecXb[i][2];
		vecX[i][2] = -Ca[0] * vecXb[i][0] - Ca[1] * vecXb[i][1]
			- Ca[2] * vecXb[i][2] + vecRef[i];

		vecY[i] = (Cb[0] - Ca[0] * Cb[3]) * vecXb[i][0]
			+ (Cb[1] - Ca[1] * Cb[3]) * vecXb[i][1]
			+ (Cb[2] - Ca[2] * Cb[3]) * vecXb[i][2]
			+ Cb[3] * vecRef[i];

		vecXb[i][0] = vecX[i][0];
		vecXb[i][1] = vecX[i][1];
		vecXb[i][2] = vecX[i][2];
	}
};

/*-----------------------------------------------------------------------------------*/
/* 4rd Order Discrete-Time Systems                                                   */
/*-----------------------------------------------------------------------------------*/
/* input  : vecRef, vecX, vecXb, Ca, Cb, Dim                                         */
/* output : vecY                                                                     */
/*-----------------------------------------------------------------------------------*/
/* Y(z)    Cb[4] + Cb[3] * z^{-1} + Cb[2] * z^{-2} + Cb[1] * z^{-3} + Cb[0] * z^{-4} */
/* ---- = ---------------------------------------------------------------------------*/
/* R(z)      1   + Ca[3] * z^{-1} + Ca[2] * z^{-2} + Ca[1] * z^{-3} + Ca[0] * z^{-4} */
/*-----------------------------------------------------------------------------------*/
/* Using Direct Method                                                               */
/*-----------------------------------------------------------------------------------*/
/*   [ x0[k] ]   [   0       1       0       0    ][ x0[k-1] ]     [ 0 ]             */
/*   [ x1[k] ]   [   0       0       1       0    ][ x1[k-1] ]     [ 0 ]             */
/*   [ x2[k] ] = [   0       0       0       1    ][ x2[k-1] ]  +  [ 0 ] * ref       */
/*   [ x3[k] ]   [ -Ca[0]  -Ca[1]  -Ca[2]  -Ca[3] ][ x3[k-1] ]     [ 1 ]             */
/*     vecX                                           vecXb                          */
/*-----------------------------------------------------------------------------------*/
/*    y[k] = (Cb[0] - Ca[0]Cb[4]) * x0[k-1]                                          */
/*         + (Cb[1] - Ca[1]Cb[4]) * x1[k-1]                                          */
/*         + (Cb[2] - Ca[2]Cb[4]) * x2[k-1]                                          */
/*         + (Cb[3] - Ca[3]Cb[4]) * x3[k-1]                                          */
/*         +  Cb[4] * Ref                                                            */
/*-----------------------------------------------------------------------------------*/

void    DiscreteFourthOrderSystem(REAL vecX[][4], REAL vecXb[][4],
	const REAL Ca[], const REAL Cb[], const NSIZE Dim,
	REAL vecRef[], REAL vecY[])
{
	register NSIZE i;

	LOOP(i, Dim)
	{
		vecX[i][0] = vecXb[i][1];
		vecX[i][1] = vecXb[i][2];
		vecX[i][2] = vecXb[i][3];
		vecX[i][3] = -Ca[0] * vecXb[i][0] - Ca[1] * vecXb[i][1]
			- Ca[2] * vecXb[i][2] - Ca[3] * vecXb[i][3] + vecRef[i];

		vecY[i] = (Cb[0] - Ca[0] * Cb[4]) * vecXb[i][0]
			+ (Cb[1] - Ca[1] * Cb[4]) * vecXb[i][1]
			+ (Cb[2] - Ca[2] * Cb[4]) * vecXb[i][2]
			+ (Cb[3] - Ca[3] * Cb[4]) * vecXb[i][3]
			+ Cb[4] * vecRef[i];

		vecXb[i][0] = vecX[i][0];
		vecXb[i][1] = vecX[i][1];
		vecXb[i][2] = vecX[i][2];
		vecXb[i][3] = vecX[i][3];
	}
};

/*-----------------------------------------------------------------------------------*/
/* Get Coefficients of Discrete-Time 2nd-Order Filter using Prewarping Method        */
/*-----------------------------------------------------------------------------------*/
/* input  : Zeta, Wn, Ts                                                             */
/* output : coefficients of discrete-time 2nd-order filter                           */
/*-----------------------------------------------------------------------------------*/

void    GetFilterCoefPrewarp(REAL Zeta, REAL Wn, REAL Ts, REAL denFLT[], REAL numFLT[])
{
	REAL Kn = Wn / tan(Wn * Ts / 2.0);
	REAL Dn = SQUARE(Kn) + 2.0 * Zeta * Wn * Kn + SQUARE(Wn);

	numFLT[2] = (SQUARE(Kn) + SQUARE(Wn)) / Dn;
	numFLT[1] = (2.0 * SQUARE(Wn) - 2.0 * SQUARE(Kn)) / Dn;
	numFLT[0] = numFLT[2];

	denFLT[1] = numFLT[1];
	denFLT[0] = (SQUARE(Kn) - 2.0 * Zeta * Wn * Kn + SQUARE(Wn)) / Dn;
};

/*-----------------------------------------------------------------------------------*/
/* Get Coefficients of Discrete-Time 2nd-Order Filter using Tustin Method            */
/*-----------------------------------------------------------------------------------*/
/* input  : Zeta, Wn, Ts                                                             */
/* output : coefficients of discrete-time 2nd-order filter                           */
/*-----------------------------------------------------------------------------------*/
/*   Y(z)   b2 + b1 * z^{-1} + b0 * z^{-2} : numFLT                                  */
/*   ---- = ------------------------------                                           */
/*   R(z)   1  + a1 * z^{-1} + a0 * z^{-2} : denFLT                                  */
/*-----------------------------------------------------------------------------------*/

void    GetFilterCoefTustin(REAL Zeta, REAL Wn, REAL Ts, REAL denFLT[], REAL numFLT[])
{
	REAL a1 = 4.0 * Zeta * Wn / Ts
		+ 1.0 * SQUARE(Wn) + 4.0 / SQUARE(Ts);
	REAL a2 = 2.0 * SQUARE(Wn) - 8.0 / SQUARE(Ts);
	REAL a3 = 1.0 * SQUARE(Wn) + 4.0 / SQUARE(Ts)
		- 4.0 * Zeta * Wn / Ts;
	REAL b1 = 1.0 * SQUARE(Wn);
	REAL b2 = 2.0 * SQUARE(Wn);
	REAL b3 = 1.0 * SQUARE(Wn);

	numFLT[0] = b3 / a1;
	numFLT[1] = b2 / a1;
	numFLT[2] = b1 / a1;

	denFLT[0] = a3 / a1;
	denFLT[1] = a2 / a1;
};

/*-----------------------------------------------------------------------------------*/
/* given value x, find the nearest index of the 1-D table towards first index        */
/*-----------------------------------------------------------------------------------*/
/* input  : tblX(1-D table), x(specific value), N(table size)                        */
/* output : x-index(returned value)                                                  */
/*-----------------------------------------------------------------------------------*/

unsigned int GetIndex(REAL x, REAL tblX[], NSIZE N)
{
	NSIZE ndx = 0;

	if (x > tblX[ndx])
	{
		do
		{
			if (x <= tblX[ndx + 1])  break;

		} while (++ndx < N - 2);
	}

	return  ndx;
}

/*-----------------------------------------------------------------------------------*/
/* 1-D linear interpolation                                                          */
/*-----------------------------------------------------------------------------------*/
/* at the specific point x,                                                          */
/* interpolates to find y(return value) from the underlying table [tblX tblY]        */
/*-----------------------------------------------------------------------------------*/
/* input  : (x,y)(specific point), tblX, tblY(lookup table), N(table size)           */
/* output : y(return value)                                                          */
/*-----------------------------------------------------------------------------------*/

REAL  Interpol1(REAL x, REAL tblX[], REAL tblY[], unsigned int N)
{
	NSIZE ndx;
	REAL   dx;

	ndx = GetIndex(x, tblX, N);

	if (x <= tblX[0])  dx = 0.0;
	else if (x < tblX[N - 1])  dx = (x - tblX[ndx]) / (tblX[ndx + 1] - tblX[ndx]);
	else                       dx = 1.0;

	return  ((1 - dx) * tblY[ndx] + dx * tblY[ndx + 1]);
}

/*-----------------------------------------------------------------------------------*/
/* 2-D linear interpolation                                                          */
/*-----------------------------------------------------------------------------------*/
/* at the specific point (x,y),                                                      */
/* interpolates to find z(return value) from tblZ = function(tblX, tblY)             */
/*-----------------------------------------------------------------------------------*/
/* input  : (x,y)(specific point), tblX,tblY(lookup table), Nx,Ny(table size)        */
/* output : z(return value)                                                          */
/*-----------------------------------------------------------------------------------*/

REAL  Interpol2(REAL tblZ[], REAL tblX[], REAL tblY[],
	unsigned int Nx, unsigned int Ny, REAL x, REAL y)
{
	NSIZE    idxX, idxY;
	REAL     dx, dy, zlow, zhigh;

	idxX = GetIndex(x, tblX, Nx);
	idxY = GetIndex(y, tblY, Ny);

	if (x <= tblX[0])  dx = 0.0;
	else if (x < tblX[Nx - 1])  dx = (x - tblX[idxX]) / (tblX[idxX + 1] - tblX[idxX]);
	else                        dx = 1.0;

	if (y <= tblY[0])  dy = 0.0;
	else if (y < tblY[Ny - 1])  dy = (y - tblY[idxY]) / (tblY[idxY + 1] - tblY[idxY]);
	else                        dy = 1.0;

	zlow = (1 - dy) * tblZ[idxX * Ny + idxY] + dy * tblZ[idxX * Ny + idxY + 1];
	zhigh = (1 - dy) * tblZ[(idxX + 1) * Ny + idxY] + dy * tblZ[(idxX + 1) * Ny + idxY + 1];

	return ((1 - dx) * zlow + dx * zhigh);
};

/*-----------------------------------------------------------------------------------*/
/* open a file with selected mode                                                    */
/*-----------------------------------------------------------------------------------*/
/* input  : File Name, File Mode                                                     */
/* output : pFile (data file pointer)                                                */
/*-----------------------------------------------------------------------------------*/
/* Mode                                                                              */
/* r : read       , w : write       ,   a : append                                   */
/* r+: read/update, w+: write/update,   a+: append/update                            */
/*-----------------------------------------------------------------------------------*/

FILE* FileOpen(char FileName[], char FileMode[])
{
	FILE* pFile;
	char MsgBuf[MAX_STRING];
	if ((pFile = fopen(FileName, FileMode)) == NULL) {
		strcpy(MsgBuf, "\n\t\t Cannot Open file... [ ");
		strcat(MsgBuf, FileName);
		strcat(MsgBuf, " ]");
		Message(MsgBuf);
	}
	return pFile;
};

/*-----------------------------------------------------------------------------------*/
/* read the first character of given data file                                       */
/*-----------------------------------------------------------------------------------*/
/* input  : pFile (data file pointer), buf                                           */
/* output :                                                                          */
/*-----------------------------------------------------------------------------------*/

void    ReadChar(FILE* pFile, char* buf)
{
	while (fgetc(pFile) != '|');

	if (!fscanf(pFile, "%18s", buf))
		Message("\n\t Data format is not correct in INPUT file...");
}

/*-----------------------------------------------------------------------------------*/
/* scan the given data file and sequentially extract the number                      */
/*-----------------------------------------------------------------------------------*/
/* input  : pFile (data file pointer)                                                */
/* output : number scanned in a line                                                 */
/*-----------------------------------------------------------------------------------*/

REAL  ScanData(FILE* pFile)
{
	float   data = NULL_DATA; /* initialized by garbage */
	char    buf[MAX_STRING]; /* character buffer for storing the data read */

	while (!fscanf(pFile, "%f", &data) || data == NULL_DATA)   fscanf(pFile, "%s", buf);

	return (REAL)(data);
};

/*-----------------------------------------------------------------------------------*/
/* line feed of the given data file stream                                           */
/*-----------------------------------------------------------------------------------*/
/* input  : pFile (data file pointer), count(number of lines to be fed)              */
/* output : pFile (data file pointer)                                                */
/*-----------------------------------------------------------------------------------*/

void  LineFeed(FILE* pFile, unsigned int count)
{
	register NSIZE   i;
	char    buf[MAX_STRING];

	for (i = 0; i < count; i++)    fgets(buf, sizeof(buf), pFile);
};

/*-----------------------------------------------------------------------------------*/
/* print out the error message on the screen                                         */
/*-----------------------------------------------------------------------------------*/
/* input  : strMessage (message character array)                                     */
/*-----------------------------------------------------------------------------------*/

void  Message(char* strMessage)
{
	puts(strMessage);
	puts("\n\t\t Press any key to exit...");
	printf("\7");
	getchar();

	_fcloseall();
	exit(1);
};

/*-----------------------------------------------------------------------------------*/

char* FileToken(char* FileName)
{
	static char tmp[MAX_STRING];
	char buf[MAX_STRING];
	char* token;

	strcpy(buf, FileName);
	token = strtok(buf, "\\");
	strcpy(tmp, FileName);

	while (token != NULL) {
		token = strtok(NULL, "\\");
		if (token == NULL) break;
		strcpy(tmp, token);
	}
	return tmp;
}

/*-----------------------------------------------------------------------------------*/
/* Discrete integration (trapezoidal method)                                         */
/*-----------------------------------------------------------------------------------*/

void DiscreteIntegral(REAL Ui[], REAL Ub[], REAL Yb[], REAL Ts, NSIZE Dim, REAL Out[])
{
	register NSIZE i;

	LOOP(i, Dim)    Out[i] = Yb[i] + 0.5 * Ts * (Ui[i] + Ub[i]);
};

/*-----------------------------------------------------------------------------------*/
/* Get Window Time (returns [msec])                                                  */
/*-----------------------------------------------------------------------------------*/

double	GetWindowTime(void)
{
	LARGE_INTEGER	liEndCounter, liFrequency;

	QueryPerformanceCounter(&liEndCounter);
	QueryPerformanceFrequency(&liFrequency);

	return(liEndCounter.QuadPart / (REAL)(liFrequency.QuadPart) * 1000.0);
};

/*----------------------------------------------------------------------------*/
/* General Discrete Filter													  */
/*----------------------------------------------------------------------------*/
/* Reference: GeneralDiscreteFilter (coded by Sejoon Ahn)					  */
/*----------------------------------------------------------------------------*/

void GenDiscFilter3(REAL rk[], REAL yk[], REAL buf[], REAL num[], REAL den[])
{
	register COUNT i;
	REAL tmp_buf[6];
	REAL in[3] = { 0.0, };
	REAL out[3] = { 0.0, };

	LOOP(i, 6) tmp_buf[i] = buf[i];

	for (i = 1; i < 6; i++) buf[i] = tmp_buf[i - 1];

	buf[3] = rk[0];

	LOOP(i, 2)	out[i + 1] = buf[i + 1];
	LOOP(i, 3)	 in[i] = buf[i + 3];
	/*
	buf[0] = (-(den[0]*out[0] + den[1]*out[1] + den[2]*out[2]) +
	(num[0]* in[0] + num[1]* in[1] + num[2]* in[2])   ) / den[0] ;
	*/
	buf[0] = (-(out[0] + den[1] * out[1] + den[2] * out[2]) +
		(num[0] * in[0] + num[1] * in[1] + num[2] * in[2]));
	yk[0] = buf[0];

	return;
}

/*-----------------------------------------------------------------------------------*/
/* Shared Memory for Laser                                                               */
/*-----------------------------------------------------------------------------------*/
//
///*=============== Transmitter ===============*/
//void SharedMem_CREATE(void)
//{
//
//	TCHAR LIDAR_NAME[] = TEXT("LMS_SMDAT");
//
//	hMampF_LASER = NULL;
//
//	hMampF_LASER = CreateFileMapping(
//		INVALID_HANDLE_VALUE,
//		NULL,
//		PAGE_READWRITE,
//		0,
//		sizeof(OUT_LASER),
//		LIDAR_NAME);
//
//	if (hMampF_LASER == NULL)
//	{
//		_tprintf(TEXT("Could not open file mapping object (%d).\n"),
//			GetLastError());
//		return 1;
//	}
//
//	laser_smdat = (OUT_LASER*)MapViewOfFile(hMampF_LASER, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(OUT_LASER));
//
//	printf("\n SM created_\n");
//
//	//getchar();
//}
//
///*=============== Receiver ===============*/
//
//void SharedMem_OPEN(void)
//{
//	TCHAR LIDAR_NAME[] = TEXT("LMS_SMDAT");
//
//	hMampF_LASER = OpenFileMapping(
//		FILE_MAP_READ,   // read/write access
//		FALSE,                 // do not inherit the name
//		LIDAR_NAME);               // name of mapping object RP_SMDAT
//
//	if (hMampF_LASER == NULL)
//	{
//		_tprintf(TEXT("1. Could not open file mapping object (%d).\n"),
//			GetLastError());
//		return 1;
//	}
//
//	laser_smdat = (OUT_LASER*)MapViewOfFile(hMampF_LASER, FILE_MAP_READ, 0, 0, sizeof(OUT_LASER));
//
//	if (laser_smdat == NULL)
//	{
//		_tprintf(TEXT("2. Could not map view of file (%d).\n"),
//			GetLastError());
//
//		CloseHandle(hMampF_LASER);
//		return 1;
//	}
//
//	printf("sm_connect\n");
//
//}
//

/*-----------------------------------------------------------------------------------*/

#endif 