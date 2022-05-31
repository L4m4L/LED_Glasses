/**
 * This module deals directly with the SPH0641LM4H-1 microphone. It's a digital mic that outputs
 * Pulse Density Modulation (PDM), which is a fancy way of saying more 1s is louder and more 0s is
 * quieter. We're using the SPI peripheral to interface with the microphone in master receive only
 * mode and we're saving data using the DMA. Things to keep in mind:
 *  - SPI1 is connected to the APB bus which runs on the PCLK which at the time of writing this is
 *    set to 53.333MHz (checkout system.c if you're unsure). With a prescaler of 32 the mic sample
 *    frequency is 1.666MHz.
 *  - The microphone takes about 100ms to start up (datasheet says 15-50ms depending on what mode
 *    you're transitioning from but I'm not so sure looking at some of the mic readings) so I
 *    leave the mic running full stick (1.333MHz) at all times by enabling the SPI in receive only
 *    master mode (see the "Receive-only mode" para. in chapter 35.5.2 of the reference
 *    manual (RM0444 rev5)).
 *
 *
 * Example:
 *
 * void some_callback_func(void)
 * {
 *     // Hot tip, don't do audio processing here (this is called inside an interrupt).
 *     notify_main_loop_audio_is_ready_or_whatever();
 *     mic_record(another_audio_buffer, length);
 * }
 *
 * void some_init_func(void)
 * {
 *     ...
 *     mic_init(some_callback_func);
 *     ...
 *     mic_record(some_audio_buffer, length);
 *     ...
 * }
 *
 *
 * TODO: If we ever decide to implement a low power mode we should create a deinit function to
 *       shut everything down.
 */

#ifndef MIC_H
#define MIC_H

#include <stdint.h>

/**
 * Initializes all the required GPIO, SPI and DMA peripherals and enables the microphone.
 * \param[inout]  recording_complete_callback  Run when the DMA has completed the recording.
 */
void mic_init(void (*recording_complete_callback)(void));

/**
 * Begin a DMA transfer from the microphone into the provided buffer. This function will return
 * the number of transfers lost if we've truncated a previously running transfer. Make sure you
 * wait for the recording_complete_callback to be called!
 */
uint16_t mic_record(uint8_t *buffer, uint16_t length);

#endif /** MIC_H */