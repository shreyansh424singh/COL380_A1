#include <bits/stdc++.h>
#include "library.hpp"
#include <omp.h>

using namespace std;

const int MAX_VAL = pow(2, 16);

map<pair<int, int>, int> m_input, m_output, m_ans;
vector<pair<int, int>> rm_input, rm_ans, rm_fans;
vector<vector<int>> in_matrix, out_matrix, final_ans_matrix, compress_ans_matrix;
int n, m, k_input, k_output;

void read_f(string input){
    // string input = "input";
    ifstream f(input, ios::binary);

    f.read((char*)&n, 4);
    f.read((char*)&m, 4);
    f.read((char*)&k_input, 4);
    in_matrix.resize(k_input);
    int size = (n/m)*(n/m);
    rm_input.resize(size);
    rm_ans.resize(size);
    rm_fans.resize(size);
    // cout<<n<<" n "<<m<<" m "<<k_input<<" k_input\n";
    for (int i = 0; i < k_input; i++) {
        int id, jd;
        f.read((char*)&id, 4);
        f.read((char*)&jd, 4);
        m_input[{id, jd}] = i+1;
        m_input[{jd, id}] = i+1;
        rm_input[i+1] = {id, jd};
        in_matrix[i].resize(m*m);
        for (int j = 0; j < m * m; j++) {
            f.read((char*)&in_matrix[i][j], sizeof(char));
        }
    }
    f.close();
}

void write_f(string output){
    // string output = "output";
    ofstream of(output, ios::binary);

    of.write((char*)&n, sizeof(n));
    of.write((char*)&m, sizeof(m));
    int kout = final_ans_matrix.size();
    of.write((char*)&kout, sizeof(kout));

    for (int i = 0; i < kout; i++) {
        int id, jd;
        id = rm_fans[i + 1].first;
        jd = rm_fans[i + 1].second;
        // cout << id << " i j " << jd << endl;
        of.write((char*)&id, sizeof(id));
        of.write((char*)&jd, sizeof(jd));
        for (int j = 0; j < m * m; j++) {
            uint16_t value = (uint16_t)final_ans_matrix[i][j];
            // cout<<value<<" ";
            of.write((char*)&value, sizeof(value));
        }
        // cout<<endl;
    }
    of.close();
}

void matrix(){

    cout<<n<<" n m "<<m<<" \n";

    auto start = std::chrono::high_resolution_clock::now();

    int i, j, k, l, p, t, w, c=1, numt=4, q=k_input, xi, xj, yi, yj;
    bool flag; vector<int> temp(m*m, 0);

    for(i=0; i<q; i++){
        k = rm_input[i+1].first,    l = rm_input[i+1].second;
        if(k==l) { continue;}
        for(j=0; j<m*m; j++) 
            temp[j] = in_matrix[i][m*(j%m)+j/m];
        in_matrix.push_back(temp);
        k_input++;
        m_input[{l, k}] = k_input;
        rm_input[k_input] = {l, k};
    }

    int g = (n/m)*(n/m+1)/2;
    compress_ans_matrix.resize(g);
    #pragma omp parallel num_threads(numt)
    {
        #pragma omp single
        {
            for(int i=0; i<g; i++) 
                #pragma omp task
                {
                    compress_ans_matrix[i].resize(m*m, 0);
                }
        }
    }
    c=1;
        for(int i=0; i<n/m; i++)
            for(int j=i; j<n/m; j++){
                rm_ans[c] = {i, j};
                m_ans[{i, j}] = c++;
            }


    c=1;
    #pragma omp parallel num_threads(numt) private(i,j,k,l,t,p,temp,xi,yi,xj,yj) shared(c, compress_ans_matrix, m_ans)
    {
        #pragma omp for
        for(i=1; i<=k_input; i++){
            xi = rm_input[i].first, yi = rm_input[i].second;
            for(j=1; j<=k_input; j++){
                xj = rm_input[j].first, yj = rm_input[j].second;
                if(yi != xj or xi > yj) continue;
                #pragma omp task
                {
                    for(k=0; k<m; k++){
                        for(l=0; l<m; l++){
                            t = 0;
                            for(p=0; p<m; p++)
                                t = Outer(t, Inner(in_matrix[i-1][k*m+p], in_matrix[j-1][p*m+l]));
                            t = (t > MAX_VAL - 1) ? MAX_VAL - 1 : t;
                            #pragma omp critical
                            {
                                compress_ans_matrix[m_ans[{xi, yj}]-1][k*m+l] += t;
                                c++;
                            }
                        }
                    }
                }
            }
        }
    }

    c=1;
    for(i=0; i<compress_ans_matrix.size(); i++){
        flag = false;
        for(j=0; j<m*m; j++) if(compress_ans_matrix[i][j]>0){ flag=true; break;}
        if(flag){
            m_ans[rm_ans[i+1]] = c;
            rm_fans[c++] = rm_ans[i+1];
            final_ans_matrix.push_back(compress_ans_matrix[i]);
        }
    }


    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time taken: " << duration << " microseconds" << std::endl;
}

int main(int argc, char *argv[]) {

    string input = argv[1];
    string output = argv[2];

    read_f(input);

    matrix();

    write_f(output);

    return 0;
}