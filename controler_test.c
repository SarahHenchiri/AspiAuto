#include "CUnit.h"
#include "Basic.h"

#include <stdio.h>

#include "controler.h"

int cnt_contact = 0;					// count number of contact

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

bool compare_wheels(wheels actual, wheels expect){
	return ((actual.left == expect.left) && (actual.right == expect.right));
}

/************* Test case functions ****************/
void straight_line(void) {
  // straight line without obstacle, gap, end
  wheels wh = controler(false,init_sens_obs(100.0,100.0,100.0,100.0),false,false,&cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(25.0,25.0)));
  // end capteur
  wh = controler(false,init_sens_obs(100.0,100.0,100.0,100.0),false,true, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(0.0,0.0)));
}

void speed_obstacle(void) {
  // full speed
  wheels wh = controler(false,init_sens_obs(100.0,100.0,100.0,100.0),false,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(25.0,25.0)));
  // obstacle error
  wh = controler(false,init_sens_obs(-100.0,-100.0,-100.0,-100.0),false,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(0.0,0.0)));
  // obstacle for all 4 sensors = 20.0cm/s
  wh = controler(false,init_sens_obs(20.0,20.0,20.0,20.0),false,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(20.0,20.0)));
  // effect of obstacle on each sensor
  wh = controler(false,init_sens_obs(15.0,100.0,100.0,100.0),false,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(15.0,15.0)));
  wh = controler(false,init_sens_obs(15.0,10.0,100.0,100.0),false,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(10.0,10.0)));
  wh = controler(false,init_sens_obs(15.0,10.0,5.0,100.0),false,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(5.0,5.0)));
  wh = controler(false,init_sens_obs(15.0,10.0,5.0,1.0),false,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(1.0,1.0)));
}

void speed_limit(void) {
  // straight line without obstacle, gap, end limited to 25
  wheels wh = controler(false,init_sens_obs(100.0,100.0,100.0,100.0),false,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(25.0,25.0)));
  // obstacle detected
  wh = controler(false,init_sens_obs(100.0,100.0,10.0,10.0),false,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(10.0,10.0)));
  // contact detected
  wh = controler(false,init_sens_obs(100.0,100.0,1.0,1.0),true,false, &cnt_contact);
  //CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(-25.0,25.0)));
}

void obstacle_contact(void) {
  // obstacle on the left
	cnt_contact=0;
  wheels wh = controler(false,init_sens_obs(1.0,1.0,100.0,100.0),true,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(15.0,-15.0)));
  // obstacle on the right
  cnt_contact=0;
  wh = controler(false,init_sens_obs(100.0,100.0,1.0,1.0),true,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(-15.0,15.0)));
  // obstacle 
  cnt_contact=0;
  wh = controler(false,init_sens_obs(1.0,1.0,1.0,1.0),true,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(15.0,-15.0)));
  // obstacle on the end
  cnt_contact=0;
  wh = controler(false,init_sens_obs(1.0,100.0,100.0,100.0),true,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(1.0,1.0)));
  cnt_contact=0;
  wh = controler(false,init_sens_obs(100.0,100.0,100.0,1.0),true,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(1.0,1.0)));
}

void wall_contact(void) {
  // counter of contact >= 0 but no new contact
	cnt_contact=0;
  wheels wh = controler(false,init_sens_obs(100.0,100.0,100.0,100.0),false,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(25.0,25.0)));
  CU_ASSERT_EQUAL(cnt_contact,0);
  cnt_contact = 1;
  wh = controler(false,init_sens_obs(10.0,10.0,10.0,10.0),false,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(10.0,10.0)));
  CU_ASSERT_EQUAL(cnt_contact,0);
  cnt_contact = 2;
  wh = controler(false,init_sens_obs(10.0,10.0,10.0,10.0),false,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(10.0,10.0)));
  CU_ASSERT_EQUAL(cnt_contact,0);
  cnt_contact = 0;
  // counter of contact >= 0 and new contact
  wh = controler(false,init_sens_obs(1.0,1.0,20.0,20.0),true,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(15,-15)));
  CU_ASSERT_EQUAL(cnt_contact,1);
  wh = controler(false,init_sens_obs(1.0,1.0,20.0,20.0),true,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(15,-15)));
  CU_ASSERT_EQUAL(cnt_contact,2);
  init_lastmove(RIGHT);
  wh = controler(false,init_sens_obs(1.0,1.0,20.0,20.0),true,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(25,-25)));
  CU_ASSERT_EQUAL(cnt_contact,0);
}

void gap_detected(void) {
  // without gap
  wheels wh = controler(false,init_sens_obs(100.0,100.0,100.0,100.0),false,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(25.0,25.0)));
  // gap detected
  init_lastmove(LEFT);
  wh = controler(true,init_sens_obs(100.0,100.0,100.0,100.0),false,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(-25.0,25.0)));
    // gap + contact
  wh = controler(true,init_sens_obs(100.0,1.0,1.0,100.0),true,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(25.0,-25.0)));
    // gap + obstacle
  wh = controler(true,init_sens_obs(100.0,10.0,10.0,100.0),false,false, &cnt_contact);
  CU_ASSERT_TRUE(compare_wheels(wh,init_wheels(-25.0,25.0)));
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
   if ( (NULL == CU_add_test(pSuite, "straight_line", straight_line))
      )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if ( (NULL == CU_add_test(pSuite, "speed_obstacle", speed_obstacle))
      )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if ( (NULL == CU_add_test(pSuite, "speed_limit", speed_limit))
      )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
	if ( (NULL == CU_add_test(pSuite, "obstacle_contact", obstacle_contact))
      )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if ( (NULL == CU_add_test(pSuite, "wall_contact", wall_contact))
      )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if ( (NULL == CU_add_test(pSuite, "gap_detected", gap_detected))
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
