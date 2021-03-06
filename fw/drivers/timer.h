#ifndef TIMER_H
#define TIMER_H

extern volatile uint32_t g_timer_ms_ticks;

void timer_init(TC0_t* timer, uint8_t int_level);
void timer_reset(TC0_t* timer);
void timer_set(TC0_t* timer, uint8_t prescaler, uint16_t period);
void timer_sleep_ms(uint16_t ms);
uint32_t timer_elapsed_ms(uint32_t prev, uint32_t curr);
void timer_rtc_set(uint32_t secs);
void timer_rtc_get(uint32_t* s, uint32_t* ms);
void timer_rtc_ms_update();

#endif
