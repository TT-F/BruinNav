// The main.cpp you can use for testing will replace this file soon.

#include <iostream>
#include <string>
#include <cassert>
#include <chrono>
#include "MyMap.h"
#include "provided.h"

using namespace std; //NCO
void foo()
{
	MyMap<string, double> nameToGPA; // maps student name to GPA
									 // add new items to the binary search tree-based map
	nameToGPA.associate("Carey", 3.5); // Carey has a 3.5 GPA
	nameToGPA.associate("David", 3.99); // David beat Carey
	nameToGPA.associate("Abe", 3.2); // Abe has a 3.2 GPA
	double* davidsGPA = nameToGPA.find("David");
	if (davidsGPA != nullptr)
		*davidsGPA = 1.5; // after a re-grade of David’s exam
	nameToGPA.associate("Carey", 4.0); // Carey deserves a 4.0
									   // replaces old 3.5 GPA
	double* lindasGPA = nameToGPA.find("Abe");
	if (lindasGPA == nullptr)
		cout << "Linda is not in the roster!" << endl;
	else
		cout << "Linda’s GPA is: " << *lindasGPA << endl;
}
void testMyMap()
{
	MyMap<string, double> nameToGPA; // maps student name to GPA
									 // add new items to the binary search tree-based map
	assert(nameToGPA.size() == 0);
	nameToGPA.associate("Carey", 3.5); // Carey has a 3.5 GPA
	nameToGPA.associate("David", 3.99); // David beat Carey
	assert(nameToGPA.size() == 2);
	nameToGPA.associate("Abe", 3.2); // Abe has a 3.2 GPA

	assert(*(nameToGPA.find("Carey")) == 3.5);
	assert(*(nameToGPA.find("David")) == 3.99);
	assert(*(nameToGPA.find("Abe")) == 3.2);

	double* davidsGPA = nameToGPA.find("David");
	if (davidsGPA != nullptr)
		*davidsGPA = 1.5; // after a re-grade of David’s exam

	assert(*(nameToGPA.find("David")) == 1.5);

	nameToGPA.associate("Carey", 4.0); // Carey deserves a 4.0
									   // replaces old 3.5 GPA

	assert(*(nameToGPA.find("Carey")) == 4.0);

	double* lindasGPA = nameToGPA.find("Linda");
	assert(lindasGPA == nullptr);

	/*if (lindasGPA == nullptr)
	cout << "Linda is not in the roster!" << endl;
	else
	cout << "Linda’s GPA is: " << *lindasGPA << endl;*/

	nameToGPA.clear();
	assert(nameToGPA.size() == 0);
	assert(nameToGPA.find("Carey") == nullptr);
	assert(nameToGPA.find("David") == nullptr);
	assert(nameToGPA.find("Abe") == nullptr);

	cout << "MyMap passed all tests" << endl;
}

void testMaps()
{
	MapLoader m;
	chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
	assert(m.load("C:/Users/Steven Shi/Source/Repos/Project4/Project4/mapdata.txt"));
	chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
	cout << "Time to load data: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << "ms" << endl;
	StreetSegment s;
	m.getSegment(4, s);
	assert(s.streetName == "13th Helena Drive" && s.segment.start.longitudeText == "-118.4798135");

	cout << "MapLoader passed all tests" << endl;


	AttractionMapper am;
	chrono::high_resolution_clock::time_point t3 = chrono::high_resolution_clock::now();
	am.init(m);
	chrono::high_resolution_clock::time_point t4 = chrono::high_resolution_clock::now();
	cout << "Time to init AttractionMapper:" << chrono::duration_cast<chrono::milliseconds>(t4 - t3).count() << "ms" << endl;
	GeoCoord gc;
	assert(am.getGeoCoord("Northwest Campus Auditorium", gc) && gc.latitudeText == "34.0718231");
	assert(am.getGeoCoord("Sproul Landing", gc) && gc.longitudeText == "-118.4505665");

	cout << "AttractionMapper passed all tests" << endl;


	SegmentMapper sm;
	chrono::high_resolution_clock::time_point t5 = chrono::high_resolution_clock::now();
	sm.init(m);
	chrono::high_resolution_clock::time_point t6 = chrono::high_resolution_clock::now();
	cout << "Time to init SegmentMapper:" << chrono::duration_cast<chrono::milliseconds>(t6 - t5).count() << "ms" << endl;
	GeoCoord lookMeUp("34.0692072", "-118.4066994");
	std::vector<StreetSegment> vecOfAssociatedSegs(sm.getSegments(lookMeUp));
	if (vecOfAssociatedSegs.empty())
	{
		cout << "Error - no segments found matching this coordinate\n";
		return;
	}
	cout << "Here are all the segments associated with your coordinate:" << endl;
	for (auto s : vecOfAssociatedSegs)
	{
		cout << "Segment’s street: " << s.streetName << endl;
		cout << "Segment’s start lat/long: " << s.segment.start.latitudeText << ", " <<
			s.segment.start.longitudeText << endl; cout << "Segment’s end lat/long: " << s.segment.end.latitudeText << ", " <<
			s.segment.end.longitudeText << endl; cout << "This segment has " << s.attractions.size() <<
			" attractions on it." << endl;
	}
	cout << "All tests passed" << endl;
}

void testNav()
{
	Navigator nv;
	nv.loadMapData("C:/Users/Steven Shi/Source/Repos/Project4/Project4/mapdata.txt");
	vector<NavSegment> segs;
	//cerr<< nv.navigate("Los Angeles Fire Department Fire Station 92", "UCLA Police Department", segs);
	//cerr << nv.navigate( "Beverly Hills Marriott", "Nuart Theatre", segs);
	//cerr << nv.navigate("Beverly Hills Marriott", "De Neve Plaza", segs);
	nv.navigate("Brentwood Country Mart", "Los Angeles Fire Department Fire Station 58", segs);

	//cerr << nv.navigate(  "Nuart Theatre", "De Neve Plaza", segs);
	//cerr << nv.navigate("Nuart Theatre", "Beverly Hills Marriott", segs);
	//cerr << nv.navigate("Beverly Hills Marriott", "The Playboy Mansion", segs);
	//cerr << nv.navigate("Nuart Theatre", "Los Angeles Fire Department Fire Station 92", segs);
	//cerr << nv.navigate("1061 Broxton Avenue", "Headlines!", segs);
	double distance = 0;
	for (size_t i = 0; i < segs.size(); i++)
	{
		if (segs[i].m_command == NavSegment::PROCEED)
		{
			distance += segs[i].m_distance;
			cout << "Proceed " << segs[i].m_direction << " " << segs[i].m_distance << " miles on " << segs[i].m_streetName << endl;
		}
		else
			cout << "Turn " << segs[i].m_direction << " onto " << segs[i].m_streetName << endl;
	}
	cout << distance << " km in total." << endl;
	cout << "Passed" << endl;
}



int main()
{
	//foo();
	//testMyMap();
	//testMaps();
	testNav();

}
