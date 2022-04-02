 # EditdistPy

 There are already several Python packages that implement levenshtein distance, which are writtend by C or C++ to improve the efficiency. In certain occasions, besides the edit distance, the edit operations are also needed. [python-Levenshtein](https://pypi.org/project/python-Levenshtein/) has the [editops](https://rawgit.com/ztane/python-Levenshtein/master/docs/Levenshtein.html#Levenshtein-editops) function but doesn't support transpose operations. So here comes the project, **editdistpy** supports the function of edit operations under [Damerau–Levenshtein distance](https://en.wikipedia.org/wiki/Damerau%E2%80%93Levenshtein_distance).


# Install 
```python
    pip install -e .
```

# Usage
The package supports distance and edit_operations two functions. They have the same parameters  
- string a
- string b
- max_distance: Only return right result under max_distance.
- is_damerau: Whether support transpose operation or not.

## distance
```python
from edit_operation import levenshtein

a = 'absolute'
b = 'absiluti'

dis = levenshtein.distance(a, b)
```
return -1 if the actual distance is beyond max_distance, by default max_distance is -1, which means no max_distance limit.

## edit_operations

```python
from editdistpy import levenshtein

a = 'absolute'
b = 'absiluti'
operations = levenshtein.edit_operations(a, b, is_damerau=True)

"""
[{'operation': 'replace', 'location': 7, 'char_x': 'e', 'char_y': 'i'}, 
 {'operation': 'replace', 'location': 3, 'char_x': 'o', 'char_y': 'i'}]
"""
```
return empty list if the actual distance is beyond max_distance.
