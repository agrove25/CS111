#include <stdio.h>
#include <mraa.h>
#include <signal.h>

void message() 
{
  printf("Hello!\n"); 
}

int main() {
  mraa_gpio_context button;
  button = mraa_gpio_init(60);
  mraa_gpio_dir(button, MRAA_GPIO_IN);

  mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, &message, NULL);

  while(1) {
    sleep(1);
  }

  return 0;
}