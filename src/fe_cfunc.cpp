#include <nvs_flash.h>
#include "fe_cfunc.h"
#include "pin_config.h"
#include "sensor.h"
#include "config.h"

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

static fe_Object* cfunc_sensor(fe_Context *ctx, fe_Object *arg) 
{
    uint8_t n = fe_tonumber(ctx, fe_nextarg(ctx, &arg));
    if(n > 7) {
        fe_error(ctx, "Invalid sensor number");
        return fe_bool(ctx, 0);
    }    
    return fe_number(ctx, Sensors.read(n));
}

static fe_Object* cfunc_config(fe_Context *ctx, fe_Object *arg)
{
    char name[256];
    float value;
    fe_Object *fe_name, *fe_value;

    if(fe_isnil(ctx, arg))
        return fe_string(ctx, Config.dump().c_str());
    fe_name = fe_nextarg(ctx, &arg);
    if(fe_type(ctx, fe_name) != FE_TSTRING) {
        fe_error(ctx, "Expected string as parameter name");
        return fe_bool(ctx, 0);
    }
    fe_tostring(ctx, fe_name, name, sizeof(name));
    if(name[0] == 0) {
        fe_error(ctx, "Invalid name");
        return fe_bool(ctx, 0);
    }
    fe_value = fe_nextarg(ctx, &arg);
    if(fe_type(ctx, fe_value) != FE_TNUMBER) {
        fe_error(ctx, "Expected number as parameter value");
        return fe_bool(ctx, 0);    
    }
    value = fe_tonumber(ctx, fe_value);
    if(!Config.define(name, value))
        fe_error(ctx, "Unknown parameter");
    return fe_bool(ctx, 0);
    
}

/*
static fe_Object* cfunc_threshold(fe_Context *ctx, fe_Object *arg) 
{
    if(fe_isnil(ctx, arg)) {
        if(!preferences.isKey("SEUIL")) {
            fe_error(ctx, "Variable SEUIL non définie");
            return fe_bool(ctx, 0);
        }
        return fe_number(ctx, preferences.getFloat("SEUIL"));
    }
    else {
        float threshold = fe_tonumber(ctx, fe_nextarg(ctx, &arg));

    }
    
}
*/



/***************************************************************************/

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

    /* Config */

    fe_set(ctx, fe_symbol(ctx, "capteur"), fe_cfunc(ctx, cfunc_sensor));
    fe_set(ctx, fe_symbol(ctx, "config"), fe_cfunc(ctx, cfunc_config));



    /**********/

 }