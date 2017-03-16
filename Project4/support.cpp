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
bool operator==(const node & b, const node & a)
{
	if (b.node_geo == a.node_geo)
		return true;
	else
		return false;
}
bool operator<(const node & a, const node & b)
{
	if (a.f_value < b.f_value)
		return true;
	else
		return false;
}
bool operator>(const node & a, const node & b)
{
	if (a.f_value > b.f_value)
		return true;
	else
		return false;
}

//std::string angle_calculator(const node & a, const node & b)
//{
//	GeoSegment temp(a.node_geo, b.node_geo);
//	double ang = angleOfLine(temp);
//	if (ang >= 0 && ang <= 22.5)
//		return "east";
//	else if (ang >= 22.5 && ang <= 67.5)
//		return "northeast";
//	else if (ang >= 67.5 && ang <= 112.5)
//		return "north";
//	else if (ang >= 112.5 && ang <= 157.5)
//		return "northwest";
//	else if (ang >= 157.5 && ang <= 202.5)
//		return "west";
//	else if (ang >= 202.5 && ang <= 247.5)
//		return "southwest";
//	else if (ang >= 247.5 && ang <= 292.5)
//		return "south";
//	else if (ang >= 292.5 && ang <= 337.5)
//		return "southeast";
//	else if (ang >= 337.5 && ang <= 360)
//		return "east";
//	else
//		return "error";
//}
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



//Streetnode& Streetnode::operator= (const StreetSegment& ss)
//{
//
//	streetName = ss.streetName;
//	segment = ss.segment;
//	attractions = ss.attractions;
//	return *this;
//}