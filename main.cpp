#include <bits/stdc++.h>
// #include <iostream>
// #include <fstream>
#include "library.hpp"
#include <omp.h>

using namespace std;

const int MAX_VAL = pow(2, 16);

map<pair<int, int>, int> m_input, m_output, m_ans;
vector<pair<int, int>> rm_input, rm_ans, rm_fans;
vector<vector<int>> in_matrix, out_matrix, final_ans_matrix, compress_ans_matrix;
int n, m, k_input, k_output;

void read_file(string input){
    // string input = "input";
    ifstream file(input, ios::binary);

    file.read((char*)&n, 4);
    file.read((char*)&m, 4);
    file.read((char*)&k_input, 4);
    in_matrix.resize(k_input);
    int size = (n/m)*(n/m);
    rm_input.resize(size);
    rm_ans.resize(size);
    rm_fans.resize(size);
    cout<<n<<" n "<<m<<" m "<<k_input<<" k_input\n";
    for (int i = 0; i < k_input; i++) {
        int iIndex, jIndex;
        file.read((char*)&iIndex, 4);
        file.read((char*)&jIndex, 4);
        m_input[{iIndex, jIndex}] = i+1;
        m_input[{jIndex, iIndex}] = i+1;
        rm_input[i+1] = {iIndex, jIndex};
        in_matrix[i].resize(m*m);
        for (int j = 0; j < m * m; j++) {
            file.read((char*)&in_matrix[i][j], sizeof(char));
        }
    }
    file.close();
}

void write_file(string output){
    // string output = "output";
    ofstream ofile(output, ios::binary);

    ofile.write((char*)&n, sizeof(n));
    ofile.write((char*)&m, sizeof(m));
    int kout = final_ans_matrix.size();
    ofile.write((char*)&kout, sizeof(kout));

    // cout << n << " n " << m << " m " << kout << " k output " <<compress_ans_matrix.size()<<" my k\n";

    for (int i = 0; i < kout; i++) {
        int iIndex, jIndex;
        iIndex = rm_fans[i + 1].first;
        jIndex = rm_fans[i + 1].second;
        // cout << iIndex << " i j " << jIndex << endl;
        ofile.write((char*)&iIndex, sizeof(iIndex));
        ofile.write((char*)&jIndex, sizeof(jIndex));
        for (int j = 0; j < m * m; j++) {
            uint16_t value = (uint16_t)final_ans_matrix[i][j];
            // cout<<value<<" ";
            ofile.write((char*)&value, sizeof(value));
        }
        // cout<<endl;
    }
    ofile.close();
}

void compare(){
    ifstream out_file("output111", ios::binary);
    out_file.read((char *)&n, sizeof(n));
    out_file.read((char *)&m, sizeof(m));
    out_file.read((char *)&k_output, sizeof(k_output));
    out_matrix.resize(k_output);
    cout << n << " n " << m << " m " << k_output << " k output " <<compress_ans_matrix.size()<<" my k\n";
    for (int i = 0; i < k_output; i++){

        if(i>= compress_ans_matrix.size()){ cout<<"k not matching\n"; break;}
        
        int iIndex, jIndex;
        out_file.read((char *)&iIndex, sizeof(iIndex));
        out_file.read((char *)&jIndex, sizeof(jIndex));
        m_output[{iIndex, jIndex}] = i + 1;
        out_matrix[i].resize(m * m);

        // cout << iIndex << " i j " << jIndex<<" "<< m_ans[{iIndex, jIndex}] << endl;
        // for (int j = 0; j < m * m; j++){
        //     cout<<final_ans_matrix[m_ans[{iIndex, jIndex}]-1][j]<<" ";
        // } cout<<endl;
        for (int j = 0; j < m * m; j++){
            out_file.read((char *)&out_matrix[i][j], sizeof(short)); // here short 2 bytes
            // cout << out_matrix[i][j] << " ";
            if(out_matrix[i][j] != final_ans_matrix[m_ans[{iIndex, jIndex}]-1][j]) cout<<"NOT CORRECT #########################################################\n";
        }
        // cout << endl;
    }
    out_file.close();

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
// real code
    // int g = (n/m)*(n/m+1)/2;
    // compress_ans_matrix.resize(g);
    // for(int i=0; i<g; i++) compress_ans_matrix[i].resize(m*m, 0);

//trying to parallelise it
    int g = (n/m)*(n/m+1)/2;
    compress_ans_matrix.resize(g);
    #pragma omp parallel num_threads(4)
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

    cout<<k_input<<" DE\n";

    c=1;
    #pragma omp parallel for private(i,j,k,l,t,p,temp,xi,yi,xj,yj) shared(c, compress_ans_matrix, m_ans)
    // #pragma omp parallel for schedule(static) num_threads(4) private(i,j,k,l,t,p,temp,xi,yi,xj,yj) shared(c, compress_ans_matrix, m_ans)
    // #pragma omp parallel num_threads(numt) private(i,j,k,l,t,p,temp,xi,yi,xj,yj) shared(c, compress_ans_matrix, m_ans)
    // {
        // #pragma omp for
        // int tid = omp_get_thread_num();
        // for(i=tid+1; i<=k_input; i+=numt){
        for(i=1; i<=k_input; i++){
            xi = rm_input[i].first, yi = rm_input[i].second;
            for(j=1; j<=k_input; j++){
                xj = rm_input[j].first, yj = rm_input[j].second;
                if(yi != xj or xi > yj) continue;
                // #pragma omp task
                // {
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
                    // }
                }
            }
        }
    // }

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

    cout<<compress_ans_matrix.size()<<" my k\n";

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time taken: " << duration << " microseconds" << std::endl;
}

int main(int argc, char *argv[]) {

    string input = argv[1];
    string output = argv[2];

    read_file(input);

    // matrix_ope();
    matrix();

    // write_file(output);

    compare();

    return 0;
}
