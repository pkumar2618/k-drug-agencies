//
//  utility.cpp
//  k-drug-rings
//
//  Created by pawan kumar on 25/09/18.
//  Copyright © 2018 iit-delhi. All rights reserved.
//

#include <iostream>
#include "utility.hpp"


using namespace std;

void cnf_for_or_over_agencies(vector<int> edge, int agencies, int n_essential_bv, map<vector<int>,int>* ptr_agentAgency, map<int,int>* ptr_extra_bv_sat_code, vector<string>* ptr_all_clauses)
{
    int extra_bv_start_key, extra_bv_end_key, extra_bv_last_op_key;
    
    int n_extra_bv = (*ptr_extra_bv_sat_code).size();
    if (n_extra_bv == 0)
    {
        n_extra_bv = n_essential_bv; //this triggers when there was no extra variable created.
    }
    else
    {
        n_extra_bv += n_essential_bv; // size of extra boolean variables plus the size of n_essential bv will
        //give the proper encoding
        
    }
    extra_bv_start_key = ++n_extra_bv;
    for(int k=1; k <= agencies; k++)
    {
        (*ptr_extra_bv_sat_code)[n_extra_bv] = n_extra_bv; //the extra boolean variables will have integer values above //the essential boolean variables.
        //first clause
        string clause = to_string((*ptr_agentAgency)[{edge[0],k}]);
        clause += " ";
        clause += to_string(-n_extra_bv);
        clause += " ";
        clause += to_string(0);
        (*ptr_all_clauses).push_back(clause);
        
        //second clause
        clause = to_string((*ptr_agentAgency)[{edge[1],k}]);
        clause += " ";
        clause += to_string(-n_extra_bv);
        clause += " ";
        clause += to_string(0);
        (*ptr_all_clauses).push_back(clause);
        
        //third clause
        clause = to_string(-(*ptr_agentAgency)[{edge[0],k}]);
        clause += " ";
        clause += to_string(-(*ptr_agentAgency)[{edge[1],k}]);
        clause += " ";
        clause += to_string(n_extra_bv);
        clause += " ";
        clause += to_string(0);
        (*ptr_all_clauses).push_back(clause);
        
        n_extra_bv++;
    }
    extra_bv_end_key = n_extra_bv;
    
    // after the above clause formed and a corresponding output variable n_extra_bv introduced for each of the
    // and term, we need to do OR of each of the n_extra_bv's.
    // the extra boolean variable formed above are total K and we will generate one more as a final output
    extra_bv_last_op_key = n_extra_bv;
    (*ptr_extra_bv_sat_code)[n_extra_bv] = n_extra_bv;
    string big_clause = to_string(-extra_bv_last_op_key);
    big_clause += " ";
    for(int k=extra_bv_start_key; k < extra_bv_end_key; k++)
    {
        
        string clause = to_string(-k);
        clause += " ";
        clause += to_string(extra_bv_last_op_key);
        clause += " ";
        clause += to_string(0);
        (*ptr_all_clauses).push_back(clause);
        
        //building the big clause;
        big_clause += to_string(k);
        big_clause += " ";
    }

    big_clause += to_string(0);
    (*ptr_all_clauses).push_back(big_clause);
    // and one final output
    string clause = to_string(extra_bv_last_op_key);
    clause += " ";
    clause += to_string(0);
    (*ptr_all_clauses).push_back(clause);
}

// the following function call is made to get the cnf for condtion that no to agencies are strict subsidiary of
// each other i.e. -(k1n1 <-> k2n1)
void cnf_for_or_over_agents(vector<int> agency_pairs, int agents, int n_essential_bv, map<vector<int>,int>* ptr_agentAgency, map<int,int>* ptr_extra_bv_sat_code, vector<string>* ptr_all_clauses)
{
    int extra_bv_start_key, extra_bv_end_key, extra_bv_last_op_key;
    
    int n_extra_bv = (*ptr_extra_bv_sat_code).size();
    if (n_extra_bv == 0)
    {
        n_extra_bv = n_essential_bv; //this triggers when there was no extra variable created.
    }
    else
    {
        n_extra_bv += n_essential_bv; // size of extra boolean variables plus the size of n_essential bv will
        //give the proper encoding
        
    }
    extra_bv_start_key = ++n_extra_bv;
    for(int n=1; n <= agents; n++)
    {
        /// writing first and clause g <-> (k1n1 and -k2n1)
        (*ptr_extra_bv_sat_code)[n_extra_bv] = n_extra_bv; //the extra boolean variables will have integer values above //the essential boolean variables.
        //first clause
        string clause = to_string((*ptr_agentAgency)[{n,agency_pairs[0]}]);
        clause += " ";
        clause += to_string((*ptr_agentAgency)[{n,agency_pairs[1]}]);
        clause += " ";
        clause += to_string(-n_extra_bv);
        clause += " ";
        clause += to_string(0);
        (*ptr_all_clauses).push_back(clause);
        
        //second clause
        clause = to_string(-(*ptr_agentAgency)[{n,agency_pairs[0]}]);
        clause += " ";
        clause += to_string(-(*ptr_agentAgency)[{n,agency_pairs[1]}]);
        clause += " ";
        clause += to_string(-n_extra_bv);
        clause += " ";
        clause += to_string(0);
        (*ptr_all_clauses).push_back(clause);

        //third clause
        clause = to_string(-(*ptr_agentAgency)[{n,agency_pairs[0]}]);
        clause += " ";
        clause += to_string((*ptr_agentAgency)[{n,agency_pairs[1]}]);
        clause += " ";
        clause += to_string(n_extra_bv);
        clause += " ";
        clause += to_string(0);
        (*ptr_all_clauses).push_back(clause);
        
        //fourth clause
        clause = to_string((*ptr_agentAgency)[{n,agency_pairs[0]}]);
        clause += " ";
        clause += to_string(-(*ptr_agentAgency)[{n,agency_pairs[1]}]);
        clause += " ";
        clause += to_string(n_extra_bv);
        clause += " ";
        clause += to_string(0);
        (*ptr_all_clauses).push_back(clause);
        n_extra_bv++;
    }
    
    extra_bv_end_key = n_extra_bv;
    
    // after the above clause formed and a corresponding output variable n_extra_bv introduced for each of the
    // and term, we need to do OR of each of the n_extra_bv's.
    // the extra boolean variable formed above are total 2 and we will generate one more as a final output
    extra_bv_last_op_key = n_extra_bv;
    (*ptr_extra_bv_sat_code)[n_extra_bv] = n_extra_bv;
    string big_clause = to_string(-extra_bv_last_op_key);
    big_clause += " ";
    for(int k=extra_bv_start_key; k < extra_bv_end_key; k++)
    {
        
        string clause = to_string(-k);
        clause += " ";
        clause += to_string(extra_bv_last_op_key);
        clause += " ";
        clause += to_string(0);
        (*ptr_all_clauses).push_back(clause);
        
        //building the big clause;
        big_clause += to_string(k);
        big_clause += " ";
    }
    
    big_clause += to_string(0);
    (*ptr_all_clauses).push_back(big_clause);
    // and one final output
    string clause = to_string(extra_bv_last_op_key);
    clause += " ";
    clause += to_string(0);
    (*ptr_all_clauses).push_back(clause);
}

// there exists an agent who is extra when compared against all other agencies i.e. (k1n1 and !k2n1)
void cnf_for_or_over_agents_extra_member(vector<int> agency_pairs, int agents, int n_essential_bv, map<vector<int>,int>* ptr_agentAgency, map<int,int>* ptr_extra_bv_sat_code, vector<string>* ptr_all_clauses)
{
    int extra_bv_start_key, extra_bv_end_key, extra_bv_last_op_key;
    
    int n_extra_bv = (*ptr_extra_bv_sat_code).size();
    if (n_extra_bv == 0)
    {
        n_extra_bv = n_essential_bv; //this triggers when there was no extra variable created.
    }
    else
    {
        n_extra_bv += n_essential_bv; // size of extra boolean variables plus the size of n_essential bv will
        //give the proper encoding
        
    }
    extra_bv_start_key = ++n_extra_bv;
    for(int n=1; n <= agents; n++)
    {
        /// writing first and clause g <-> (k1n1 and !k2n1)
        (*ptr_extra_bv_sat_code)[n_extra_bv] = n_extra_bv; //the extra boolean variables will have integer values above //the essential boolean variables.
        //first clause
        string clause = to_string((*ptr_agentAgency)[{n,agency_pairs[0]}]);
        clause += " ";
        clause += to_string(-n_extra_bv);
        clause += " ";
        clause += to_string(0);
        (*ptr_all_clauses).push_back(clause);
        
        //second clause
        clause = to_string(-(*ptr_agentAgency)[{n,agency_pairs[1]}]);
        clause += " ";
        clause += to_string(-n_extra_bv);
        clause += " ";
        clause += to_string(0);
        (*ptr_all_clauses).push_back(clause);

        //third clause
        clause = to_string(-(*ptr_agentAgency)[{n,agency_pairs[0]}]);
        clause += " ";
        clause += to_string((*ptr_agentAgency)[{n,agency_pairs[1]}]);
        clause += " ";
        clause += to_string(n_extra_bv);
        clause += " ";
        clause += to_string(0);
        (*ptr_all_clauses).push_back(clause);
        n_extra_bv++;
    }
    
    extra_bv_end_key = n_extra_bv;
    
    // after the above clause formed and a corresponding output variable n_extra_bv introduced for each of the
    // and term, we need to do OR of each of the n_extra_bv's.
    // the extra boolean variable formed above are total 2 and we will generate one more as a final output
    extra_bv_last_op_key = n_extra_bv;
    (*ptr_extra_bv_sat_code)[n_extra_bv] = n_extra_bv;
    string big_clause = to_string(-extra_bv_last_op_key);
    big_clause += " ";
    for(int k=extra_bv_start_key; k < extra_bv_end_key; k++)
    {
        
        string clause = to_string(-k);
        clause += " ";
        clause += to_string(extra_bv_last_op_key);
        clause += " ";
        clause += to_string(0);
        (*ptr_all_clauses).push_back(clause);
        
        //building the big clause;
        big_clause += to_string(k);
        big_clause += " ";
    }
    
    big_clause += to_string(0);
    (*ptr_all_clauses).push_back(big_clause);
    // and one final output
    string clause = to_string(extra_bv_last_op_key);
    clause += " ";
    clause += to_string(0);
    (*ptr_all_clauses).push_back(clause);
}

vector<string> split(string message, string delimiter)
{
    vector<string> result;
    auto i = 0, pos = 0, length = 0, temp=0;
    temp = message.find(delimiter.c_str(), pos);
    while (temp != -1) {
        length = temp - pos;
        result.push_back(message.substr(pos, length));
        pos = temp + delimiter.size();
        temp = message.find(delimiter.c_str(), pos);
        i++;
    }
    result.push_back(message.substr(pos));
    i++;
    return result;
}
void PrintGraph(map<vector<int>,int> graph)
{
    map<vector<int>,int>::iterator it;
    for (it=graph.begin(); it!=graph.end(); ++it)
    {
        vector<int> key = it->first;
        cout<< key[0]<<", "<<key[1]<<endl;
    }
}
void PrintGraph(map<vector<int>,int> graph, bool val)
{
    map<vector<int>,int>::iterator it;
    for (it=graph.begin(); it!=graph.end(); ++it)
    {
        vector<int> key = it->first;
        cout<< key[0]<<", "<<key[1]<<"->"<<graph[key]<<endl;
    }
}
