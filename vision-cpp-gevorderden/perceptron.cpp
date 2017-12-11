/*  Functionaliteit: Werking van een Neuron (Perceptron)

W1*I1 + w2*I2 = input

input > threshold = > output = 1

input < threshold = > output = 0

aanpassing w1, w2 en threshold op basis van een gelabeld paar(I1, I2, correctoutput)

aanpassing threshold = -(correctoutput � output)

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

using namespace std;


double perceptronOutput(double x1, double x2, double W1, double W2, double threshold) {
	// bereken input
	double input = W1*x1 + W2*x2;

	// bepaal output
	if (input > threshold) return 1.0;
	else return 0.0;
} // perceptronOutput


void main() {

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
			output = perceptronOutput(tabel[rij][0], tabel[rij][1], W1, W2, threshold);

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
			 << perceptronOutput(tabel[rij][0], tabel[rij][1], W1, W2, threshold) << endl;
	}

	cout << "Press ENTER to continue" << endl;
	cin.ignore();

} // main
		




