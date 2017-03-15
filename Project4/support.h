#ifndef Sup_h
#define Sup_h
#include "provided.h"
#include <string>
#include <vector>

class support
{
public:
	support();
	~support();
	
};
std::string geo_to_num(GeoCoord input_geo);
//double heuristic_est_dis(GeoCoord start, GeoCoord end);
bool operator< (const GeoCoord& Geo_1, const GeoCoord& Geo_2);
bool operator> (const GeoCoord& Geo_1, const GeoCoord& Geo_2);
bool operator== (const GeoCoord& Geo_1, const GeoCoord& Geo_2);
//bool operator== (const StreetSegment& st_1, const StreetSegment& st_2);
//bool operator< (const StreetSegment& st_1, const StreetSegment& st_2);
//bool operator> (const StreetSegment& st_1, const StreetSegment& st_2);
//
//struct Streetnode : public StreetSegment
//{
//	StreetSegment* prevStreet;
//	GeoCoord middle_loc;
//	StreetSegment* nextStreet;
//	//Streetnode& operator= (const StreetSegment& ss);
//};
//
//void copy(Streetnode& sn, const StreetSegment& input);
//void vector_copy(std::vector<Streetnode>& sn, const std::vector<StreetSegment>& ss);

struct node
{
	node() {};
	node(GeoCoord geo, std::string streetname) : node_geo(geo), street_name(streetname)
	{};
	/*bool operator== (const node& a)
	{
		if (node_geo == a.node_geo && street_name == a.street_name)
			return true;
		else
			return false;
	}
	bool operator< (const node& a)
	{
		if (node_geo < a.node_geo)
			return true;
		else
			return false;
	}
	bool operator> (const node& a)
	{
		if (node_geo > a.node_geo)
			return true;
		else
			return false;
	}*/

	GeoCoord node_geo;
	std::string street_name;
};

bool operator==(const node& b, const node& a);


bool operator< (const node& a, const node& b);


bool operator> (const node& a, const node& b);



#endif // !Sup_h

