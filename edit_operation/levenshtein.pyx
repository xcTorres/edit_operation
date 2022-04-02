# distutils: language = c++
# cython: language_level=3

from libcpp cimport bool
from libcpp.vector cimport vector
from libcpp.memory cimport shared_ptr

cdef extern from "_levenshtein.hpp":

    ctypedef shared_ptr[EditOperation] EditOperationSharedPtr

    int Distance(
        const char* string1,
        const char* string2,
        int maxDistance,
        bool is_damerau
    )

    vector[EditOperationSharedPtr] EditOperations(
        const char* string1,
        const char* string2,
        int max_distance,
        bool is_damerau);
    
    cdef cppclass EditOperation:
        const char* operation_type
        int index
        char x
        char y

    


cpdef distance(string1, string2, max_distance=-1, is_damerau=False):
    cdef bytes py_bytes = string1.encode()
    cdef char* c_string1 = py_bytes
    
    py_bytes = string2.encode()
    cdef char* c_string2 = py_bytes

    return Distance(c_string1, c_string2, max_distance, is_damerau)


cpdef edit_operations(string1, string2, max_distance=-1, is_damerau=False):
    cdef bytes py_bytes = string1.encode()
    cdef char* c_string1 = py_bytes
    
    py_bytes = string2.encode()
    cdef char* c_string2 = py_bytes

    operations = EditOperations(c_string1, c_string2, max_distance, is_damerau)

    return [{
        "operation": op.get().operation_type.decode("utf-8"),
        "location": op.get().index,
        "char_x": chr(op.get().x),
        "char_y": chr(op.get().y),
    } for op in operations
    ]
