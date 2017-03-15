#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <chrono>
#include <list>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <stack>
using namespace std;

class NavigatorImpl
{
public:
	NavigatorImpl();
	~NavigatorImpl();
	bool loadMapData(string mapFile);
	NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
	AttractionMapper am;
	SegmentMapper sm;
	MapLoader ml;
	vector<GeoCoord> surrond_cord(const GeoCoord& start) const;
	string angle_calculator(const GeoCoord& a, const GeoCoord& b);
	bool reconstruct_path(GeoCoord& current, MyMap<GeoCoord, GeoCoord>& prev,  vector<NavSegment>& direction) const;
	struct node
	{
		node() {};
		node(GeoCoord geo, std::string streetname) : node_geo(geo), street_name(streetname)
		{};
		bool operator== (const node& a)
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
		}

		GeoCoord node_geo;
		std::string street_name;
	};
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	cerr << "entering loading " << endl;
	if (ml.load(mapFile))
	{
		cerr << "lm check point 1" << endl;
		am.init(ml);
		sm.init(ml);
		return true;
	}
	else
		return false;
}

vector<GeoCoord> NavigatorImpl::surrond_cord(const GeoCoord& input) const
{
	vector<StreetSegment> streets = sm.getSegments(input);
	vector<GeoCoord> cord;
	for (unsigned i = 0; i < streets.size(); i++)
	{

		if (!(input == streets[i].segment.end))
			cord.push_back(streets[i].segment.end);
		if (!(input == streets[i].segment.start))
			cord.push_back(streets[i].segment.start);
		for (unsigned k = 0; k < streets[i].attractions.size(); k++)
			if (!(input == streets[i].attractions[k].geocoordinates))
				cord.push_back(streets[i].attractions[k].geocoordinates);
	}
	return cord;
}

string NavigatorImpl::angle_calculator(const GeoCoord & a, const GeoCoord & b)
{
	GeoSegment temp(a, b);
	double ang = angleOfLine(temp);
	if (ang >= 0 && ang <= 22.5)
		return "east";
	if (ang >= 22.5 && ang <= 67.5)
		return "northeast";
	if (ang >= 67.5 && ang <= 112.5)
		return "north";
	if (ang >= 112.5 && ang <= 157.5)
		return "northwest";
	if (ang >= 157.5 && ang <= 202.5)
		return "west";
	if (ang >= 202.5 && ang <= 247.5)
		return "southwest";
	if (ang >= 247.5 && ang <= 292.5)
		return "south";
	if (ang >= 292.5 && ang <= 337.5)
		return "southeast";
	else if (ang >= 337.5 && ang <= 360)
		return "east";

}

bool NavigatorImpl::reconstruct_path(GeoCoord & current, MyMap<GeoCoord, GeoCoord>& prev, vector<NavSegment>& direction) const
{
	stack<GeoCoord> geo_hld;
	GeoCoord* previous_cord = &current;

	while (previous_cord != nullptr)
	{
		cerr << previous_cord->latitudeText << "," << previous_cord->longitudeText << endl;

		geo_hld.push(*previous_cord);
		previous_cord = prev.find(*previous_cord);
	}
	


	return true;
}


NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	////chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

	GeoCoord start_cord, end_cord;
	if (!am.getGeoCoord(start, start_cord))
		return NAV_BAD_SOURCE;
	if (!am.getGeoCoord(end, end_cord))
		return NAV_BAD_DESTINATION;


	vector<GeoCoord> openset;
	MyMap<GeoCoord, string> closedset;
	//vector<GeoCoord> closedset;

	openset.push_back(start_cord);

	MyMap<GeoCoord, GeoCoord> prevnode;

	MyMap<GeoCoord, double> gScore;
	gScore.associate(start_cord, 0);
	MyMap<GeoCoord, double> fScore;
	fScore.associate(start_cord, distanceEarthKM(start_cord, end_cord));
	//int count = 0;
	while (!openset.empty())
	{
		//cerr << "========================WHILE LOOP=====================" << endl;
		unsigned delete_index = 0;
		GeoCoord current = openset[0];
		double temp_lowest_f_score = *fScore.find(openset[0]);
		for (int i = 0; i < openset.size();i++)
		{
			//cerr << "========================1st FOR LOOP=====================" << endl;
			double temp_fScore = *fScore.find(openset[i]);
			if (temp_fScore < temp_lowest_f_score)	
				temp_lowest_f_score = temp_fScore;
			if (temp_fScore < temp_lowest_f_score)
				delete_index = i;
		}

		current = openset[delete_index];
		if (current == end_cord)
		{
			if (reconstruct_path(end_cord, prevnode, directions))
				/*chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
				cout << "Time to find route: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << "ms" << endl;*/
				return NAV_SUCCESS;
		}
		openset.erase(openset.begin() + delete_index);
		closedset.associate(current, "abc");

		vector<GeoCoord> surround = surrond_cord(current);
		
		for (int i = 0; i < surround.size();i++)
		{

			if (closedset.find(surround[i]) != nullptr)

				continue;
			
			double tent_gScore = *gScore.find(current) + distanceEarthKM(current, surround[i]);
			if (!geo_included(surround[i], openset))				
				openset.push_back(surround[i]);
			
			else if (tent_gScore >= *gScore.find(surround[i]))
				continue;

			prevnode.associate(surround[i], current);
			gScore.associate(surround[i], tent_gScore);
			fScore.associate(surround[i], *gScore.find(surround[i]) + distanceEarthKM(surround[i], end_cord));

		}
		
	}
	
	return NAV_NO_ROUTE;


}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
	m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
	delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
	return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
	return m_impl->navigate(start, end, directions);
}
