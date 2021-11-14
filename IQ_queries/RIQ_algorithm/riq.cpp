 /**
 * @file comp.cpp
 * @author work
 * @date 2021/11/09 19:15:39
 * @version $Revision$ 
 * @brief 
 *  
 **/

#include <iostream>
#include <cstdio>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <limits.h>
#include <queue>
#include <stdio.h>
#include <ctime>
#include <sys/time.h>
using namespace std;



vector<string> vec(string pre_x, string pre_y) {
    vector<string> tmp;
    tmp.push_back(pre_x);
    tmp.push_back(pre_y);
    return tmp;
}

vector<string> vec3(string pre_x, string pre_y, string pre_z) {
    vector<string> tmp;
    tmp.push_back(pre_x);
    tmp.push_back(pre_y);
    tmp.push_back(pre_z);
    return tmp;
}

// print matrix
void print(vector<vector<string> >& v) {
    for (int i = 0; i < v.size(); i ++) {
        stringstream ss; 
        ss << v[i][0]; 
        for (int j = 1; j < v[i].size(); j++) {
            ss << "\t" << v[i][j];
        }
        cout << ss.str() << endl;
        ss.clear();
        ss.str("");
    }
}

// compute time_diff
double time_diff(struct timeval start, struct timeval end) {
    return 1000000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
}

void compress_left(vector<vector<string> >& cur, vector<vector<string> >& res) {
    // add first row
    res.push_back(vec(cur[0][0], cur[0][1]));
    string pre_x = cur[0][0];
    int pre_cnt = 1;

    // other rows
    for (int i = 1; i < cur.size(); ) {
        if (cur[i][0] != pre_x) {
            // skip pre_cnt rows，process the next row
            int next_i = i + pre_cnt;
            if (next_i >= cur.size()) {
                break;
            }

            pre_x = cur[i][0];
            if (cur[i][0] != cur[next_i][0]) {
                res[res.size() - 1][0] += "+" + cur[i][0];
                i = next_i;
                continue;
            } else { // the same x，process next_i 
                i = next_i;
            }
        }

        // process cur i
        pre_cnt ++;
        if (cur[i][0] == res[res.size() - 1][0]) {
            res[res.size() - 1][0] = "";
        }
        res.push_back(vec(cur[i][0], cur[i][1]));
        i++;
    }

    // reverse
    reverse(res.begin(), res.end());
}


void compress_right(vector<vector<string> >& cur, vector<vector<string> >& res) {
    // add first row
    res.push_back(vec(cur[0][0], cur[0][1]));
    string pre_x = cur[0][0];
    int pre_cnt = 1;

    // other rows
    for (int i = 1; i < cur.size(); ) {
        if (cur[i][0] != pre_x) {
            int next_i = i + pre_cnt;
            if (next_i > cur.size()) {
                break;
            } else if (next_i == cur.size()) {  // save the last x
                res.push_back(vec(cur[cur.size() - 1][0], ""));
                break;
            }

            pre_x = cur[i][0];
            if (cur[i][0] != cur[next_i][0]) {
                res.push_back(vec(cur[i][0], ""));
                i = next_i;
                continue;
            } else { // same x，process next_i
                i = next_i;
                // process cur i
                pre_cnt ++;
                res.push_back(vec(cur[i][0], cur[i][1]));
                i++;
            }
        } else {
            // process cur i
            pre_cnt ++;
            res[res.size() - 1][1] += "+" + cur[i][1];
            i++;
        }
    }

    // reverse
    reverse(res.begin(), res.end());
}

void compress_3col(vector<vector<string> >& cur, vector<vector<string> >& res) {
    // add first row
    res.push_back(vec3(cur[0][0], cur[0][1], cur[0][2]));
    string pre_x = cur[0][0], pre_y = cur[0][1];
    int prex_cnt = 1;
    int prey_cnt = 1;

    // other lines
    // other rows
    for (int i = 1; i < cur.size(); ) {
        // x change
        if (cur[i][0] != pre_x) {
            // skip prex_cnt rows，process the next row
            int next_i = i + prex_cnt;
            if (next_i > cur.size()) {
                break;
            } else if (next_i == cur.size()) {
                res[res.size() - 1][0] += "+" + cur[i][0];
                break;
            }

            pre_x = cur[i][0];
            if (cur[i][0] != cur[next_i][0]) {
                res[res.size() - 1][0] += "+" + cur[i][0];
            }
            i = next_i;
        } else {
            if (pre_y != cur[i][1]) {
                int next_i = i + prey_cnt;
                if (next_i > cur.size()) {
                    break;
                } else if (next_i == cur.size()) {  // save the last x
                    if (res[res.size() - 1][0] == cur[cur.size() - 1][0]) {
                        res[res.size() - 1][0] = "";
                    }
                    res.push_back(vec3(cur[cur.size() - 1][0], cur[cur.size() - 1][1], ""));
                    break;
                }

                // update prex_cnt
                prex_cnt += prey_cnt;

                // x change or y change
                pre_y = cur[i][1];
                if (cur[i][0] != cur[next_i][0] || cur[i][1] != cur[next_i][1]) {
                    if (res[res.size() - 1][0] == cur[i][0]) {
                        res[res.size() - 1][0] = "";
                    }
                    res.push_back(vec3(cur[i][0], cur[i][1], "")); // add ""  // res[res.size() - 1][2]
                    i = next_i;
                    continue;
                } else { // same x，process next_i
                    i = next_i;
                    // process cur i
                    prey_cnt ++;
                    prex_cnt ++;
                    if (res[res.size() - 1][0] == cur[i][0]) {
                        res[res.size() - 1][0] = "";
                    }
                    res.push_back(vec3(cur[i][0], cur[i][1], cur[i][2]));
                    i++;
                }
            } else {
                prey_cnt ++;
                prex_cnt ++;
                res[res.size() - 1][2] += "+" + cur[i][2];
                i++;
            }
        }
    }

    // reverse
    reverse(res.begin(), res.end());
}

int length(string& s) {
    if (s == "") 
        return 0;

    int len = 1;
    int t = s.find('+',0); 
    while(t != string::npos) 
    {
        len += 1;
        t ++;
        t = s.find('+',t); 
    }
    return len;
}

// trans lineage graph into lineage matrix
void trans_lm(vector<vector<string> >& res, vector<vector<int> >& lm, int& M, int& N) {
    M = res.size();
    N = res[0].size();
    for (int i = 0; i < res.size(); i++) {
        vector<int> vv;
        for (int j = 0; j < res[i].size(); j++) {
            vv.push_back(length(res[i][j]));
        }
        lm.push_back(vv);
    }
}

// dp alg details
void print(vector<vector<int> >& v) {
    for (int i = 0; i < v.size(); i ++) {
        for (int j = 0; j < v[i].size(); j++) {
            cout << v[i][j] << " ";
        }
        cout << endl;
    }
}

void print_last(vector<vector<int> >& v) {
    for (int i = v.size() - 1; i < v.size(); i ++) {
        for (int j = 0; j < v[i].size(); j++) {
            cout << v[i][j] << " ";
        }
        cout << endl;
    }
}

/*
 * input: in.txt
 * formats：sep = ' '
 * 1th row: row col
 * 
 */
void dp_resi(vector<vector<int> >& lm, vector<vector<int> >& sdm, int M, int N) {
    // [0][0]
    sdm[0][0] = lm[0][0];

    // first row
    for (int j = 1; j < N; j++) {
        sdm[0][j] = min(sdm[0][j - 1], lm[0][j]);
    }

    // first col
    for (int i = 1; i < M; i++) {
        sdm[i][0] = sdm[i - 1][0] + lm[i][0];
    }

    // other 
    for (int i = 1; i < M; i++) {
        for (int j = 1; j < N; j++) {
            sdm[i][j] = min(sdm[i][j-1], sdm[i-1][j] + lm[i][j]);    
        }
    }
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
 * input：database data
 * formats: sep = ' '
 * output：the result of compress
 *
 * parameter：
 * argv[1]：null / r / 3
 */
int main(int argc, char **argv) {
    string filename = "in.txt";
    filename = argv[1];

    vector<vector<string> > res;
    vector<vector<string> > resr;
    vector<vector<string> > cur;

    struct timeval start, start1, start2, start3;
    
    // get_input
    get_input(cur, filename);

    gettimeofday(&start, NULL);
    if (argc == 3) {
        // compress left (argv[1])
        compress_left(cur, res);
        gettimeofday(&start1, NULL);
        // print the left lineage graph
        // print(res);

        // compress right (argv[1])
        filename = argv[2];
        get_input(cur, filename);
        
        gettimeofday(&start2, NULL);
        compress_right(cur, resr);
        gettimeofday(&start3, NULL);
        // print the right lineage graph
        // print(resr);

        cout << "compress left:: the running time is: " << std::fixed << time_diff(start, start1) << endl;
        cout << "compress right:: the running time is: " << std::fixed << time_diff(start2, start3) << endl;
    } else if (argc == 2) {
        // compress
        if (cur[0].size() == 3) {
            compress_3col(cur, res);
        } else {
            compress_left(cur, res);
        }

        gettimeofday(&start1, NULL);
        // print the lineage graph
        // print(res);

        // edit
        vector<vector<int> > lm;
        int M = 0, N = 0;
        trans_lm(res, lm, M, N);
        vector<vector<int> > sdm(M, vector<int>(N, INT_MAX));
        // dp resi alg
        gettimeofday(&start2, NULL);
        dp_resi(lm, sdm, M, N);
        gettimeofday(&start3, NULL);
        cout << "dp resilience = "; 
        print_last(sdm);

        // print lineage matrix
        // cout << M << " " << N << endl;
        // print(lm);
        cout << "compress:: the running time is: " << std::fixed << time_diff(start, start1) << endl;
        cout << "dp_alg:: the running time is: " << std::fixed << time_diff(start2, start3) << endl;
    } 
	
    return 0;
}
