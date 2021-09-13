/* This module deals directly with the SPH0641LM4H-1 microphone. It's a digitial mic that outputs
** Pulse Density Modulation, which is a fancy way of saying more 1s is louder and more 0s is
** quieter. We're using the SPI peripheral to read the microphone data because it provides a clock
** and can read serial data and we handle the data in a interrupt. Things to keep in mind:
**     - SPI1 is connected to the APB bus which runs on the PCLK which at the time of writing this
**       is set to 53.333MHz (checkout system.c if you're unsure). With a prescaler of 32 the mic
**       sample frequency is 1.666MHz.
**     - The microphone takes about 100ms to start up (datasheet says 15-50ms depending on what
**       mode you're transitioning from but I'm not so sure looking at some of the mic readings)
**       so I leave the mic running full stick (1.333MHz) at all times by enabling the SPI in
**       receive only master mode (see the "Receive-only mode" para. in chapter 35.5.2 of the
**       reference manual (RM0444 rev5)).
*/

#ifndef MIC_H
#define MIC_H

#include "stdint.h"

// Initilises the SPI interface for the microphone but does not enable the interrupts (make sure
// to call mic_enable() afterwards) and saves the mic_receive_callback.
// \param[in/out]  mic_receive_callback  A callback function that takes raw mic data 16bits at a
//                                       time.
void mic_init(void (*mic_receive_callback)(uint16_t));
// Returns the mic sample rate.
double mic_get_sample_rate(void);
// Enables the mic interrupt and clears stale mic data.
void mic_enable(void);
// Disables the mic interrupt.
void mic_disable(void);

#endif // #ifndef MIC_H