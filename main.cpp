#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "kiss_fftr.h"

#include <math.h>
#include <string.h>

#include <time.h>

#include <iostream>
#include <vector>
#define DEBUG // RUN or DEBUG

#define solution2 // solution1 = crestfactor , solution2 = power to insignal

#define ITERATIONS // Criterio de parada, TIME or ITERATIONS
int repetitions;
#ifdef TIME
#define timeseconds 200
#define T_MAX 10000
#endif
#ifdef ITERATIONS
/* Maximum number of iterations */
#define T_MAX 800 // 200 segundos
#endif

/* Random number generator in [0, 1] */
#define Rand() ((double)rand() / RAND_MAX)

/* Structure of a particle */
typedef struct {
    double* x;
    double* v;
    double f;
    double f2;
    double pbest;
    double* x_star;
} ParticleRec, *Particle;

time_t t;
const double pi = 3.1415926535897932384626433832795;

/*
Parameters for PSO
*/

/* Number of particles */
#define Nparticles 50

/* The value of inertia weight at t=0 (W_0) and t=T_MAX (W_T) */
#define W_0 0.9      // 0,9
#define W_T 0.4      // 0,4
#define MAX_V 2 * pi // 2,0
/* The cognitive parameter (c1) and the social parameter (c2) */
#define c1 2
#define c2 2

#define oldapproach

/*
Definitions for a problem
*/

/* Objective function for minimization: problem dependent */
#define better(y1, y2) (y1 < y2)

int FFT_SIZE;

/* Number of variables: problem dependent */
int Nvariables;

int rvalues[2] = { -1, 1 };

double a1, a2;

double pbest_mean;

float temp1, temp2, diversity;
float Pinsignal, Ptotal, Pcoefficients, SNR;
float CF_mean;
float cferror;

double m_nmdf;

float resultmean[T_MAX], diversitymean[T_MAX];

Particle P;

char buff[255];
char* pch;

double* incoefficients = malloc(1 * sizeof(double));
double* result = malloc(1 * sizeof(double));
int* positions = malloc(1 * sizeof(int));
float* modulus = malloc(1 * sizeof(float));

float bestresult;

float inputvector[255];

void getparamenters()
{
    FILE* fparameters;
    fparameters = fopen("input.txt", "r");
    if(fparameters  == NULL) {
        printf("No input file");
        exit(0);
    }
    fscanf(fparameters, "%s", buff);

    FFT_SIZE = atoi(buff);
    incoefficients = realloc(incoefficients, FFT_SIZE / 2 * sizeof *incoefficients);
    result = realloc(result, FFT_SIZE / 2 * sizeof *result);
    fscanf(fparameters, "%s", buff);

    Nvariables = atoi(buff);
    positions = realloc(positions, Nvariables * sizeof *positions);
    modulus = realloc(modulus, Nvariables * sizeof *modulus);

    fscanf(fparameters, "%s", buff);

    pch = strtok(buff, ",");
    for(int i = 0; i < Nvariables; i++) {
        if(pch == NULL) {
            printf("Less harmonics than the number defined");
            exit(0);
        }
        positions[i] = atoi(pch);
        pch = strtok(NULL, ",");
    }
    if(pch != NULL) {
        printf("More harmonics than the number defined");
        exit(0);
    }

    fscanf(fparameters, "%s", buff);
    pch = strtok(buff, ",");
    for(int i = 0; i < Nvariables; i++) {
        if(pch == NULL) {
            printf("Less coefficients than the number defined");
            exit(0);
        }
        modulus[i] = atof(pch);
        pch = strtok(NULL, ",");
        //printf("%f,",modulus[i]);
    }

    if(pch != NULL) {
        printf("More coefficients than the number defined");
        exit(0);
    }

    fscanf(fparameters, "%s", buff);
    repetitions = atoi(buff);
    fclose(fparameters);
}

#ifdef DEBUG
double diversityMeasure()
{
    double diversity = 0;
    double aux_1 = 0;
    double aux_2 = 0;
    unsigned short int a = 0;
    unsigned short int b = 0;
    unsigned short int d = 0;
    for(a = 0; a < Nparticles; a++) {
        for(b = (a + 1); b < Nparticles; b++) {
            aux_1 = 0;
            for(d = 0; d < Nvariables; d++) { // SOMATÃ“RIO VARIANDO OS GENES ATÃ‰ O NUMERO DE NvariablesENSÃ•ES
                aux_1 += pow(P[a].x[d] - P[b].x[d], 2);
            }
            aux_1 = sqrt(aux_1);        // RAIZ QUADRADA DO RESULTADO DO SOMATÃ“RIO
            aux_1 = aux_1 / Nvariables; // DIVIDE RESULTADO PELO NUMERO DE NvariablesENSÃ•ES
            if(b == (a + 1) || aux_2 > aux_1) {
                aux_2 = aux_1; // ATRIBUI O MENOR RESULTADO A AUX_2
            }
        }
        diversity += log((double)1.0 + aux_2); // CALCULA O LOGARITMO NATURAL DO RESULTADO SOMADO DE UM
    }
    // ATRIBUI O RESULTADO A N_NMDF
    if(m_nmdf < diversity) {
        m_nmdf = diversity;
    }
    return diversity / m_nmdf;
}
#endif

void Evaluate(Particle P)
{
    int i;
    kiss_fft_cpx out[FFT_SIZE / 2 + 1];
    kiss_fft_scalar in[FFT_SIZE];

    kiss_fftr_cfg cfg;
    kiss_fftr_cfg icfg;
    cfg = kiss_fftr_alloc(FFT_SIZE, 0, NULL, NULL);
    icfg = kiss_fftr_alloc(FFT_SIZE, 1, NULL, NULL);

    for(i = 0; i < FFT_SIZE / 2 + 1; i++) {
        out[i].r = 0;
        out[i].i = 0;
    }

    for(i = 0; i < FFT_SIZE; i++) {
        in[i] = 0;
    }

    for(i = 0; i < Nvariables; i++) {
        out[positions[i]].r = modulus[i] * cos(P->x[i]);
        out[positions[i]].i = modulus[i] * sin(P->x[i]);
    }

    // fftw_plan q;
    // q = fftw_plan_r2r_1d(FFT_SIZE, coefficients, iinsignal, FFTW_HC2R, FFTW_ESTIMATE);

    kiss_fftri(icfg, out, in);

    for(i = 0; i < FFT_SIZE; i++) {

        if(in[i] > 0) {
            in[i] = 1;
        }
        if(in[i] < 0) {
            in[i] = -1;
        }
        if(in[i] == 0) {
            in[i] = floor(Rand() + 0.5);
        }
    }

    kiss_fftr(cfg, in, out);

    for(i = 0; i < FFT_SIZE / 2; i++) {
        result[i] = sqrt(pow(out[i].r, 2) + pow(out[i].i, 2));
    }

#ifdef solution1
    CF_mean = 0;
    for(i = 0; i < Nvariables; i++) {
        CF_mean += result[positions[i]] / Nvariables;
    }

    for(i = 0; i < Nvariables; i++) {
        P->f += pow((CF_mean - result[positions[i]]), 2);
    }
#endif

#ifdef solution2

    Pinsignal = 0;
    Ptotal = 0;
    for(i = 0; i < Nvariables; i++) {
        Pinsignal += result[positions[i]];
    }
    for(i = 0; i < FFT_SIZE / 2; i++) {
        Ptotal += result[i];
    }
    P->f = Ptotal / Pinsignal;

    P->f2 = 0;
    for(int i = 0; i < Nvariables; i++) {
        P->f2 += abs(result[positions[i]] / Pinsignal - modulus[i] / Pcoefficients);
    }
        printf("%f,",P->f);
    //printf("\n\n\n\n\n\n");

#endif

#ifdef solution3
    bestresult = 0;
    P->f = 0;
    
    for(i = 0; i < FFT_SIZE / 2; i++) {
        if(result[i] > bestresult)
        {
            bestresult = result[i];
        }
    }
    
    for(i = 0; i < FFT_SIZE / 2; i++) {
        result[i] = result[i]/bestresult;
    }
    //printf("\n\n\n\n\n\n");
    for(i = 0; i < FFT_SIZE / 2; i++) {
        P->f += fabs(inputvector[i] - result[i]);
        //printf("%f,\n",fabs(inputvector[i] - result[i]));
        //printf("%f,",P->f);
    }
    
    //printf("\n\n\n\n\n\n");
    
#endif

    free(cfg);
    free(icfg);
}

/* update pbest */
void UpdateBest(Particle P)
{
    int j;

    for(j = 0; j < Nvariables; j++)
        P->x_star[j] = P->x[j];
    P->pbest = P->f;
}

/* Initialization of particles: problem dependent */
/* The function returns the index of the best particle */
int Initialize(Particle P, int n)
{
    int i, j;
    int G; /* the index of the best particle */

    G = 0;
    for(i = 0; i < n; i++) {
        for(j = 0; j < Nvariables; j++) {
            P[i].x[j] = Rand() * 2 * pi; /* problem dependent */
            P[i].v[j] = Rand() * 2 * pi; /* problem dependent */
        }
        Evaluate(&P[i]);
        UpdateBest(&P[i]);
        if(better(P[i].f, P[G].f))
            G = i;
    }
    return G;
}

/*
Definitions for PSO
*/

/* allocate lol data structures */
#define lol(type, n, msg) (type*) lolCell(sizeof(type), n, msg)

void* lolCell(int size, int n, char* msg)
{
    void* lol;

    if((lol = malloc(size * n)) == NULL) {
        fprintf(stderr, "Cannot allocate memory for %d %s\n", n, msg);
        exit(1);
    }
    return lol;
}

/* allocate "n" lol particles */
Particle lolParticles(int n)
{
    int i;
    Particle P;

    P = lol(ParticleRec, n, "particles");
    for(i = 0; i < n; i++) {
        P[i].x = lol(double, Nvariables, "x");
        P[i].v = lol(double, Nvariables, "v");
        P[i].x_star = lol(double, Nvariables, "x*");
    }
    return P;
}

/* Print a particle */
void Print(Particle P)
{
    /*    int j;

    for(j=0; j<Nvariables; j++)
    printf("%f ", P->x_star[j]);
    printf(" = %f", P->pbest);*/
    printf("%f,", P->pbest);
}

/* Particle Swarm Optimization */
int main()
{
    srand(time(NULL));
    getparamenters();

    m_nmdf = 0;

    int t, i, j;

    for(i = 0; i < Nvariables; i++)
        Pcoefficients += modulus[i];
        
            for(i = 0; i < Nvariables; i++) {
        inputvector[positions[i]] = modulus[i];
    }

    int G;
    double w;

    P = lolParticles(Nparticles);
    G = Initialize(P, Nparticles);
    w = W_0;
    for(t = 1; t <= T_MAX; t++) {
        for(i = 0; i < Nparticles; i++) {
            for(j = 0; j < Nvariables; j++) {

                P[i].v[j] = w * P[i].v[j] + c1 * Rand() * (P[i].x_star[j] - P[i].x[j]) +
                    c2 * Rand() * (P[G].x_star[j] - P[i].x[j]);
                if(P[i].v[j] < -MAX_V)
                    P[i].v[j] = -MAX_V;
                else if(P[i].v[j] > MAX_V)
                    P[i].v[j] = MAX_V;
                P[i].x[j] += P[i].v[j];
                if(P[i].x[j] >= 2 * pi)
                    P[i].x[j] = 2 * pi;
                if(P[i].x[j] < 0)
                    P[i].x[j] = 0;
            }
            Evaluate(&P[i]);
            if(better(P[i].f, P[i].pbest)) {
                if(better(P[i].f, P[G].pbest))
                    G = i;
                UpdateBest(&P[i]);
            }
        }
#ifdef DEBUG

        // Media mejor resultado particulas
        pbest_mean = 0;
        for(int l = 0; l < Nparticles; l++) {
            pbest_mean += P[l].pbest / Nparticles;
        }

        resultmean[t - 1] += P[G].pbest;
        diversitymean[t - 1] += diversityMeasure();

#endif
        // printf("\n");
        w -= (W_0 - W_T) / T_MAX;
#ifdef TIME
        if(Ttime.read() > timeseconds) {
#ifdef RUN
            printf("%i,", t);
#endif
            t = 1000000;
        }
#endif
    }

    kiss_fft_cpx out[FFT_SIZE / 2 + 1];
    kiss_fft_scalar in[FFT_SIZE];

    kiss_fftr_cfg cfg;
    kiss_fftr_cfg icfg;
    cfg = kiss_fftr_alloc(FFT_SIZE, 0, NULL, NULL);
    icfg = kiss_fftr_alloc(FFT_SIZE, 1, NULL, NULL);

    for(i = 0; i < FFT_SIZE / 2 + 1; i++) {
        out[i].r = 0;
        out[i].i = 0;
    }

    for(i = 0; i < FFT_SIZE; i++) {
        in[i] = 0;
    }

    for(i = 0; i < Nvariables; i++) {
        out[positions[i]].r = modulus[i] * cos(P[G].x_star[i]);
        out[positions[i]].i = modulus[i] * sin(P[G].x_star[i]);
    }
    kiss_fftri(icfg, out, in);

    for(i = 0; i < FFT_SIZE; i++) {

        if(in[i] > 0) {
            in[i] = 1;
        }
        if(in[i] < 0) {
            in[i] = -1;
        }
        if(in[i] == 0) {
            in[i] = floor(Rand() + 0.5);
        }
    }

    FILE* fptrF;

    fptrF = fopen("output.txt", "w");

    fprintf(fptrF, "Pontos:\n");
    for(int i = 0; i < FFT_SIZE; i++) {
        fprintf(fptrF, "%.0f,", in[i]);
    }

    kiss_fftr(cfg, in, out);

    for(i = 0; i < FFT_SIZE / 2; i++) {
        result[i] = sqrt(pow(out[i].r, 2) + pow(out[i].i, 2));
    }

    for(int i = 0; i < FFT_SIZE / 2; i++) {
        incoefficients[i] = 0;
    }

    fprintf(fptrF, "\n\nPcição: Desejado - Obtod\n");
    for(i = 0; i < Nvariables; i++) {
        incoefficients[positions[i]] = modulus[i] * 100;
    }
    
    /*    bestresult = 0;
   
    
    for(i = 0; i < FFT_SIZE / 2; i++) {
        if(result[i] > bestresult)
        {
            bestresult = result[i];
        }
    }
    
    
      for(int i = 0; i < FFT_SIZE / 2; i++) {
        fprintf(fptrF, "%i: %.0f - %.0f\n", i, incoefficients[i], result[i] *100/ bestresult);
    }  */
    
    
    for(int i = 0; i < FFT_SIZE / 2; i++) {
        fprintf(fptrF, "%i: %.0f - %.0f\n", i, incoefficients[i], result[i] * 200 / FFT_SIZE);
   }
    fprintf(fptrF, "\n\n%f\n", P[G].f);

    fclose(fptrF);
    system("notepad output.txt");
    return (0);
}
