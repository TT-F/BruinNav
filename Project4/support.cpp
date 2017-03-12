#include "support.h"
#include<cmath>
#include<string>


support::support()
{
}


support::~support()
{
}

std::string geo_to_num(GeoCoord input_geo)
{
	std::string output; 
	output += input_geo.latitudeText;
	output += input_geo.longitudeText;
	return output; 
}
