#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <chrono>
#include <list>
#include <string>
#include <vector>
#include <iostream>
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
		for (unsigned k = 0; k < streets[i].attractions.size(); k++)
			if (!(input == streets[i].attractions[k].geocoordinates))
				cord.push_back(streets[i].attractions[k].geocoordinates);
		if (!(input == streets[i].segment.end))
			cord.push_back(streets[i].segment.end);
		if (!(input == streets[i].segment.start))
			cord.push_back(streets[i].segment.start);		
	}
	return cord;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

	GeoCoord start_cord, end_cord;
	if (!am.getGeoCoord(start, start_cord))
		return NAV_BAD_SOURCE;
	if (!am.getGeoCoord(end, end_cord))
		return NAV_BAD_DESTINATION;

	
	vector<GeoCoord> openset; 
	vector<GeoCoord> closedset;

	openset.push_back(start_cord);

	MyMap<GeoCoord, GeoCoord> prevnode; 

	MyMap<GeoCoord, double> gScore;
	gScore.associate(start_cord, 0);
	MyMap<GeoCoord, double> fScore; 
	fScore.associate(start_cord, distanceEarthKM(start_cord, end_cord));

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
			{
				temp_lowest_f_score = temp_fScore;
				delete_index = i;
				current = openset[i];
			}			
		}
		//cerr << sm.getSegments(current).front().streetName << endl;
		if (current == end_cord)
		{
			return NAV_SUCCESS;
		}
		openset.erase(openset.begin() + delete_index);
		closedset.push_back(current);
		
		vector<GeoCoord> surround = surrond_cord(current);
		for (int i = 0; i < surround.size();i++)
		{
			//cerr << "========================2nd FOR LOOP=====================" << endl;
			if (geo_included(surround[i], closedset))
				continue;
			double tent_gScore = *fScore.find(current) + distanceEarthKM(current, surround[i]);
			if (!geo_included(surround[i], openset))
				openset.push_back(surround[i]);
			else if (tent_gScore >= *fScore.find(surround[i]))
				continue;

			prevnode.associate(current, surround[i]);
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
