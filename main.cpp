//
//  main.cpp
//  k-drug-rings
//
//  Created by pawan kumar on 24/09/18.
//  Copyright © 2018 iit-delhi. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <map>

#include "utility.hpp"

using namespace std;

int main(int argc, const char * argv[])
{
    string line;
    const string encode = "encode"; //flag to use the program to encode the problem in cnf sat format
    const string decode = "decode"; //flag to help take the sat output and decode to give the agencies and there member
    int nodes_in_graph = 0;
    int edges_in_graph = 0;
    int subgraphs_in_graph = 0;
    map<vector<int>,int> graph; // the graph will only holde information on edge which exists(0/1)
    map<vector<int>,int> graph_c; // this is complement of the complete graph with same number of vertices and edge
    // edge of graph_c = n(n-1)/ - edge-in-graph
    map<vector<int>,int> agentAgency; //the map used to store the mapping between variable agentAgency to an integer
    int n_essential_bv = 0;// list all the essential variables which is equal to number of ndoes


    if(argv[2]==encode)
    {
      string input_graph = argv[1];
      input_graph += ".graph";
      ifstream inFileHandle;
      inFileHandle.open(input_graph.c_str());
      // inFileHandle.open("input.graph");
      if (!inFileHandle)
      {
          cerr << "Unable to open input file";
          exit(1);
      }
      vector<int> edge={0,0};
      int i=0;
      while (getline(inFileHandle, line))
      {
          vector<string> graph_line = split(line, " ");
          if (i==0)
          {
              nodes_in_graph = atoi(graph_line[0].c_str());
              edges_in_graph = atoi(graph_line[1].c_str());
              subgraphs_in_graph = atoi(graph_line[2].c_str());
              //subgraphs_in_graph = 3;
          }
          else
          {
              edge[0]=atoi(graph_line[0].c_str());
              edge[1]=atoi(graph_line[1].c_str());
              graph[edge]=1; //populating the graphs
          }
          i++;
      }
      inFileHandle.close();
      // printing the populted graph
      // cout<<"printing the given graph"<<endl;
      // PrintGraph(graph);
      n_essential_bv= nodes_in_graph*subgraphs_in_graph;

      ofstream out_cache_handle;// ("param.cache");
      out_cache_handle.open ("param.cache");
      out_cache_handle << "n_essential_bv "<< n_essential_bv<<"\n";
      out_cache_handle << "nodes_in_graph "<< nodes_in_graph <<"\n";
      out_cache_handle << "subgraphs_in_graph "<< subgraphs_in_graph <<"\n";
      out_cache_handle.close();

      int edges_in_graph_c = nodes_in_graph*(nodes_in_graph -1)/2 - edges_in_graph;
      for(int v=1; v<nodes_in_graph; v++)
      {
          for(int next_v = v+1; next_v<=nodes_in_graph; next_v++)
          {
              if((graph.find({v,next_v})!=graph.end()) or (graph.find({next_v,v})!=graph.end()))
              {
                  continue;
              }
              else
              {
                  graph_c[{v,next_v}]=1;
              }
          }
      }

      // cout<<"printing the complement graph"<<endl;

      // define the boolean variables agentAgencies which has a size of NK (number of Agents a* number of Agencies)
      for(int n=1; n <= nodes_in_graph; n++)
      {
          for(int k=1; k <= subgraphs_in_graph; k++)
          {
              agentAgency[{n,k}] = (n-1)*subgraphs_in_graph + k;
          }
      }
      // cout<< "printing the nk-bool varaibles"<<endl;
      // PrintGraph(agentAgency, true);

  //    Writing Clauses
  //    building the CNF for sat;
      string clause;
      vector<string> all_clauses;
      n_essential_bv= nodes_in_graph*subgraphs_in_graph;
      int agencies=subgraphs_in_graph;
      map<int,int> extra_bv_sat_code;

  ////////////////////////////
      // each agent has only one phone and makes call to members of only his agency
      //  given an edge in the graph the two nodes belong to the same agency. so if 1-2 and 1-4 is given then it would imply that 1 and 2 are in one agency and 1 and 4 in another agency, thus 1 would belong to two agency, note that the sat solver must be provided with constraint that there is not edge 2-4, else it would put all these nodes in the same agency.

      // we will use a function that would take the two node corresponding to an edge and give a cnf formula for relation or-over-agency(n1ki and n2ki), call it cnf-for-or-over-agencies(edge,agencies ...)

      map<vector<int>,int>::iterator it;
      for (it=graph.begin(); it!=graph.end(); ++it)
      {
          vector<int> key = it->first;

          cnf_for_or_over_agencies(key, agencies, n_essential_bv, &agentAgency, &extra_bv_sat_code, &all_clauses);
      }


      // for edges which are in complement graph, the agent can't be in same agency.
      for (it=graph_c.begin(); it!=graph_c.end(); ++it)
      {
          for(int k=1; k <= subgraphs_in_graph; k++)
          {
              vector<int> key = it->first;
              string clause = to_string(-agentAgency[{key[0],k}]);
              clause += " ";
              clause += to_string(-agentAgency[{key[1],k}]);
              clause += " ";
              clause += to_string(0);
              all_clauses.push_back(clause);
          }
      }

      // agencies are not strict subsidiary of another agency
      // there exist an agent who doesn't belong to two agencies.
      /*for (int k=1; k<agencies; k++)
      {
          for (int next_k=k+1; next_k<=agencies; next_k++)
          {
              vector<int> key = {k,next_k};
              cnf_for_or_over_agents(key, nodes_in_graph, n_essential_bv, &agentAgency, &extra_bv_sat_code, &all_clauses);
          }
      }*/
     // there exist an agent who is extra when compared aginst all other agencies
     //
      for (int k=1; k<=agencies; k++)
      {
          for (int next_k=1; next_k<=agencies ; next_k++)
          {
		          if (next_k==k) continue;
		          else
		          {

              		vector<int> key = {k,next_k};
              		cnf_for_or_over_agents_extra_member(key, nodes_in_graph, n_essential_bv, &agentAgency, &extra_bv_sat_code, &all_clauses);
		          }
          }
      }

     //
     //agency must have atleast one agent
      for (int k=1; k<=agencies; k++)
      {
              string clause;
      	      for(int n=1; n <= nodes_in_graph; n++)
              {
                  clause += to_string(agentAgency[{n,k}]);
                  clause += " ";
              }
              clause += to_string(0);
              all_clauses.push_back(clause);
      }

      //////////
      // cout<<"printing the total clauses formed on the output file "<<endl;
  //  ofstream outFileHandle ("test.satinput",ios::app);

      ofstream outFileHandle;// ("test.satinput");
      outFileHandle.open ("test.satinput");
      auto n_total_clauses = all_clauses.size();
      int n_total_bv = n_essential_bv + extra_bv_sat_code.size();
      outFileHandle << "p cnf "<< n_total_bv <<" "<<n_total_clauses<<"\n";

  //  f(is_open(outFileHandle))
      for(int i=0; i< n_total_clauses; i++)
      {
          outFileHandle << all_clauses[i] <<"\n";
      }
      outFileHandle.close();
    }
// reading the sat output file and creating the decoded file
  else if(argv[2] == decode)
  {
    ifstream inFileHandle;
    inFileHandle.open("test.satoutput");
    ifstream in_cache_handle;
    in_cache_handle.open("param.cache");
    vector<int> agent_agency_bv;
    if (!inFileHandle or !in_cache_handle)
    {
        cerr << "Sat Output file or parameter cache of the first run not found";
        exit(2);
    }

    while (getline(in_cache_handle, line))
    {
    	vector<string> cache_line = split(line, " ");
    	if (cache_line[0]=="n_essential_bv") n_essential_bv = atoi(cache_line[1].c_str());
    	else if (cache_line[0]=="nodes_in_graph") nodes_in_graph = atoi(cache_line[1].c_str());
    	else if (cache_line[0]=="subgraphs_in_graph") subgraphs_in_graph = atoi(cache_line[1].c_str());
    }
    in_cache_handle.close();
    vector<vector<int>> subgraphs(subgraphs_in_graph);
    /*cout<<"Printing"<<endl;
    for(int i=0; i< subgraphs_in_graph; i++)
	{
		cout<< subgraphs[i].size()<<endl;
	}
	*/
    vector<int> subgraphs_size;
    subgraphs_size.assign(subgraphs_in_graph, 0);

    ofstream out_file_handle;// ("test.subgraph");
    string file_name = argv[1];
    file_name += ".subgraphs";
    //out_file_handle.open(argv[1] += ".subgraphs"); //the test.subgraph file name will be provided at runtime
    out_file_handle.open(file_name); //the test.subgraph file name will be provided at runtime

    vector<int> nodes;
    int i=0;
    int flag_decode = false;
    while (getline(inFileHandle, line))
    {
        vector<string> sat_line = split(line," ");
        if (i==0)
        {
            string solution = sat_line[0].c_str();
            if(solution == "UNSAT")
            {
              out_file_handle << 0 <<"\n";
	            out_file_handle.close();
              break;
            }
        }

        else if (i==1)
        {
          for (int bv=0; bv<n_essential_bv; bv++)
          {
              agent_agency_bv.push_back(atoi(sat_line[bv].c_str()));
          }
	        flag_decode = true;
        }
        else
        {
          exit(3);
        }
        i++;
    }
    inFileHandle.close();

    //populate the subgraphs
    //for(int subs=0; subs < subgraphs_in_graph; subs++ )
    //{
      //subgraphs.push_back("");
      //subgraph_size.push_back(0);
    //}
    int agency;
    int node;
    if(flag_decode == true)
    {
      for (int bv=0; bv<n_essential_bv; bv++)
      {
          if(agent_agency_bv[bv] < 0)
        	{
        	   continue;
        	}
        	else
        	{
        	   agency = (agent_agency_bv[bv] % (subgraphs_in_graph));
             if(agency == 0)
             {
                node = (agent_agency_bv[bv] / subgraphs_in_graph);
             }
             else
             {
                node = (agent_agency_bv[bv] / subgraphs_in_graph) + 1;
             }
             (subgraphs[agency]).push_back(node);
             subgraphs_size[agency] += 1;
        	}
      }

      for(int subs=0; subs < subgraphs_in_graph; subs++ )
      {
        	out_file_handle << "#"<< subs+1 <<" "<< subgraphs_size[subs] <<endl;
        	for (int k=0; k< subgraphs_size[subs]; k++)
  	      {
  	         if(k<subgraphs_size[subs]-1)
  	         {
  	            out_file_handle << subgraphs[subs][k] <<" ";
             }
  	         else
  	         {
  	            out_file_handle << subgraphs[subs][k] <<endl;
             }
  	      }
      }
    out_file_handle.close();
    flag_decode = false;
    }
  }
  return 0;
}
