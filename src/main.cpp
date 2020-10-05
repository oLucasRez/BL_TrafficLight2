#include "mbed.h"

enum State
{
  RED,
  GREEN,
  YELLOW,
  ALLERT,
  OFF
};
State state = RED;

InterruptIn button(p5);
DigitalOut red_light(p6);
DigitalOut green_light(p7);
DigitalOut yellow_light(p8);
Timeout timeout;
Ticker ticker;

float time_press = 0;
bool press_flag = false;

State previous_state = RED;

void to_red();
void to_green();
void to_yellow();
void to_allert();
void to_previous();
void to_off();
void start_press();
void finish_press();
void clear_all();

int main()
{
  to_red();
  button.fall(&finish_press);
  button.rise(&start_press);
  while (1)
  {
    if (press_flag)
      time_press += 0.1;
    wait(0.1);
  }
}

void start_press()
{
  time_press = 0;
  press_flag = true;
}

void finish_press()
{
  printf("button pressed for %.1fs\n", time_press);
  if (time_press <= 3)
  {
    if (state == GREEN)
      to_yellow();
  }
  else if (time_press <= 10)
  {
    if (state == ALLERT)
      to_previous();
    else
      to_allert();
  }
  else
  {
    if (state == OFF)
      to_red();
    else
      to_off();
  }
  press_flag = false;
}

void to_red()
{
  clear_all();
  red_light = 1;
  state = RED;
  timeout.attach(&to_green, 10);
  printf("state RED\n");
}

void to_green()
{
  clear_all();
  green_light = 1;
  state = GREEN;
  timeout.attach(&to_yellow, 20);
  printf("state GREEN\n");
}

void to_yellow()
{
  clear_all();
  yellow_light = 1;
  state = YELLOW;
  timeout.attach(&to_red, 4);
  printf("state YELLOW\n");
}

void yellow_blink()
{
  yellow_light = !yellow_light;
}
void to_allert()
{
  clear_all();
  previous_state = state;
  state = ALLERT;
  ticker.attach(&yellow_blink, 0.5);
  printf("state ALLERT\n");
}

void to_previous()
{
  clear_all();
  switch (previous_state)
  {
  case RED:
    to_red();
    break;
  case GREEN:
    to_green();
    break;
  case YELLOW:
    to_yellow();
    break;
  case OFF:
    to_off();
    break;
  }
}

void to_off()
{
  clear_all();
  state = OFF;
  printf("state OFF\n");
}

void clear_all()
{
  red_light = 0;
  green_light = 0;
  yellow_light = 0;
  timeout.detach();
  ticker.detach();
}