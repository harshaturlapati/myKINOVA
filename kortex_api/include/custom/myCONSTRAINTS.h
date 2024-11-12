#include <Eigen/Dense>
#include <vector>

class myCONSTRAINTS {

public:
	myKINEMATICS KIN;
	Eigen::Matrix4d T_Pi, T_d, T_E_d;
	Eigen::VectorXd W_cstr, W_task;



	myCONSTRAINTS() {// default constructor

	}


};