#include <iostream>
#include <vector>
#include <cmath>
#include "Point.h"
using namespace std;
class Neuron{
protected:
	Point position_;
	double signal_;
	double attenuation;
	Neuron **neighbours;
	int connected;
public:
	Neuron(double x , double y , double att=1):position_(x,y){
		attenuation=att;
		connected=0;
		neighbours = new Neuron * [100];
	}
	void accumulate(double received_signal){
		signal_=received_signal*attenuation;}
	Point position(){
		return position_;
	}
	double signal(){
		return signal_;}

	void fire(double rs){
		accumulate(rs);
		propagate();
	}
	void propagate(){
		for(int index=0;index<connected;index++){
				neighbours[index]->fire(signal_);
			}
	}
	void operator += (Neuron *N){
		neighbours[connected++]=N;}

	void display() {
		if(connected!=0){
		cout<<"The neuron at position "<<position_<<"with an attenuation factor of "<< attenuation <<" is connected to following ("<<connected<<") neuron(s):";
		cout<<endl;
		for(int i=0;i<connected;i++){
			cout<<"Neuron at position "<<neighbours[i]->position_<<endl;
		}}
		else{
			cout<<"The neuron at position"<< position_<<" with an attenuation factor of "<<attenuation <<" is not connected to any neuron."<<endl;
		}
	}
};
ostream& operator << (ostream &out ,Neuron &N){
	N.display();
	return out;
}
int main ()
{
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

  Neuron neuron4(0, 0, 0.75);
  cout << neuron4 << endl;
  neuron4.fire(10);
  neuron4.fire(10);
  cout << "Signal :" << endl;
  cout << neuron4.signal() << endl;

  //Copy Paste the test code of Logical Gate OR
  cout << "===== Test of Logical gate \"or\"   =====" << endl << endl;

  // Add the code for other logical gates..
  cout << "===== Test of Logical gates \"And, etc\"   =====" << endl << endl;


 // Copy paste the Test Code for xnor gate here.
  cout << "===== Test of Logical gate \"xnor\"   =====" << endl << endl;

  return 0;
}
