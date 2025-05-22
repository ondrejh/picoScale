#include "include.h"


#define LED_FAST_MS 100
#define LED_SLOW_MS 1000
#define SCALE_READING_MS 200

#define BUFFER_SIZE 1024


#ifndef SIMUL
// basic board function

void pico_led_init(void) {
  /*gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);*/
  ws2812_init(16);
}

void pico_set_led(bool led_on) {
  //gpio_put(PICO_DEFAULT_LED_PIN, led_on);
  put_pixel(urgb_u32(0x00, led_on ? 0x10 : 0x00, 0x00));

}
#else // ifndef SIMUL

#define PORT 12345
#define hx711_is_ready() true

#endif // ifdef SIMUL


int main() {
#ifndef SIMUL
  stdio_init_all();

  pico_led_init();
  hx711_init();
#endif // ifndef SIMUL

  bool led = false;
  bool connected = false;
  uint32_t t = millis();
  uint32_t td = LED_FAST_MS;
  uint32_t tscale = t;

  char buff[BUFFER_SIZE] = {0};
  int bufp = 0;

#ifdef SIMUL

  int server_fd, client_fd = -1;
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);

  printf("Starting server...\n");

  // 1. vytvoreni socketu
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  printf("Socket created OK.\n");

  // 2. nastaveni adresy
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  address.sin_port = htons(PORT);

  // 3. bind
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Binding OK.\n");


  // 4. listen
  if (listen(server_fd, 3) < 0) {
    perror("listen failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Listening OK.\n");

  // 5. nastaveni neblokujiciho rezimu
  fcntl(server_fd, F_SETFL, O_NONBLOCK);

  printf("Server is running on port %d.\n", PORT);

#endif

  // inittialize data
  init_json_data("picoScale");

  while (true) {

    // grab current values
    uint32_t now = millis();
#ifndef SIMUL
    bool con = stdio_usb_connected();
#else // ifndef SIMUL
    bool con = false;
#endif // ifdef SIMUL

    // connected status has changed
#ifdef SIMUL
    if (client_fd < 0) {
      client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
      if (client_fd >= 0) {
        printf("Client connected\n");
        fcntl(client_fd, F_SETFL, O_NONBLOCK);
      }
    }
    con = (client_fd >= 0); 
#endif // ifdef SIMUL

    if (con != connected) {
      connected = con;
      td = connected ? LED_SLOW_MS : LED_FAST_MS;
      t = now;
    }

#ifndef SIMUL
    // get serial input (non blocking)
    if (connected) {
      while ( tud_cdc_available() ) {
        // echo
        char *c = &buff[bufp++];
        *c = getchar();
        if ((*c == '\n') || (*c == '\r')) {
          *c = '\0';
          parse_json(buff);
          bufp = 0;
          int size = printf_json(data);
          printf("\n%ld\n", size);
        }
        else {
          if (bufp >= BUFFER_SIZE) {
            bufp = BUFFER_SIZE - 1;
          }
        }
      }
    }
    else {
      bufp = 0;
    }
#else // ifndef SIMUL

    if (client_fd >= 0) {
      memset(buff, 0, BUFFER_SIZE);
      ssize_t bytes = read(client_fd, buff, BUFFER_SIZE - 1);
      if (bytes > 0) {
        printf("Client sent: %s\n", buff);
        send(client_fd, buff, strlen(buff), 0);
      } else if (bytes == 0) {
        printf("Client disconnected\n");
        close(client_fd);
        client_fd = -1;
      } else if ((errno != EAGAIN) && (errno != EWOULDBLOCK)) {
        perror("read error");
        close(client_fd);
        client_fd = -1;
      }
    }
#endif // ifndef SIMUL

    // slow timer (blink)
    if ( now - t >= td ) {
      t = now;
#ifndef SIMUL
      pico_set_led( led = !led );
#endif // ifndef SIMUL
    }

    // scale reading timer
    if (( now - tscale >= SCALE_READING_MS ) && (hx711_is_ready())) {
      tscale = now;
#ifndef SIMUL
      uint32_t r = hx711_read();
      put_raw_scale(r);
#else // ifndef SIMUL
      printf("ahoj\n");
#endif // ifdef SIMUL
    }

#ifdef SIMUL
    sleep(0.02);
#endif // ifdef SIMUL
  } // while (true)

#ifndef SIMUL

  int size = printf_json(data);
  printf("\n%d\n", size);

  close(server_fd);

#endif // ifdef SIMUL

  return 0;
}
