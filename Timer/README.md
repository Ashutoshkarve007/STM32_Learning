*IDLE LINE DETECTION*

https://controllerstech.com/uart-dma-with-idle-line-detection/

edit:

https://github.com/dekuNukem/STM32_tutorials/blob/master/lesson4_timers_and_pwm/README.md

# STM32 Timer Interrupts, PWM, and Watchdog

[Landing Page: Intro and Required Hardwares](../README.md)

[Lesson 0: Setup and Blinking LED](../lesson0_intro_blinkLED/README.md)

[Lesson 1: UART Transmit](../lesson1_serial_helloworld/README.md)

[Lesson 2: External GPIO Interrupts](../lesson2_external_interrupt/README.md)

[Lesson 3: UART Receive and External Files](../lesson3_serial_recv_interrupt)

**`THIS LESSON`** Lesson 4: Timer Interrupts, PWM, and Watchdog

[Lesson 5: SPI and I2C Communication](../lesson5_spi_i2c/README.md)

[Lesson 6: Real-time Operating Systems (RTOS)](../lesson6_rtos/README.md)

[Lesson 99: Miscellaneous](../lesson99_miscellaneous/README.md)

## Introduction

Timers are another commonly used peripheral in microcontrollers. They are used to keep track of time, raise periodic interrupts, drive PWM outputs, and many more besides.

In this lesson we'll take a look at two most common use cases: Timer interrupt and PWM output.

We'll also briefly cover the watchdog timer while we're at it.

## STM32 Timers

Each STM32 variant has several built-in timers. They are numbered from TIM1 to TIM20, and grouped into different types:

* `General purpose timers`: As the name suggests, those are the standard timers with functions like output compare, one-pulse, input capture, sensor interface, interrupts, PWM output, and many more.

* `Advanced timers`: Those come with even more bells and whistles, usually highly specialized like motor control and digital power conversion.

* `Basic timers`: Barebone timers with no outputs. Usually used for timekeeping.

* `Other timers`: Chip-specific timers like low-power or high-resolution timers.

Usually the higher-end the chip, the more timer it has. The details of STM32 timers is quite a rabbit hole, but we'll keep it simple in this lesson.

If you want to learn more, [Here is an cross-series overview](resources/timer_overview.pdf) of STM32 timers. There are also helpful information in the [timer section of the datasheet](resources/datasheet.pdf) of chip we're using.

## Preparation

Again, we'll be expanding upon [Lesson 1](../lesson1_serial_helloworld/README.md), so make a copy of the [project file](../lesson1_serial_helloworld/sample_code) and double check [the hookup](../lesson1_serial_helloworld/README.md#hookup).

## Timer Interrupts

Periodic interrupts is one of the most common use case for timers. They are mostly used for timekeeping or performing repetitive tasks. Arduino use it in `millis()` to keep track of milliseconds, video controllers use it to generate frame sync pulses, and operating systems use it for task switching to give the illusion of multitasking.

We're going to implement a simple example of timer interrupt: printing `hello world` every 100 milliseconds.

Open the `test.ioc` file, to the left of the screen we can see the available timers on this chip:

![Alt text](https://user-images.githubusercontent.com/89137956/191455772-9e12b3eb-8deb-47da-886f-f0a905e1ad6f.png)


If you bothered to look at [page 18 of the datasheet](resources/datasheet.pdf), you'll find TIM1 is an advanced timer, while the rest are general purpose timers. They all have 16-bit resolution. TIM1 and TIM3 has 4 channels, and the rest has only 1 channel.

In this example I'm going to use TIM17. Expand it and check the `Activated` box:

![Alt text](https://user-images.githubusercontent.com/89137956/191456069-144b6f9f-8ae5-40cc-8106-cf7f18ae2d94.png)

This enables timer 17. Now go to the `Configuration` tab, and click the newly appeared `TIM17` button:

![Alt text](https://user-images.githubusercontent.com/89137956/191456156-53d11a81-3c21-456c-8ae6-efb98342ea8a.png)

Now we have a bunch of settings change:

![Alt text](https://user-images.githubusercontent.com/89137956/191456231-6042d271-38d9-4b2e-a9f6-826e9a0e0825.png)

The part we're interested in is `prescaler` and `counter period`, as they determine the timer interrupt frequency. Some maths is going to be involved, but hopefully nothing too heady.

### Timer prescaler

Each timer is essentially a counter with programmable clock input. At each clock period, the counter increments by 1.

You can find timers' input clock speed on the `Clock Configuration` tab. In this case we see that the `APB1 Timer Clock` is 48MHz, same as system clock:

![Alt text](https://user-images.githubusercontent.com/89137956/191456361-3e1f8182-a0b3-4daa-b9a9-85daa3d3b13b.png)

This means by default, the counter in timers counts up at 48MHz, which is obviously way too fast. Fortunately, we can slow down this clock by dividing it with a custom value. This value is called the *prescaler*.

Timer prescaler in STM32 is an unsigned 16-bit number by which the input clock is divided. The table below illustrates how it works:

![Alt text](https://user-images.githubusercontent.com/89137956/191456428-1794b4fa-8f92-45a5-8deb-66eb1c5a1702.png)

As you can see, the input clock is divided by `prescaler + 1`. Not too bad.

### Timer counter period

Another thing to keep in mind while setting up timer interrupts is the `counter period`.

`counter period` is the upper limit of the timer counter, at which the counter will reset.

In other words, the counter will count up from 0 to `counter period`, at the clock speed obtained with the `prescaler` above. Once the counter reaches the `counter period`, it will reset to 0 and start all over again.

If interrupt is enabled, a `PeriodElapsed` interrupt will raise when the rollover happens. This is the periodic timer interrupt that we were talking about earlier. 

### Getting the right number

It's easy to see that both `prescaler` and `counter period` affect the frequency of timer interrupts, and we need to calculate those two values based on our requirements.

Generally, you use `prescaler` to get the clock speed in the ballpack, then use `counter period` to fine tune the frequency.

For example we want our timer interrupt to happen every 100ms. To do this can use a 1KHz counter clock. Dividing input clock frequency by the desired output, we get a ratio of `48000000 / 1000 = 48000`. However since `prescaler` starts at 0, we need to subtract 1 from the ratio. Therefore our final `prescaler` is 47999, and this gives us a 1KHz clock to the timer counter.

Now that the counter ticks up every 1ms, we can set `counter period` to 100. As a result the counter counts up to 100 then resets, generating an interrupt every 100ms.

### Finding the right combination

It's easy to see that different combinations of `prescaler` and `counter period` can achieve the same result. For example we used `prescaler = 47999` and `counter_period = 100` for the 100ms interrupts above, however `prescaler = 23999` and `counter_period = 200`, or even `prescaler = 479` and `counter_period = 10000` works just as well. Again, it's up to you to decide what to use, with some nuances:

* Since `prescaler` and `counter period` are both unsigned 16-bit integers, neither of them can exceed 65535. 

* `prescaler` balances the range and resolution of your timer interrupt. A small `prescaler` allows you to fine tune the accuracy of periodic interrupt, but also limits the range.

For example, when `prescaler = 47`, counter clock is 1MHz. 1 counter tick is 1us, so you can adjust `counter_period` in 1us increments from 1us to 65535us = 65.53ms. 

While when `prescaler = 47999`, counter clock is 1KHz. 1 counter tick is 1ms and you can adjust `counter_period` in 1ms increments, giving you the range from 1ms to 65535ms.

Notice how in the first scenario you can fine tune the timer interrupt accuracy down to 1us, but the maximum period of timer interrupt is only 65 milliseconds. While in the second scenario the max period is 65 seconds, but can only be adjusted in 1ms increments.

There is also a detailed formula on page 10 of the [timer overview](resources/timer_overview.pdf) if you want more information.

### Try it out

Anyway, since that we have figured out the `prescaler` and `counter period`, we can type them in:

![Alt text](https://user-images.githubusercontent.com/89137956/191456520-3c6848ca-1d3d-4b98-8d7f-9a018c0fd782.png)

Make sure to enable the interrupt in the NVIC tab as well:

![Alt text](https://user-images.githubusercontent.com/89137956/191456585-9090ba92-b4e3-4fe8-b43d-abd57a5df01e.png)

That's it! Regenerate the code and launch Keil MDK.

As usual, we look into [provided library files](sample_code/Drivers/STM32F0xx_HAL_Driver/) to see what we can use. Looking at the timer file [stm32f0xx_hal_tim.h](sample_code/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_tim.h), we can see a sea of library functions near the end, most of them for advanced features. 

For this simple example, we just need `HAL_TIM_Base_Start_IT()` to start the timer interrupt. Put it before the main loop:

![Alt text](https://user-images.githubusercontent.com/89137956/191456823-d9b0e6c7-9cbd-4f54-92b2-ef4e7adb881c.png)

Then we need to write our interrupt callback function. The timer overflow interrupt in STM32 HAL is called `HAL_TIM_PeriodElapsedCallback()`. It has a `__weak` attribute, meaning if we write our own function with the same name and arguments, the compiler will use the new one instead.

So just copy this function's definition and put it anywhere you like in `main.c`, for this example it just prints `hello world`:

![Alt text](https://user-images.githubusercontent.com/89137956/191456906-77bb00da-0aa3-4d9e-b301-e5238d466b86.png)

Compile and upload, and you'll see it's printing `hello world` every 100ms:

![Alt text](https://user-images.githubusercontent.com/89137956/191456964-dfd28350-6f65-41a8-b0fd-198c04df9e68.png)

You can use timer interrupts like this to periodically update a screen, scan button matrix input, read a sensor, and much more.

Periodic timer interrupts like this happens at a consistent rate outside the main loop. They are more accurate than using `HAL_Delay()`, and more importantly they are not blocking, allowing CPU to do other tasks in the mean time.

You can find the [finished project here](sample_code).

## PWM Output

Another popular timer function is outputting PWM signals. Take a look at the [sparkfun tutorial](https://learn.sparkfun.com/tutorials/pulse-width-modulation) if you're not familiar with this topic.

Put simply, PWM works by switching a signal on and off very rapidly. The more time the signal stays on, the more power is delivered to the output. This is how [analogWrite()](https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/) function in Arduino works.

In STM32 we have much more options and control for PWM generation compared to Arduino, although we'll stick to the basics for this lesson by using it to dim the on-board LED on PA4.

### Timer configuration

We'll continue working on the project from the previous section. Go back to STM32Cube and switch to `pinout` page:

![Alt text](https://user-images.githubusercontent.com/89137956/191457091-679a972f-560e-4385-bbcf-95f0bd69d094.png)

Just like Arduino, only certain pins on STM32 has PWM output. You can find out by left clicking a pin and see if it as `TIMXXCHY` function, where `XX` is timer number, and `Y` is channel number. Let's see what's on the on-board LED, `PA4`:

![Alt text](https://user-images.githubusercontent.com/89137956/191457134-a0eee82c-c431-4e4c-b8b2-a6332ed5d0e8.png)

Bingo! `PA4` has `TIM14CH1`, meaning it's the output of timer 14 channel 1, and we can use it for PWM.

Click `TIM14CH1` to switch to that function:

![Alt text](https://user-images.githubusercontent.com/89137956/191457176-eaa64296-c567-4754-bffc-978d9659182d.png)

Then on the side bar, activate TIM14, then set channel 1 function to `PWM Generation CH1`:

![Alt text](https://user-images.githubusercontent.com/89137956/191457225-447c0ec8-69fc-413a-9bd0-6e54c7ba2cec.png)

Go to the `configuration` page and click on the newly appeared `TIM14` button:

![Alt text](https://user-images.githubusercontent.com/89137956/191457286-4da623f4-b321-4487-973d-afd28e503b8b.png)

Now we're back at the counter settings, except this time there is an additional PWM1 section:

![Alt text](https://user-images.githubusercontent.com/89137956/191457337-86b15298-8572-453e-9617-8f6dbd344acb.png)

Just like last time, we have `prescaler` and `counter period`. Except in this case, instead of the interrupt frequency, they determine the PWM frequency.

Let's say we want the PWM frequency of our LED to be 1KHz. One option is to use `prescaler` of 47 and `counter period` of 1000.

The `prescaler` of 47 divides the 48MHz system clock down to 1MHz for the TIM14 counter, making it count up every 1us. And the `counter period` of 1000 makes the counter reset once the count has reached 1000. This way the counter resets every 1000uS, which is 1ms, resulting in a PWM frequency of 1KHz.

The new parameter `pulse` determines the duty cycle of the PWM output, calculated as `pulse / counter_period`.

For example, if `counter period` is 1000 and you set `pulse` to 900, then the duty cycle is `900/1000 = 90%`, meaning the output stays high 90% of the time during a single PWM period. Obviously, `pulse` should be less or equal to `counter period`.

Anyway, let's type the numbers in and see what happens:

![Alt text](https://user-images.githubusercontent.com/89137956/191457469-c7341d5c-9f91-435c-a1d3-b5a11dd54018.png)

Generate the code, then go to Keil IDE.

### Try it out

To see PWM in action we need to make a few changes in the code. Before the main loop, call a few more initialization functions:

```
HAL_TIM_Base_Start(&htim14);
HAL_TIM_PWM_Init(&htim14);
HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
```

Then delete everything in the main loop. It should look like this:

![Alt text](resources/newcode.png)

Compile and upload. Now LED should be much dimmer than before, almost unnoticeable compared to the power LED:

![Alt text](resources/photo.jpg)

And if you have a logic analyzer, take a look at the waveform:

![Alt text](resources/logic.png)

You'll see that the PWM signal has a period of 1ms, and stays high 90% of the time, exactly what we want.

Eagle eyed viewer might spot the LED is dim even though the duty cycle is high. Remember that the LED on PA4 is connected to 3.3V on the other end, so it lights up when PA4 is LOW.

### Changing duty cycle on the fly

Setting up duty cycle in STM32Cube is all well and good, but it would be more useful if we can change it while our program is running. Luckily you can manipulate peripheral registers directly in STM32 HAL. For adjusting duty cycle, simply write into `CCRx` register like this:

```
htim14.Instance->CCR1 = 900;
```
CCR1 is for channel 1, change it to CCR2, CCR3, CCR4 if you you're using other channels.

Low-level peripheral registers is another massive rabbit hole that I'm not going into. After all, abstracting them is the job for HAL library in the first place.

However if you're feeling adventurous, feel free to dive in the thousand-page [reference manual](../resources/datasheets/stm32f0_reference_manual.pdf) and find out the details about every single peripheral registers. Then you can find their `typedef`s in the beginning of [device header file](sample_code_pwm/Drivers/CMSIS/Device/ST/STM32F0xx/Include/stm32f030x6.h) and manipulate them directly.

Anyway, we can try it out by adding this in the main loop:

```
for (int i = 0; i < 1000; i+=3)
{
  htim14.Instance->CCR1 = i;
  HAL_Delay(1);
}
for (int i = 1000; i > 0; i-=3)
{
  htim14.Instance->CCR1 = i;
  HAL_Delay(1);
}
```

It ramps up the duty cycle, then back down again, resulting in a smooth "breathing" animation:

![Alt text](resources/breath.gif)

You can find the [finished code here](sample_code_pwm)

What we're doing here is fairly similar to just calling `analogWrite()` in Arduino. However, even in this simple example we have much more control of our PWM properties. We can set the PWM frequency to whatever we like, while Arduino is fixed to a measly 490Hz or 980Hz. We also have control of the PWM resolution by changing `counter period` from 0 to 65535, while Arduino is fixed to 255.

Of course what we're covering here is the very basics of STM32 timers, and they can do much more than that. If you're interested in experimenting with advanced timer functions feel free to go back to [timer overview](resources/timer_overview.pdf) and the [datasheet](resources/datasheet.pdf) 

## Watchdog Timer

Watchdog timers(WDT) are often used to reset the system when a malfunction occurs. In normal operation the WDT counts up(or down) while your program periodically resets it. This is called "kicking the dog". If your program crashed and did not kick WDT in time, it will overflow and generate a hardware reset signal. This restarts the system and recovers it from the crash.

### STM32 watchdog timers

STM32 has two watchdog timers: **Independent Watchdog** (IWDG) and **System Window Watchdog** (WWDG).

IWDG is a 12-bit down-counter clocked from an independent internal clock source. It counts down from 4095 and resets the system if it reaches 0.

WWDG has more bells and whistles, featuring fancy stuff like early warning interrupt and so on.

Usually IWDG is more than enough, so that's what we're looking at.

### IWDG setup

Enable the IWDG:

![Alt text](resources/iwdg.png)

Click its button:

![Alt text](resources/ibut.png)

And now we're at its configuration window:

![Alt text](resources/icon.png)

Usually there is no need to change `IWDG window value` and `IWDG reload value`. Of course you can [read more here](resources/en.STM32L4_WDG_TIMERS_IWDG.pdf).

The bit we're interested in is, again, the prescaler. This determine how fast the IWDG counts down towards reset. Assuming you didn't change the other two values, you can consult the table below to see how long is the reset window:

![Alt text](resources/itab.png)

After regenerating the code, simply use `HAL_IWDG_Refresh(&hiwdg)` to kick the dog before the countdown reaches 0.

Keep in mind that the internal 40KHz clock powering the IWDG is not very accurate, and can vary as much as ±10%. So leave some slack when setting IWDG parameters.

Here is a simple [finished project](sample_code_pwm) with IWDG. It prints out an incrementing number while also kicking the dog. If you comment out the `HAL_IWDG_Refresh()` function, you'll see that the counting restarts every 3 seconds or so, indicating the IWDG is resetting the chip.

## Homework

Now that we have covered the basics about STM32 timers, it's up to you to implement something useful with them. This homework is a bit longer than usual, but it does challenge you to put everything you learned into use.

The goal is to **implement an accurate microsecond timebase with STM32 timers**.

In the end you should have a **`uint32_t micros()`** function which returns the number of microseconds since start-up, and a **`delay_us(uint32_t us)`** function which delays the number of microseconds in the argument.

I suggest implementing the functions in separate files [like we did last time](../lesson3_serial_recv_interrupt), so you can reuse them afterwards.

You can access the current counter value in a timer with `htimXX.Instance->CNT`, where `XX` is the timer number.

For further hints and the answer, [click here](./homework_hints.md).

## Next Steps

In the next lesson we'll look at how to communicate using SPI and I2C with our STM32 chip.

[CLICK ME TO GO TO NEXT LESSON](../lesson5_spi_i2c/README.md)

## Questions?

If you have any questions, feel free to [open an issue](https://github.com/dekuNukem/stm32_the_easy_way/issues) or email me at `dekunukem gmail com`. The former is preferable since it helps other people too.
