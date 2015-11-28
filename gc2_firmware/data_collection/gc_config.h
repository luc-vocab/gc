#ifndef _GC_CONFIG_H_
#define _GC_CONFIG_H_

#define CONFIG_EEPROM_ADDR 0
#define MAX_HOSTNAME_LENGTH 40
#define CONFIG_SET_MARKER 57376

#include "gc_client.h"

struct ConfigData {
  char hostname[MAX_HOSTNAME_LENGTH];
  int port;
  uint32_t device_id;
  uint16_t config_set_marker;
};

class GcConfig {
public:
  GcConfig(GcClient &gc_client);
  void init();
  void set_config(String command);

  // will be exposed as cloud variables
  String p_hostname;
  int p_port;
  uint32_t p_device_id;
  int p_setup_done;
private:
  GcClient &m_gc_client;
};

#endif /* _GC_CONFIG_H_ */
