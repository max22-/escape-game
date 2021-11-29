#include <setjmp.h>
#include <BluetoothSerial.h>
#include "fe_bluetooth.h"
extern "C" {
    #include <fe.h>
}
#include "fe_cfunc.h"
#include "config.h"


static BluetoothSerial btSerial;
static jmp_buf toplevel;
static char data[10000];
static fe_Context *ctx = NULL;
static int gc;
static fe_Object *obj;
static void repl_task(void *);

bool fe_begin()
{
    bool result = true;
    result &= btSerial.begin(DEVICE_NAME);
    Serial.println("Starting Fe repl...");
    ctx = fe_open(data, sizeof(data));
    fe_register_cfuncs(ctx);
    gc = fe_savegc(ctx);
    xTaskCreate(repl_task, "Fe repl", 10000, NULL, 0, NULL);
    return result;
}

void fe_end()
{
    fe_close(ctx);
    btSerial.end();
}

static char readbt(fe_Context *ctx, void *userdata)
{
    char c;
    while(btSerial.available() == 0);
    btSerial.readBytes(&c, 1);
    return c;
}

static void writebt(fe_Context *ctx, void *userdata, char c)
{
    btSerial.write(c);
}

static void onerror(fe_Context *ctx, const char *msg, fe_Object *cl) 
{
  btSerial.printf("error: %s\n", msg);
  longjmp(toplevel, -1);
}

static void repl_task(void *)
{
    Serial.println("repl started");
    fe_handlers(ctx)->error = onerror;
    setjmp(toplevel);
    for(;;) {
        obj = fe_read(ctx, readbt, NULL);
        if(!obj) { break; }
        obj = fe_eval(ctx, obj);
        fe_write(ctx, obj, writebt, NULL, 0);
        btSerial.println();
        btSerial.flush();
        fe_restoregc(ctx, gc);
    }
    fe_end();
    vTaskDelete(NULL);
}