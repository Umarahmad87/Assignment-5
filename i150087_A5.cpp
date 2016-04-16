#include <iostream>
#include <vector>
#include <cmath>
#include<iomanip>
#include "Point.h"
using namespace std;
class Neuron {
protected:
	Point position_;
	double signal_;
	double attenuation;
	Neuron **neighbours;
	int connected;
public:
	Neuron(double x, double y, double att = 1) :
			position_(x, y) {
		signal_ = 0;
		attenuation = att;
		connected = 0;
		neighbours = new Neuron *[100];
	}
	void virtual accumulate(double received_signal) {
		signal_ = received_signal * attenuation;
	}
	Point position() {
		return position_;
	}
	double signal() {
		return signal_;
	}

	void virtual fire(double rs) {
		accumulate(rs);
		propagate();
	}
	void virtual propagate() {
		for (int index = 0; index < connected; index++) {
			neighbours[index]->fire(signal_);
		}
	}
	void virtual operator +=(Neuron *N) {
		neighbours[connected++] = N;
	}
	double Signal() {
		return signal_;
	}
	void display() {
		if (connected != 0) {
			cout << "The neuron at position " << position_
					<< "with an attenuation factor of " << attenuation
					<< " is connected to following (" << connected
					<< ") neuron(s):";
			cout << endl;
			for (int i = 0; i < connected; i++) {
				cout << "Neuron at position " << neighbours[i]->position_
						<< endl;
			}
		} else {
			cout << "The neuron at position" << position_
					<< " with an attenuation factor of " << attenuation
					<< " is not connected to any neuron." << endl;
		}
	}
	bool operator ||(Neuron &N) {
		bool o_;
		o_ = signal_ or N.signal_;
		return o_;
	}
	bool operator &&(Neuron &N) {
		bool o_;
		o_ = signal_ and N.signal_;
		return o_;
	}
	bool operator !() {
		return not (this->signal_);
	}
	virtual ~Neuron() {
		delete[] neighbours;
	}
};
ostream& operator <<(ostream &out, Neuron &N) {
	N.display();
	return out;
}

class CumulativeNeuron: public Neuron {
protected:
	int total_incoming_connections;
	int counter;

public:
	CumulativeNeuron(double x, double y, double att = 1) :
			Neuron(x, y, att) {
		total_incoming_connections = 0;
		counter = 0;
	}
	void operator +=(Neuron *N) {
		neighbours[connected++] = N;
		counter++;
	}
	void accumulate(double received_signal) {
		Neuron::accumulate(received_signal);
		signal_ = attenuation / (1 + exp(-signal_));
		/*for(int i=0;i<connected;i++){
		 signal_ = signal_ + neighbours[i]->signal();
		 }*/
	}
	void propagate() {
		for (int index = 0; index < connected; index++) {
			neighbours[index]->fire(signal_);
		}
		counter = 0;
	}
	int Counter() {
		return counter;
	}

};

class ComplexNeuron: public Neuron {

public:
	ComplexNeuron(int x_,int y_,double A=1):Neuron(x_,y_,A){

	}
	void attach(double att, Neuron* N) {
		this->attenuation=att;
		this->neighbours[connected++]=N;
	}


	void  fire(double rs) {
		accumulate(rs);
		for (int index = 0; index < connected; index++) {
					neighbours[index]->fire(signal_ * attenuation);
				}
		}
	bool operator | (ComplexNeuron &N) {
			bool o_;
			o_ = (signal_ and N.signal_) or ( not(signal_) and not(N.signal_));
			return o_;
		}
};

class ComplexCumulativeNeuron:public CumulativeNeuron{

public:
	ComplexCumulativeNeuron(int x,int y,double att):CumulativeNeuron(x,y,att) {

	}
	void attach(double att, Neuron* N) {
			this->attenuation=att;
			this->neighbours[connected++]=N;}
	void fire(double rs) {
			accumulate(rs);
			propagate();
		}

};


int main() {
	cout << "===== Test of 1st part  =====" << endl << endl;

	Neuron neuron1(0, 1, 0.5);
	Neuron neuron2(1, 0, 1.0);
	Neuron neuron3(1, 1, 2.0);

	neuron1 += &neuron2;
	neuron1 += &neuron3;
	neuron2 += &neuron3;

	cout << neuron1 << endl;

	neuron1.fire(10);
	cout << "Signals :" << endl;
	cout << neuron1.signal() << endl;
	cout << neuron2.signal() << endl;
	cout << neuron3.signal() << endl;

	// Test of  part 2
	cout << endl << "===== Test of 2nd part =====" << endl << endl;

	CumulativeNeuron neuron4(0, 0, 0.75);
	cout << neuron4 << endl;
	neuron4.fire(10);
	cout << "Signal :" << endl;
	cout << setprecision(2) << neuron4.signal() << endl;

	// Test of Logical OR Gate
	cout << "===== Test of Logical gate \"or\" =====" << endl << endl;
	bool n;
	// building the architecture...
	// Here n1 and n2 will be our inputs, n0 will be always 1.
	Neuron n0(2, 0, -10);
	Neuron n1(1, 0, 20.0);
	Neuron n2(0, 0, 20.0);
	CumulativeNeuron n3(1, 3, 1); // will act as output
	n0 += &n3;
	n1 += &n3;
	n2 += &n3;
	// Now lets check the system.
	n0.fire(1); // will always fire 1...
	n1.fire(0);
	n2.fire(1);
	cout << "If a=0 and b=1 then a|b == " << n3; //
	n = n1 || n2;
	cout << "Clipped Signal:" << n << endl << endl;

	n1.fire(1);
	n2.fire(1);
	cout << "If a=1 and b=1 then a|b == " << n3;
	n = n1 || n2;
	cout << "Clipped Signal:" << n << endl << endl;

	n1.fire(0);
	n2.fire(0);
	cout << "If a=0 and b=0 then a|b == " << n3; //
	n = n1 || n2;
	cout << "Clipped Signal:" << n << endl << endl;
	// Add the code for other logical gates..
	cout << "===== Test of Logical gate \" and \" =====" << endl << endl;

	n1.fire(0);
	n2.fire(1);
	cout << "If a=0 and b=1 then a&b == " << n3; //
	n = n1 && n2;
	cout << "Clipped Signal:" << n << endl << endl;

	n1.fire(1);
	n2.fire(1);
	cout << "If a=1 and b=1 then a&b == " << n3;
	n = n1 && n2;
	cout << "Clipped Signal:" << n << endl << endl;

	n1.fire(0);
	n2.fire(0);
	cout << "If a=0 and b=0 then a&b == " << n3; //
	n = n1 && n2;
	cout << "Clipped Signal:" << n << endl << endl;
	cout << "===== Test of Logical gate \" not \" =====" << endl << endl;
	n1.fire(0);
	n = !n1;
	cout << " If a=0 then ~a ==" << n3 << "Clipped Signal:" << n << endl
			<< endl;
	n1.fire(1);
	n = !n1;
	cout << " If a=1 then ~a ==" << n3 << "Clipped Signal:" << n << endl
			<< endl;
// Copy paste the Test Code for xnor gate here.
	cout << "===== Test of Logical gate \"xnor\"   =====" << endl << endl;



	// Test of ComplexNeuron

	ComplexNeuron x0(2, 0);
	ComplexNeuron x1(1, 0); // x1 as input
	ComplexNeuron x2(0, 0); // x2 as input
	ComplexCumulativeNeuron x3(1, 3, 1);
	ComplexCumulativeNeuron x4(0, 3, 1);
	ComplexNeuron x6(3, 3, 1);
	ComplexCumulativeNeuron x5(6, 2, 1);
	// layer-1 neurons
	x0.attach(-30,&x3);
	x0.attach(10,&x4);
	x1.attach(20,&x3);
	x1.attach(-20,&x4);
	x2.attach(20,&x3);
	x2.attach(-20,&x4);
	// layer-2 neurons
	x6.attach(-10,&x5);
	x3.attach(20,&x5);
	x4.attach(20,&x5);
	x0.fire(1);
	x6.fire(1);
	// Lets start checking the system by calling the fire method of input neurons
	x1.fire(0);
	x2.fire(1);
	cout<< "If a=0 and b=1 then  ̃(aˆb) == " << x5;
	n = x1 | x2;
	cout << "Clipped Signal:" << n << endl << endl;
	x1.fire(1);
	x2.fire(1);
	cout<< "If a=1 and b=1 then  ̃(aˆb) == " << x5;
	n = x1 | x2;
	cout << "Clipped Signal:" << n << endl << endl;
	x1.fire(1);
	x2.fire(0);
	cout<< "If a=1 and b=0 then  ̃(aˆb) == " << x5;
	n = x1 | x2;
	cout << "Clipped Signal:" << n << endl << endl;
	x1.fire(0);
	x2.fire(0);
	cout<< "If a=0 and b=0 then  ̃(aˆb) == " << x5; //
	n = x1 | x2;
	cout << "Clipped Signal:" << n << endl << endl;
	return 0;
}
