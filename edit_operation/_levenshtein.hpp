#ifndef EDITDISTPY__LEVENSHTEIN_HPP_
#define EDITDISTPY__LEVENSHTEIN_HPP_

#include<vector>
#include<memory>
using namespace std;

int Distance(
  const char* string1,
  const char* string2,
  int max_distance,
  bool is_damerau);

int** InternalDistance(
  const char* string1,
  const char* string2,
  const int len1, 
  const int len2,
  int start,
  int max_distance,
  bool is_damerau);


struct EditOperation{
  public:
    const char* operation_type;
    int index;
    char x;
    char y;
  
  public:
    EditOperation();
    ~EditOperation(){};

    EditOperation(const char* operation_type, int index,
                  char x, char y){
      this->operation_type = operation_type;
      this->index = index;
      this->x = x;
      this->y = y;
    }

};

typedef std::shared_ptr<EditOperation> EditOperationSharedPtr;

vector<EditOperationSharedPtr> EditOperations(
  const char* string1,
  const char* string2,
  int max_distance,
  bool is_damerau);


vector<EditOperationSharedPtr> EditOperationsFromMatrix(
  const char* string1,
  const char* string2,
  const int len1, 
  const int len2,
  int start,
  int** dp,
  int max_distance,
  bool is_damerau);

#endif  // EDITDISTPY__LEVENSHTEIN_HPP_
