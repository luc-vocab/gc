#include <application.h>

void setup(void)
{

}

void loop(void)
{
  Particle.publish("status", "hello world");
  delay(3000);
}
