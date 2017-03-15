#include "support.h"
#include "provided.h"
#include <iomanip>
#include <sstream>
#include<cmath>
#include<string>
using namespace std;

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
//
//double heuristic_est_dis(GeoCoord start, GeoCoord end)
//{
//	double x = start.latitude - end.latitude;
//	double y = start.longitude - end.longitude;
//	double dis = sqrt(pow(x,2)+pow(y,2));
//	return dis; 
//}

bool operator< (const GeoCoord& Geo_1, const GeoCoord& Geo_2)
{
	return (Geo_1.latitude < Geo_2.latitude );
}

bool operator> (const GeoCoord& Geo_1, const GeoCoord& Geo_2)
{
	return (Geo_1.latitude > Geo_2.latitude );
}

bool operator== (const GeoCoord& Geo_1, const GeoCoord& Geo_2)
{
	return (Geo_1.latitude == Geo_2.latitude && Geo_1.longitude == Geo_2.longitude);
}
//
//bool operator== (const StreetSegment& st_1, const StreetSegment& st_2)
//{
//	if (st_1.segment.start == st_2.segment.start && st_1.segment.end == st_2.segment.end)
//		return true;
//	if (st_1.segment.end == st_2.segment.start && st_1.segment.start == st_2.segment.end)
//		return true;
//	return false;
//}
//
//bool operator< (const StreetSegment& st_1, const StreetSegment& st_2)
//{
//	return(st_1.segment.start < st_2.segment.start );
//}
//bool operator> (const StreetSegment& st_1, const StreetSegment& st_2)
//{
//	return(st_1.segment.start > st_2.segment.start );
//}
//
//void copy(Streetnode& sn, const StreetSegment & input)
//{
//	sn.attractions = input.attractions;
//	sn.segment = input.segment;
//	sn.streetName = input.streetName;
//	double lati = (input.segment.start.latitude+input.segment.end.latitude)*0.5;
//	double lonti = (input.segment.start.longitude + input.segment.end.longitude)*0.5;
//	sn.middle_loc.latitude = lati;
//	sn.middle_loc.longitude = lonti;
//	std::ostringstream strs;
//	strs << fixed << setprecision(7) << sn.middle_loc.latitude;
//	sn.middle_loc.latitudeText = strs.str();
//	std::ostringstream strs1;
//	strs1 << fixed << setprecision(7) << sn.middle_loc.longitude;
//	sn.middle_loc.longitudeText = strs1.str();
//
//}
//
//void vector_copy(std::vector<Streetnode>& sn, const std::vector<StreetSegment>& ss)
//{
//	if (!sn.empty())
//		sn.clear();
//	for (int i = 0; i < ss.size();i++)
//	{
//		Streetnode k; 
//		copy(k, ss[i]);
//		sn.push_back(k);
//	}
//}

bool geo_included(const GeoCoord& coord, const std::vector<GeoCoord>& vec)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (vec[i] == coord)
			return true;
	}
	return false;
}

//Streetnode& Streetnode::operator= (const StreetSegment& ss)
//{
//
//	streetName = ss.streetName;
//	segment = ss.segment;
//	attractions = ss.attractions;
//	return *this;
//}