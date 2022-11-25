
#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"

#include <string.h>
#include <stdio.h> /* For printf() */

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Configuration */
#define SEND_INTERVAL (8 * CLOCK_SECOND)
//static linkaddr_t dest_addr ={{ 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 }};
static linkaddr_t dest_addr ={{ 0x36, 0x29, 0x93, 0x1c, 0x00, 0x74, 0x12, 0x00 }};

//static linkaddr_t dest_addr = {{ 0x01, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00 }};
#if MAC_CONF_WITH_TSCH
#include "net/mac/tsch/tsch.h"
static linkaddr_t coordinator_addr = {{ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};
#endif /* MAC_CONF_WITH_TSCH */

/*---------------------------------------------------------------------------*/
PROCESS(nullnet_example_process, "NullNet unicast example");
AUTOSTART_PROCESSES(&nullnet_example_process);

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(nullnet_example_process, ev, data)
{
  //static char str[12] = "Hello-World!"
  static struct etimer periodic_timer;
  static int count = 258;
  uint8_t payload[64] = {0};
  PROCESS_BEGIN();
#if MAC_CONF_WITH_TSCH
  if (linkaddr_cmp(&coordinator_addr,&linkaddr_node_addr)) {
    printf("Server set as coordinator");
    tsch_set_coordinator(true);
  }
  else {
    printf("Server not set as coordinator");
  }
  //tsch_set_coordinator(linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr)); //make coordinator if equal
#endif /* MAC_CONF_WITH_TSCH */
  /* Initialize NullNet */
  nullnet_buf = payload;
  //nullnet_buf = (uint8_t)
  nullnet_len = 12;//sizeof(count);
  //nullnet_set_input_callback(input_callback);

  if(!linkaddr_cmp(&dest_addr, &linkaddr_node_addr)) { //If not equal
    printf("LLAddress and DestAddr not equal. Proceeding..");
    etimer_set(&periodic_timer, SEND_INTERVAL);
    while(1) {
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
      LOG_INFO("Sending %u to ", count);
      LOG_INFO_LLADDR(&dest_addr);
      LOG_INFO_("\n");
      printf("sending %u to client \n", count);
      NETSTACK_NETWORK.output(&dest_addr);
      count++;
      etimer_reset(&periodic_timer);
    }
  }
  printf("Dest and Current addr is equal");
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
