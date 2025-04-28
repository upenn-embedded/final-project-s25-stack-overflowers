
## Final Project Report

### 1. Video

[Insert final project video here]

* The video must demonstrate your key functionality.
* The video must be 5 minutes or less.
* Ensure your video link is accessible to the teaching team. Unlisted YouTube videos or Google Drive uploads with SEAS account access work well.
* Points will be removed if the audio quality is poor - say, if you filmed your video in a noisy electrical engineering lab.

### 2. Images

[Insert final project images here]

*Include photos of your device from a few angles. If you have a casework, show both the exterior and interior (where the good EE bits are!).*

### 3. Results

*What were your results? Namely, what was the final solution/design to your problem?*

#### 3.1 Software Requirements Specification (SRS) Results

*Based on your quantified system performance, comment on how you achieved or fell short of your expected requirements.*

*Did your requirements change? If so, why? Failing to meet a requirement is acceptable; understanding the reason why is critical!*

*Validate at least two requirements, showing how you tested and your proof of work (videos, images, logic analyzer/oscilloscope captures, etc.).*


| ID     | Description                                                                                                                                                                                                                                                                               |
| ------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| SRS-01 | The microcontroller will process each letter of the text input and connect it to its corresponding braille. We can test its accuracy by plugging in sample characters and evaluating the different outputs of the MCU.                                                                    |
| SRS-02 | The system will translate each recognized letter into a PWM signals sending specific rotation angle for the Braille stamp mechanism, ensuring accurate letter selection. In this phase, we can actually measure the rotation angle for each character once we have our stamper connected. |
| SRS-03 | The stamping should be precisely timed, ensuring that the stepper motor finishes its rotation completely before the stamp is pressed again to prevent misalignment.                                                                                                                       |
| SRS-04 | The linear actuator will move the stamping mechanism, positioning the stamp at the correct location on the paper. The actuator should only move the stamp after it is pressed.                                                                                                            |
| SRS-05 | The software will have debouncing to filter out noise from the keyboard.                                                                                                                                                                                                                  |

#### 3.2 Hardware Requirements Specification (HRS) Results

*Based on your quantified system performance, comment on how you achieved or fell short of your expected requirements.*

*Did your requirements change? If so, why? Failing to meet a requirement is acceptable; understanding the reason why is critical!*

*Validate at least two requirements, showing how you tested and your proof of work (videos, images, logic analyzer/oscilloscope captures, etc.).*

| ID     | Description                                                                                                                                                                                                                                                                       |
| ------ | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| HRS-01 | The device will require a keyboard module for text entry, this will communicate with the ATmega238 via PS/2.                                                                                                                                                                      |
| HRS-02 | It will also display the typed text to a LCD screen (I2C interface) to provide user feedback before printing. We will test the keyboard and the screen by seeing whether the later is displaying the correct keyboard inputs in real time.                                        |
| HRS-03 | The MCU will generate a PWM signal to control a stepper motor, which will rotate the Braille stamp mechanism to the correct character position. While working on this part we will ensure that the PWM signals are accurately rotating the stamp with a negligible error accuracy |
| HRS-04 | A linear actuator will move the stamp up and down to imprint the letters on to the paper A motor driver will be used to control the linear actuator, ensuring reliable movement without overloading the MCU.                                                                      |
| HRS-05 | The power system will need 12V power source and a buck converter for 5V to power the MCU, LCD, Stepper, and Linear Actuator.                                                                                                                                                      |
### 4. Conclusion

Reflect on your project. Some questions to address:

* What did you learn from it?
* What went well?
* What accomplishments are you proud of?
* What did you learn/gain from this experience?
* Did you have to change your approach?
* What could have been done differently?
* Did you encounter obstacles that you didn’t anticipate?
* What could be a next step for this project?

## References

Fill in your references here as you work on your final project. Describe any libraries used here.
