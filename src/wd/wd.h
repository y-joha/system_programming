/*********************************
 * Reviewer: Ishmor				
 * Author: Yohai Azoulai			
 * File: wd.c					
 * ******************************/

#ifndef __WD_H__
#define __WD_H__


#define ENV_NAME "WD_PID"

#include <stdio.h>          /*prints*/

#include "scheduler.h" 

void *MakeMeImmortal(int argc, char **argv);

void DoNotRessurect(void *arg);

#endif 

