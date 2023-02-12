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
        // m_input[{jIndex, iIndex}] = i+1;
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

    int i, j, k, l, p, t, w, c=1, numt=4, q=k_input;
    bool flag; vector<int> temp(m*m);

    for(i=0; i<q; i++){
        k = rm_input[i+1].first, l = rm_input[i+1].second;
        if(k==l) { continue;}
        for(j=0; j<m*m; j++) 
            temp[j] = in_matrix[i][m*(j%m)+j/m];
        in_matrix.push_back(temp);
        m_input[{l, k}] = 1+k_input++;
    }

    int size = k_input * k_input;
    ans_matrix.resize(size);
    for(int i=0; i<size; i++) ans_matrix[i].resize(m*m, 0);

    cout<<k_input<<" DE\n";

    for(i=1; i<=k_input; i++){
        for(j=1; j<=k_input; j++){
            for(k=0; k<m; k++){
                for(l=0; l<m; l++){
                    t = 0;
                    for(p=0; p<m; p++){
                        t = Outer(t, Inner(in_matrix[i-1][k*m+p], in_matrix[j-1][p*m+l]));
                    }
                    // cout<<"...... ";
                    ans_matrix[c-1][k*m+l] = t;
                    // cout<<t<<" CR\n";
                    // cout<<"%%%%%%\n";
                }
            }
            m_temp[{i, j}] = c++;
        }
    }

    // cout<<c<<" HFGRGR\n";

    // cout<<m_input[{0, 3}]<<"  "<<m_input[{3, 0}]<<" jbg "<<m_temp[{m_input[{0, 3}], m_input[{3, 0}]}]<<" "<<m_temp[{m_input[{3, 0}], m_input[{0, 3}]}]<<endl;
    // int x1 = m_temp[{m_input[{0, 3}], m_input[{3, 0}]}], x2 = m_temp[{m_input[{3, 0}], m_input[{0, 3}]}];
    // for(i=0; i<m*m; i++) cout<<in_matrix[m_input[{0, 3}]-1][i]<<" "; cout<<endl;
    // for(i=0; i<m*m; i++) cout<<in_matrix[m_input[{3, 0}]-1][i]<<" "; cout<<endl;
    // for(i=0; i<m*m; i++) cout<<ans_matrix[x1][i]<<" "; cout<<endl;
    // for(i=0; i<m*m; i++) cout<<ans_matrix[x2][i]<<" "; cout<<endl;



    c=1;
    for(i=0; i<n/m; i++){
        for(j=i; j<n/m; j++){
            flag=false;
            for(k=0; k<m*m; k++){
                // if(ans_matrix[i*m+k/m][j*m+k%m]>0)  flag=true;
                // temp[k] = ans_matrix[i*m+k/m][j*m+k%m];
                t=0;
                for(l=0; l<n/m; l++){
                    if(m_input[{i, l}]>0 and m_input[{l, j}]>0){
                        // cout<<i<<"  "<<j<<" "<<l<<" * "<<m_input[{i, l}]<<" "<<m_input[{l, j}]<<" t "<<m_temp[{m_input[{i, l}], m_input[{l, j}]}]<<" "<<ans_matrix[m_temp[{m_input[{i, l}], m_input[{l, j}]}]][k]<<endl;
                        t = Outer(t, ans_matrix[m_temp[{m_input[{i, l}], m_input[{l, j}]}]-1][k]);
                    }
                }
                if(t>0){
                    temp[k]=t; flag=true;
                }
            }
            if(flag){
                // cout<<i<<" "<<j<<endl;
                // for(auto &e:temp) cout<<e<<" "; cout<<endl;
                m_ans[{i, j}] = c++;
                compress_ans_matrix.push_back(temp);
            }
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
