/*
 * test_suite.c
 *
 *  Created on: Apr 27, 2026
 *      Author: Alexander
 */

#include <test_suite.h>
#include <stdio.h>
#include <attounit.h>


TEST_MAIN()
void test_suite(void) {
	int code = attomain();
	printf("Attotest exited with code %d\n", code);
}
