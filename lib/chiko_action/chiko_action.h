#ifndef __CHIKO_ACTION__
#define __CHIKO_ACTION__

#define ACTION_TASK_PRIORITY 3

#include <RTOS.h>

#define U_LONGLONGMAX 18446744073709551615ULL

struct routines{
  void (*EnterRoutine)();
  void (*LoopRoutine)(); 
  void (*ExitRoutine)(); 
};

class action{
  private:
  public:
  TaskHandle_t actionTaskHandle;
  routines taskRoutines;
  bool executeAction = false;
  unsigned long long LoopItrations = U_LONGLONGMAX;
  void create(void (*EnterRoutine)(), void (*LoopRoutine)(), void (*ExitRoutine)() );
  void begin(unsigned long long MaxLoopCount = U_LONGLONGMAX);
  void stop();
  
};



#endif