#include <Eigen/Dense>
#include <vector>

class myKINEMATICS {

public:
	Eigen::Matrix4d T_E;
	Eigen::Matrix3d R_E;
	Eigen::Vector3d e_0, e_1, e_2, e_3, p_E;
	Eigen::Matrix<double, 6, 7> Jb, Jg;

	void getFK()
	{
		// T_E = ;
	}

	void getJb()
	{
		//  = ;
	}

	void getJg()
	{
		//  = ;
	}


	myKINEMATICS() {// default constructor
		//e_0.
	}


};