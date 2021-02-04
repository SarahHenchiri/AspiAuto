#include "CUnit.h"
#include "Basic.h"

#include <stdio.h>

#include "controler.h"


int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }


/************* Test case functions ****************/
void speed_obstacle(void) {
  // full speed
  CU_ASSERT_EQUAL(controler(false, init_sens_obs(100.0,100.0,100.0,100.0),false,false),  init_wheels(25.0,25.0));
  // obstacle error
  CU_ASSERT_EQUAL(controler(false, init_sens_obs(-100.0,-100.0,-100.0,-100.0),false,false),  init_wheels(0.0,0.0));
  // obstacle for all 4 sensors = 20.0cm/s
  CU_ASSERT_EQUAL(controler(false, init_sens_obs(20.0,20.0,20.0,20.0),false,false),  init_wheels(25.0,25.0));
  // effect of obstacle on each sensor
  CU_ASSERT_EQUAL(controler(false, init_sens_obs(15.0,100.0,100.0,100.0),false,false),  init_wheels(15.0,15.0));
  CU_ASSERT_EQUAL(controler(false, init_sens_obs(15.0,10.0,100.0,100.0),false,false),  init_wheels(10.0,10.0));
  CU_ASSERT_EQUAL(controler(false, init_sens_obs(15.0,10.0,5.0,100.0),false,false),  init_wheels(5.0,5.0));
  CU_ASSERT_EQUAL(controler(false, init_sens_obs(15.0,10.0,5.0,1.0),false,false),  init_wheels(1.0,1.0));
}


/************* Test Runner Code goes here **************/
int main ( void )
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if ( CUE_SUCCESS != CU_initialize_registry() )
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite( "controler_test_suite", init_suite, clean_suite );
   if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ( (NULL == CU_add_test(pSuite, "speed_obstacle", speed_obstacle))
      )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   // Run all tests using the basic interface
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   printf("\n");
   CU_basic_show_failures(CU_get_failure_list());
   printf("\n\n");

   /* Clean up registry and return */
   CU_cleanup_registry();
   return CU_get_error();
}
