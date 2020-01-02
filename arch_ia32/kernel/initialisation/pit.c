#include "pit.h"
#include "../../stdlib/video.h"
#include "pic.h"

void Init_PIT(char channel, unsigned int frequence) {
    frequency = frequence;
    calculate_frequency();
    pit_send_command(0b00110100 | (channel << 6));
    /* Recharger la valeur du compteur*/
    set_pit_count(channel, PIT_reload_value);
}

char read_back_channel(char channel) {
    char command = 0x00;

    switch (channel) {
        case PIT_0:
            command |= READ_BACK_TIMER_0(1);
            break;
        case PIT_1:
            command |= READ_BACK_TIMER_1(1);
            break;

        case PIT_2:
            command |= READ_BACK_TIMER_2(1);
            break;
        default:
            break;
    }

    command |= LATCH_STATUS_FLAG(0) | LATCH_COUNT_FLAG(0) | READ_BACK_COMMAND;

    pit_send_command((char)command);

    return read_pit_count(PIT_0);
}

void conserv_status_byte() {
    char status = read_back_channel(PIT_0);
    if (status != 0x34) Init_PIT(PIT_0, frequency);
}