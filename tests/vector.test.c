// written by Claude AI

#include <assert.h>
#include <string.h>
#include "../include/vector.h" // Assuming the header file is named vector.h

// Test structure to verify the library works with custom types
typedef struct {
    int id;
    char name[32];
} Person;

void test_int_vector() {
    printf("Testing integer vector...\n");
    
    // Initialize vector
    vec_t vec;
    VEC_MAKE(&vec, int);
    
    // Test initial state
    assert(vec_size(&vec) == 0);
    assert(vec_empty(&vec) == true);
    
    // Test pushing elements
    for (int i = 0; i < 10; i++) {
        VEC_PUSH(&vec, int, i);
        assert(vec_size(&vec) == (usize)(i + 1));
        assert(*(int*)VEC_GET(&vec, int, i) == i);
    }
    
    // Test back access
    assert(*(int*)VEC_BACK(&vec, int) == 9);
    
    // Test popping elements
    int* popped = (int*)vec_pop(&vec);
    assert(*popped == 9);
    assert(vec_size(&vec) == 9);
    
    // Test pop_back (which doesn't return the value)
    vec_pop_back(&vec);
    assert(vec_size(&vec) == 8);
    assert(*(int*)VEC_BACK(&vec, int) == 7);
    
    // Test swapback (swaps element at index with the last element)
    vec_swapback(&vec, 2);
    assert(*(int*)VEC_GET(&vec, int, 2) == 7);
    assert(vec_size(&vec) == 7);
    
    // Test clearing the vector
    vec_clear(&vec);
    assert(vec_size(&vec) == 0);
    assert(vec_empty(&vec) == true);
    
    // Test capacity expansion by pushing more elements than the starting capacity
    for (int i = 0; i < VEC_STARTING_CAPACITY * 2; i++) {
        VEC_PUSH(&vec, int, i);
    }
    assert(vec_size(&vec) == VEC_STARTING_CAPACITY * 2);
    
    // Clean up
    vec_destroy(&vec);
    printf("Integer vector tests passed!\n");
}

void test_struct_vector() {
    printf("Testing struct vector...\n");
    
    // Initialize vector for Person structs
    vec_t persons;
    VEC_MAKE(&persons, Person);
    
    // Push some person records
    for (int i = 0; i < 5; i++) {
        Person p = {.id = i, .name = ""};
        sprintf(p.name, "Person %d", i);
        VEC_PUSH(&persons, Person, p);
    }
    
    assert(vec_size(&persons) == 5);
    
    // Test retrieving elements
    for (int i = 0; i < 5; i++) {
        Person* p = VEC_GET(&persons, Person, i);
        assert(p->id == i);
        char expected_name[32] = "";
        sprintf(expected_name, "Person %d", i);
        assert(strcmp(p->name, expected_name) == 0);
    }
    
    // Test memory size
    assert(vec_mem_size(&persons) == 5 * sizeof(Person));
    
    // Clean up
    vec_destroy(&persons);
    printf("Struct vector tests passed!\n");
}

void test_edge_cases() {
    printf("Testing edge cases...\n");
    
    // Test with empty vector
    vec_t vec;
    VEC_MAKE(&vec, double);
    
    // Back on empty vector should return NULL
    assert(vec_back(&vec) == NULL);
    
    // Clear empty vector
    vec_clear(&vec);
    assert(vec_size(&vec) == 0);
    
    // Push and then get element at index 0
    double val = 3.14;
    VEC_PUSH(&vec, double, val);
    assert(*(double*)VEC_GET(&vec, double, 0) == val);
    
    vec_destroy(&vec);
    printf("Edge case tests passed!\n");
}

int main() {
    printf("Running vector library tests\n");
    
    test_int_vector();
    test_struct_vector();
    test_edge_cases();
    
    printf("All tests passed successfully!\n");
    return 0;
}
