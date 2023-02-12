#include <bits/stdc++.h>
// #include <iostream>
// #include <fstream>
#include "library.hpp"
#include <omp.h>

using namespace std;

const int MAX_VAL = pow(2, 16);

map<pair<int, int>, int> m_input, m_output, m_ans;
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

void matrix_ope(){
    auto start = std::chrono::high_resolution_clock::now();

    vector<int> temp(m*m);
    bool flag;
    int i, j, k, t, q, w, c=1;

    // #pragma omp parallel for private(i,j,k,t,q,w,temp,flag,c) shared(m_input, ans_matrix, in_matrix, compress_ans_matrix)   // a lot variation in time avg~23000 ms 
    #pragma omp parallel for schedule(static) num_threads(4) private(i,j,k,t,q,w,temp,flag,c)          //gives time around 10000 ms
        for(i=0; i<n; i++){
            for(j=max(i-m, 0); j<n; j++){
                t = 0;
                for(k=0; k<n; k++){
                    // // this when mapping of blocks with i>j is not done
                    // if(m_input[{min(i/m, k/m), max(i/m, k/m)}]==0 or m_input[{min(k/m, j/m), max(k/m, j/m)}]==0) continue;
                    // // when mapping of blocks with i>j is done
                    if(m_input[{i/m, k/m}]==0 or m_input[{k/m, j/m}]==0) { k+=m-1; continue;}
                    if(i/m <= k/m) q = in_matrix[m_input[{i/m, k/m}]-1][m*(i%m)+k%m];
                    else q = in_matrix[m_input[{k/m, i/m}]-1][m*(k%m)+i%m];
                    if(k/m <= j/m) w = in_matrix[m_input[{k/m, j/m}]-1][m*(k%m)+j%m];
                    else w = in_matrix[m_input[{j/m, k/m}]-1][m*(j%m)+k%m];
                    t = Outer(t, Inner(q, w));
                    t = (t > MAX_VAL - 1) ? MAX_VAL - 1 : t;
                }
                ans_matrix[i][j] = t;
            }
        }


    // auto start = std::chrono::high_resolution_clock::now();

    //     int i, j, k, l, t, q, w, c=1, fi, si, fj, sj, xi, yi, xj, yj;
    //     bool flag; vector<int> temp(m*m);

    // // #pragma omp parallel for private(i,j,k,l,t,q,w,temp,flag,c,fi,si,fj,sj,xi,yi,xj,yj) shared(rm_input, ans_matrix, in_matrix, compress_ans_matrix)   // ~ 24014 microseconds
    // // #pragma omp parallel for schedule(static) num_threads(4) private(i,j,k,l,t,q,w,temp,flag,c,fi,si,fj,sj,xi,yi,xj,yj)    // ~13127 microseconds
    //     for(i=1; i<=k_input; i++){
    //         for(j=1; j<=k_input; j++){
    //             fi = rm_input[i].first, si = rm_input[i].second;
    //             fj = rm_input[j].first, sj = rm_input[j].second;
    //             for(k=0; k<m*m; k++){
    //                 xi = fi*m + k/m, yi = si*m + k%m;
    //                 for(l=0; l<m*m; l++){  //directly calculating compress ans matrix is also possible
    //                 // try to calculate ans matrix for only upper triangle elements
    //                     xj = fj*m + l/m, yj = sj*m + l%m;
    //                     t = Inner(in_matrix[i-1][k], in_matrix[j-1][l]);
    //                     if(xj==yi) ans_matrix[xi][yj] = Outer(ans_matrix[xi][yj], t);
    //                     if(fi!=si){
    //                         if(xi==xj) ans_matrix[yi][yj] = Outer(ans_matrix[yi][yj], t);
    //                     }
    //                     if(fj!=sj){
    //                         if(yj==yi) ans_matrix[xi][xj] = Outer(ans_matrix[xi][xj], t);
    //                     }
    //                     if(fi!=si and fj!=sj){
    //                         if(xi==yj) ans_matrix[yi][xj] = Outer(ans_matrix[yi][xj], t);
    //                     }
    //                 }
    //             }
    //         }
    //     }

        for(i=0; i<n/m; i++){
            for(j=i; j<n/m; j++){
                flag=false;
                for(k=0; k<m*m; k++){
                    if(ans_matrix[i*m+k/m][j*m+k%m]>0)  flag=true;
                    temp[k] = ans_matrix[i*m+k/m][j*m+k%m];
                }
                if(flag){
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

    ans_matrix.resize(n);
    for(int i=0; i<n; i++) ans_matrix[i].resize(n, 0);

    cout<<n<<" n m "<<m<<" \n";

    matrix_ope();

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
