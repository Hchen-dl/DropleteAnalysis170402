#include "WatershedAlgorithm_3.h"

int main(Mat src,std::string imgName )
{
	WatershedAlgorithm watershedAlgorithm;
	watershedAlgorithm.run(src, imgName);
}