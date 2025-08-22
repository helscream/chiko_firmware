#include "action.h"



static void actionTask(void *param){
  action* thisAction;
  thisAction = (action*) param;
  thisAction->taskRoutines.EnterRoutine();
  while(thisAction->executeAction ){
    thisAction->taskRoutines.LoopRoutine();
    
    if (thisAction->LoopItrations < U_LONGLONGMAX && thisAction->LoopItrations != 0){
      thisAction->LoopItrations--;
    }else if (thisAction->LoopItrations == 0) {
      thisAction->executeAction = false;
    }
  }
  thisAction->taskRoutines.ExitRoutine();
  thisAction->actionTaskHandle = NULL;
  vTaskDelete(xTaskGetCurrentTaskHandle());
}

 void action::create(void (*EnterRoutine)(), void (*LoopRoutine)(), void (*ExitRoutine)() ){
    
    taskRoutines.EnterRoutine = EnterRoutine;
    taskRoutines.LoopRoutine = LoopRoutine;
    taskRoutines.ExitRoutine = ExitRoutine;
    
 }

 void action::begin(unsigned long long MaxLoopCount){
  executeAction = true;
  LoopItrations = MaxLoopCount;
  if (actionTaskHandle == NULL){
    xTaskCreate(actionTask, "ActionTask", 5000, this, ACTION_TASK_PRIORITY,&actionTaskHandle);
  }
 }

 void action::stop(void){
  executeAction = false;
 }