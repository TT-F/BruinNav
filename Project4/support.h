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

bool geo_included(const GeoCoord& coord, const std::vector<GeoCoord>& vec);



#endif // !Sup_h

