/*  Functionaliteit: Werking van een Neuron (Perceptron)

W1*I1 + w2*I2 = input

input > threshold = > output = 1

input < threshold = > output = 0

aanpassing w1, w2 en threshold op basis van een gelabeld paar(I1, I2, correctoutput)

aanpassing threshold = -(correctoutput – output)

aanpassing W1 = aanpassing threshold * I1

aanpassing W2 = aanpassing threshold * I2

Jan Oostindie 
Avans Hogeschool 
Breda
email: jac.oostindie@avanns.nl
dd 9-9-2016

*/

#include <iostream>
#include <iomanip>
#include "perceptron.h"

using namespace std;

double perceptronOut(double *input, double *weightFactors, int size, double threshold)
{
	// bereken input
	double y = 0.0;
	for (int i = 0; i < size; i++)
	{
		y += input[i] * weightFactors[i];
	}

	// bepaal output
	if (y > threshold) return 1.0;
	else return 0.0;
}

double perceptronOutput2(double x1, double x2, double W1, double W2, double threshold) {
	// bereken input
	double input = W1*x1 + W2*x2;

	// bepaal output
	if (input > threshold) return 1.0;
	else return 0.0;
} // perceptronOutput

double perceptronOutput3(double x1, double x2, double x3, double W1, double W2, double W3, double threshold) {
	// bereken input
	double input = W1*x1 + W2*x2 + W3*x3;

	// bepaal output
	if (input > threshold) return 1.0;
	else return 0.0;
} // perceptronOutput


void examplePerceptron() {

	// waarheidstabel AND functie
	double tabel[4][3] = { { 0.0, 0.0, 0.0 }, 
	                       { 0.0, 1.0, 0.0 }, 
						   { 1.0, 0.0, 0.0 }, 
						   { 1.0, 1.0, 1.0 } };

	// startwaarden van de weegfactoren (willekeurig)
	double W1 = 1003;
	double W2 = 456;

	// startwaarde van de threshold (willekeurig)
	double threshold = 185;

	// input en output variabele
	double input, output, deltaOutput;

	// aanpassingen
	double deltaThreshold = 0.0, deltaW1 = 0.0, deltaW2 = 0.0;

    // boolean flag wordt gehezen als output meer dan EPSILON afwijkt van verwachte output
	bool flag = false;
	const int EPSILON = 0.000001;

	// waarheidstabel afdrukken 
	cout << "Waarheidstabel waarvoor het perceptron wordt getraind" << endl;
	for (int rij = 0; rij < 4; rij++) {
		cout << setw(3) << tabel[rij][0] << setw(3) << tabel[rij][1] << setw(3) 
			<< tabel[rij][2] << endl;
		cout << endl;
	}
	cout << "Press ENTER to continue" << endl;
	cin.ignore();

	int aantalRuns = 0;
	do {
		// voer een run uit
		flag = false;
		for (int rij = 0; rij < 4; rij++) {

			// bereken actuele output van het nog ongetrainde perceptron
			output = perceptronOutput2(tabel[rij][0], tabel[rij][1], W1, W2, threshold);

			// bereken de afwijking van de actuele output t.o.v. verwachte output
			deltaOutput = (output - tabel[rij][2]);

			if (abs(deltaOutput) > EPSILON) {
				
				flag = true;

				// berekening aanpassing W1 en W2
				deltaW1 = -deltaOutput*tabel[rij][0];
				deltaW2 = -deltaOutput*tabel[rij][1];

				// threshold, W1 en W2 aanpassen
				threshold += deltaOutput;
				W1 += deltaW1;
				W2 += deltaW2;

			} // if

		} // for

		cout << "Runs " << ++aantalRuns << 
			"   Threshold = " << threshold << endl;
	}
	while (flag);

	cout << "Berekende threshold = " << threshold << endl;
	cout << "Berekende W1 = " << W1 << endl;
	cout << "Berekende W2 = " << W2 << endl;
	cout << "Press ENTER to continue" << endl;
	cin.ignore();

	// genereer waarheidstabel met getrainde perceptron
	cout << "waarheidstabel gegenereerd met perceptron => " << endl;
	for (int rij = 0; rij < 4; rij++) {
		cout << setw(3) << tabel[rij][0] << setw(3) << tabel[rij][1] << setw(3)
			 << perceptronOutput2(tabel[rij][0], tabel[rij][1], W1, W2, threshold) << endl;
	}

	cout << "Press ENTER to continue" << endl;
	cin.ignore();

} // main

void vccp3_1b()
{
	// waarheidstabel x1 AND x2 OR x3 functie
	// parameters: a, b, c
	// tabel[2 ^ amount of parameters][amount of parameters + 1]
	double tabel[8][4] = {
		{ 0.0, 0.0, 0.0, 0.0 },
		{ 1.0, 0.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0, 0.0 },
		{ 1.0, 1.0, 0.0, 1.0 },
		{ 0.0, 0.0, 1.0, 1.0 },
		{ 1.0, 0.0, 1.0, 1.0 },
		{ 0.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
	};

	// startwaarden van de weegfactoren (willekeurig)
	double W1 = 1003;
	double W2 = 456;
	double W3 = 603;

	// startwaarde van de threshold (willekeurig)
	double threshold = 185;

	// input en output variabele
	double input, output, deltaOutput;

	// aanpassingen
	double deltaThreshold = 0.0, deltaW1 = 0.0, deltaW2 = 0.0, deltaW3 = 0.0;

	// boolean flag wordt gehezen als output meer dan EPSILON afwijkt van verwachte output
	bool flag = false;
	const double EPSILON = 0.000001;

	// waarheidstabel afdrukken 
	cout << "Waarheidstabel waarvoor het perceptron wordt getraind" << endl;
	for (int rij = 0; rij < 8; rij++) {
		cout << setw(3) << tabel[rij][0] << setw(3) << tabel[rij][1] << setw(3) << tabel[rij][2] << setw(3)
			<< tabel[rij][3] << endl;
		cout << endl;
	}
	cout << "Press ENTER to continue" << endl;
	cin.ignore();

	int aantalRuns = 0;
	do {
		// voer een run uit
		flag = false;
		for (int rij = 0; rij < 8; rij++) {

			// bereken actuele output van het nog ongetrainde perceptron
			output = perceptronOutput3(tabel[rij][0], tabel[rij][1], tabel[rij][2], W1, W2, W3, threshold);

			// bereken de afwijking van de actuele output t.o.v. verwachte output
			deltaOutput = (output - tabel[rij][3]);

			if (abs(deltaOutput) > EPSILON) {

				flag = true;

				// berekening aanpassing W1 en W2 en W3
				deltaW1 = -deltaOutput*tabel[rij][0];
				deltaW2 = -deltaOutput*tabel[rij][1];
				deltaW3 = -deltaOutput*tabel[rij][2];

				// threshold, W1 en W2 en W3 aanpassen
				threshold += deltaOutput;
				W1 += deltaW1;
				W2 += deltaW2;
				W3 += deltaW3;

			} // if

		} // for

		cout << "Runs " << ++aantalRuns <<
			"   Threshold = " << threshold << endl;
	} while (flag);

	cout << "Berekende threshold = " << threshold << endl;
	cout << "Berekende W1 = " << W1 << endl;
	cout << "Berekende W2 = " << W2 << endl;
	cout << "Berekende W3 = " << W3 << endl;
	cout << "Press ENTER to continue" << endl;
	cin.ignore();

	// genereer waarheidstabel met getrainde perceptron
	cout << "waarheidstabel gegenereerd met perceptron => " << endl;
	for (int rij = 0; rij < 8; rij++) {
		cout << setw(3) << tabel[rij][0] << setw(3) << tabel[rij][1] << setw(3) << tabel[rij][2] << setw(2)
			<< " | " << perceptronOutput3(tabel[rij][0], tabel[rij][1], tabel[rij][2], W1, W2, W3, threshold) << endl;
	}

	cout << "Press ENTER to continue" << endl;
	cin.ignore();
}

constexpr int power(int base, int exponent) {
	int value = 1;
	for (int i = 0; i < exponent; i++) {
		value *= base;
	}
	return value;
}

void vccp3_1b2()
{
	// waarheidstabel x1 AND x2 OR x3 functie
	// parameters: a, b, c
	// tabel[2 ^ amount of parameters][amount of parameters + 1]
	const int parameters = 3;
	const int rows = power(2, parameters);
	const int columns = (parameters + 1);
	double tabel[rows][columns] = {
		{ 0.0, 0.0, 0.0, 0.0 },
		{ 1.0, 0.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0, 0.0 },
		{ 1.0, 1.0, 0.0, 1.0 },
		{ 0.0, 0.0, 1.0, 1.0 },
		{ 1.0, 0.0, 1.0, 1.0 },
		{ 0.0, 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
	};

	// startwaarden van de weegfactoren (willekeurig)
	double W[parameters];
	
	// startwaarde van de threshold (willekeurig)
	double threshold = 185;

	// input en output variabele
	double input, output, deltaOutput;

	// aanpassingen
	double deltaThreshold = 0.0;
	double deltaW[parameters];

	for (int i = 0; i < parameters; i++) {
		W[i] = ((int)rand() / RAND_MAX) * 1000;
		deltaW[i] = 0.0;
	}

	// boolean flag wordt gehezen als output meer dan EPSILON afwijkt van verwachte output
	bool flag = false;
	const int EPSILON = 0.000001;

	// waarheidstabel afdrukken 
	cout << "Waarheidstabel waarvoor het perceptron wordt getraind" << endl;
	for (int row = 0; row < rows; row++) {
		for (int column = 0; column < parameters; column++) {
			cout << setw(3) << tabel[row][column];
		}		
		cout << setw(2) << " | " << tabel[row][columns - 1];
		cout << endl;
	}
	cout << "Press ENTER to continue" << endl;
	cin.ignore();

	int aantalRuns = 0;
	do {
		// voer een run uit
		flag = false;
		for (int row = 0; row < rows; row++) {

			// bereken actuele output van het nog ongetrainde perceptron
			output = perceptronOut(tabel[row], W, parameters, threshold);

			// bereken de afwijking van de actuele output t.o.v. verwachte output
			deltaOutput = (output - tabel[row][columns - 1]);

			if (abs(deltaOutput) > EPSILON) {

				flag = true;
				
				// berekening aanpassing W
				for (int p = 0; p < parameters; p++) {
					deltaW[p] = -deltaOutput*tabel[row][p];					
				}

				// threshold, W aanpassen
				threshold += deltaOutput;				
				for (int p = 0; p < parameters; p++) {
					W[p] = W[p] + deltaW[p];
				}

			} // if

		} // for

		cout << "Runs " << ++aantalRuns <<
			"   Threshold = " << threshold << endl;
	} while (flag);

	cout << "Berekende threshold = " << threshold << endl;
	for (int i = 0; i < parameters; i++) {
		cout << "Berekende W" << i << " = " << W[i] << endl;
	}
	cout << "Press ENTER to continue" << endl;
	cin.ignore();

	// genereer waarheidstabel met getrainde perceptron
	cout << "waarheidstabel gegenereerd met perceptron => " << endl;
	for (int row = 0; row < rows; row++) {
		for (int column = 0; column < columns - 1; column++) {
			cout << setw(3) << tabel[row][column];
		}
		cout << setw(2) << " | " << perceptronOut(tabel[row], W, parameters, threshold) << endl;
	}

	cout << "Press ENTER to continue" << endl;
	cin.ignore();
}
		




