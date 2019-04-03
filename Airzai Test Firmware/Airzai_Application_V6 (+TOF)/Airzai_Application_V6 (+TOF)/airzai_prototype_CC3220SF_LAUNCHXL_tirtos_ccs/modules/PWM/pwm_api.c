#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

/* PWM Driver Header files */
#include <ti/drivers/PWM.h>

#include "board.h"
#include "pwm_api.h"

static PWM_Params pwmParams;
static PWM_Handle pwm0 = NULL;
static PWM_Handle pwm1 = NULL;

/* units in microseconds */
static uint32_t   pwmPeriod = 9;
static uint32_t   duty = 5;

void api_pwm_init(void)
{
    PWM_init();

    /* PWM Params init */
    PWM_Params_init(&pwmParams);
    pwmParams.dutyUnits = PWM_DUTY_US;
    pwmParams.dutyValue = duty;
    pwmParams.periodUnits = PWM_PERIOD_US;
    pwmParams.periodValue = pwmPeriod;

    /* Open PWM0 */
    pwm0 = PWM_open(Board_PWM0, &pwmParams);
    api_pwm0_disable();
    if (!pwm0)
    {
        //Display_printf(display, 0, 0, "Failed to initialized PWM0.\n");
        while (1);
    }
    /* Open PWM1 */
    pwm1 = PWM_open(Board_PWM1, &pwmParams);
      api_pwm1_disable();
      if (!pwm1)
      {
          //Display_printf(display, 0, 0, "Failed to initialized PWM0.\n");
          while (1);
      }
}

void api_pwm0_enable(void)
{
    PWM_start(pwm0);
    /* set the PWM duty and start the capture */
    PWM_setDuty(pwm0, duty);
}

void api_pwm1_enable(void)
{
    PWM_start(pwm1);
    /* set the PWM duty and start the capture */
    PWM_setDuty(pwm1, duty);
}

void api_pwm0_disable(void)
{
    PWM_stop(pwm0);

}

void api_pwm1_disable(void)
{
    PWM_stop(pwm1);

}


