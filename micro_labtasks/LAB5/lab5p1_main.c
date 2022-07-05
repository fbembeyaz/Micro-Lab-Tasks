
#include "bsp.h"


int main(void) {

	BSP_system_init();

	while(1){

		uart_tx(uart_rx());

}
    return 0;
}

