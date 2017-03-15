#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <chrono>
//#include <list>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <stack>
#include <queue>
#include <functional>
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
	vector<node> surrond_cord(const node& input, const node& end) const;
	string angle_calculator(const node& a, const node& b) const;
	bool reconstruct_road(node& g1, node& g2, StreetSegment& s) const; 
	bool reconstruct_path(node& current, MyMap<node, node>& prev,  vector<NavSegment>& direction) const;
	bool geo_included(const node& coord, const std::vector<node>& vec) const;
	double GetAngleABC(const GeoCoord& a, const GeoCoord& b, const GeoCoord& c) const; 


	template<class T,class Container = std::vector<T>, class Compare = std::less<typename Container::value_type>
> class MyQueue : public std::priority_queue<T, Container, Compare>
	{
	public:
		typedef typename
			std::priority_queue<T,Container,Compare>::container_type::const_iterator const_iterator;

		bool find(const T&val) const
		{
			auto first = this->c.cbegin();
			auto last = this->c.cend();
			while (first != last) 
			{
				if (*first == val) return true;
				++first;
			}
			return false;
		}

		const_iterator find_it(const T&val) const
		{
			auto first = this->c.cbegin();
			auto last = this->c.cend();
			while (first != last) {
				if (*first == val) return first;
				++first;
			}
			return last;
		}

		const_iterator end() const
		{	
			auto last = this->c.cend();
			return last;
		}

		const_reference find_g(const T&val) const
		{	
			auto first = this->c.cbegin();
			auto last = this->c.cend();
			while (first != last) {
				if (*first == val) return *first;
				++first;
			}
			return *last;
		}
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

string directiont(const GeoSegment& line1, const GeoSegment& line2)
{
	if (angleBetween2Lines(line1, line2) > 180.0)
		return "right";
	else
		return "left";
	//return angleBetween2Lines(line1, line2) > 180.0 ? "right" : "left";
}

vector<node> NavigatorImpl::surrond_cord(const node & input, const node& end) const
{
	vector<StreetSegment> streets = sm.getSegments(input.node_geo);
	vector<node> cord;
	for (unsigned i = 0; i < streets.size(); i++)
	{

		if (!(input.node_geo == streets[i].segment.end))
		{
			node temp;
			temp.node_geo = streets[i].segment.end;
			//temp.street_name = streets[i].streetName;
			temp.g_value = input.g_value + distanceEarthMiles(input.node_geo, temp.node_geo);
			temp.f_value = temp.g_value + distanceEarthMiles(temp.node_geo, end.node_geo);
			cord.push_back(temp);
		}
		if (!(input.node_geo == streets[i].segment.start))
		{
			node temp;
			temp.node_geo = streets[i].segment.start;
			//temp.street_name = streets[i].streetName;
			temp.g_value = input.g_value + distanceEarthMiles(input.node_geo, temp.node_geo);
			temp.f_value = temp.g_value + distanceEarthMiles(temp.node_geo, end.node_geo);
			cord.push_back(temp);
		}	
		for (unsigned k = 0; k < streets[i].attractions.size(); k++)
			if (!(input.node_geo == streets[i].attractions[k].geocoordinates))
			{
				node temp;
				temp.node_geo = streets[i].attractions[k].geocoordinates;
				//temp.street_name = streets[i].streetName;
				temp.g_value = input.g_value + distanceEarthMiles(input.node_geo, temp.node_geo);
				temp.f_value = temp.g_value + distanceEarthMiles(temp.node_geo, end.node_geo);
				cord.push_back(temp);
			}
	}
	return cord;
}

string NavigatorImpl::angle_calculator(const node & a, const node & b) const
{
	GeoSegment temp(a.node_geo, b.node_geo);
	double ang = angleOfLine(temp);
	if (ang >= 0 && ang <= 22.5)
		return "east";
	else if (ang >= 22.5 && ang <= 67.5)
		return "northeast";
	else if (ang >= 67.5 && ang <= 112.5)
		return "north";
	else if (ang >= 112.5 && ang <= 157.5)
		return "northwest";
	else if (ang >= 157.5 && ang <= 202.5)
		return "west";
	else if (ang >= 202.5 && ang <= 247.5)
		return "southwest";
	else if (ang >= 247.5 && ang <= 292.5)
		return "south";
	else if (ang >= 292.5 && ang <= 337.5)
		return "southeast";
	else if (ang >= 337.5 && ang <= 360)
		return "east";

}

bool NavigatorImpl::reconstruct_road(node & a, node & b, StreetSegment & s) const
{
	vector<StreetSegment> ss1 = sm.getSegments(a.node_geo);
	vector<StreetSegment> ss2 = sm.getSegments(b.node_geo);
	//cerr << "successfully construct" << endl;
	for (unsigned i = 0; i < ss1.size();i++)
	{
		for (unsigned k = 0; k < ss2.size();k++)
		{

			if (ss1[i].streetName == ss2[k].streetName)
			{
				//cerr << "found" << endl;
				s = ss1[i];
				//cerr << "successfully return" << endl;
				return true;
			}
		}
	}
	return false; 
}

bool NavigatorImpl::reconstruct_path(node & current, MyMap<node, node>& prev, vector<NavSegment>& direction) const
{
	stack<node> geo_hld;
	node* previous_cord = &current;

	while (previous_cord != nullptr)
	{
		cerr << previous_cord->node_geo.latitudeText << "," << previous_cord->node_geo.longitudeText << endl;

		geo_hld.push(*previous_cord);
		previous_cord = prev.find(*previous_cord);
	}
	vector<NavSegment> nav;
	while (!geo_hld.empty())
	{
		/*node t1 = geo_hld.top();
		geo_hld.pop();
		if (geo_hld.empty())
			break;
		node t2 = geo_hld.top();
		
		string dir;
		string name; 
		double distance; 
		GeoSegment gse;
		vector<NavSegment> route;

		if (t1.street_name != t2.street_name)
		{
			geo_hld.pop();
			
			node t3 = geo_hld.top();
			geo_hld.push(t2);
			GeoSegment a;
			GeoSegment b;
			a.start = t1.node_geo;
			a.end = t2.node_geo;
			b.start = t2.node_geo;
			b.end = t3.node_geo;
			cerr << t1.street_name << endl << GetAngleABC(t1.node_geo,t2.node_geo,t3.node_geo) << endl << t2.street_name << " " << t3.street_name << endl << endl;;
			if (GetAngleABC(t1.node_geo, t2.node_geo, t3.node_geo) < 0)
				dir = "right";
			else
				dir = "left";
			name = t2.street_name;
			NavSegment at(dir, name);
			direction.push_back(at);
			name = t2.street_name;
			dir = angle_calculator(t1, t2);
			distance = distanceEarthKM(t1.node_geo, t2.node_geo);
			gse.start = t1.node_geo;
			gse.end = t2.node_geo;
			NavSegment att(dir, name, distance, gse);
			direction.push_back(att);
		}
		if (t1.street_name == t2.street_name)
		{
			name = t1.street_name;
			dir = angle_calculator(t1, t2);
			distance = distanceEarthKM(t1.node_geo, t2.node_geo);
			gse.start = t1.node_geo;
			gse.end = t2.node_geo;
			NavSegment at(dir, name, distance, gse);
			direction.push_back(at);
		}*/
		//cerr << "entering while loop" << endl;
		node g1 = geo_hld.top();
		geo_hld.pop();
		if (geo_hld.empty())
			break;
		node g2 = geo_hld.top();
		//cerr << "pass nodes " << endl;
		StreetSegment s;
		//cerr << "street segment " << endl;
		if (!reconstruct_road(g1, g2, s))
			return false;
		//cerr << "road found" << endl;
		string dir= angle_calculator(g1, g2);
		string name= s.streetName;
		double distance = distanceEarthMiles(g1.node_geo, g2.node_geo);
		GeoSegment gse = GeoSegment(g1.node_geo, g2.node_geo);
		NavSegment cs( dir,name , distance, gse);
		if (!nav.empty())
		{
			NavSegment ps = nav[nav.size() - 1];
			if (ps.m_streetName != cs.m_streetName)
			{
				NavSegment ts(directiont(ps.m_geoSegment, cs.m_geoSegment), cs.m_streetName);
				nav.push_back(ts);
			}
		}
		nav.push_back(cs);
	}
	direction = nav;
	
	return true;
}

bool NavigatorImpl::geo_included(const node & coord, const std::vector<node>& vec) const
{
	
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (vec[i].node_geo == coord.node_geo)
			return true;
	}
	return false;
}

double NavigatorImpl::GetAngleABC(const GeoCoord & a, const GeoCoord & b, const GeoCoord & c) const
{

	GeoCoord ab;
	ab.latitude = a.latitude - b.latitude;
	ab.longitude = a.longitude - b.longitude;
	GeoCoord cb;
	cb.latitude = c.latitude - b.latitude;
	cb.longitude = c.longitude - b.longitude;

	double dot= (ab.latitude*cb.latitude + ab.longitude*cb.longitude);
	double cross = (ab.latitude * cb.longitude - ab.longitude * cb.latitude);
	double alpha = atan2(cross, dot);
	return (double)floor(alpha * 180 / 3.1415926 + 0.5);
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	
	node start_cord, end_cord;
	if (!am.getGeoCoord(start, start_cord.node_geo))
		return NAV_BAD_SOURCE;
	if (!am.getGeoCoord(end, end_cord.node_geo))
		return NAV_BAD_DESTINATION;

	MyQueue<node, vector<node>, greater<node>> openset;
	vector<node> closeset; 
	start_cord.g_value = 0;
	start_cord.f_value = start_cord.g_value + distanceEarthMiles(start_cord.node_geo, end_cord.node_geo);
	start_cord.prev = nullptr;

	openset.push(start_cord);

	
	//=====================A* =================================
	while (!openset.empty())
	{		
		node current = openset.top();
		double lowest_f_score = current.f_value;
		if (current == end_cord)
		{
			//TODO***************
			node* ptr = &current;
			while (ptr->prev != nullptr)
			{
				cerr << ptr->node_geo.latitudeText << "," << ptr->node_geo.longitudeText << endl;
				ptr = ptr->prev;
			}
			
			return NAV_SUCCESS;
		}
		openset.pop();
		closeset.push_back(current);

		node evaluating;
		
		vector<node> evalu = surrond_cord(current, end_cord);
		for (int i = 0;i < evalu.size();i++)
		{
			if (geo_included(evalu[i], closeset))
				continue;
			if (!openset.find(evalu[i]))
			{
				evalu[i].prev = new node;
				*evalu[i].prev=closeset.back();
				openset.push(evalu[i]);
			}			
			else if (openset.find_it(evalu[i]) != openset.end())
				if (evalu[i].g_value >= openset.find_g(evalu[i]).g_value)
					continue;
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
