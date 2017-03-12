#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	//vector<StreetSegment> seg_map;
	MyMap<string, vector<StreetSegment*>> My_segment;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
	My_segment.clear();
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	int ml_size = ml.getNumSegments();
	//cerr << "entring segament mapper init" << endl;
	for (int i = 0; i < ml_size;i++)
	{
		//cerr << "entering for loop " << i << " times" << endl;
		vector<StreetSegment*> segments_loader;
		StreetSegment *st_ptr = new StreetSegment;

		vector<GeoCoord> att_geo;
		ml.getSegment(i, *st_ptr);
		segments_loader.push_back(st_ptr);
		//cerr << "SGM for loop check point A" << endl;
		//check if My_segment has same coridnate 
		//copy and add if there are 
		//otherwise, use regular association 
		string numst = geo_to_num(st_ptr->segment.start);
		string numed = geo_to_num(st_ptr->segment.end);
		//cerr << "SGM for loop check point B" << endl;
		if (My_segment.find(numst) != nullptr)
		{
			segments_loader = *My_segment.find(numst);
			segments_loader.push_back(st_ptr);
			My_segment.associate(numst, segments_loader);
		}
		else
			My_segment.associate(numst, segments_loader);
		if (My_segment.find(numed) != nullptr)
		{
			segments_loader = *My_segment.find(numed);
			segments_loader.push_back(st_ptr);
			My_segment.associate(numed, segments_loader);
		}
		else
			My_segment.associate(numed, segments_loader);
		//Attraction
		int att_size = st_ptr->attractions.size();
		int walk = 0;
		//	cerr << "SGM for loop check point C" << endl;
		while (walk < att_size)
		{
			//cerr << "SGM While---------- loop check point A" << endl;
			string loc = geo_to_num(st_ptr->attractions[walk].geocoordinates);
			if (My_segment.find(loc) != nullptr)
			{
				segments_loader = *My_segment.find(loc);
				segments_loader.push_back(st_ptr);
				My_segment.associate(loc, segments_loader);
			}
			else
				My_segment.associate(loc, segments_loader);
			walk++;
		}
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	string loc = geo_to_num(gc);
	vector<StreetSegment> segments;
	const vector<StreetSegment*>* ptr = My_segment.find(loc);
	if (ptr != nullptr)
	{
		for (int i = 0;i < ptr->size();i++)
			segments.push_back(*(*ptr)[i]);
	}
	return segments;
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
