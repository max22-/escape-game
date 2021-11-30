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

static fe_Object* cfunc_read_threshold(fe_Context *ctx, fe_Object *arg) 
{
    if(!preferences.isKey("SEUIL")) {
        fe_error(ctx, "Variable SEUIL non définie");
        return fe_bool(ctx, 0);
    }
    return fe_number(ctx, preferences.getUShort("SEUIL"));
}

static fe_Object* cfunc_write_threshold(fe_Context *ctx, fe_Object *arg) 
{
    uint16_t threshold = fe_tonumber(ctx, fe_nextarg(ctx, &arg));
    preferences.putUShort("SEUIL", threshold);
    return fe_number(ctx, threshold);
}

static fe_Object* cfunc_read_day(fe_Context *ctx, fe_Object *arg) 
{
    if(!preferences.isKey("JOUR")) {
        fe_error(ctx, "Variable JOUR non définie");
        return fe_bool(ctx, 0);
    }
    return fe_number(ctx, preferences.getFloat("JOUR"));
}

static fe_Object* cfunc_write_day(fe_Context *ctx, fe_Object *arg) 
{
    float val = fe_tonumber(ctx, fe_nextarg(ctx, &arg));
    if(val < 0 || val > 100) {
        fe_error(ctx, "Valeur incorrecte (entre 0 et 100)");
        return fe_bool(ctx, 0);
    }
    preferences.putFloat("JOUR", val);
    return fe_number(ctx, val);
}

static fe_Object* cfunc_read_night(fe_Context *ctx, fe_Object *arg) 
{
    if(!preferences.isKey("NUIT")) {
        fe_error(ctx, "Variable NUIT non définie");
        return fe_bool(ctx, 0);
    }
    return fe_number(ctx, preferences.getFloat("NUIT"));
}

static fe_Object* cfunc_write_night(fe_Context *ctx, fe_Object *arg) 
{
    float val = fe_tonumber(ctx, fe_nextarg(ctx, &arg));
    if(val < 0 || val > 100) {
        fe_error(ctx, "Valeur incorrecte (entre 0 et 100)");
        return fe_bool(ctx, 0);
    }
    preferences.putFloat("NUIT", val);
    return fe_number(ctx, val);
}


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
    fe_set(ctx, fe_symbol(ctx, "lire-seuil"), fe_cfunc(ctx, cfunc_read_threshold));
    fe_set(ctx, fe_symbol(ctx, "ecrire-seuil"), fe_cfunc(ctx, cfunc_write_threshold));
    fe_set(ctx, fe_symbol(ctx, "lire-jour"), fe_cfunc(ctx, cfunc_read_day));
    fe_set(ctx, fe_symbol(ctx, "ecrire-jour"), fe_cfunc(ctx, cfunc_write_day));
    fe_set(ctx, fe_symbol(ctx, "lire-nuit"), fe_cfunc(ctx, cfunc_read_night));
    fe_set(ctx, fe_symbol(ctx, "ecrire-nuit"), fe_cfunc(ctx, cfunc_write_night));


    /**********/

}