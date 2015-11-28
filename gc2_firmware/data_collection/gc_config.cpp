#include "gc_config.h"
#include "common.h"
#include "utils.h"

GcConfig::GcConfig(GcClient &gc_client) : m_gc_client(gc_client), p_setup_done(0) {

}

void GcConfig::init() {

  // see if configuration exists
  ConfigData cfgData;
  EEPROM.get(CONFIG_EEPROM_ADDR, cfgData);

  if(cfgData.config_set_marker == CONFIG_SET_MARKER) {
    // config has already been set
    String hostname((char *) cfgData.hostname);

    p_hostname = hostname;
    p_port = cfgData.port;
    p_device_id = cfgData.device_id;
    p_setup_done = 1;

    m_gc_client.configure(hostname, cfgData.port, cfgData.device_id);
  }
}

void GcConfig::set_config(String command) {
  // format: dev2.photozzap.com,7001,1234234
  int j = command.indexOf(",");
  int k = command.indexOf(",", j+1);

  p_hostname = command.substring(0,j);
  p_port = command.substring(j+1,k).toInt();
  p_device_id = command.substring(k+1).toInt();

  // persist this configuration to eeprom
  ConfigData cfgData;
  cfgData.port = p_port;
  cfgData.device_id = p_device_id;
  cfgData.config_set_marker = CONFIG_SET_MARKER;
  p_hostname.getBytes((unsigned char *) cfgData.hostname, MAX_HOSTNAME_LENGTH);

  p_setup_done = 1;

  EEPROM.put(CONFIG_EEPROM_ADDR, cfgData);

  m_gc_client.configure(cfgData.hostname, cfgData.port, cfgData.device_id);

  DEBUG_LOG("config: hostname: " + p_hostname + " port: " + String(p_port) + " deviceId: " + String(p_device_id));
  validation_tone();


}
