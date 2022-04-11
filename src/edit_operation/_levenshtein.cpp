#include <iostream>
 #include <cstring>
#include "_levenshtein.hpp"

using namespace std;

const char** EditOperationType = new const char* [4]{"delete", "insert", "replace", "transpose"};

int Distance(
    const char* string1
  , const char* string2
  , int max_distance
  , bool is_damerau)
{

    int strlen1 = strlen(string1), strlen2 = strlen(string2);
    if (max_distance>=0 && abs(strlen2-strlen1) > max_distance) {
        return -1;
    }   

    if (strlen1 > strlen2) {
        std::swap(string1, string2);
        std::swap(strlen1, strlen2);
    }

    // Trim suffix
    while (strlen1 != 0
        && string1[strlen1-1] == string2[strlen2-1]) {
        --strlen1;
        --strlen2;
    }

    if (max_distance >= 0 && strlen1 == 0) {
        return strlen2 <= max_distance ? strlen2 : -1;
    }

    // Trim prefix
    int start = 0;
    while (start != strlen1 && string1[start] == string2[start]) {
        ++start;
    }
    if (start != 0) {
        strlen1 -= start;
        strlen2 -= start;
    }

    if(max_distance >= 0 && strlen1 == 0){
        return strlen2 <= max_distance ? strlen2 : -1;
    }

    int** dp = InternalDistance(string1, string2, strlen1, strlen2, start, max_distance, is_damerau);
    if(dp == NULL){
        return -1;
    }else{
        int distance = dp[strlen1][strlen2];
        for(int i = 0; i <= strlen1; ++i) {
            delete[] dp[i];   
        }
        delete[] dp;
        return distance;
    }

}


int** InternalDistance(
  const char* string1,
  const char* string2,
  const int strlen1, 
  const int strlen2,
  int start,
  int max_distance,
  bool is_damerau)
{
    // Dynamic allocate the memory for dp array
    int** dp = new int*[strlen1+1];
    for(int i=0; i<=strlen1; i++)
        dp[i] = new int[strlen2+1];

    for(int i=0; i<=strlen1; i++)
        dp[i][0] = i;
    
    for(int j=0; j<=strlen2; j++)
        dp[0][j] = j;

    int len_diff = strlen2 - strlen1;
    for(int i=1; i<=strlen1; i++){
        for(int j=1; j<=strlen2; j++){
            if(string1[start+i-1] == string2[start+j-1])
                dp[i][j] = dp[i-1][j-1];
            else
                dp[i][j] = min(dp[i-1][j-1], min(dp[i][j-1], dp[i-1][j])) + 1;
            
            if(is_damerau){
                if(i>1 && j>1 && string1[start+i-1]==string2[start+j-2] && 
                                 string1[start+i-2]==string2[start+j-1])
                    dp[i][j] = min(dp[i][j], dp[i-2][j-2] + 1); // transposition 
            }
        }

        // Early stop
        if(max_distance>=0 && dp[i][i+len_diff] > max_distance){
            //Free each sub-array
            for(int i = 0; i <= strlen1; ++i) {
                delete[] dp[i];   
            }
            delete[] dp;
            return NULL;
        }
    }
    
    return dp;
}

vector<EditOperationSharedPtr> EditOperations(
  const char* string1,
  const char* string2,
  int max_distance,
  bool is_damerau){
    
    int strlen1 = strlen(string1), strlen2 = strlen(string2);
    if (max_distance>=0 && abs(strlen2-strlen1) > max_distance) {
        return vector<EditOperationSharedPtr>();
    }   

    bool is_transpose = false;
    if (strlen1 > strlen2) {
        std::swap(string1, string2);
        std::swap(strlen1, strlen2);
        is_transpose = true;
    }

    // Trim suffix
    while (strlen1 != 0
        && string1[strlen1-1] == string2[strlen2-1]) {
        --strlen1;
        --strlen2;
    }

    if (max_distance >= 0 && strlen1 == 0 && strlen2 > max_distance) {
        return vector<EditOperationSharedPtr>();
    }

    // Trim prefix
    int start = 0;
    while (start != strlen1 && string1[start] == string2[start]) {
        ++start;
    }
    if (start != 0) {
        strlen1 -= start;
        strlen2 -= start;
    }

    if(max_distance >= 0 && strlen1 == 0 && strlen2 > max_distance){
        return vector<EditOperationSharedPtr>();
    }


    int** dp = InternalDistance(string1, string2, strlen1, strlen2, start, max_distance, is_damerau);
    if(dp == NULL)
        return vector<EditOperationSharedPtr>();
        
    if(is_transpose){
        int** transpose_dp = new int*[strlen2+1];
        for(int i=0; i<=strlen2; i++)
            transpose_dp[i] = new int[strlen1+1];

        for(int i=0; i<=strlen1; i++)
            for(int j=0; j<=strlen2; j++){
                transpose_dp[j][i] = dp[i][j];
            }

        for(int i = 0; i <= strlen1; ++i) {
            delete[] dp[i];   
        }
        delete[] dp;
        return EditOperationsFromMatrix(string2, string1, strlen2, strlen1, 
                                        start, transpose_dp, max_distance, is_damerau);
    }

    return EditOperationsFromMatrix(string1, string2, strlen1, strlen2, 
                                    start, dp, max_distance, is_damerau);
  }


vector<EditOperationSharedPtr> EditOperationsFromMatrix(
  const char* string1,
  const char* string2,
  const int len1, 
  const int len2,
  int start,
  int** dp,
  int max_distance,
  bool is_damerau){

    int m=len1, n=len2;
    vector<EditOperationSharedPtr> operations;

    // EditOperationSharedPtr op(new EditOperation(OperationToInt(insertion), 0, '@', 1));
    // operations.push_back(std::move(op));
    while(n>=0 || m>=0){
        // transportation
        if(is_damerau)
            if(m>1 && n>1 && string1[start+m-1]==string2[start+n-2] && string1[start+m-2]==string2[start+n-1] 
              && dp[m-2][n-2]+1 == dp[m][n]){
                  EditOperationSharedPtr op(new EditOperation(EditOperationType[3], start+m-2, 
                                                                       string1[start+m-2], string2[start+n-2]));
                  operations.push_back(std::move(op));
                  m -= 2;
                  n -= 2;
                  continue;
              } 
        
        // insertion
        if(n>0 && dp[m][n-1]+1 == dp[m][n]){
            EditOperationSharedPtr op;
            if(start+m <= 0){
                op = EditOperationSharedPtr(new EditOperation(EditOperationType[1], start+m, '@', string2[start+n-1]));
            }else{
                op = EditOperationSharedPtr(new EditOperation(EditOperationType[1], start+m, string1[start+m-1], string2[start+n-1]));
            }
            operations.push_back(std::move(op));
            n -= 1;
            continue;
        }

        // deletion
        if(m>0 && dp[m-1][n]+1 == dp[m][n]){
            EditOperationSharedPtr op;
            if(start+m-1 <= 0){
                op = EditOperationSharedPtr(new EditOperation(EditOperationType[0], start+m-1, '@', string1[start+m-1]));
                
            }else{
                op = EditOperationSharedPtr(new EditOperation(EditOperationType[0], start+m-1, string1[start+m-2], string1[start+m-1]));
            }
            operations.push_back(std::move(op));
            m -= 1;
            continue;
        }

        // substitution
        if(m>0 && n>0 && dp[m-1][n-1]+1 == dp[m][n]){
            EditOperationSharedPtr op(new EditOperation(EditOperationType[2], start+m-1, string1[start+m-1], string2[start+n-1]));
            operations.push_back(std::move(op));
            m -= 1;
            n -= 1;
            continue;
        }
        
        m -= 1;
        n -= 1;
    }

    for(int i = 0; i <= len1; ++i) {
        delete[] dp[i];   
    }
    delete[] dp;

    return operations;
}

 