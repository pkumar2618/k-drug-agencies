//
//  utility.hpp
//  k-drug-rings
//
//  Created by pawan kumar on 25/09/18.
//  Copyright © 2018 iit-delhi. All rights reserved.
//

#ifndef utility_hpp
#define utility_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <map>

using namespace std;
// the following function is usde to get the cnf for condition that given an edge in the graph people belong to same
// agency
void cnf_for_or_over_agencies(vector<int> edge, int agencies, int n_essential_bv, map<vector<int>,int>* ptr_agentAgency, map<int,int>* ptr_extra_bv_sat_code, vector<string>* ptr_all_clauses);
// the following function call is made to get the cnf for condtion that no to agencies are strict subsidiary of
// each other
void cnf_for_or_over_agents(vector<int> agency_pairs, int agents, int n_essential_bv, map<vector<int>,int>* ptr_agentAgency, map<int,int>* ptr_extra_bv_sat_code, vector<string>* ptr_all_clauses);
void cnf_for_or_over_agents_extra_member(vector<int> agency_pairs, int agents, int n_essential_bv, map<vector<int>,int>* ptr_agentAgency, map<int,int>* ptr_extra_bv_sat_code, vector<string>* ptr_all_clauses);
vector<string> split(string message, string delimiter);
void PrintGraph(map<vector<int>,int> graph);
void PrintGraph(map<vector<int>,int> graph, bool val);

#endif /* utility_hpp */
