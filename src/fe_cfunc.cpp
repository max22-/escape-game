#include "fe_cfunc.h"
#include "pin_config.h"

static fe_Object* cfunc_restart(fe_Context *ctx, fe_Object *arg) {
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
    return fe_bool(ctx, 0);
}

static fe_Object* cfunc_pinMode(fe_Context *ctx, fe_Object *arg) {
    uint8_t pin = fe_tonumber(ctx, fe_nextarg(ctx, &arg));
    uint8_t mode = fe_tonumber(ctx, fe_nextarg(ctx, &arg));
    pinMode(pin, mode);
    return fe_bool(ctx, 0);
}

static fe_Object* cfunc_digitalWrite(fe_Context *ctx, fe_Object *arg) {
    uint8_t pin = fe_tonumber(ctx, fe_nextarg(ctx, &arg));
    uint8_t val = fe_tonumber(ctx, fe_nextarg(ctx, &arg));
    digitalWrite(pin, val);
    return fe_bool(ctx, 0);
}




void fe_register_cfuncs(fe_Context *ctx)
{
    /* IO pins */
    fe_set(ctx, fe_symbol(ctx, "ES0"), fe_number(ctx, (float)IO0));
    fe_set(ctx, fe_symbol(ctx, "ES1"), fe_number(ctx, (float)IO1));
    fe_set(ctx, fe_symbol(ctx, "ES2"), fe_number(ctx, (float)IO2));
    fe_set(ctx, fe_symbol(ctx, "ES3"), fe_number(ctx, (float)IO3));
    fe_set(ctx, fe_symbol(ctx, "ES4"), fe_number(ctx, (float)IO4));
    fe_set(ctx, fe_symbol(ctx, "ES5"), fe_number(ctx, (float)IO5));
    /* *********/
    /* Constants */
    fe_set(ctx, fe_symbol(ctx, "INPUT"), fe_number(ctx, (float)INPUT));
    fe_set(ctx, fe_symbol(ctx, "INPUT_PULLUP"), fe_number(ctx, (float)INPUT_PULLUP));
    fe_set(ctx, fe_symbol(ctx, "OUTPUT"), fe_number(ctx, (float)OUTPUT));
    fe_set(ctx, fe_symbol(ctx, "HIGH"), fe_number(ctx, (float)HIGH));
    fe_set(ctx, fe_symbol(ctx, "LOW"), fe_number(ctx, (float)LOW));
    /* ***********/

    fe_set(ctx, fe_symbol(ctx, "restart"), fe_cfunc(ctx, cfunc_restart));
    fe_set(ctx, fe_symbol(ctx, "pinMode"), fe_cfunc(ctx, cfunc_pinMode));
    fe_set(ctx, fe_symbol(ctx, "digitalWrite"), fe_cfunc(ctx, cfunc_digitalWrite));
}