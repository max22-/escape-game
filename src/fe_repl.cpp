#include "fe_repl.h"
#include <Arduino.h>
#include <WiFi.h>
#include <setjmp.h>
extern "C" {
#include <fe.h>
}
#include "config.h"
#include "fe_cfunc.h"

static WiFiServer wifiServer(23);
WiFiClient client;
static jmp_buf toplevel;
static char data[10000];
static fe_Context *ctx = NULL;
static int gc;
static fe_Object *obj;
static void repl_task(void *);

void fe_begin() {
  Serial.println("Starting Fe repl...");
  wifiServer.begin();
  ctx = fe_open(data, sizeof(data));
  fe_register_cfuncs(ctx);
  gc = fe_savegc(ctx);
  xTaskCreate(repl_task, "Fe repl", 10000, NULL, 0, NULL);
}

void fe_end() {
  fe_close(ctx);
  wifiServer.end();
}

static char readwifi(fe_Context *ctx, void *userdata) {
  char c;
  while (client.available() == 0 && client.connected())
    ;
  if (!client.connected())
    return 0;
  c = client.read();
  return c;
}

static void writewifi(fe_Context *ctx, void *userdata, char c) {
  if (client.connected())
    client.write(c);
}

static void onerror(fe_Context *ctx, const char *msg, fe_Object *cl) {
  if (client.connected())
    client.printf("error: %s\n", msg);
  longjmp(toplevel, -1);
}

static void repl_task(void *) {
  Serial.println("repl started");
  fe_handlers(ctx)->error = onerror;
  while (true) {
    client = wifiServer.available();
    if (client) {
      setjmp(toplevel);
      while (client.connected()) {
        client.print("> ");
        obj = fe_read(ctx, readwifi, nullptr);
        if (!obj) {
          break;
        }
        obj = fe_eval(ctx, obj);
        fe_write(ctx, obj, writewifi, nullptr, 0);
        client.println();
        fe_restoregc(ctx, gc);
      }
      client.stop();
    }
    delay(100);
  }
  fe_end();
  vTaskDelete(NULL);
}