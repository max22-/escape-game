#include "profilab_tcp.h"
#include <Arduino.h>
#include "config.h"

static void profilab_encode(profilab_data_t data, /* return value --> */ unsigned char *buffer);
static profilab_data_t profilab_decode(unsigned char *buffer);

template<typename T>
void set_bit(T& number, uint8_t index, bool value)
{
    number = (number & ~(1UL << index)) | (value << index);
}

ProfilabTCP::ProfilabTCP(uint16_t port) : wifiServer(port), port(port) {
    for(int i = 0; i < 16; i++) { 
        callbacks[i] = 0;
        last_sent[i] = 0;
        queue_size[i] = 0;
    }
}

void ProfilabTCP::begin()
{
    wifiServer.begin(port);
}

void ProfilabTCP::handle()
{
    unsigned char buffer[12];
    switch(state) {
        case WAITING:
            wifiClient = wifiServer.available();
            if(wifiClient) {
                printf("Client connected : %s\n", wifiClient.remoteIP().toString().c_str());
                clear_queues();
                state = CLIENT_CONNECTED;
            }
            break;

        case CLIENT_CONNECTED:
            if(!wifiClient.connected()) {
                printf("client disconnected\n");
                wifiClient.stop();
                state = WAITING;
                break;
            }

            if(wifiClient.available() >= 12) {
                wifiClient.readBytes(buffer, sizeof(buffer));
                profilab_data_t data = profilab_decode(buffer);
                if(data.channel == 0) {
                    uint16_t new_rx_data = (uint16_t)data.n;
                    uint16_t changed = rx_data ^ new_rx_data;
                    for(int i = 0; i < 16; i++)
                        if((changed & (1 << i)) && callbacks[i] != nullptr)
                            callbacks[i]((new_rx_data & (1 << i)) != 0);
                    rx_data = new_rx_data;
                }
            }

            uint16_t new_tx_data = tx_data;
            for(int i = 0; i < 16; i++) {
                if(queue_size[i] > 0 && (millis() - last_sent[i]) > PROFILAB_MESSAGE_DELAY) {
                    set_bit(new_tx_data, i, queue_pop(i));
                    last_sent[i] = millis();
                }
            }
            if(new_tx_data != tx_data) {
                profilab_data_t data = {0, new_tx_data + 0.1};
                profilab_encode(data, buffer);
                wifiClient.write(buffer, sizeof(buffer));
                tx_data = new_tx_data;
                /* Info */
                for(int i = 0; i < 16; i++)
                    printf("%2d  ", i);
                printf("\n");
                for(int i = 0; i < 16; i++)
                    printf(" %d  ", (new_tx_data & (1 << i)) > 0);
                //printf("\t{channel=%d, n=%f}"", data.channel, data.n);
                printf("\n\n");
            }

            break;
    }
}

void ProfilabTCP::tx(uint8_t pin, bool val)
{
    if(pin >= 16)
        return;
    uint8_t s = queue_size[pin];
    if( s > 7 
        || (s > 0 && queue_peek_back(pin) == val)
        || (s == 0 && (((tx_data & (1<<pin)) > 0) == val))
    ) return;
    queue_push_back(pin, val);
}

void ProfilabTCP::rx(uint8_t pin, void (*callback)(bool))
{
    if(pin < 16)
        callbacks[pin] = callback;
}

bool ProfilabTCP::queue_peek_back(uint8_t pin)
{
    uint8_t s = queue_size[pin];
    if(s > 0)
        return queue[pin] & (1 << (s - 1));
    else return false;
}

bool ProfilabTCP::queue_pop(uint8_t pin)
{
    if(pin >= 16 || queue_size[pin] == 0)
        return false;
    queue_size[pin]--;
    bool result = queue[pin] & 1;
    queue[pin] >>= 1;
    return result;
}

void ProfilabTCP::queue_push_back(uint8_t pin, bool val)
{
    if(pin >= 16 || queue_size[pin] > 7)
        return;
    uint8_t s = queue_size[pin]++;
    set_bit(queue[pin], s, (val ? 1 : 0));
}

void ProfilabTCP::clear_queues()
{
    for(int i = 0; i < 16; i++) {
        queue_size[i] = 0;
        queue[i] = 0;
    }
}

static void profilab_encode(profilab_data_t data, /* return value --> */ unsigned char *buffer)
{
    double n = data.n;
    for(int i = 0; i < 12; i++) buffer[i] = 0;
    buffer[11] = data.channel;
    unsigned char sign = n < 0;
    if(n < 0) n*= -1;
    int exp = 0;
    for(exp = 0; n >= 2.0; n /= 2.0, ++exp);
    for(; n < 1.0; n*= 2.0, --exp);
    exp += 16383;
    buffer[9] = exp / 256;
    buffer[8] = exp % 256;
    if(sign) buffer[8] |= 1 << 8;
    n *= 128;
    for(int i = 7; i >= 1; i--) {
        unsigned char t = (unsigned char)n;
        buffer[i] = t;
        n = (n - t) * 256;
    }
}

static profilab_data_t profilab_decode(unsigned char *buffer)
{
    profilab_data_t result;
    result.channel = buffer[11];
    double n;

    n = 0;
    for(int i = 1; i <= 7; i++)
        n = buffer[i] + n / 256.0;
    n /= 128.0;
    if(buffer[9] < 0)
        n *= -1.0;
    unsigned short exp = buffer[8] + (buffer[9] << 8);
    double exponent = exp - 16383;
    n *= pow(2, exponent);

    result.n = n;
    return result;
}