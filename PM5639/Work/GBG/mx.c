 /*
 ; FILE NAME        mx.c
 ; PURPOSE          To user calibrate a 5639 sensor.
 ; SPEC             This program is a simplified version (command
 ;                  line operation) of the PM8550 program written
 ;                  in PASCAL.  Algorithms used are direct
 ;                  translation to C of those used in PASCAL program.
 ; DESIGN           
 ; CODING           Janusz Kuzminski
 ; CODE INSP.
 ; TEST             The program was tested by applying particular
 ;                  numeric values to both original PASCAL program and 
 ;                  this program and then checking that results are
 ;                  the same.
 ; LAST UPDT:       15.08.2003
 ;
 ; NOTES:           Invocation:
 ;                    mx r 
 ;                    to restore FACTORY K-matrix
 ;                  or:
 ;                    mx x y Y
 ;                    to perform calibration, where x, y and Y are known coordinates
 ;                    of white on user's monitor (obtained for instance by 
 ;                    measurement with some reference instrument.
*/
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <math.h>
#include "xcom.h"

#define  UL unsigned long
#define  UC unsigned char

#define  VERSION (float) 1.0

#define NMAX            12 /* maximum number of equations */
#define NR_OF_MEAS (int) 7
#define FACTORY    (int) 1
#define USER       (int) 0
#define SDELAY           1
/*
  DefaultMaxLum  : real = 1000;    {max luminance in NIT}
  DefaultMinLum  : real = 10;      {min luminance in NIT}
  MaxCoor : REAL     = 0.60;       {max xy/u'v' koordinat}
  MinCoor : REAL     = 0.10;       {max xy/u'v' koordinat}
*/
#define MAXLUM   (double) 1000.0
#define MINLUM   (double)   10.0
#define MAX_X    (double)    0.60
#define MIN_X    (double)    0.10

typedef struct yy
  {
  double RonR; /* red channel on red   screen */
  double GonR;
  double BonR;
  double RonG; /* red channel on green screen */
  double GonG;
  double BonG;
  double RonB; /* green channel on blue screen */
  double GonB;
  double BonB;
  double RonW;
  double GonW; /* green channel on white screen */
  double BonW;
  } INDIV;

typedef union ZZ
  {
  INDIV members;
  double entire[12]; 
  } CHAN_ON_COLOR;

typedef struct xx
  {
  double RonW;
  double GonW;
  double BonW;
  } WHITE_MEAS;

typedef struct tt
  {
  double x_W;
  double y_W;
  double lum_W;
  } USER_ENTRY;



/*******************************************************************/
/*                      Global variables                           */
/*                    NO GLOBAL VARIABLES!                         */
/*******************************************************************/
#if 0
double mata[4][5] =
  {
  {1.0, -2.0, -1.0, 2.0, -2.0},
  {2.0,  0.0,  1.0, 2.0,  5.0},
  {2.0,  0.0,  4.0, 1.0,  7.0},
  {1.0,  6.0,  1.0, 2.0, 16.0}
  }; 

double KLM[9] =
  {
  31460.0,  1225.0, -3761.0,
   -172.0, 20982.0,   268.0,
    197.0,  -435.0, 27536.0
  };

double mat36[9] =
  {
  44,  184,  255,  179,  255,   19,    3,  57,   32
  };
#endif



/******************* function prototypes ***************************/

int gaussian_elimination(int, double a[][NMAX + 1]);
int find_mx(int, double a[][NMAX + 1], double kmat[][NMAX + 1], CHAN_ON_COLOR, CHAN_ON_COLOR *);
int find_k(int size, double coefs[][NMAX + 1], CHAN_ON_COLOR *, CHAN_ON_COLOR *);
int  get_sensor_id(int *, int);
int  read_kmatrix(int, double *, int);
void send_string(char *, int);
int  get_string(char *str, int);
int wp_on(int);
int wp_off(int);
int get_white_mx(WHITE_MEAS *, int);
int get_line_mx(WHITE_MEAS *, int);
int extract_white_meas(char *, WHITE_MEAS *);
int write_kmatrix(int, double *, int);
int user_or_factory(int *, int);
int set_user_factory(int, int);
int valid_solution(double *);
int set_integration_time(int, int);
void give_info(void);
int process_args(char *arg1, char *arg2, char *arg3, USER_ENTRY * entry_value);
int process_user_entry(double *, USER_ENTRY *, CHAN_ON_COLOR *, WHITE_MEAS *);
void do_exit(int, int, char *);

int main(int argc, char ** argv)
  {
  int    i, k;
  char   temp[30];
  double a[NMAX][NMAX + 1];    /* a = [A, B] in AX = B */
  double kmat[NMAX][NMAX + 1];
  double mat[9];
  double KorrMulFactor;
  int    port;

  CHAN_ON_COLOR result;
  CHAN_ON_COLOR ref_values;
  CHAN_ON_COLOR tmp;
  WHITE_MEAS    white_result;
  USER_ENTRY    user_entry;
  int           dim;

  printf("MX Version %.2f.\n", VERSION);
  port  = 0; /* com1 */

  if (!Xcom_Init(port, 4800L, 8, 0, 2, 1200L))
    {
    printf("Cannot open com port1.\n");
    exit(1);
    }

  switch (argc)
    {
    case 2:          /* user entered dummy param: restore factory matrix */
      if (!user_or_factory(&i, port))
        do_exit(1, port, "");
      switch (i)
        {
        case FACTORY: /* if sensor is factory calibrated, do not restore k-matrix */
          do_exit(1, port, "Sensor is FACTORY, Not restoring!\n");
        case USER:    /* if sensor is user calibrated, do restore k-matrix */
          printf("Sensor is USER, restoring...\n");
          wp_off(port);
          if (!read_kmatrix(158, mat, port))
            do_exit(1, port, "");
          if (!write_kmatrix(103, mat, port))
            {
            wp_on(port);
            do_exit(2, port, "Copy of K-Matrix written incorrectly!\n");
            }
          else
            {
            set_user_factory(FACTORY, port);
            wp_on(port);
            do_exit(0, port, "");
            }
        default:
          do_exit(1, port, "");
          }
      break;
    case 4: /* user entered x y and Y */
      if (!process_args(argv[1], argv[2], argv[3], &user_entry))
        {
        give_info();
        exit (1);
        }
      break;
    default: /* wrong command line parameters */
      give_info();
      do_exit(1, port, "");
      break;
    }

  wp_off(port);

  if (get_sensor_id(&i, port))
    printf("Sensor ID = %d\n", i);
  else
    do_exit(1, port, "");

  if (i == 57)
    KorrMulFactor = 1E4;   /* if it is '94 sensor   */
  else
    KorrMulFactor = 1E5;   /* if it is other sensor */        

  set_integration_time(250, port);

  if (!read_kmatrix(103, mat, port))
    do_exit(1, port, "");

#if 0
  else
    {
    strcpy(temp, "Matrix from head:\r\n");
    send_string(temp, port2);

    for (i = 0; i < 9; i++)
      {
      sprintf(temp, "K[%d] = %.2f\r\n", i, mat[i]);
      send_string(temp, port2);
/*      printf("kmat-read[%d] = %lf\n", i, mat[i]); */
      }
    }
#endif

  if (user_or_factory(&i, port))
    {
    if (i == FACTORY)
      {
      printf("Sensor is FACTORY\n");
      if (!write_kmatrix(158, mat, port))
        {
        do_exit(1, port, "Copy of K-Matrix written incorrectly!\n");
        }
      }
    else
      printf("Sensor is USER\n");
    }
  else
    do_exit(1, port, "");


  /* 
     The purpose of the calibration is to find new
     K-matrix and store it in the head.
     We will solve a set of the following equations,
     where K-matrix is the one read from head,
     ref_values.members.XonX heve values asserted 
     below, and result.members.XonX are unknowns.

    |a b c|      |result.members.RonR|     |ref_values.members.RonR|
    |d e f|   *  |result.members.GonR|  =  |ref_values.members.GonR|
    |g h j|      |result.members.BonR|     |ref_values.members.BonR|
    
    |a b c|      |result.members.RonG|     |ref_values.members.RonG|
    |d e f|   *  |result.members.GonG|  =  |ref_values.members.GonG|
    |g h j|      |result.members.BonG|     |ref_values.members.BonG|
    
    |a b c|      |result.members.RonB|     |ref_values.members.RonB|
    |d e f|   *  |result.members.GonB|  =  |ref_values.members.GonB|
    |g h j|      |result.members.BonB|     |ref_values.members.BonB|

    ^^^^^^^      ^^^^^^^^^^^^^^^^^^^^^     ^^^^^^^^^^^^^^^^^^^^^^^^^
    K-matrix           Unknowns                  Constants

    Function find_mx() will do that.
*/
/* Constants below are the same as in "Chromacity Coordinates of EBU Phosphors" */
/* in 'The color truth' by Philips                                              */

  ref_values.members.RonR = 0.64;
  ref_values.members.GonR = 0.33;
  ref_values.members.BonR = 1.0 - ref_values.members.RonR - ref_values.members.GonR;
  ref_values.members.RonG = 0.29;
  ref_values.members.GonG = 0.60;
  ref_values.members.BonG = 1.0 - ref_values.members.RonG - ref_values.members.GonG;
  ref_values.members.RonB = 0.15;
  ref_values.members.GonB = 0.06;
  ref_values.members.BonB = 1.0 - ref_values.members.RonB - ref_values.members.GonB;

/* Constants below are the same as in "Chromacity Coordinates and Correlated    */
/* Color Temperature of Some White References", for D6500                       */
/* in 'The color truth' by Philips                                              */

/* The values below are not used in find_mx().  They will later be overwritten  */
/* by user's entry for white reference ???? see process_user_entry()            */
  ref_values.members.RonW = 0.31;
  ref_values.members.GonW = 0.33;
  ref_values.members.BonW = 1.0 - ref_values.members.RonW - ref_values.members.GonW;

  dim = 3;

#if 1
  kmat[0][0]  = mat[0]; kmat[0][1]   = mat[1]; kmat[0][2]   = mat[2];
  kmat[1][0]  = mat[3]; kmat[1][1]   = mat[4]; kmat[1][2]   = mat[5];
  kmat[2][0]  = mat[6]; kmat[2][1]   = mat[7]; kmat[2][2]   = mat[8];
#endif

  for (k = 0; k < dim; k++)
    for (i = 0; i < dim; i++)
     kmat[k][i] /= KorrMulFactor;

  if (!find_mx(dim, a, kmat, ref_values, &result))
    do_exit(1, port, "Cannot solve equations!\n");

#if 0
  printf("Our K-matrix:\n");
  for (k = 0; k < dim; k++)
    for (i = 0; i < dim; i++)
     printf("kmat[%d]= %lf\n", k + i, kmat[k][i] * KorrMulFactor);
  printf("After FindMX:\n");
  for (k = 0; k < dim * dim; k++)
    printf("  x[%d] = %lf\n", k, result.entire[k]);

#endif


/*
   Now user makes measurement made on white rectangle on his monitor.
   This measurement result goes into:
                                      result.members.XonW

   User must know coordinates for white on his monitor, possibly 
   by using a reference instrument.  User intputs these coordinates
   into the program.  They go into:
                                      ref_values.members.XonW
*/
#if 1
  if (!get_white_mx(&white_result, port))  /* measure with sensor on white */
    do_exit(1, port, "");

  result.members.RonW     = white_result.RonW;  /* returned by get_white_mx */
  result.members.GonW     = white_result.GonW;  /* returned by get_white_mx */
  result.members.BonW     = white_result.BonW;  /* returned by get_white_mx */
#else
  result.members.RonW     = 638.90; /* 696.67;  returned by get_white_mx */
  result.members.GonW     = 846.02; /* 951.26;  returned by get_white_mx */
  result.members.BonW     = 622.54; /* 810.05;  returned by get_white_mx */
#endif

/* user entries processing */
  process_user_entry(mat, &user_entry, &ref_values, &white_result);

/* 
   Now we have the following set of equations:

    |a b c|      |result.members.RonR|     |ref_values.members.RonR|
    |d e f|   *  |result.members.GonR|  =  |ref_values.members.GonR|
    |g h j|      |result.members.BonR|     |ref_values.members.BonR|
    
    |a b c|      |result.members.RonG|     |ref_values.members.RonG|
    |d e f|   *  |result.members.GonG|  =  |ref_values.members.GonG|
    |g h j|      |result.members.BonG|     |ref_values.members.BonG|
    
    |a b c|      |result.members.RonB|     |ref_values.members.RonB|
    |d e f|   *  |result.members.GonB|  =  |ref_values.members.GonB|
    |g h j|      |result.members.BonB|     |ref_values.members.BonB|

    |a b c|      |result.members.RonW|     |ref_values.members.RonW|
    |d e f|   *  |result.members.GonW|  =  |ref_values.members.GonW|
    |g h j|      |result.members.BonW|     |ref_values.members.BonW|

    ^^^^^^^      ^^^^^^^^^^^^^^^^^^^^^     ^^^^^^^^^^^^^^^^^^^^^^^^^
    Unknowns         Coefficients                 Constants

  We rewrite these to save space:

    |a b c|      |C1|     |A|
    |d e f|   *  |C2|  =  |B|
    |g h j|      |C3|     |C|
    
    |a b c|      |C4|     |D|
    |d e f|   *  |C5|  =  |E|
    |g h j|      |C6|     |F|
    
    |a b c|      |C7|     |G|
    |d e f|   *  |C8|  =  |H|
    |g h j|      |C9|     |I|

    |a b c|      |W1|     |M|
    |d e f|   *  |W2|  =  |N|
    |g h j|      |W3|     |O|

    ^^^^^^^      ^^^^     ^^^^
    Unknowns   Coeffs   Constants

  We must find K-matrix, which now is our unknown.  Note that we have
  12 equations and 9 unknowns.  We will reshuffle the data as follows:

  |...........   0  0  0 -A  0  0|      |a|                 |0|
  |...........   0  0  0 -B  0  0|      |b|                 |0|
  |...........  C1 C2 C3  u  0  0|      |c|                 |0|
  |...........   0  0  0  0 -D  0|      |d|                 |0|
  |...........   0  0  0  0 -E  0|      |e|                 |0|
  |...........  C4 C5 C6  0  t  0|  *   |f|       =         |0|
  |...........   0  0  0  0  0 -G|      |g|                 |0|
  |...........   0  0  0  0  0 -H|      |h|                 |0|
  |...........  C7 C8 C9  0  0  s|      |j|                 |0|
  |...........   0  0  0  0  0  0|      |k|                 |M|
  |............  0  0  0  0  0  0|      |l|                 |N|
  |.........    W1 W2 W3  0  0  0|      |m|                 |O|

         ^^^^^^^                    ^^^^^^^^^^           ^^^^^^^^
          Coeffs                     K-matrix            Constants
                                    (Unknowns)

  where: u = 1 - A - B,
         t = 1 - D - E,
         s = 1 - G - H.

We now have 12 equations with 12 unknowns, 9 of which is our K-matrix,
which we must find.  We will discard the last 3 unknowns (k, l and m).

The theory behind all this is UNKNOWN but unknowns k, l and m are 
now present in equations multiplied by A, B, etc. with '-' sign.

Data is reshuffled according to original algorithm (Pascal).

All this is done inside the find_k() function.

To recapitulate:

C1....C9 were calculaed by find_mx()
W1....W3 are measurement results from white rectangle on user's monitor.
M......O were calculated using x, y and Y values entered by user.
A......I are constants from Chromacity Coordinates of EBU Phosphors
*/

  dim = 12;                /* number of equations */

  if (!find_k(dim, a, &result, &ref_values))
    {
    do_exit(1, port, "Cannot find K_Matrix\n");
    }
  else
    {
    for (k = 0; k < 9; k++)
      mat[k] = a[k][dim];

    for (i = 0; i < 9; i++)
      mat[i] *= KorrMulFactor;
#if 0
    printf("Found K_Matrix:\n");
    for (i = 0; i < 9; i++)
      printf("MAT[%d] = %lf\n", i, mat[i]);
#endif
    }


  if (!valid_solution(mat))
    {
    do_exit(1, port, "Some matrix elements exceed 32000!\n");
    }
  else
    {
    if (!write_kmatrix(103, mat, port))
      do_exit(1, port, "Matrix not written correcltly\n"); 
    else
      set_user_factory(USER, port);
    }

  wp_on(port); 
  Xcom_Close(port);
  printf("Calibration successful!\n");
  return (0);
  }

void do_exit(int code, int comport, char *message)
  {
  Xcom_Close(comport);
  printf("%s", message);
  exit (code);
  }

/**********************************************************************
 * NAME       int process_user_entry()
 * PURPOSE    To evaluate user's entry from command line (CIE coordins
 *            for white on user's monitor)
 *            
 * ARGUMENTS  double kmatrix[]    - K-matirx
 *            USER_ENTRY *user    - pointer to values entered by user
 *                                  (while values of user's monitor)
 *            CHAN_ON_COLOR *refs - pointer to results
 *            WHITE_MEAS *white   - poiter to results of white
 *                                  measurements performed by user
 *
 * RETURNS    1
 *
 * EXTERNS    None.
 *
 * NOTES:     This function evaluates white coordinates for user's
 *            monitor, which user entered via command line parameters.
 *            If user wants luminance calibration only, the algorithm 
 *            for this case is performed using k_matrix.
 *            Otherwise, the other algorithm is performed.
 *            
/**********************************************************************/
int process_user_entry(double *k_matrix, USER_ENTRY *user, CHAN_ON_COLOR *refs, WHITE_MEAS *white)
  {
  double x;
  double y;
  double z;

  if (user->y_W == 0.0 && user->y_W == 0.0)
    {
#if 0
 /*-------- beregning af xy og Y p† user monitor -------------------------
     X := KElement[1] * X65 + KElement[2] * Y65 + KElement[3] * Z65;
     Y := KElement[4] * X65 + KElement[5] * Y65 + KElement[6] * Z65;
     Z := KElement[7] * X65 + KElement[8] * Y65 + KElement[9] * Z65;

 if OnlyLumCal then            
  begin                        
   Xp65 := X / (X + Y + Z);
   Yp65 := Y / (X + Y + Z);
  end;
*/
#endif
    printf("Luminance calibration only!\n");
    x = k_matrix[0] * white->RonW + k_matrix[1] * white->GonW + k_matrix[2] * white->BonW;
    y = k_matrix[3] * white->RonW + k_matrix[4] * white->GonW + k_matrix[5] * white->BonW;
    z = k_matrix[6] * white->RonW + k_matrix[7] * white->GonW + k_matrix[8] * white->BonW;

    refs->members.RonW = x / (x + y + z);
    refs->members.GonW = user->lum_W;
    refs->members.BonW = y / (x + y + z);
    }
  else
    {
    refs->members.RonW = user->lum_W / user->y_W * user->x_W;
    refs->members.GonW = user->lum_W;
    refs->members.BonW = user->lum_W / user->y_W * (1 - user->x_W - user->y_W);
    }
  return (1);
  }


int process_args(char *arg1, char *arg2, char *arg3, USER_ENTRY * entry_value)
  {

  entry_value->x_W   = atof(arg1);
  entry_value->y_W   = atof(arg2);
  entry_value->lum_W = atof(arg3);
  printf("User entry:\n");
  printf("x = %.2f, y = %.2f, Y = %.2f\n", entry_value->x_W, entry_value->y_W, entry_value->lum_W);

  if (entry_value->lum_W > MAXLUM || entry_value->lum_W < MINLUM)
    {
    printf("Parameters out of range!\n\n");
    return (0);
    }
  
  if (entry_value->y_W == 0.0 && entry_value->y_W == 0.0)
    {
    return (1);
    }

  if (entry_value->y_W > MAX_X || entry_value->y_W < MIN_X)
    {
    printf("Parameters out of range!\n\n");
    return (0);
    }
  if (entry_value->x_W > MAX_X || entry_value->x_W < MIN_X)
    {
    printf("Parameters out of range!\n\n");
    return (0);
    }
  return (1);
  }


void give_info(void)
  {
  printf("MX.exe: program for user calibration of PT5639 color sensors.\n");
  printf("Version %.2f.\n", VERSION);
  printf("Invocation: mx x y Y\n");
  printf("where x, y and Y are known CIE1931 coordinates of a white\n");
  printf("rectangle on user screen.\n");
  printf("or:\n");
  printf("            mx r\n");
  printf("where r is a dummy parameter to restore factory calibration.\n");
  printf("Luminance value (Y) must be between 10.0 and 1000.0.\n");
  printf("Coordinate values (x and y) must be between 0.1 and 0.6.\n");
  printf("You must hold sensor on screen while running this calibration program.\n");
  }
/*
   Kontrollerer om en af de 9 K-matrixelementer er absolut st›rre
   end 32000, idet inden matrixelementerne skrives i EýPROM,
   multipliceres de med KorrMulFactor. Hver element skal kunne indeholdes
   i en INTEGER;
*/

/**********************************************************************
 * NAME       int valid_solution(double kmatrix[])
 * PURPOSE    To see if matrix elements exceed 32000.
 *            
 * ARGUMENTS  double kmatrix[] - K-matirx to check.
 *
 * RETURNS    0 if at least one element is bigger than 32000
 *            1 otherwise
 *
 * EXTERNS    None.
 *
 * NOTES:     K-matrix elements are stored inside head's EEprom as 
 *            signed integers (2 bytes), so if elements are bigger 
 *            than 32000 they cannot be stored and used properly.
 *            
/**********************************************************************/
int valid_solution(double kmatrix[])
  {
  int i;
  double limit;

  limit = 32767.5;  /* 32767,5 */

  for (i = 0; i < 9; i++)
    if (fabs(kmatrix[i] > limit))
      return (0);
  return (1);
  }


/**********************************************************************
 * NAME       int find_k()
 * PURPOSE    To find k-matrix
 *            
 * ARGUMENTS  int size                  - nr of equations
 *            double coefs[][NMAX + 1]) - array with coeficients and results
 *            CHAN_ON_COLOR * coef_val  - see comments before call to this function
 *            CHAN_ON_COLOR * constants - see comments before call to this function
 *
 * RETURNS    0 if equations cannot be solved
 *            1 otherwise
 *
 * EXTERNS    None.
 *
 * NOTES:     
 *            
/**********************************************************************/
int find_k(int size, double coefs[][NMAX + 1], CHAN_ON_COLOR * coef_val, CHAN_ON_COLOR * constants)
  {
  int row;
  int col;
  
  /* zero the coefs array */
  for (row = 0; row < size; row++)
    for (col = 0; col < size; col++)
      coefs[row][col] = 0.0;

  /* zero the constants column in coefs[][] array */
  for (col = 0; col < size; col++)
     coefs[col][size] = 0.0;

  /* fill coefficients: 1-st 3 rows (XonR) */
    row = 0;
    for (col = 0; col < 3; col++)
      coefs[row][col] = coef_val->entire[col];      /* 0, 1, 2 */
    row = 1;
    for (col = 3; col < 6; col++)
      coefs[row][col] = coef_val->entire[col - 3];  /* 0, 1, 2 */
    row = 2;
    for (col = 6; col < 9; col++)
      coefs[row][col] = coef_val->entire[col - 6];  /* 0, 1, 2 */

  /* fill coefficients: 2-nd 3 rows (XonG) */
    row = 3;
    for (col = 0; col < 3; col++)
      coefs[row][col] = coef_val->entire[col + 3];      /* 3, 4, 5 */
    row = 4;
    for (col = 3; col < 6; col++)
      coefs[row][col] = coef_val->entire[col - 3 + 3];  /* 3, 4, 5 */
    row = 5;
    for (col = 6; col < 9; col++)
      coefs[row][col] = coef_val->entire[col - 6 + 3];  /* 3, 4, 5 */

  /* fill coefficients: 3-rd 3 rows (XonB) */
    row = 6;
    for (col = 0; col < 3; col++)
      coefs[row][col] = coef_val->entire[col + 6];      /* 6, 7, 8 */
    row = 7;
    for (col = 3; col < 6; col++)
      coefs[row][col] = coef_val->entire[col - 3 + 6];  /* 6, 7, 8 */
    row = 8;
    for (col = 6; col < 9; col++)
      coefs[row][col] = coef_val->entire[col - 6 + 6];  /* 6, 7, 8 */

  /* fill coefficients: 4-th 3 rows (XonW) */
    row = 9;
    for (col = 0; col < 3; col++)
      coefs[row][col] = coef_val->entire[col + 9];      /* 9, 10, 11 */
    row = 10;
    for (col = 3; col < 6; col++)
      coefs[row][col] = coef_val->entire[col - 3 + 9];  /* 9, 10, 11 */
    row = 11;
    for (col = 6; col < 9; col++)
      coefs[row][col] = coef_val->entire[col - 6 + 9];  /* 9, 10, 11 */

  /* fill coefficients: columns 9 - 11 from constants (with -) (XonR, XonG and XonB) */
    col = 9;
    for (row = 0; row < 3; row++)
      coefs[row][col] = -constants->entire[row];
    col = 10;
    for (row = 3; row < 6; row++)
      coefs[row][col] = -constants->entire[row];
    col = 11;
    for (row = 6; row < 9; row++)
      coefs[row][col] = -constants->entire[row];


  /*load the constans vector (all are zeor except last 3, user's white entry) */
  for (col = 9; col < 12; col++)
    coefs[col][size]  = constants->entire[col];

#if 0
  for (row = 0; row < 12; row++)
    {
    for (col = 0; col < 12; col++)
      printf("a[%d][%d] = %lf\n", row, col, coefs[row][col]);
    }
#endif

  /* solve equations */
  if (!gaussian_elimination(size, coefs))
    return 0;
  else
    return 1;
  }

/**********************************************************************
 * NAME       int find_mx()
 * PURPOSE    To solve a set of equations by Gaussian Elimination
 *            method.
 *            
 * ARGUMENTS  int size                  - nr of equations
 *            double coefs[][NMAX + 1]  - array with coeficients and results
 *            double kmat[][NMAX + 1]   - two dimensional (3x3)array
 *                                        containing k-matrix 
 *            CHAN_ON_COLOR rgb         - constants
 *            CHAN_ON_COLOR * outbuf    - buffer for result values
 *
 * RETURNS    0 if equations cannot be solved
 *            1 otherwise
 *
 * EXTERNS    None.
 *
 * NOTES:     
 *            
/**********************************************************************/
int find_mx(int size, double coefs[][NMAX + 1], double kmat[][NMAX + 1],
            CHAN_ON_COLOR rgb, CHAN_ON_COLOR * outbuf)
  {
  int k;
  int i;


                /***********************************/
                /*         RED CHANNEL             */
                /*                                 */ 
                /* |a b c|      |X1|     |RonR|    */
                /* |d e f|   *  |X2|  =  |GonR|    */
                /* |g h j|      |X3|     |BonR|    */
                /*                                 */
                /***********************************/

  /* copy k-matrix into coefs */
  for (k = 0; k < size; k++)
    for (i = 0; i < size; i++)
      coefs[k][i] = kmat[k][i];

  /* copy constants into last column of coefs */
  coefs[0][size]  = rgb.members.RonR * 50.0;
  coefs[1][size]  = rgb.members.GonR * 50.0;
  coefs[2][size]  = rgb.members.BonR * 50.0;

  /* solve equations */
  if (!gaussian_elimination(size, coefs))
    return 0;

  /* copy solution into output buffer */
  for (k = 0; k < size; k++)
    outbuf->entire[k] = coefs[k][size];

                /***********************************/
                /*        GREEN CHANNEL            */
                /*                                 */ 
                /* |a b c|      |X4|     |RonG|    */
                /* |d e f|   *  |X5|  =  |GonG|    */
                /* |g h j|      |X6|     |BonG|    */
                /*                                 */
                /***********************************/

  /* copy k-matrix into coefs, coefs was changed during gaussian_elimination() */
  for (k = 0; k < size; k++)
    for (i = 0; i < size; i++)
      coefs[k][i] = kmat[k][i];

  coefs[0][size]  = rgb.members.RonG * 50.0;
  coefs[1][size]  = rgb.members.GonG * 50.0;
  coefs[2][size]  = rgb.members.BonG * 50.0;

  if (!gaussian_elimination(size, coefs))
    return 0;

  for (k = 3; k < size + 3; k++)
    outbuf->entire[k] = coefs[k - 3][size];

                /***********************************/
                /*         BLUE CHANNEL            */
                /*                                 */ 
                /* |a b c|      |X7|     |RonB|    */
                /* |d e f|   *  |X8|  =  |GonB|    */
                /* |g h j|      |X9|     |BonB|    */
                /*                                 */
                /***********************************/

  for (k = 0; k < size; k++)
    for (i = 0; i < size; i++)
      coefs[k][i] = kmat[k][i];

  coefs[0][size]  = rgb.members.RonB * 50.0;
  coefs[1][size]  = rgb.members.GonB * 50.0;
  coefs[2][size]  = rgb.members.BonB * 50.0;

  if (!gaussian_elimination(size, coefs))
    return 0;

  for (k = 6; k < size + 6; k++)
    outbuf->entire[k] = coefs[k - 6][size];
  return 1;
  }


/**********************************************************************
 * NAME       int gaussian_elimination(int n, double a[][NMAX + 1])
 * PURPOSE    To solve a set of n equations with n unknowns
 *            by Gaussian Elimination method.
 *            
 * ARGUMENTS  int n                - nr of equations
 *            double a[][NMAX + 1])- array with coeficients and results
 *
 * RETURNS    0 if equations cannot be solved
 *            1 otherwise, with results residing in last column at a[][n]
 *
 * EXTERNS    None.
 *
 * NOTES:     This function is taken from
 *              "Introductory Numerical Analysis"
 *               by Mircea Andrecut
 *            which also contains description of the method.
 *            
/**********************************************************************/
int gaussian_elimination(int n, double a[][NMAX + 1])
  {
  int    i, k, p, q, t;
  int    row[NMAX];           /* vector with row numbers */
  double x[NMAX], sum, m;     /* solution vector */

  /* initialize the vector with row numbers */
  for (i = 1; i <= n; i++)
    row[i - 1] = i - 1;

  /* start upper triangularization routine  */
  for (p = 1; p <= n - 1; p++)
    {
    for (k = p + 1; k <= n; k++)
      {
      if (fabs(a[row[k - 1]][p - 1]) > fabs(a[row[p - 1]][p - 1]))
        {
        t          = row[p - 1];
        row[p - 1] = row[k - 1];
        row[k - 1] = t;
        }
      }
    if (a[row[p - 1]][p - 1] == 0.0)
      {
      printf("ERROR: the matrix is singular!\n");
      return 0;
      }
    for (k = p + 1; k <= n; k++)
      {
      m = a[row[k - 1]][p - 1] / a[row[p - 1]][p - 1];
      for (q = p + 1; q <= n + 1; q++)
        {
        a[row[k - 1]][q - 1] -= m * a[row[p - 1]][q - 1];
        }
      }
    } 
  /* end of upper triangularization routine  */
  /* start the backward substitution routine */
  if (a[row[n - 1]][n - 1] == 0.0)
    {
    printf("ERROR: the matrix is singular!\n");
    return 0;
    }

  x[n - 1] = a[row[n - 1]][n] / a[row[n - 1]][n - 1];

  for (k = n - 1; k >= 1; k--)
    {
    sum = 0;
    for (q = k + 1; q <= n; q++)
      sum += a[row[k - 1]][q - 1] * x[q - 1];
    x[k - 1] = (a[row[k - 1]][n] - sum) / a[row[k - 1]][k - 1];
    }
  /* end the backward substitution routine */
  /* solution vector */
  for (k = 1; k <= n; k++)
    a[k - 1][n] = x[k - 1];
  return 1;
  }

/**********************************************************************/
/*******                                                         ******/
/**********************************************************************/

#if 1

/**********************************************************************/
/* NAME       int get_white_mx(WHITE_MEAS * res)
 * PURPOSE    To get averaged measurement result from sensor operating
 *            in MX mode on white screen.
 *
 * ARGUMENTS  WHITE_MEAS * res - pointer to structure where result is
 *            put.
 *
 * RETURNS     0 if there is no response from sensor,
 *             1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     This function takes NR_OF_MEAS measurements of sensor
 *            operating in MX mode (unmodified AD output) and 
 *            averages them and returns them at res.
 *            
 *            Relults are divided by 25.0 (integration time) 
 *            and multiplied by 20.0 (unknown)
/**********************************************************************/
int get_white_mx(WHITE_MEAS * res, int serport)
  {
  int i;
  WHITE_MEAS temp;
  WHITE_MEAS aver[NR_OF_MEAS];
  double     sumx;
  double     sumy;
  double     sumz;

  send_string("MS", serport);
  send_string("MX", serport);
  send_string("MC", serport);
  Xcom_Flush(serport);

  for (i = 0; i < NR_OF_MEAS; i++)
    {
    if (!get_line_mx(&temp, serport))
      return (0);
    aver[i].RonW = temp.RonW;
    aver[i].GonW = temp.GonW;
    aver[i].BonW = temp.BonW;
#if 0
    printf("x = %lf y = %lf z = %lf\n", aver[i].RonW, aver[i].GonW, aver[i].BonW);  
#endif
    }
  send_string("MS", serport);
  send_string("MS", serport);
  sumx = 0.0;
  sumy = 0.0;
  sumz = 0.0;
  for (i = 0; i < NR_OF_MEAS; i++)
    {
    sumx += aver[i].RonW;
    sumy += aver[i].GonW;
    sumz += aver[i].BonW;
    }
  res->RonW = sumx / (double)NR_OF_MEAS / 25.0 * 20.0;
  res->GonW = sumy / (double)NR_OF_MEAS / 25.0 * 20.0;
  res->BonW = sumz / (double)NR_OF_MEAS / 25.0 * 20.0;
#if 0
  printf("\navx = %lf avy = %lf avz = %lf\n", res->RonW, res->GonW, res->BonW);  
#endif
  return (1);
  }

/**********************************************************************
 * NAME       int get_line_mx(WHITE_MEAS * ptr)
 * PURPOSE    To fetch measurement result from sensor operating
 *            in MX mode.
 * ARGUMENTS  WHITE_MEAS * ptr - pointer to structure holding result
 *
 * RETURNS     0 if a character does not arrive during RXTIMEOUT,
 *             1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     In MX mode, the sensor outputs unmodified AD output
 *            as follows:
 *                        nX,nY,nZ,INTEGR_TIME<CR>
 *            As this function may be entered in the middle of a 
 *            transmission, it first waits until a CR is received and
 *            then collects the results.
 *            It then converts result from ASCII to int.
/**********************************************************************/
int get_line_mx(WHITE_MEAS * ptr, int from)
  {
  int   i;
  uchar c;
  char  str[80];
  char  buff[80];
  
  /* first wait until present transmission is over */
  do
    {
    if (!Xcom_get_data(&c, from))
      {
      printf("Timeout on receive.\n");
      return (0);
      }
    } while (c != ('\r'));

  /* then collect sent string */
  i = 0;
  do
    {
    if (!Xcom_get_data(&c, from))
      {
      printf("Timeout on receive.\n");
      return (0);
      }
    str[i++] = c;
    if (i > 79)
      {
      printf("Too many chars in line.\n");
      return (0);
      }
    } while (c != ('\r'));
  str[i] = '\0';
#if 0
  printf("%s\n", str); */
#endif
  /* now extract nX,nY and nZ from the string */
  i = 0;
  if (extract_white_meas(str, ptr))
    {
#if 0
    printf("x = %lf y = %lf z = %lf\n", ptr->RonW, ptr->GonW, ptr->BonW);
#endif
    return (1);
    }
  else
    return (0);
  }

/**********************************************************************
 * NAME       int extract_white_meas(char *ascii, WHITE_MEAS *res)
 *
 * PURPOSE    To extract 3 double values from string received from
 *            sensor operating in MX mode.
 *
 * ARGUMENTS  char * ascii     - string containing sensor response
 *            WHITE_MEAS * res - pointer to structure holding result
 *
 * RETURNS    1
 *
 * EXTERNS    None.
 *
 * NOTES:     In MX mode, the sensor outputs unmodified AD output
 *            as follows:
 *                        nX,nY,nZ,INTEGR_TIME<CR>, like:
 *                        811.10,1068.6,795.52,25.0
 *            This function converts 3 coma separated ascii values into
 *            3 double values.
/**********************************************************************/
int extract_white_meas(char *ascii, WHITE_MEAS *res)
  {
  char tmp[80];
  int  i;
  int  j;
  double  values[3];

  for (i = 0; i < 3; i++)
    {
    j = 0;
    while (*ascii != ',')
      {
      tmp[j++] = *(ascii++);
      if (j == 79)
        return (0);
      }
    ascii++;
    tmp[j] = '\0';
    values[i] = atof(tmp);
    }
  res->RonW = values[0];
  res->GonW = values[1];
  res->BonW = values[2];
  return (1);
  }

/**********************************************************************
 * NAME       int wp_on(void)
 *
 * PURPOSE    To set sensor write protection on
 *
 * ARGUMENTS  None
 *
 * RETURNS    0
 *
 * EXTERNS    None.
 *
 * NOTES:     To set sensor write protection off the following command
 *            sequence must be sent to sensor:
 *            "MS;WR79;WR102;WR102;"
 *            
 *            Two bytes in the E2PROM must both contain 0xFF
 *            to allow writing in the write protected memory.
 *            If only ONE BIT is cleared in any of these two 
 *            bytes, the E2PROM is write protected.
 *            Both these bytes are located in the write
 *            protected memory.
 *            The E2PROM conatins 0xFF in these bytes when leaving
 *            the factory, thereby setting the write-protection OFF
 *            during first time calibration.
 *            To initialize write protection, ie. reset the 
 *            two bytes in the E2PROM use the command:
 *            
 *            WR0;
 *            
 *            To suspend write protection, ie. set the two 
 *            bytes to 0xFF, isssue the following
 *            command sequence exactly as shown below:
 *            
 *            MS;WR79;WR102;WR102
/**********************************************************************/
int wp_on(int where)
  {
  send_string("MS", where);
  send_string("WR0", where);
  return 0;
  }

/**********************************************************************
 * NAME       int wp_off(void)
 *
 * PURPOSE    To set sensor write protection off
 *
 * ARGUMENTS  None
 *
 * RETURNS    0
 *
 * EXTERNS    None.
 *
 * NOTES:     To set sensor write protection off the following command
 *            sequence must be sent to sensor:
 *            "MS;WR79;WR102;WR102;"
/**********************************************************************/
int wp_off(int where)
  {
  send_string("MS", where);
  send_string("MS;WR79;WR102;WR102", where);
  return 0;
  }

/**********************************************************************
 * NAME       int get_sensor_id(int *i)
 *
 * PURPOSE    To get a number identifying sensor type
 *
 * ARGUMENTS  None
 *
 * RETURNS    0 on communication errors
 *            1 otherwise
 *            
 * EXTERNS    None.
 *
 * NOTES:     on exit, *i = value @ memory address 61, if successful
 *            
 DefInternID: array[1..10,1..4] of byte = {addr 58 - 61}
      (
      (1,0,0,  1),            {5639/90 serie III }
      (1,0,0,  2),            {5639/90 serie IV  }
      (0,0,0,  0),            {                  }
      (0,0,0,  0),            {                  }
      (1,0,0, 32),            {5639/92 serie I   }
      (1,0,0, 33),            {5639/92 serie II  }
      (0,0,0,  0),            {                  }
      (1,0,0, 56),            {5639/93           }
      (1,0,0, 40),            {5639/916          }
      (1,0,0, 57)             {5639/94           }
      );
 *            
/**********************************************************************/
int get_sensor_id(int * id, int where)
  {
  int  buff[4];
  char strbuff[20];
  int  i;

  send_string("MS", where);
  send_string("MA58", where);
  for (i = 0; i < 4; i++)
    {
    send_string("RM", where);
    if (!get_string(strbuff, where))
      return (0);
    buff[i] = atoi(strbuff);
    }
  *id = buff[3];
  return (1);
  }

int set_integration_time(int i_time, int where)
  {
  char strbuff[20];
  char tmpb[10];

  send_string("MS", where);
  strcpy(strbuff, "SI", where);
  itoa(i_time, tmpb, 10);
  strcat(strbuff, tmpb);
  send_string(strbuff, where);
  return (1);
  }
/**********************************************************************
 * NAME       int user_or_factory(int * what)
 * PURPOSE    To see if sensor is FACTORY or USER calibrated
 *            
 * ARGUMENTS  None
 *
 * RETURNS    0 on communication errors
 *            *what = USER,    if USER calibrated
 *            *what = FACTORY, if FACTORY calibrated
 *
 * EXTERNS    None.
 *
 * NOTES:     EEPROM memory @ 157 contains 1 if FACTORY or 0 if USER
 *            
/**********************************************************************/
int user_or_factory(int * what, int where)
  {
  char strbuff[20];
  int  i;

  send_string("MS", where);
  send_string("MA157", where);
  send_string("RM", where);
  if (!get_string(strbuff, where))
    return (0);
  i = atoi(strbuff);
  if (i)
    *what = FACTORY; /* FACTORY */
  else
    *what = USER;    /* USER    */
  return (1);
  }

int set_user_factory(int what, int where)
  {
  char strbuff[20];
  char temp[20];
  int  i;

  send_string("MS", where);
  send_string("MA157", where);
  strcpy(strbuff, "SM", where);
  sprintf(temp, "%d", what);
  strcat(strbuff, temp);
  send_string(strbuff, where);
  }
/**********************************************************************
 * NAME       int read_kmatrix(int addr, double *matbuff)
 * PURPOSE    To read kmatrix stored in sensor's EEPROM
 *            
 * ARGUMENTS  int addr        - EEPROM address where matrix is stored
 *            double *matbuff - array to put matrix into
 *
 * RETURNS    0 if matrix cannot be read,
 *            1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     The correction matrix double[9] is stored as int (2 bytes)
 *            so 18 bytes are read.  The command 'MA' sets memory
 *            address of the first read, address is then automatically
 *            incremented on each subsequent read. 
 *            See: memmap39.doc for details on how data is placed in 
 *            EEPROM.
/**********************************************************************/
int read_kmatrix(int addr, double *matbuff, int where)
  {
  char tmpb[40];
  char buff[40];
  char ch;
  char cl;
  int  c;
  int  i;

  send_string("MS", where);
  strcpy(buff, "MA", where);
  itoa(addr, tmpb, 10);
  strcat(buff, tmpb);
  send_string(buff, where);

  for (i = 0; i < 9; i++)
    {
    send_string("RM", where);
    if (!get_string(buff, where))
      return (0);
    ch = atoi(buff);  /* get HI value */
    send_string("RM", where);
    if (!get_string(buff, where))
      return (0);
    cl = atoi(buff);  /* get LO value */
    c = 256 * ch + cl;
    matbuff[i] = (double) c;
    }
  return (1);
  }

/**********************************************************************
 * NAME       int write_kmatrix(int addr, double *matbuff)
 * PURPOSE    To write kmatrix into sensor's EEPROM
 *            
 * ARGUMENTS  int addr        - EEPROM address where matrix is stored
 *            double *matbuff - array holding matrix values
 *
 * RETURNS    0 if matrix cannot be written correctly
 *            1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     See: memmap39.doc for details on how data is placed in 
 *            EEPROM.
 *            This function reads the matrix again after write and 
 *            compares results with original values.
/**********************************************************************/
int write_kmatrix(int addr, double *matbuff, int prt)
  {
  double check[9];
  char   tmpb[40];
  char   buff[40];
  char   ch;
  char   cl;
  unsigned int    c;
  int    i;

  send_string("MS", prt);
  strcpy(buff, "MA");
  itoa(addr, tmpb, 10);
  strcat(buff, tmpb);
  send_string(buff, prt);

  for (i = 0; i < 9; i++)
    {
    strcpy(buff, "SM");
    c  = (unsigned int)matbuff[i];
    ch = c / 256;
    sprintf(tmpb, "%d", ch);
    strcat(buff, tmpb);
    send_string(buff, prt);

    strcpy(buff, "SM");
    c  = (unsigned int)matbuff[i];
    cl = c % 256;
    sprintf(tmpb, "%d", cl);
    strcat(buff, tmpb);
    send_string(buff, prt);
    }
  /* now check if matrix is written correctly */
  if (!read_kmatrix(addr, check, prt))
    do_exit(1, prt, "");

  for (i = 0; i < 9; i++)
    if ((int) check[i] != (int) matbuff[i])
      {
      return (0);
      }
  return 1;
  }

/**********************************************************************
 * NAME       void send_string(char *str)
 * PURPOSE    To send a string to sensor
 *            
 * ARGUMENTS  char *str - string to send
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None.
 *
 * NOTES:     This function adds terminator to the string, as required
 *            by sensor command protocol.
/**********************************************************************/
void send_string(char *str, int prt)
  {
  while (*str)
    Xcom_TxData(*(str++), prt);
  Xcom_TxData(';', prt);
  }

/**********************************************************************
 * NAME       int get_string(char *str)
 * PURPOSE    To get a string terminated by \r sent by sensor
 *            
 * ARGUMENTS  char *str - string
 *
 * RETURNS    0 on communication errors
 *            1 otherwise
 *
 * EXTERNS    None.
 *
 * NOTES:     
 *            
/**********************************************************************/
int get_string(char *str, int prt)
  {
  int    i;
  uchar  c;
  
  i = 0;
  do
    {
    if (!Xcom_get_data(&c, prt))
      {
      printf("Timeout on receive.\n");
      return (0);
      }
    str[i++] = c;
    } while (c != '\r');
  str[i] = '\0';
  return (1);
  }
#endif
