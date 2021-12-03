#ifndef LIGHT_H
#define LIGHT_H

class LightClass {
public:
  LightClass();
  void begin();
  void run_task(void (*task)(void *));
  /**
   * @brief Sets the light level
   *
   * @param level Light level in percent
   * @param time time (optional) in seconds
   */
  void set_level(float level, float time = 0);

private:
  LightClass(const LightClass &) = delete;
  LightClass &operator=(const LightClass &) = delete;

  TaskHandle_t task_handle = nullptr;
  float current_level = 0;
};

extern LightClass Light;

#endif