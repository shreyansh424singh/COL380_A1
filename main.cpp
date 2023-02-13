#include <bits/stdc++.h>
// #include <iostream>
// #include <fstream>
#include "library.hpp"
#include <omp.h>

using namespace std;

const int MAX_VAL = pow(2, 16);

map<pair<int, int>, int> m_input, m_output, m_ans, m_temp;
map<int, pair<int, int>> rm_input;
vector<vector<int>> in_matrix, out_matrix, ans_matrix, compress_ans_matrix;
int n, m, k_input, k_output;

void read_file(){
    string input = "input";
    ifstream file(input, ios::binary);

    file.read((char*)&n, sizeof(n));

    
    file.read((char*)&m, sizeof(m));
    file.read((char*)&k_input, sizeof(k_input));
    in_matrix.resize(k_input);
    cout<<n<<" n "<<m<<" m "<<k_input<<" k_input\n";
    for (int i = 0; i < k_input; i++) {
        int iIndex, jIndex;
        file.read((char*)&iIndex, sizeof(iIndex));
        file.read((char*)&jIndex, sizeof(jIndex));
        m_input[{iIndex, jIndex}] = i+1;
        m_input[{jIndex, iIndex}] = i+1;
        rm_input[i+1] = {iIndex, jIndex};
        in_matrix[i].resize(m*m);
        // cout<<iIndex<<" i j "<<jIndex<<endl;
        for (int j = 0; j < m * m; j++) {
            file.read((char*)&in_matrix[i][j], sizeof(char));
            // cout<<in_matrix[i][j]<<" ";
        }
        // cout<<endl;
    }
    file.close();

    // ifstream out_file("output", ios::binary);
    // out_file.read((char *)&n, sizeof(n));
    // out_file.read((char *)&m, sizeof(m));
    // out_file.read((char *)&k_output, sizeof(k_output));
    // out_matrix.resize(k_output);
    // cout << n << " n " << m << " m " << k_output << " k output\n";
    // for (int i = 0; i < k_output; i++){
    //     int iIndex, jIndex;
    //     out_file.read((char *)&iIndex, sizeof(iIndex));
    //     out_file.read((char *)&jIndex, sizeof(jIndex));
    //     m_output[{iIndex, jIndex}] = i + 1;
    //     out_matrix[i].resize(m * m);
    //     cout << iIndex << " i j " << jIndex << endl;
    //     for (int j = 0; j < m * m; j++){
    //         out_file.read((char *)&out_matrix[i][j], sizeof(short)); // here short 2 bytes
    //         cout << out_matrix[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // out_file.close();
}

void matrix(){

    cout<<n<<" n m "<<m<<" \n";

    auto start = std::chrono::high_resolution_clock::now();

    int i, j, k, l, p, t, w, c=1, numt=4, q=k_input, xi, xj, yi, yj;
    bool flag; vector<int> temp(m*m);

    for(i=0; i<q; i++){
        k = rm_input[i+1].first, l = rm_input[i+1].second;
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
    for(int i=0; i<g; i++) compress_ans_matrix[i].resize(m*m, 0);
    for(int i=0, c=1; i<n/m; i++)
        for(int j=i; j<n/m; j++)
            m_ans[{i, j}] = c++;

    cout<<k_input<<" DE\n";

    c=1;
    for(i=1; i<=k_input; i++){
        xi = rm_input[i].first, yi = rm_input[i].second;
        for(j=1; j<=k_input; j++){
            xj = rm_input[j].first, yj = rm_input[j].second;
            if(yi != xj or xi > yj) continue;
            for(k=0; k<m; k++){
                for(l=0; l<m; l++){
                    t = 0;
                    for(p=0; p<m; p++)
                        t = Outer(t, Inner(in_matrix[i-1][k*m+p], in_matrix[j-1][p*m+l]));
                    temp[k*m+l] = t;
                }
            }
            if(m_ans[{xi, yj}] == 0) m_ans[{xi, yj}] = c++;
            for(k=0; k<m*m; k++){
                temp[k] = Outer(compress_ans_matrix[m_ans[{xi, yj}]-1][k], temp[k]);
            }
            compress_ans_matrix[m_ans[{xi, yj}]-1] = temp;
        }
    }
    cout<<compress_ans_matrix.size()<<" my k\n";

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time taken: " << duration << " microseconds" << std::endl;
}

int main() {
    read_file();

    // matrix_ope();
    matrix();

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
        //     cout<<compress_ans_matrix[m_ans[{iIndex, jIndex}]-1][j]<<" ";
        // } cout<<endl;
        for (int j = 0; j < m * m; j++){
            out_file.read((char *)&out_matrix[i][j], sizeof(short)); // here short 2 bytes
            // cout << out_matrix[i][j] << " ";
            if(out_matrix[i][j] != compress_ans_matrix[m_ans[{iIndex, jIndex}]-1][j]) cout<<"NOT CORRECT #########################################################\n";
        }
        // cout << endl;
    }
    out_file.close();

    return 0;
}
