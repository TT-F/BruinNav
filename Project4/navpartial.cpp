MyMap<StreetSegment, StreetSegment> closedset;
	//list<StreetSegment> closedset; 
	vector<StreetSegment>* openset = new vector<StreetSegment>;
	cerr << "entering nav" << endl;
	//====================validating entry==================
	GeoCoord st_geo; //camefrom 
	GeoCoord end_geo; //destination 
	if (!am.getGeoCoord(start, st_geo))
		return NAV_BAD_SOURCE;
	if (!am.getGeoCoord(end, end_geo))
		return NAV_BAD_DESTINATION;

	//====================find a route======================

	//-------------------score------------------------------
	double g_score = 0;
	double h_score = heuristic_est_dis(st_geo,end_geo);
	double f_score = g_score+h_score; 

	//-----------------------starting segament----------------
	vector<StreetSegment> st_sm_vec = sm.getSegments(st_geo); //getting all segments related to the starting geo 
																//there should only be one segment; 
	StreetSegment* st_sm = &st_sm_vec.front();
	//closedset.push_back(*st_sm);
	StreetSegment* prev_seg = st_sm; //previous street segement 

	vector<StreetSegment> st_ass_1 = sm.getSegments(st_sm->segment.end); //street connected to the end of starting segement
	vector<StreetSegment> st_ass_2 = sm.getSegments(st_sm->segment.start); //streeets connected to the start of starting segement 
	cerr << "nav check point 1" << endl;
	openset->insert(openset->end(), st_ass_1.begin(), st_ass_1.end()); //putting all street segements into oepnset 
	openset->insert(openset->end(), st_ass_2.begin(), st_ass_2.end());
	//-----------------delete starting street---------------
	for (int i = 0; i < openset->size();i++)
	{
		StreetSegment* temp = &openset->at(i);
		//if(temp->segment.start == st_sm->segment.end && temp->segment.end == st_sm->segment.start) //if the street segement has oppostie starting/ending cord as starting segemetns 
		//	openset->erase(openset->begin() + i);
		//if (temp->segment.end == st_sm->segment.end && temp->segment.start == st_sm->segment.start) //if the street segement has same starting/ending cord as starting segemetns 
		//	openset->erase(openset->begin() + i);
		if (*temp == *st_sm)
		{
			openset->erase(openset->begin() + i);
			cerr << "i delete" << endl;
		}
			
	}

	//-----------------------------------------------------
	cerr << "nav check point 2" << endl;
	while (!openset->empty())
	{
		cerr << "enterting while loop" << endl;
		cerr << "f score " << f_score << endl;
		StreetSegment* temp_sm = new StreetSegment;
		StreetSegment* curr_shortest_sm = new StreetSegment;
		int delete_position = -1;
		for (int i = 0; i < openset->size();i++)
		{
			cerr << "enterting for openset loop " << i << " loc" << openset->at(i).streetName<< endl;
			*temp_sm = openset->at(i);
			GeoCoord sm_geo = temp_sm->segment.end; //check the starting cord. 
			double temp_h_score = heuristic_est_dis(sm_geo, end_geo);
			double temp_f_score = g_score + temp_h_score;
			cerr << "temp f score " << temp_f_score << endl;
			if (temp_f_score < f_score)
			{
				f_score = temp_f_score;
				curr_shortest_sm = temp_sm;
				delete_position = i;
			}				
			if (sm_geo == end_geo) 
				return NAV_SUCCESS;
		}		
		cerr << "check point leaving /delet pos " << delete_position << endl;



		closedset.associate(*prev_seg, *curr_shortest_sm);
		if (delete_position >= 0)
		{
			openset->erase(openset->begin() + delete_position);  //??????????????Is the iterator correct???
		}
		
		vector<StreetSegment> st_ass_csm = sm.getSegments(curr_shortest_sm->segment.end);
		vector<StreetSegment> st_ass_csm_1 = sm.getSegments(curr_shortest_sm->segment.start);
		st_ass_csm.insert(st_ass_csm.end(), st_ass_csm_1.begin(), st_ass_csm_1.end());
		for (int i = 0; i < st_ass_csm.size(); i++)
			if (*curr_shortest_sm == st_ass_csm[i])
				st_ass_csm.erase(st_ass_csm.begin() + i);


		for (int i = 0; i < st_ass_csm.size();i++)
		{
			cerr << "enterting for se_ass_csm loop " << i << " loc" << st_ass_csm[i].streetName << endl;
			if (closedset.find(st_ass_csm[i]))
				continue;
			double csm_g_score = heuristic_est_dis(st_geo, curr_shortest_sm->segment.end);
			cerr << "csm g score " << csm_g_score << endl;
			double tent_g_score = csm_g_score + heuristic_est_dis(curr_shortest_sm->segment.end,st_ass_csm[i].segment.end);
			cerr << "tent g score " << tent_g_score << endl;
			bool ten_is_better = false; 
			if (openset->size() == 0)
				openset->push_back(st_ass_csm[i]);
			else
			{
				for (int k = 0; k < openset->size();k++)
				{
					cerr << "entering openset loop================== in st ass csm" << endl;
					if (st_ass_csm[i] == openset->at(k))
						if (tent_g_score < heuristic_est_dis(st_geo, st_ass_csm[i].segment.end))
							ten_is_better = true;
						else
							ten_is_better = false;
					else if (k == openset->size() - 1)
						openset->push_back(st_ass_csm[i]);
				}
			}
			
			if (ten_is_better = true)
			{
				prev_seg = curr_shortest_sm;
				g_score = tent_g_score;
				h_score = heuristic_est_dis(st_ass_csm[i].segment.end,end_geo);
				cerr << "true h score " << h_score << endl;
				f_score = g_score + h_score;
				
			}
		}
	}
	return NAV_NO_ROUTE;



	cerr << "entering navigation" << endl;

	//====================validating entry==================
	GeoCoord st_geo; //camefrom 
	GeoCoord end_geo; //destination 
	if (!am.getGeoCoord(start, st_geo))
		return NAV_BAD_SOURCE;
	if (!am.getGeoCoord(end, end_geo))
		return NAV_BAD_DESTINATION;

	vector<StreetSegment> st_sm_vec = sm.getSegments(st_geo);
	Streetnode start_street;
	copy(start_street, st_sm_vec[0]);
	start_street.prevStreet = &st_sm_vec[0];

	vector<StreetSegment> end_sm_vec = sm.getSegments(end_geo);
	Streetnode end_street;
	copy(end_street, end_sm_vec[0]);
	end_street.prevStreet = &end_sm_vec[0];

	cerr << "finish validating entry and converting entry" << endl;

	//===========acuatlly using start/end =============

	GeoCoord start_loc = start_street.segment.end;

	double temp_x = (end_street.segment.start.latitude + end_street.segment.end.latitude)*0.5;
	double temp_y = (end_street.segment.start.longitude + end_street.segment.end.longitude)*0.5;
	GeoCoord end_loc;
	end_loc.latitude = temp_x;
	end_loc.longitude = temp_y;

	cerr << "finish converting geo loc for start and end" << endl;
	//====================container======================

	vector<Streetnode> closedset;
	vector<Streetnode> openset;
	MyMap<GeoCoord, bool> has_evaluated;

	//==================finding route=====================
	//-------------------score------------------------------
	double g_score = 0;
	double h_score = heuristic_est_dis(start_loc, end_loc);
	double f_score = g_score + h_score;
	cerr << "finish calculating init score" << endl;
	//-----------------------starting segaments----------------
	vector_copy(openset, sm.getSegments(start_loc));
	for (int i = 0; i < openset.size();i++)
		openset[i].prevStreet = &start_street;


	cerr << "going to enter while loop/ A* alog." << endl;
	//-------------------A* alog.----------------------
	while (!openset.empty())
	{
		cerr << endl;
		cerr << "====================== while loop =======================" << endl;
		cerr << "f score " << f_score << endl;
		Streetnode current_shortest_node;
		int delete_pos = 0;
		GeoCoord segment_geo;
		for (int i = 0; i < openset.size();i++)
		{
			cerr << "======================1st for loop ======================= " << i << " loc" << openset[i].streetName << endl;
			segment_geo = openset[i].middle_loc;
			double temp_h_score = heuristic_est_dis(start_loc, segment_geo);
			double temp_f_score = g_score + temp_h_score;
			cerr << "---------------------------temp f score " << temp_f_score << endl;
			if (temp_f_score <= f_score)
			{
				f_score = temp_f_score;
				current_shortest_node = openset[i];
				delete_pos = i;
			}
			if (segment_geo == end_loc)
				return NAV_SUCCESS;
		}
		cerr << "check point leaving /delet pos " << delete_pos << endl;
		closedset.push_back(current_shortest_node);
		if (delete_pos >= 0)
			openset.erase(openset.begin() + delete_pos);
		has_evaluated.associate(current_shortest_node.middle_loc, false);
		//-----------------getting next segment--------------------
		GeoCoord temp_geo;
		if (current_shortest_node.segment.end == current_shortest_node.prevStreet->segment.end ||
			current_shortest_node.segment.end == current_shortest_node.prevStreet->segment.start)
			temp_geo = current_shortest_node.segment.start;
		else
			temp_geo = current_shortest_node.segment.end;
		vector<Streetnode> next_segments;
		vector_copy(next_segments, sm.getSegments(temp_geo));
		for (int i = 0;i < next_segments.size();i++)
			next_segments[i].prevStreet = &current_shortest_node;

		for (int i = 0; i < next_segments.size();i++)
		{
			cerr << "evaluating next segments " << endl;
			for (int j = 0; j < closedset.size();j++)
			{
				if (closedset[j].middle_loc == next_segments[i].middle_loc)
					continue;
			}


			double short_g_score = heuristic_est_dis(start_loc, current_shortest_node.middle_loc);
			double tent_g_score = short_g_score + heuristic_est_dis(current_shortest_node.middle_loc, next_segments[i].middle_loc);
			cerr << "---------------------------tent g score " << tent_g_score << endl;
			bool ten_is_better = false;
			if (openset.size() == 0)
				openset.push_back(next_segments[i]);
			else
			{
				for (int k = 0; k < openset.size();k++)
				{
					cerr << "======================3rd for loop ======================= " << k << endl;
					if (next_segments[i] == openset[k])
					{
						cerr << " heu g comp " << heuristic_est_dis(start_loc, next_segments[i].middle_loc) << endl;
						if (tent_g_score < heuristic_est_dis(start_loc, end_loc))
							ten_is_better = true;
						else
							ten_is_better = false;
						cerr << "ten's value" << ten_is_better << endl;
					}
					else if (k == (openset.size() - 1))
					{
						openset.push_back(next_segments[i]);
						cerr << "third loop pushed" << endl;
						break;
					}

				}
			}
			if (ten_is_better == true)
			{
				cerr << "tent g is better is true " << endl;
				current_shortest_node.nextStreet = &next_segments[i];
				g_score = tent_g_score;
				h_score = heuristic_est_dis(next_segments[i].middle_loc, end_loc);
				f_score = g_score + h_score;
			}
			/*if (has_evaluated.find(next_segments[i].middle_loc))
			{
			double short_g_score = heuristic_est_dis(start_loc, current_shortest_node.middle_loc);
			double tent_g_score = short_g_score + heuristic_est_dis(current_shortest_node.middle_loc, next_segments[i].middle_loc);
			cerr << "---------------------------tent g score " << tent_g_score << endl;
			if (!geo_included(next_segments[i].middle_loc, openset))
			openset.push_back(next_segments[i]);
			else if (tent_g_score >= heuristic_est_dis(start_loc, end_loc))
			{
			cerr << "NOT QUALIFIED" << endl;
			continue;
			}
			current_shortest_node.nextStreet = &next_segments[i];
			g_score = tent_g_score;
			h_score = heuristic_est_dis(next_segments[i].middle_loc, end_loc);
			f_score = g_score + h_score;
			}*/

		}

	}
