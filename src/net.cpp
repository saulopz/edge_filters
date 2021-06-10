/* Neural network C code created by QwikNet V2.23
 * Network file name: untitled
 * Structure:         9/10/1
 * Date:              Thu Aug 30 08:45:32 2001
 */

#include <math.h>
#include <stdlib.h>

#define INPUTS 9
#define HIDDEN1 10
#define OUTPUTS 1

#define HIDDEN1_INDEX (INPUTS)
#define OUTPUT_INDEX (HIDDEN1_INDEX + HIDDEN1)

#define NUMNEURONS (INPUTS + HIDDEN1 + OUTPUTS)
static double NEURON[NUMNEURONS];

/* Weight arrays */
static const double W_In_H1[HIDDEN1][INPUTS + 1] = {
	1.46306, -0.48353, 0.556802, 0.696731, -3.54228, 0.580612, -1.29206, 2.21547, -0.218727, 0.30247,
	0.12464, -0.363792, -0.00912601, -0.208304, 1.57896, -0.759195, 0.448252, -0.246646, -0.347245, -0.128616,
	1.36362, -2.34813, 0.198959, -1.62104, 3.97698, 4.22806e-005, -1.18597, -0.227945, 0.0399828, -0.338878,
	0.391645, -0.321871, 0.721798, -0.257518, 1.97014, -0.427979, -0.39448, 0.592101, -0.633118, 0.149267,
	0.473705, 0.669365, 0.679692, 0.557485, -2.159, 0.399681, 0.00418039, 0.82986, 0.637189, 0.035582,
	0.804048, 1.72548, 1.35932, -0.0380864, -2.99959, 0.576475, 0.904021, 1.08142, 0.991563, 0.25353,
	0.33809, -0.319366, 0.451423, -0.977038, -2.31819, -0.223899, -0.826951, 0.0597971, 0.0875238, -0.515679,
	-0.20823, 0.640548, 0.349454, 1.45503, -2.91341, 0.269586, 0.793465, 0.326139, 0.357733, 0.13929,
	1.64543, -0.776498, 1.18467, 0.695131, -3.7529, 0.287333, -1.59288, 2.46328, -0.76261, 0.421524,
	-0.178771, 0.779554, 0.395544, 1.52267, -3.41258, 0.351137, 0.67866, 0.489285, 0.376394, 0.408207};
static const double W_H1_Out[OUTPUTS][HIDDEN1 + 1] = {
	4.45606, -2.40843, -5.90517, -3.18314, 2.35856, 4.03491, 3.45828, 3.09768, 5.04998, 3.6273, -2.01805};

/* Scale factors */
static const double R_MIN[INPUTS + OUTPUTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const double R_MAX[INPUTS + OUTPUTS] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
static const double S_MIN[INPUTS + OUTPUTS] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, 0};
static const double S_MAX[INPUTS + OUTPUTS] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

/* Nonlinearity functions */
static double sigmoid(double x) { return (1.0 / (1.0 + exp(-x))); }
//static double ftanh( double x ) { return tanh( x ); }
//static double gaussian( double x ) { return exp( -x*x/2 ); }
//static double linear( double x ) { return x; }

/* Compute the network response 
 * Arguments: 
 *   inputs   pointer to (double) array containing the input values 
 *   outputs  pointer to (double) array which receives the results 
 * Return: 
 *   NonZero (1) if successful, 0 if not. 
 */
int eval_net(const double *inputs, double *outputs)
{
	double F;
	int i, j;

	/* Check for invalid arguments */
	if (inputs == NULL || outputs == NULL)
		return 0;

	/* Zero neuron sums */
	for (i = 0; i < NUMNEURONS; i++)
		NEURON[i] = 0.0;

	/* Apply inputs */
	for (i = 0; i < INPUTS; i++)
	{
		F = (S_MAX[i] - S_MIN[i]) / (R_MAX[i] - R_MIN[i]); /* scale factor */
		NEURON[i] = F * inputs[i] + S_MIN[i] - F * R_MIN[i];
	}

	/* Compute hidden layer #1 */
	for (i = 0; i < HIDDEN1; i++)
	{
		for (j = 0; j < INPUTS; j++)
		{
			NEURON[HIDDEN1_INDEX + i] += NEURON[j] * W_In_H1[i][j];
		}
		NEURON[HIDDEN1_INDEX + i] += W_In_H1[i][INPUTS]; /* BIAS */
		NEURON[HIDDEN1_INDEX + i] = sigmoid(NEURON[HIDDEN1_INDEX + i]);
	}

	/* Compute network outputs */
	for (i = 0; i < OUTPUTS; i++)
	{
		for (j = 0; j < HIDDEN1; j++)
		{
			NEURON[OUTPUT_INDEX + i] += NEURON[HIDDEN1_INDEX + j] * W_H1_Out[i][j];
		}
		NEURON[OUTPUT_INDEX + i] += W_H1_Out[i][HIDDEN1]; /* BIAS */
		NEURON[OUTPUT_INDEX + i] = sigmoid(NEURON[OUTPUT_INDEX + i]);
	}

	/* Copy outputs to output array */
	for (i = 0; i < OUTPUTS; i++)
	{
		F = (R_MAX[i + INPUTS] - R_MIN[i + INPUTS]) / (S_MAX[i + INPUTS] - S_MIN[i + INPUTS]); /* unscale factor */
		outputs[i] = F * NEURON[OUTPUT_INDEX + i] + R_MIN[i + INPUTS] - F * S_MIN[i + INPUTS];
	}

	return 1;
}
