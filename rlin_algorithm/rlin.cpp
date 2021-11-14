#include <iostream>
#include <queue>
#include <vector>
#include <ctime>
#include <limits.h>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <sys/time.h>
#include <map>
#include <set>
#define MAXMAX 10000
using namespace std;
#define INF INT_MAX


// eage or point
typedef struct Eage {
    int x;
    int y;
    int val;

    Eage (int xx, int yy, int vval = 1) {
        x = xx;
        y = yy;;
        val = vval;
    }

    bool operator< (const Eage& r) const
    {
        if (x == r.x) {
            return y < r.y;
        } else {
            return x < r.x;
        }
    }
}eage;

// compute time_diff
double time_diff(struct timeval start, struct timeval end) {
    return 1000000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
}

// generate graph
void generate_graph_new(vector<vector<string> >& input, int& eages_num, int& points_num, set<eage>& res_eages) {
    struct timeval start, middle, end;
    gettimeofday(&start, NULL);

    // Variable declaration
    set<pair<string, string> > vv; // mid eages set

    map<string, pair<int, int> > nodeids;
    int nodeid = 2;

    // the left points in order to add start node
    set<string> left_points;
    // the right points in order to add end node
    set<string> right_points;

    for (int i = 0; i < input.size(); i++) {
        // node s id
        if (nodeids.find(input[i][0]) == nodeids.end()) {
            nodeids[input[i][0]] = make_pair(nodeid, 0);
            nodeid ++;
        }

        left_points.insert(input[i][0]);

        // other points 
        for (int j = 1; j < input[i].size(); j++) {
            // search node id
            if (nodeids.find(input[i][j]) == nodeids.end()) {
                nodeids[input[i][j]] = make_pair(nodeid, 0);
                nodeid ++;
            }

            vv.insert(make_pair(input[i][j - 1], input[i][j]));
        }
        right_points.insert(input[i][input[i].size() - 1]);
    }

    gettimeofday(&middle, NULL);

    // split point into eages 
    for (set<pair<string, string> >::iterator it = vv.begin(); it != vv.end(); it++) {
        // split the left-most points
        if (nodeids[it->first].second == 0) {
            nodeids[it->first].second = nodeid;
            nodeid ++;
            res_eages.insert(eage(nodeids[it->first].first, nodeids[it->first].second, 1));
        }

        // split the right-most points
        if (nodeids[it->second].second == 0) {
            nodeids[it->second].second = nodeid;
            nodeid ++;
            res_eages.insert(eage(nodeids[it->second].first, nodeids[it->second].second, 1));
        }

        // eages
        res_eages.insert(eage(nodeids[it->first].second, nodeids[it->second].first, MAXMAX));
    }

    // add the left nodes eage
    set<string>::iterator it;
    for (it = left_points.begin(); it != left_points.end(); it++) {
        res_eages.insert(eage(1, nodeids[*it].first, MAXMAX));
    }

    // add the right nodes eage
    for (it = right_points.begin(); it != right_points.end(); it++) {
        res_eages.insert(eage(nodeids[*it].second, nodeid, MAXMAX));
    }
    gettimeofday(&end, NULL);

    // dump info
    eages_num = res_eages.size(); // the num of eages
    points_num = nodeid; // the num of points

    cout << "the max_flow gen_graph running time is: " << std::fixed << time_diff(start, end) << endl;
}

bool bfs(int** cap, int s, int t, vector<int> &level, int n)
{
    level.clear();
    level.resize(n, -1);
    level[s] = 0;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (u == t) { //u == t level[u] = 1,2,3....
            return true;
        }
        for (int v = 0; v < n; v++) {
            if (level[v] < 0 && cap[u][v] > 0) {
                level[v] = level[u] + 1;
                q.push(v);
            }
        }
    }
    return false;
}
int dfs(int** cap, int u, int t, vector<int> &level, int bt, vector<int> &cur, int n)
{
    if (u == t) {
        return bt;
    }
    for (int v = cur[u]; v < n; v++) {
        if (level[u] + 1 == level[v] && cap[u][v] > 0) {
            int flow = dfs(cap, v, t, level, min(bt, cap[u][v]), cur, n);
            if (flow > 0) {
                cap[u][v] -= flow;
                cap[v][u] += flow;
                cur[u] = v;
                return flow;
            }
        }
    }
    level[u] = 0;
    return 0;
}
int augment(int** cap, int s, int t, vector<int> &p)
{
    int v = t;
    int u = p[v];
    int bt = cap[u][t];
    while (u != s) {
        v = u;
        u = p[v];
        if (cap[u][v] < bt) {
            bt = cap[u][v];
        }
    }
    for (int v = t; v != s; v = p[v]) {
        u = p[v];
        cap[u][v] -= bt;
        cap[v][u] += bt;
    }
    return bt;
}
int dfs_iter(int** cap,int s,int t,vector<int> &level, int n)
{
    vector<int> cur(n);
    vector<int> p(n);
    int u = s;
    while (u != t)
    {
        int v;
        for (v = cur[u]; v<n ;v++)
        {
            if (cap[u][v] > 0 && level[v] == level[u] + 1)
            {
                break;
            }
        }
        cur[u] = v;
        if (v < n)
        {
            p[v] = u;
            u = v;
            continue;
        }
        if (u == s)
        {
            return 0;
        }
        level[u] = 0;
        u = p[u];
    }
    return augment(cap,s,t,p);
}
int dfs(int** cap, int s, int t, vector<int> &level, int n)
{
    vector<int> cur(n, 1);
    vector<int> p(n);
    int u = s;
    p[s] = 0;
    while (u != t) {
        int v;
        for (v = cur[u]; v < n; v++) {
            if (level[u] + 1 == level[v] && cap[u][v] > 0) {
                break;
            }
        }
        cur[u] = v;
        if (v == n) {
            if (u == s) {
                return 0;
            }
            level[u] = 0;
            u = p[u];
            continue;
        } else {
            u = v;
            p[v] = u;
        }

    }
    return augment(cap, s, t, p);
}

int dinic(int** cap, int s, int t, int n)
{
    vector<int> level(n, -1);
    level[s] = 0;
    int flow, max_flow = 0;
    while (bfs(cap, s, t, level, n)) {
        vector<int> cur(n, 0);
        while ((flow = dfs(cap, s, t, level, INF, cur, n)) > 0) {
            max_flow += flow;
        }
    }
    return max_flow;
}

void get_input(vector<vector<string> >& cur, string filename) {
    cur.clear();
    ifstream in(filename.c_str());
    // freopen(filename.c_str(), "r", stdin);
    string s, t;
    getline(in, s);
    while (s != "") {
        vector<string> v;
        stringstream ss;
        ss << s;
        while(ss >> t) {
            v.push_back(t);
        }
        cur.push_back(v);
        getline(in, s);
    }
}

/*
 * INPUT: in.txt
 * format£ºsep = ' '
 * 1th row£ºeages_num m
 * 2th£ºpoint_num n
 * 
 * OUTPUT: stdout
 */
int main(int argc, char * argv[])
{
    string filename = "ingraph.txt";
    if (argc > 1) {
        filename = argv[1];
    }

    // read input
    vector<vector<string> > input;
    get_input(input, filename);

    // gen network graph
    int m = 0, n = 0;
    set<eage> res_eages;
    generate_graph_new(input, m, n, res_eages);

    int **cap = new int*[n+1];
    for (int i = 0; i < n + 1; i++)
        cap[i] = new int[n+1];
    for (int i = 0; i < n + 1; i++)
        for (int j = 0; j < n + 1; j++)
            cap[i][j] = 0;
    
    // cout << m << endl;
    // cout << n << endl;
    for (set<eage>::iterator it = res_eages.begin(); it != res_eages.end(); it++) {
        int u, v, c;
        u = it->x;
        v = it->y;
        c = it->val;
        cap[u][v] += c;
        // cout << u << " " << v << " " << c << endl;
    }

    // for (int i = 0; i < n + 1; i++) {
    //     fot (int j = 0; j < n + 1; j++) {
    //         cout << cap[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    struct timeval start, end;  
    // max_flow/min_cur alg for IQ query
    gettimeofday(&start, NULL); 
    cout << "resilience = " << dinic(cap, 1, n, n+1) << endl;
    gettimeofday(&end, NULL); 
    double timediff = 1000000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    cout << "resilience::max-flow alg: the running time is: " << std::fixed << timediff << endl << endl;

    for(int i=0;i<n+1;i++)
        delete[] cap[i];
    delete[] cap;
	
    return 0;
}
