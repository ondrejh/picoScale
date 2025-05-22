#ifndef __SIMUL_H__
#define __SIMUL_H__

#define PORT 12345
#define hx711_is_ready() true
#define hx711_read() 654321
#define ws2812_init(x) while(0){};
#define put_pixel(x) while(0){};

void server_init();
bool client_connected();
int client_read(char *buff);
void client_write(char *buff, int buflen);

#endif // __SIMUL_H__
