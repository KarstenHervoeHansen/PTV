 /*
 ; FILE NAME        RGB2.c
 ; PURPOSE          To measure RGB values using 5639 sensor operated
 ;                  in XY mode using crt matrix read from file.
 ;                  To read and display a CRT matrix from file.
 ;                  To learn crt matrix, save it to file and measure
 ;                  RGB using it.
 ;
 ; SPEC             Janusz Kuzminski
 ; DESIGN           
 ; CODING           Janusz Kuzminski
 ; CODE INSP.
 ; TEST             The program was tested by comparing results to those
 ;                  obtained from PM39_20 program, which has 
 ;                  RGB measure function.  .
 ;
 ; LAST UPDT:       2.09.2004
 ;
 ; NOTES:           Invocation:
 ;                    rgb2 m
 ;                    to measure RGB values using CRT matrix read from file.
 ;                  or:
 ;                    rgb2 r 
 ;                    to read and display CRT matrix from file.
 ;                  or:
 ;                    rgb2 l
 ;                    to learn phosphor and save CRT matrix to file
 ;                    and measure RGB using the learnt matrix.
 ;
 ; TBD:             
*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <setjmp.h>
#include <math.h>
#include "xcom.h"

#define  UL unsigned long
#define  UI unsigned int
#define  UC unsigned char

#define  VERSION (float) 2.0

#define MATFILE  "matrix.dat"

typedef struct xx
  {
  float X;
  float Y;
  float Z;
  } XY_MX_MEAS;


typedef struct yy
  {
  float rref;
  float gref;
  float bref;
  } RGB_REF;

typedef struct zz
  {
  float X_of_RED;
  float Y_of_RED;
  float Z_of_RED;

  float X_of_GRE;
  float Y_of_GRE;
  float Z_of_GRE;

  float X_of_BLU;
  float Y_of_BLU;
  float Z_of_BLU;
  } LEARN_MEAS;


float KMAT[] =  /* FACTORY k-matrix for v3.6 sensor */
  {
  +11448.0,
     -77.0,
    -237.0,
    +825.0,
   +8394.0,
    -253.0,
     +73.0,
    -308.0,
  +13689.0
  };

/*        Prototypes                */
void give_info(void);
int  process_cmd_args(int *, int *, char *, char *);
int  read_crt_matrix_from_file(float *matbuff);
int  write_crt_matrix_to_file(float *matbuff);
int  learn_phosphor(float *pho, int port);
int  get_learn_measurements(LEARN_MEAS *learn, int where);
int  calculate_crt_matrix(float * crt, LEARN_MEAS * ptr);
int  calc_crt_matrix_exactly(float * crt, LEARN_MEAS * ptr);
int  calc_crt_matrix_roughly(float * crt, LEARN_MEAS * ptr);
void measure_rgb(float *matbuff, int where);
void calc_rgb_reference(float *pho, RGB_REF *ref);
int  get_measurement(XY_MX_MEAS * ptr, int from);
int  extract_3_values(char *ascii, XY_MX_MEAS *res);
void do_exit(int code, int comport, char *message);
void send_string(char *, int);
int  get_string(char *str, int);
int  MatrixInversion(float* Atemp, int n, float* AInverse);
int  a_solve(float* C, int Crow, int Ccol, float* B, int Brow, int Bcol, float* A);
void MatrixMultiply(float* A, float* B, int m, int p, int n, float* C);
void MatrixTranspose(float* A, int m, int n, float* C);

int main(int argc, char ** argv)
  {
  int    i, k;
  char   temp[30];
  float  mat[9];
  int    port;
  char   c;

  if (argc != 2)
    {
    give_info();
    exit(0);
    }


  printf("RGB Version %.2f.\n", VERSION);
  port  = 0; /* com1 */

  if (!Xcom_Init(port, 4800L, 8, 0, 2, 2000L))
    {
    printf("main: Cannot open com port1.\n");
    exit(1);
    }

  switch (argv[1][0])
    {
    case (int) 'R':                /* read matrix x */
    case (int) 'r':                /* read matrix x */
      if (!read_crt_matrix_from_file(mat))
        do_exit(1, port, "main: Cannot read matrix.\n");
      printf("CRT-matrix from file:\n");
      for (i = 0; i < 9; i++)
        printf("mat[%d] = %+10.3f\n", i, mat[i]);
      break;
    case (int) 'M':                /* measure using matrix x */
    case (int) 'm':                /* measure using matrix x */
      if (!read_crt_matrix_from_file(mat))
        do_exit(1, port, "Cannot read matrix.\n");
      printf("CRT-matrix read from file:\n");
      for (i = 0; i < 9; i++)
        printf("mat[%d] = %+10.3f\n", i, mat[i]);
      measure_rgb(mat, port);
      break;
    case (int) 'L':                /* learn matrix x and measure */
    case (int) 'l':                /* learn matrix x and measure */
#if 1
      if (!learn_phosphor(mat, port))
        do_exit(1, port, "Cannot learn phosphor matrix.\n");

      printf("CRT-matrix obtained:\n");
      for (i = 0; i < 9; i++)
        printf("mat[%d] = %+10.3f\n", i, mat[i]);
    
      measure_rgb(mat, port);
#else
      write_crt_matrix_to_file(KMAT);
#endif
      break;
    default:
      give_info();
      do_exit(1, port, "main: Wrong params\n");
    }
  Xcom_Close(port);
  return (0);
  }


void give_info(void)
  {
  printf("RGB2.exe: program to measure RGB values using PT5639 color sensors.\n");
  printf("Version %.2f.\n", VERSION);
  printf("Invocation: rgb2 r\n");
  printf("to read and display CRT matrix from file.\n");
  printf("or:\n");
  printf("            rgb2 m\n");
  printf("to measure RGB values using CRT matrix read from file.\n");
  printf("or:\n");
  printf("            rgb2 l\n");
  printf("to learn phosphor and save it as CRT matrix to file,\n");
  }

void do_exit(int code, int comport, char *message)
  {
  Xcom_Close(comport);
  printf("%s", message);
  exit (code);
  }


/**********************************************************************
 * NAME       int learn_phosphor(float *pho, int where)
 * PURPOSE    To calculate new phosphor matrix nr and save it in sensor.
 *
 * ARGUMENTS  float *pho  - CRT matrix to fill with results
 *            int where    - comport
 *
 * RETURNS    0 if not successful
 *            1 otherwise
 *
 * EXTERNS    None.
 *
 * NOTES:     This function sets the sensor into XY measuring mode.
 *            In XY measuring mode sensor outputs CIE 1939 thichromatic
 *            coordinates X, Y and Z.
 *            Then the sensor measures XYZ values of red, green and blue 
 *            screens on user CRT.
 *            Then, using these measurement results, the crt matrix is 
 *            calculated, in function calculate_crt_matrix().
 *            Sensor write protection is then set to off.
 *            The calculated crt matrix is then stored in sensor's 
 *            memory as crt matrix nr.
 *            Write protection is then set to on.
 **********************************************************************/
int learn_phosphor(float *pho, int where)
  {
  LEARN_MEAS meas;

  send_string("MS;XY;MC", where); /* set XY mode */
  if (!get_learn_measurements(&meas, where))
    return (0);
  send_string("MS", where);       /* measurement stop */

  if (!calculate_crt_matrix(pho, &meas))
    return (0);
  if (!write_crt_matrix_to_file(pho))
    {
    printf("Matrix cannot be written\n");
    return (1);
    }
  return (1);
  }

/**********************************************************************
 * NAME       int get_learn_measurements(LEARN_MEAS *learn, int where)
 * PURPOSE    To get XYZ measurements from red, green and blue of 
 *            user crt.
 *            
 * ARGUMENTS  LEARN_MEAS *learn - pointer to result
 *            int where         - comport
 *
 * RETURNS    1 - if success
 *            0 - otherwise
 *
 * EXTERNS    None.
 *
 * NOTES:     
 **********************************************************************/
int get_learn_measurements(LEARN_MEAS *learn, int where)
  {
  XY_MX_MEAS tmp;

  printf("Place sensor on RED and press any key\n");
  while (!kbhit())
    ;
  getch();
  if (!get_measurement(&tmp, where))
    return (0);
  learn->X_of_RED = tmp.X;
  learn->Y_of_RED = tmp.Y;
  learn->Z_of_RED = tmp.Z;

  printf("Place sensor on GREEN and press any key\n");
  while (!kbhit())
    ;
  getch();
  if (!get_measurement(&tmp, where))
    return (0);
  learn->X_of_GRE = tmp.X;
  learn->Y_of_GRE = tmp.Y;
  learn->Z_of_GRE = tmp.Z;

  printf("Place sensor on BLUE and press any key\n");
  while (!kbhit())
    ;
  getch();
  if (!get_measurement(&tmp, where))
    return (0);
  learn->X_of_BLU = tmp.X;
  learn->Y_of_BLU = tmp.Y;
  learn->Z_of_BLU = tmp.Z;
  
  return (1);
  }


/**********************************************************************
 * NAME       int calculate_crt_matrix(float * crt, LEARN_MEAS * ptr)
 * PURPOSE    To calculate CRT matrix
 *            
 * ARGUMENTS  float *crt      - pointer to result
 *            LEARN_MEAS * ptr - pointer to measurements taken on red,
 *                               green and blue rectangles in XY mode.
 *
 * RETURNS    1 - if success
 *            0 - otherwise
 *
 * EXTERNS    None.
 *
 * NOTES:     
 *            This function uses two algorithms, one original, taken
 *            from PM39_20 program or one using matrix inversion 
 *            algorithm. The #if 0 -- #if 1 condition below allows to 
 *            switch between these two algorithms.  Calculations in 
 *            both algorithms are followed by a test run, where we put
 *            input data into equations and check that they are the 
 *            right solution.

  The matrix inversion algorithm works as follows:
  We take (in XY mode) X, Y and Z measurements  on red, green and blue
  color on our CRT.
  We write the following equations, which are (XYZ-->RGB) transformations
  using a crt matrix, giving desired values of RGB.  For red screen, we
  want R to be equal to X_of_RED, G and B to be equal 0, and so on.

  For red screen:

    RR = X_of_RED = CRT[0]*X_of_RED + CRT[1]*Y_of_RED + CRT[2]*Z_of_RED
    GG =    0     = CRT[3]*X_of_RED + CRT[4]*Y_of_RED + CRT[5]*Z_of_RED
    BB =    0     = CRT[6]*X_of_RED + CRT[7]*Y_of_RED + CRT[8]*Z_of_RED

  For green screen:

    RR =    0     = CRT[0]*X_of_GRE + CRT[1]*Y_of_GRE + CRT[2]*Z_of_GRE
    GG = Y_of_GRE = CRT[3]*X_of_GRE + CRT[4]*Y_of_GRE + CRT[5]*Z_of_GRE
    BB =    0     = CRT[6]*X_of_GRE + CRT[7]*Y_of_GRE + CRT[8]*Z_of_GRE

  For blue screen:

    RR =    0     = CRT[0]*X_of_BLU + CRT[1]*Y_of_BLU + CRT[2]*Z_of_BLU
    GG =    0     = CRT[3]*X_of_BLU + CRT[4]*Y_of_BLU + CRT[5]*Z_of_BLU
    BB = Z_of_BLU = CRT[6]*X_of_BLU + CRT[7]*Y_of_BLU + CRT[8]*Z_of_BLU

  Munching on.....

    |X_of_RED|           |X_of_RED Y_of_RED Z_of_RED|
    |   0    | = |CRT| * |X_of_RED Y_of_RED Z_of_RED|
    |   0    |           |X_of_RED Y_of_RED Z_of_RED|

    |   0    |           |X_of_GRE Y_of_GRE Z_of_GRE|
    |Y_of_GRE| = |CRT| * |X_of_GRE Y_of_GRE Z_of_GRE|
    |   0    |           |X_of_GRE Y_of_GRE Z_of_GRE|

    |   0    |           |X_of_BLU Y_of_BLU Z_of_BLU|
    |   0    | = |CRT| * |X_of_BLU Y_of_BLU Z_of_BLU|
    |Z_of_BLU|           |X_of_BLU Y_of_BLU Z_of_BLU|

   After normalizing......

     |1 0 0|   |   |   |B[0] B[3] B[6]|
     |0 1 0| = |CRT| * |B[1] B[4] B[7]|
     |0 0 1|   |   |   |B[2] B[5] B[8]|

        C    =  CRT  *        B
   and as can be seen from the obove, the crt matrix is inverted
   matrix B.

  Note that the matrix obtained by this method has all different elements
  and it takes 18 bytes to store the matrix as integers.

  The original PM39_method basically solves the same set of equations, however, 
  elements B[0], B[4] and B[8] are assumed to be equal to 1 (or some
  scale factor).  Resulting crt matrix is not exact solution, but it
  requires only 12 bytes to store it (the diagonal elements are known)
  and in practice the accuracy obtained is adequate.  Note that input
  data for this method is placed differently inside matrix B, as 
  compared to matrix inversion method.

  Note that in both methods, all elements of crt matrix are multiplied
  by 8192.0, which is a scale factor.

  Below are results of test run on the same input data for both methods.

         Input data:       Exact method:      Approx. method:
      ---------------------------------------------------------
      X_of_RED = 31.06   R = +31.06000000    R = +22.83389648
      Y_of_RED = 17.28   G =  -0.00000000    G =  +0.00190918
      Z_of_RED =  1.84   B =  +0.00000000    B =  -0.00229736
      
      X_of_GRE = 26.96   R =  +0.00000000    R =  +0.00035278
      Y_of_GRE = 55.53   G = +55.53000000    G = +40.67575195
      Z_of_GRE =  9.36   B =  +0.00000000    B =  -0.00194458
      
      X_of_BLU = 13.24   R =  +0.00000000    R =  +0.00167725
      Y_of_BLU =  6.11   G =  -0.00000000    G =  -0.00680054
      Z_of_BLU = 68.51   B = +68.51000000    B = +67.96523560
      
                        CRT-matrix:          CRT-matrix:
                        m[0] = +11143.278    m[0] =  +8192.000
                        m[1] =  -5124.137    m[1] =  -3767.000
                        m[2] =  -1696.519    m[2] =  -1247.000
                        m[3] =  -6234.325    m[3] =  -4566.000
                        m[4] = +11183.826    m[4] =  +8192.000
                        m[5] =   +207.405    m[5] =   +151.000
                        m[6] =   +390.713    m[6] =   +387.000
                        m[7] =  -1581.563    m[7] =  -1569.000
                        m[8] =  +8257.542    m[8] =  +8192.000

  The first method is obviously more accurate, however, in practice
  the second method is probably more that adequate.
 **********************************************************************/
int calculate_crt_matrix(float * crt, LEARN_MEAS * ptr)
  {
  int i;
  float aa, rr1;
  float R, G, B;

  /* Below is some sample input data used to test both algorithms */

#if 0
  ptr->X_of_RED = 31.06;
  ptr->Y_of_RED = 17.28;
  ptr->Z_of_RED =  1.84;
  
  ptr->X_of_GRE = 26.96;
  ptr->Y_of_GRE = 55.53;
  ptr->Z_of_GRE =  9.36;
  
  ptr->X_of_BLU = 13.24;
  ptr->Y_of_BLU =  6.11;
  ptr->Z_of_BLU = 68.51;
#endif

#if 1  /************ with MatrixInversion algorithm */
  printf("Calculating matrix exactly:\n");
  if (!calc_crt_matrix_exactly(crt, ptr))
    {
    printf("Cannot find solution.\n");
    return (0);
    }

#else  /************ from PM39_20 C program (measurements in XY mode) */
  printf("Calculating matrix roughly:\n");
  if (!calc_crt_matrix_roughly(crt, ptr))
    {
    printf("Cannot find solution.\n");
    return (0);
    }
#endif

#if 0  /* Test run: put input data into equations and see if they solve them */
  printf("Test run:\n");
  R = crt[0] * ptr->X_of_RED + crt[1] * ptr->Y_of_RED + crt[2] * ptr->Z_of_RED;
  G = crt[3] * ptr->X_of_RED + crt[4] * ptr->Y_of_RED + crt[5] * ptr->Z_of_RED;
  B = crt[6] * ptr->X_of_RED + crt[7] * ptr->Y_of_RED + crt[8] * ptr->Z_of_RED;
  printf("R = %+5.8f\n",   R);
  printf("G = %+5.8f\n",   G);
  printf("B = %+5.8f\n\n", B);

  R = crt[0] * ptr->X_of_GRE + crt[1] * ptr->Y_of_GRE + crt[2] * ptr->Z_of_GRE;
  G = crt[3] * ptr->X_of_GRE + crt[4] * ptr->Y_of_GRE + crt[5] * ptr->Z_of_GRE;
  B = crt[6] * ptr->X_of_GRE + crt[7] * ptr->Y_of_GRE + crt[8] * ptr->Z_of_GRE;
  printf("R = %+5.8f\n",   R);
  printf("G = %+5.8f\n",   G);
  printf("B = %+5.8f\n\n", B);

  R = crt[0] * ptr->X_of_BLU + crt[1] * ptr->Y_of_BLU + crt[2] * ptr->Z_of_BLU;
  G = crt[3] * ptr->X_of_BLU + crt[4] * ptr->Y_of_BLU + crt[5] * ptr->Z_of_BLU;
  B = crt[6] * ptr->X_of_BLU + crt[7] * ptr->Y_of_BLU + crt[8] * ptr->Z_of_BLU;
  printf("R = %+5.8f\n", R);
  printf("G = %+5.8f\n", G);
  printf("B = %+5.8f\n", B);
#endif

  for (i = 0; i < 9; i++)
    crt[i] *= 8192.0;

  return (1);
  }

/**********************************************************************
 * NAME       int calc_crt_matrix_exactly(float * crt, LEARN_MEAS * ptr)
 * PURPOSE    To calculate CRT matrix
 *            
 * ARGUMENTS  float *crt      - pointer to result
 *            LEARN_MEAS * ptr - pointer to measurements taken on red,
 *                               green and blue rectangles in XY mode.
 *
 * RETURNS    1 - if success
 *            0 - otherwise
 *
 * EXTERNS    None.
 *
 * NOTES:     
 *            This function solves the following matrix equation:
 *
 *                  |1 0 0|   |   |   |B[0] B[3] B[6]|
 *                  |0 1 0| = |CRT| * |B[1] B[4] B[7]|
 *                  |0 0 1|   |   |   |B[2] B[5] B[8]|
 *
 **********************************************************************/
int calc_crt_matrix_exactly(float * crt, LEARN_MEAS * ptr)
  {
  float B[9];

  /* Prepare input data (normalize matrix equations) */
  /* red */
  B[0] = ptr->X_of_RED / ptr->X_of_RED;  /* 1 */
  B[1] = ptr->X_of_GRE / ptr->Y_of_GRE;
  B[2] = ptr->X_of_BLU / ptr->Z_of_BLU;
  /* green */
  B[3] = ptr->Y_of_RED / ptr->X_of_RED;
  B[4] = ptr->Y_of_GRE / ptr->Y_of_GRE;  /* 1 */
  B[5] = ptr->Y_of_BLU / ptr->Z_of_BLU;
  /* blue */
  B[6] = ptr->Z_of_RED / ptr->X_of_RED;
  B[7] = ptr->Z_of_GRE / ptr->Y_of_GRE;
  B[8] = ptr->Z_of_BLU / ptr->Z_of_BLU;  /* 1 */

  if (!MatrixInversion(B, 3, crt))
    {
    printf("calc_crt_matrix_exactly(): Cannot invert matrix.\n");
    return (0);
    }
  return (1);
  }

/**********************************************************************
 * NAME       int calc_crt_matrix_roughly(float * crt, LEARN_MEAS * ptr)
 * PURPOSE    To calculate CRT matrix
 *            
 * ARGUMENTS  float *crt      - pointer to result
 *            LEARN_MEAS * ptr - pointer to measurements taken on red,
 *                               green and blue rectangles in XY mode.
 *
 * RETURNS    1
 *
 * EXTERNS    None.
 *
 * NOTES:     This function is taken directly from PM39_20 program.
 **********************************************************************/
int calc_crt_matrix_roughly(float * crt, LEARN_MEAS * ptr)
  {
  int i;
  float aa, rr1;
  float B[9];

  /* Prepare input data (normalize matrix equations) */
  /* red */
  B[0] = ptr->X_of_RED / ptr->X_of_RED;  /* 1 */
  B[1] = ptr->Y_of_RED / ptr->X_of_RED;
  B[2] = ptr->Z_of_RED / ptr->X_of_RED;
  /* green */
  B[3] = ptr->X_of_GRE / ptr->Y_of_GRE;
  B[4] = ptr->Y_of_GRE / ptr->Y_of_GRE;  /* 1 */
  B[5] = ptr->Z_of_GRE / ptr->Y_of_GRE;
  /* blue */
  B[6] = ptr->X_of_BLU / ptr->Z_of_BLU;
  B[7] = ptr->Y_of_BLU / ptr->Z_of_BLU;
  B[8] = ptr->Z_of_BLU / ptr->Z_of_BLU;  /* 1 */

  /* Calculate for RED phosphor */
  aa = B[6];
  for (i = 0; i < 10; i++)
    {
    rr1 = B[3] - aa  * B[5];
    aa  = B[6] - rr1 * B[7];
    }
  crt[2] = - (int)(aa  * 8192.0);
  crt[1] = - (int)(rr1 * 8192.0);
  /* Calculate for GREEN phosphor */
  aa = B[7];
  for (i = 0; i < 10; i++)
    {
    rr1 = B[1] - aa  * B[2];
    aa  = B[7] - rr1 * B[6];
    }
  crt[5] = - (int)(aa  * 8192.0);
  crt[3] = - (int)(rr1 * 8192.0);
  /* Calculate for BLUE phosphor */
  aa = B[5];
  for (i = 0; i < 10; i++)
    {
    rr1 = B[2] - aa  * B[1];
    aa  = B[5] - rr1 * B[3];
    }
  crt[7] = - (int)(aa  * 8192.0);
  crt[6] = - (int)(rr1 * 8192.0);

  crt[0] = crt[4] = crt[8] = 8192.0; /* int_scale = 8192.0 */

  for (i = 0; i < 9; i++)
    crt[i] /= 8192.0;
  return (1);
  }


/**********************************************************************
 * NAME       void measure_rgb(float *crt, int where)
 * PURPOSE    To measure RGB until a key is pressed
 *            
 * ARGUMENTS  float *crt - array containing CRT matrix
 *            int where   - comport
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None.
 *
 * NOTES:     This function sets XY mode in 5639, takes measurement and
 *            converts X, Y and Z to RGB using CRT matrix in crt.
 *            It returns when a key is pressed.
 *            The algor is:
 *                         |RBar|   |   |   |X|
 *                         |GBar| = |CRT| * |Y|
 *                         |BBar|   |   |   |Z|
 *
 *            Output values are scaled by ABSref
 **********************************************************************/
void measure_rgb(float *crt, int where)
  {
  XY_MX_MEAS rsl;
  float     RBar;
  float     GBar;
  float     BBar;
  RGB_REF    scale;
  float     basis;
  int        i;

  calc_rgb_reference(crt, &scale);

  send_string("MS;XY;MC", where); /* set XY mode */

  do
    {
    if (!get_measurement(&rsl, where))
      {
      printf("measure_rgb(): Cannot get measurement.\n");
      return;
      }
    /* Calculate RGB out of XYZ */
    RBar = (rsl.X * crt[0] + rsl.Y * crt[1] + rsl.Z * crt[2]);
    GBar = (rsl.X * crt[3] + rsl.Y * crt[4] + rsl.Z * crt[5]);
    BBar = (rsl.X * crt[6] + rsl.Y * crt[7] + rsl.Z * crt[8]);
    
    /* Scale the results */
    if (RBar > 0.0)
      {
/*      basis = 90.0 / rsl.Y; */
      basis = 1.0;

      RBar = RBar / scale.rref * basis;
      GBar = GBar / scale.gref * basis;
      BBar = BBar / scale.bref * basis;
      }

    printf("R = %+1.2f G = %+1.2f B = %+1.2f\r", RBar, GBar, BBar);
    }  while (!kbhit());
  }

/**********************************************************************
 * NAME       calc_rgb_reference(float *pho, RGB_REF *ref)
 * PURPOSE    To calculate scale factors for RGB measurements.
 *
 * ARGUMENTS  float  *pho  - CRT matrix used for RGB measurements
 *            RGB_REF *ref  - pointer to result
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None.
 *
 * NOTES:     This function uses algorithm taken from PM39_20 software.
 **********************************************************************/
void calc_rgb_reference(float *pho, RGB_REF *ref)
  {
  const float D6500[] =
    {
    0.3127, 0.3291, 0.1978, 0.4683, 0.1978, 1,42359
    };
  float X, Y, Z, sumXYZ;

  /* Find X, Y, Z from 90.0 NIT and White reference */

  Y      = 90.0;
  sumXYZ = Y / D6500[1];
  X      = D6500[0] * sumXYZ;
  Z      = sumXYZ - X - Y;

  ref->rref = pho[0] * X + pho[1] * Y + pho[2] * Z;
  ref->gref = pho[3] * X + pho[4] * Y + pho[5] * Z;
  ref->bref = pho[6] * X + pho[7] * Y + pho[8] * Z;
  }

/**********************************************************************
 * NAME       int get_measurement(XY_MX_MEAS * ptr, int from)
 * PURPOSE    To fetch measurement result from sensor operating
 *            in XY mode.
 * ARGUMENTS  XY_MX_MEAS * ptr - pointer to structure holding result
 *            int from         - comport
 *
 * RETURNS     0 if a character does not arrive during RXTIMEOUT, or
 *               if too many characters are received.
 *             1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     In XY mode, the sensor outputs XYZ values
 *            as follows:
 *                        nX,nY,nZ<CR>
 *            As this function may be entered in the middle of a 
 *            transmission, it first waits until a CR is received and
 *            then collects the result.
 *            It then converts result from ASCII to int.
 **********************************************************************/
int get_measurement(XY_MX_MEAS * ptr, int from)
  {
  int   i;
  uchar c;
  char  str[80];
  char  buff[80];
  
  Xcom_Flush(from);
  /* first wait until present transmission is over */
  do
    {
    if (!Xcom_get_data(&c, from))
      {
      printf("get_measurement(): Timeout on receive.\n");
      return (0);
      }
    } while (c != ('\r'));

  /* then collect sent string */
  i = 0;
  do
    {
    if (!Xcom_get_data(&c, from))
      {
      printf("get_measurement(): Timeout on receive.\n");
      return (0);
      }
    str[i++] = c;
    if (i > 79)    /* if buffer overflow, return and do not cause */
      {            /* eventual exception error                    */
      printf("get_measurement(): Too many characters received.\n");
      return (0);
      }
    } while (c != ('\r'));
  str[i] = '\0';
#if 0
  printf("get_measurement():                 %s\n", str);
#endif
  /* now extract nX,nY and nZ from the string */
  i = 0;
  if (extract_3_values(str, ptr))
    {
#if 0
    printf("get_measurement(): x = %lf y = %lf z = %lf\n", ptr->X, ptr->Y, ptr->Z);
#endif
    return (1);
    }
  else
    return (0);
  }

/**********************************************************************
 * NAME       int extract_3_values(char *ascii, XY_MX_MEAS *res)
 *
 * PURPOSE    To extract 3 float values from string received from
 *            sensor operating in XY mode.
 *
 * ARGUMENTS  char * ascii     - string containing sensor response
 *            XY_MX_MEAS * res - pointer to structure holding result
 *
 * RETURNS    0 - if data in *ascii is not meaningful
 *            1 - on success
 *
 * EXTERNS    None.
 *
 * NOTES:     In XY mode, the sensor outputs XYZ values
 *            as follows:
 *                        nX,nY,nZ,<CR>, like:
 *                        811.10,1068.66,795.52
 *
 *            In MX mode, the sensor outputs unmodified AD output
 *            as follows:
 *                        nX,nY,nZ,INTEGR_TIME<CR>, like:
 *                        811.10,1068.6,795.52,25.0
 *            This function converts 3 coma separated ascii values into
 *            3 float values.
 **********************************************************************/
int extract_3_values(char *ascii, XY_MX_MEAS *res)
  {
  char tmp[80];
  int  i;
  int  j;
  float  values[3];

  for (i = 0; i < 3; i++)
    {
    j = 0;
    while ((*ascii != ',') && (*ascii != '\r'))
      {
      tmp[j++] = *(ascii++);
      if (j == 79)    /* if buffer overflow, return and do not cause */
        {             /* eventual exception error                    */
        printf("extract_3_values(): Too many characters in line.\n");
        return (0);
        }
      }
    ascii++;
    tmp[j] = '\0';
    values[i] = atof(tmp);
    }
  res->X = values[0];
  res->Y = values[1];
  res->Z = values[2];
  return (1);
  }

/**********************************************************************
 * NAME       int read_crt_matrix_from_file(float *matbuff)
 * PURPOSE    To read crt matrix stored in file
 *            
 * ARGUMENTS  float *matbuff - array to put matrix into
 *
 * RETURNS    0 if matrix cannot be read,
 *            1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     fscanf(infile,"%f",&cmat[i][j]);
 **********************************************************************/
int read_crt_matrix_from_file(float *matbuff)
  {
  int    i;
  FILE * matfile;

  matfile = fopen(MATFILE, "r+");
  if (matfile == NULL)
    {
    printf("Cannot open matrix file!\n");
    return (0);
    }
  for (i = 0; i < 9; i++)
    fscanf(matfile, "%f", &matbuff[i]);
  fclose(matfile);
  return 1;
  }

/**********************************************************************
 * NAME       int write_crt_matrix_to_file(float *matbuff)
 * PURPOSE    To write crt matrix stored in file
 *            
 * ARGUMENTS  float *matbuff - array containing matrix
 *
 * RETURNS    0 on errors,
 *            1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     
 **********************************************************************/
int write_crt_matrix_to_file(float *matbuff)
  {
  int    i;
  FILE * matfile;

  matfile = fopen(MATFILE, "w");
  if (matfile == NULL)
    {
    printf("Cannot open matrix file!\n");
    return (0);
    }
  for (i = 0; i < 9; i++)
    fprintf(matfile, "%+10.3f\n", matbuff[i]);
  fclose(matfile);
  return 1;
  }


/**********************************************************************
 * NAME       void send_string(char *str, int prt)
 * PURPOSE    To send a string to sensor
 *            
 * ARGUMENTS  char *str - string to send
 *            int prt   - comport
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None.
 *
 * NOTES:     This function adds terminator to the string, as required
 *            by sensor command protocol.
 **********************************************************************/
void send_string(char *str, int prt)
  {
  while (*str)
    Xcom_TxData(*(str++), prt);
  Xcom_TxData(';', prt);
  }

/**********************************************************************
 * NAME       int get_string(char *str, int prt)
 * PURPOSE    To get a string terminated by \r sent by sensor
 *            
 * ARGUMENTS  char *str - string
 *            int prt   - comport
 *
 * RETURNS    0 on communication errors
 *            1 otherwise
 *
 * EXTERNS    None.
 *
 * NOTES:     
 *            
 **********************************************************************/
int get_string(char *str, int prt)
  {
  int    i;
  uchar  c;
  
  i = 0;
  do
    {
    if (!Xcom_get_data(&c, prt))
      {
      printf("get_string: Timeout on receive.\n");
      return (0);
      }
    str[i++] = c;
    } while (c != '\r');
  str[i] = '\0';
  return (1);
  }

/********************************************************************/
/*               MATRIX MANIPULATION ROUTINES                       */
/********************************************************************/

/* Matrix Inversion Routine                                         */
/* A = input matrix (n x n)                                         */
/* n = dimension of A                                               */
/* AInverse = inverted matrix (n x n)                               */
/* This function inverts a matrix based on the Gauss Jordan method. */
/* The function returns 1 on success, 0 on failure.                 */

int MatrixInversion(float* Atemp, int n, float* AInverse)
  {
  int i, j, iPass, imx, icol, irow;
  float det, temp, pivot, factor;
  float* ac;
  float* A;


  ac = (float*)calloc(n * n, sizeof(float));
  
  if (ac == NULL)
    {
    printf("Cannot allocate memory!\n");
    return 0;
    }

  /* This function destroys contents of first argument.  So we copy
     first atgument to another array                               */
  A = (float*)calloc(n * n, sizeof(float));
  
  if (A == NULL)
    {
    free(A);
    printf("Cannot allocate memory!\n");
    return 0;
    }

  for (i = 0; i < n * n; i++)
    A[i] = Atemp[i];


  det = 1.0;
  for (i = 0; i < n; i++)
    {
    for (j = 0; j < n; j++)
      {
      AInverse[n*i+j] = 0.0;
      ac[n*i+j]       = A[n*i+j];
      }
    AInverse[n*i+i] = 1.0;
    }
  /* The current pivot row is iPass.                                    */
  /* For each pass, first find the maximum element in the pivot column. */
  for (iPass = 0; iPass < n; iPass++)
    {
    imx = iPass;
    for (irow = iPass; irow < n; irow++)
      {
      if (fabs(A[n*irow+iPass]) > fabs(A[n*imx+iPass]))
        imx = irow;
      }
    /* Interchange the elements of row iPass and row imx in both A and AInverse. */
    if (imx != iPass)
      {
      for (icol = 0; icol < n; icol++)
        {
        temp                   = AInverse[n*iPass+icol];
        AInverse[n*iPass+icol] = AInverse[n*imx+icol];
        AInverse[n*imx+icol]   = temp;
        if (icol >= iPass)
          {
          temp            = A[n*iPass+icol];
          A[n*iPass+icol] = A[n*imx+icol];
          A[n*imx+icol]   = temp;
          }
        }
      }
    /* The current pivot is now A[iPass][iPass].             */
    /* The determinant is the product of the pivot elements. */
    pivot = A[n*iPass+iPass];
    det   = det * pivot;
    if (det == 0.0) 
      {
      free(A);
      free(ac);
      printf("Matrix determinant = 0\n");
      return 0;
      }
    for (icol = 0; icol < n; icol++)
      {
      /* Normalize the pivot row by dividing by the pivot element. */
      AInverse[n*iPass+icol] = AInverse[n*iPass+icol] / pivot;
      if (icol >= iPass) A[n*iPass+icol] = A[n*iPass+icol] / pivot;
      }
    for (irow = 0; irow < n; irow++)
      /* Add a multiple of the pivot row to each row.  The multiple factor  */
      /* is chosen so that the element of A on the pivot column is 0.       */
      {
      if (irow != iPass)
        factor = A[n*irow+iPass];
      for (icol = 0; icol < n; icol++)
        {
        if (irow != iPass)
          {
          AInverse[n*irow+icol] -= factor * AInverse[n*iPass+icol];
          A[n*irow+icol] -= factor * A[n*iPass+icol];
          }
        }
      }
    }
  free(A);
  free(ac);
  return 1;
  }

/* Matrix Multiplication Routine                    */
/* A = input matrix (m x p)                         */
/* B = input matrix (p x n)                         */
/* m = number of rows in A                          */
/* p = number of columns in A = number of rows in B */
/* n = number of columns in B                       */
/* C = output matrix = A*B (m x n)                  */

void MatrixMultiply(float* A, float* B, int m, int p, int n, float* C)
  {
  int i, j, k;

  for (i = 0; i < m; i++)
    for(j = 0; j < n; j++)
      {
      C[n * i + j] = 0;
      for (k = 0; k < p; k++)
        C[n * i + j] = C[n * i + j] + A[p * i + k] * B[n * k + j];
      }
  }

/* Matrix Transpose Routine                       */
/* A = input matrix (m x n)                       */
/* m = number of rows in A                        */
/* n = number of columns in A                     */
/* C = output matrix = the transpose of A (n x m) */

void MatrixTranspose(float* A, int rows, int cols, float* C)
  {
  int i, j;

  for (i = 0; i < rows; i++)
    for(j = 0; j < cols; j++)
      C[rows*j+i] = A[cols*i+j];
  }

/**********************************************************************
 * NAME       int a_solve()
 * PURPOSE    To solve a set of matrix equations
 *            
 * ARGUMENTS  float* C - matrix C
 *            int Crow  - nr of rows in matrix C
 *            int Ccol  - nr of cols in matrix C
 *            float* B - matrix B
 *            int Brow  - nr of rows in matrix B
 *            int Bcol  - nr of cols in matrix B
 *            float* A - matrix to put results into
 *            
 * RETURNS    0 if matrixes have erroneous dimensions
 *            1 otherwise
 *
 * EXTERNS    None.
 *
 * NOTES:     This function performs so called A-solve algorithm on
 *            matrix equation of the form:
 *              C = A * B
 *            where C, A and B are matrixes and A is the one to find.
 *            The algorithm is taken from:
 *              "Some common problems in Monitor Measurement..."
 *              Part 1. Matrix mathematics and tools to convert rgb2XYZ
 *            by Tom Lianza, which contains explanation of the method.
 *            
 **********************************************************************/
int a_solve(float* C, int Crow, int Ccol, float* B, int Brow, int Bcol, float* A)
  {
  float BT[16];
  float BTT[32];
  float tmp[16];
  float inv[16];

  int i;

  if (Ccol != Bcol)
    {
    printf("Matrix multiply impossible!\n");
    return (0);
    }

  /*                                                     [3*4]  [4*3]    */
  MatrixTranspose(B, Brow, Bcol, BT);              /*      B  -> BT      */

  /*                                                [3*4][4*3]  [3*3]    */
  MatrixMultiply(C, BT, Crow, Ccol, Brow, tmp);    /* C  * BT -> tmp     */

  /* multiply B * BT *//*                           [3*4][4*3]  [3*3]    */
  MatrixMultiply(B, BT, Brow, Bcol, Brow, BTT);    /* B  * BT -> BTT     */
    

  if (MatrixInversion(BTT, Brow, inv) == 0)        /* inv(B * BT) -> inv */
    {
    printf("Matrix cannot be inverted!\n");
    return(0);
    }

  MatrixMultiply(tmp, inv, Brow, Brow, Brow, A);   /* (C * BT) * inv(B * BT) -> A */
  return (1);
  }

/****************************************************************/
