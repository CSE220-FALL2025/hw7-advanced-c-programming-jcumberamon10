#include "unit_tests.h"
#include "../include/hw7.h"




#define TEST_TIMEOUT 10
TestSuite(student_tests, .timeout=TEST_TIMEOUT); 


TestSuite(student_advanced_expr, .timeout=TEST_TIMEOUT, .disabled=false);