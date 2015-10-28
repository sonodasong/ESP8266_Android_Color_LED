#include <avr/interrupt.h>
#include <ucos_ii.h>

/*******************************************************************************************************/

#if OS_TMR_EN > 0u
static INT16U OSTmrCtr;

#if OS_TIME_TICK_HOOK_EN == 0u
#error "enable OSTimeTickHook() in order to call OSTmrSignal()"
#endif

#if OS_SCHED_LOCK_EN == 0u
#error "enable schedule lock in order to use timer"
#endif

#endif

/*******************************************************************************************************/

OS_STK *OSTaskStkInit(void (*task)(void *pd), void *pdata, OS_STK *ptos, INT16U opt)
{
	INT16U  tmp;

	(void)opt;                              /* 'opt' is not used, prevent warning                       */
	tmp      = (INT16U)OS_TaskReturn;       /* Put task start address on top of "hardware stack"        */
	*ptos--  = (INT8U)(tmp & 0xFF);
	tmp      = tmp >> 8;
	*ptos--  = (INT8U)(tmp & 0xFF);
	tmp      = (INT16U)task;                /* Save PC return address                                   */
	*ptos--  = (INT8U)(tmp & 0xFF);
	tmp      = tmp >> 8;
	*ptos--  = (INT8U)(tmp & 0xFF);

	*ptos--  = (INT8U)0x00;                 /* R0    = 0x00                                             */
	*ptos--  = (INT8U)0x00;                 /* R1    = 0x00                                             */
	*ptos--  = (INT8U)0x02;                 /* R2    = 0x02                                             */
	*ptos--  = (INT8U)0x03;                 /* R3    = 0x03                                             */
	*ptos--  = (INT8U)0x04;                 /* R4    = 0x04                                             */
	*ptos--  = (INT8U)0x05;                 /* R5    = 0x05                                             */
	*ptos--  = (INT8U)0x06;                 /* R6    = 0x06                                             */
	*ptos--  = (INT8U)0x07;                 /* R7    = 0x07                                             */
	*ptos--  = (INT8U)0x08;                 /* R8    = 0x08                                             */
	*ptos--  = (INT8U)0x09;                 /* R9    = 0x09                                             */
	*ptos--  = (INT8U)0x10;                 /* R10   = 0x10                                             */
	*ptos--  = (INT8U)0x11;                 /* R11   = 0x11                                             */
	*ptos--  = (INT8U)0x12;                 /* R12   = 0x12                                             */
	*ptos--  = (INT8U)0x13;                 /* R13   = 0x13                                             */
	*ptos--  = (INT8U)0x14;                 /* R14   = 0x14                                             */
	*ptos--  = (INT8U)0x15;                 /* R15   = 0x15                                             */
	*ptos--  = (INT8U)0x16;                 /* R16   = 0x16                                             */
	*ptos--  = (INT8U)0x17;                 /* R17   = 0x17                                             */
	*ptos--  = (INT8U)0x18;                 /* R18   = 0x18                                             */
	*ptos--  = (INT8U)0x19;                 /* R19   = 0x19                                             */
	*ptos--  = (INT8U)0x20;                 /* R20   = 0x20                                             */
	*ptos--  = (INT8U)0x21;                 /* R21   = 0x21                                             */
	*ptos--  = (INT8U)0x22;                 /* R22   = 0x22                                             */
	*ptos--  = (INT8U)0x23;                 /* R23   = 0x23                                             */
	tmp      = (INT16U)pdata;
	*ptos--  = (INT8U)(tmp & 0xFF);         /* 'p_arg' passed in R24:R25                                */
	tmp      = tmp >> 8;
	*ptos--  = (INT8U)(tmp & 0xFF);
	*ptos--  = (INT8U)0x26;                 /* R26 X = 0x26                                             */
	*ptos--  = (INT8U)0x27;                 /* R27   = 0x27                                             */
	*ptos--  = (INT8U)0x28;                 /* R28 Y = 0x28                                             */
	*ptos--  = (INT8U)0x29;                 /* R29   = 0x29                                             */
	*ptos--  = (INT8U)0x30;                 /* R30 Z = 0x30                                             */
	*ptos--  = (INT8U)0x31;                 /* R31   = 0x31                                             */
	*ptos--  = (INT8U)0x80;                 /* SREG  = Interrupts enabled                               */
	
	return ((OS_STK *)ptos);
}

/*******************************************************************************************************/

#if OS_CPU_HOOKS_EN > 0u
void  OSTaskCreateHook (OS_TCB *ptcb)
{
#if OS_APP_HOOKS_EN > 0u
	App_TaskCreateHook(ptcb);
#else
	(void)ptcb;
#endif
}
#endif

#if OS_CPU_HOOKS_EN > 0u
void  OSTaskDelHook (OS_TCB *ptcb)
{
#if OS_APP_HOOKS_EN > 0u
	App_TaskDelHook(ptcb);
#else
	(void)ptcb;
#endif
}
#endif

#if (OS_CPU_HOOKS_EN > 0u) && (OS_TASK_SW_HOOK_EN > 0u)
void  OSTaskSwHook (void)
{
#if OS_APP_HOOKS_EN > 0u
	App_TaskSwHook();
#endif
}
#endif

#if OS_CPU_HOOKS_EN > 0u
void  OSTaskIdleHook (void)
{
#if OS_APP_HOOKS_EN > 0u
	App_TaskIdleHook();
#endif
}
#endif

#if OS_CPU_HOOKS_EN > 0u
void  OSTaskStatHook (void)
{
#if OS_APP_HOOKS_EN > 0u
	App_TaskStatHook();
#endif
}
#endif

#if (OS_CPU_HOOKS_EN > 0u) && (OS_TIME_TICK_HOOK_EN > 0u)
void  OSTimeTickHook (void)
{
#if OS_APP_HOOKS_EN > 0u
	App_TimeTickHook();
#endif

#if OS_TMR_EN > 0u
	OSTmrCtr++;
	if (OSTmrCtr >= (OS_TICKS_PER_SEC / OS_TMR_CFG_TICKS_PER_SEC)) {
		OSTmrCtr = 0;
		OSTmrSignal();
	}
#endif
}
#endif

#if OS_CPU_HOOKS_EN > 0u
void  OSInitHookBegin (void)
{
#if OS_TMR_EN > 0u
	OSTmrCtr = 0;
#endif
}
#endif

#if OS_CPU_HOOKS_EN > 0u
void  OSInitHookEnd (void)
{
}
#endif

#if OS_CPU_HOOKS_EN > 0u
void  OSTCBInitHook (OS_TCB *ptcb)
{
#if OS_APP_HOOKS_EN > 0u
	App_TCBInitHook(ptcb);
#else
	(void)ptcb;
#endif
}
#endif

#if OS_CPU_HOOKS_EN > 0u
void  OSTaskReturnHook (OS_TCB  *ptcb)
{
#if OS_APP_HOOKS_EN > 0u
	App_TaskReturnHook(ptcb);
#else
	(void)ptcb;
#endif
}
#endif

ISR(TIMER1_COMPA_vect)
{
	OSIntEnter();
	OSTimeTick();
	OSIntExit();
}