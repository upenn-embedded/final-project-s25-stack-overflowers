[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/WXeqVgks)

# final-project-skeleton

* Team Number: 14
* Team Name: Stack Overflowers
* Team Members: Sofia Hedlund and Amanda Tapia Garbelloto
* GitHub Repository URL: https://github.com/upenn-embedded/final-project-s25-stack-overflowers.git
* GitHub Pages Website URL: [for final submission]

## Final Project Proposal

### 1. Abstract

*In a few sentences, describe your final project.*

We want to make a braille printer that the user can input text into with a keyboard and have it print out the text into readable braille bumps. We plan to do this by 3D printing braille letters onto a stamp that will then rotated according to the text input and stamped down onto the paper with a motor. This project will use power management, interrupts for reading from the keyboard, and timers to control the stepper motor.

### 2. Motivation

*What is the problem that you are trying to solve? Why is this project interesting? What is the intended purpose?*

The project aims to enhance accessibility for people with reduced vision. By automatically transforming text into braille, people who can't see text are able to expand their integration to reading materials by possessing a personal device that can make any text available for them without requiring extra translators or specialized equipment.

### 3. System Block Diagram

*Show your high level design, as done in WS1 and WS2. What are the critical components in your system? How do they communicate (I2C?, interrupts, ADC, etc.)? What power regulation do you need?*
![flow](flow.png)

### 4. Design Sketches

*What will your project look like? Do you have any critical design features? Will you need any special manufacturing techniques to achieve your vision, like power tools, laser cutting, or 3D printing?*
![DesignSketch](DesignSketch.png)

We will need to 3D print the braille stamp to attach to the motor and also need some kind of container to hold the LCD and the wiring needed to power the system. We will also have to 3D print a stand to hold the linear actuator.

### 5. Software Requirements Specification (SRS)

*Formulate key software requirements here. Think deeply on the design: What must your device do? How will you measure this during validation testing? Create 4 to 8 critical system requirements.*

*These must be testable! See the Final Project Manual Appendix for details. Refer to the table below; replace these examples with your own.*

**5.1 Definitions, Abbreviations**

Here, you will define any special terms, acronyms, or abbreviations you plan to use for hardware

PWM- Pulse Width Modulation

ASCII- American Standard Code for Information Exchange

LCD- Liquid Crystal Display

**5.2 Functionality**

| ID     | Description                                                                                                                                                                                                                                                                               |
| ------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| SRS-01 | The microcontroller will process each letter of the text input and connect it to its corresponding braille. We can test its accuracy by plugging in sample characters and evaluating the different outputs of the MCU.                                                                    |
| SRS-02 | The system will translate each recognized letter into a PWM signals sending specific rotation angle for the Braille stamp mechanism, ensuring accurate letter selection. In this phase, we can actually measure the rotation angle for each character once we have our stamper connected. |
| SRS-03 | The stamping should be precisely timed, ensuring that the stepper motor finishes its rotation completely before the stamp is pressed again to prevent misalignment.                                                                                                                       |
| SRS-04 | The linear actuator will move the stamping mechanism, positioning the stamp at the correct location on the paper. The actuator should only move the stamp after it is pressed.                                                                                                            |
| SRS-05 | The software will have debouncing to filter out noise from the keyboard.                                                                                                                                                                                                                  |

### 6. Hardware Requirements Specification (HRS)

*Formulate key hardware requirements here. Think deeply on the design: What must your device do? How will you measure this during validation testing? Create 4 to 8 critical system requirements.*

*These must be testable! See the Final Project Manual Appendix for details. Refer to the table below; replace these examples with your own.*

**6.1 Definitions, Abbreviations**

Here, you will define any special terms, acronyms, or abbreviations you plan to use for hardware

Stepper Motor- DC motor that has very precise conotrol that will help us avoid error when rotating the stamp to the position of the letter.

Linear Actuator- Moves in a straight line so that we can move the stamp up and down.

**6.2 Functionality**

| ID     | Description                                                                                                                                                                                                                                                                       |
| ------ | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| HRS-01 | The device will require a keyboard module for text entry, this will communicate with the ATmega238 via PS/2.                                                                                                                                                                      |
| HRS-02 | It will also display the typed text to a LCD screen (I2C interface) to provide user feedback before printing. We will test the keyboard and the screen by seeing whether the later is displaying the correct keyboard inputs in real time.                                        |
| HRS-03 | The MCU will generate a PWM signal to control a stepper motor, which will rotate the Braille stamp mechanism to the correct character position. While working on this part we will ensure that the PWM signals are accurately rotating the stamp with a negligible error accuracy |
| HRS-04 | A linear actuator will move the stamp up and down to imprint the letters on to the paper A motor driver will be used to control the linear actuator, ensuring reliable movement without overloading the MCU.                                                                      |
| HRS-05 | The power system will need 12V power source and a buck converter for 5V to power the MCU, LCD, Stepper, and Linear Actuator.                                                                                                                                                      |

### 6. Bill of Materials (BOM)

*What major components do you need and why? Try to be as specific as possible. Your Hardware & Software Requirements Specifications should inform your component choices.*

*In addition to this written response, copy the Final Project BOM Google Sheet and fill it out with your critical components (think: processors, sensors, actuators). Include the link to your BOM in this section.*

The major components that we need are a stepper motor/driver to spin the braille stamp and a linear actuator/driver to move the stamp up and down. We chose our stepper motor in order have a lot precision over the amount of degrees the motor turns, so that the correct braille letter is stamped. We also chose for it to be 12V in order to ensure that it will have enough torque required to spin the braille stamp accurately. The driver will let us send simple STEP and DIR signals to control the stepper which will help us easily control the stepper. We chose a linear actuator to stamp down and a H-bridge so that we can move the actuator both up and down. Our budget was a main factor in choosing our linear actuator, because a lot of them were quite pricey (around 70$), so we chose the most affordable option we could find that fit with our system. We chose our keboard to be able to communicate over PS/2 because it is simpler to work with then USB communication which would likely need outside libraries. We will also use the LCD screen from Lab 4 and use the ATmega328Pb Xplained mini board.

[BOM](https://docs.google.com/spreadsheets/d/10AtQ8pPZeS8G-I5d4TY12WMqAhfvfrOR0J54DZgVhsI/edit?usp=sharing)

### 8. Final Demo Goals

*How will you demonstrate your device on demo day? Will it be strapped to a person, mounted on a bicycle, require outdoor space? Think of any physical, temporal, and other constraints that could affect your planning.*

We will demonstrate our device on demo day by entering in text to the keyboard, which will then be displayed on the LCD and then printed in braille by the stepper motor which will rotate to position the correct letter and the linear actuator which will stamp it down. Some physical constraints are that we need to have enough table space for the keyboard, LCD container, stepper motor, and paper. Because we are stamping the text out letter-by-letter, printing out a word or a short phrase will be more realistic to demonstrate out project without excessive time delay. At the end of the demo, the user can feel the braille stamped out on the paper, and can also compare it against the text on the LCD.

### 9. Sprint Planning

*You've got limited time to get this project done! How will you plan your sprint milestones? How will you distribute the work within your team? Review the schedule in the final project manual for exact dates.*

| Milestone  | Functionality Achieved                                                                                                                                    | Distribution of Work                                                                                                                      |
| ---------- | --------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------- |
| Sprint #1  | -3D print braille stamp<br />-Create rotating code for the stamp that maps characters to braille positions                                               | 3D printing- Amanda<br />Code- Sofia                                                                                                      |
| Sprint #2  | -Add keyboard/LCD screen that displays the input text<br />-3D print stand for the linear actuator and stepper<br />-Debug rotation issues with the stamp | Keyboard/LCD- Sofia<br />3D print- Amanda <br />Debug- both                                                                              |
| MVP Demo   | -Create code to move linear actuator up and down<br />-Debug movement with linear actuator                                                                | Code/movement- both                                                                                                                       |
| Final Demo | -Refine precision of the motor movements                                                                                                                 | Both**This is the end of the Project Proposal section. The remaining sections will be filled out based on the milestone schedule.** |

## Sprint Review #1

### Last week's progress

During this sprint, we finished the code to connect the serial terminal to the LCD screen so that it displays the text the user inputs. We also implemented the logic behind mapping the characters to their braille representations in binary, and the code to turn the pins the solenoids will be attached on to HIGH. We also started CAD for the structure that will hold the solenoids above the paper. We have committed the code for this part on github.

![1743798297162](image/README/1743798297162.png)

### Current state of project

After the project presentation we got some feedback from the class on how to improve the stamping mechanism, so instead of using a wheel we will be working with six solenoids to support the full braille pattern.

We also took the professor's advice and are using our serial terminal to type out what we want to be translated to braille, instead of connecting another keyboard.

So, for each letter typed into the terminal, we have built the code that is recognizing them and translating it to the braille pattern, relating each dot to its correspondent solenoid and activating them. Since our parts have not arrived we still haven't tested their functionality, but so far our printed results seem to be in order. We have also wired the LCD screen and tested that it correctly displays what is being typed.

We have now ordered all the hardware parts needed for the project, but they haven't been delivered yet, so we are hoping to have them by next week so that we can also put them together with the stands we are designing and 3D printing. We have started working on the CAD structures for our supports and are planning on printing them this next week.

### Next week's plans

Next week we are going to be working on the hardware and testing the actual implementation of our code to the devices as soon as they arrive. So far we have the code for the solenoids push and pull, however since they were ordered later, we are probably going to implement first the movement of the linear actuator, which should follow the x-axis from right to left (since we are going to be stamping upside down).  Sofia will be working on combining the software and the hardware and debugging any issues that come up which will take around 5 hours, and will be considered “done” once the solenoids correctly move up and down with the character inputs and the linear actuator is able to move corresponding to each character.

We are also going to use the 3D printers to build the support for the solenoids and the box to keep most of our hardware connections, including the breadboards and ATmega238. Amanda will be working on 3D printing these supports with SolidWorks, which we are estimating will take around 5 hours and will be considered “done” once we send off the print request.

## Sprint Review #2

### Last week's progress

The solenoids that we ordered came, so we wired them with diodes and transistors and connected them to our MCU. From there, we debugged some issues with them correctly moving in and out, and realized two out of the six solenoids didn’t work properly, so we will be reordering them. We also completed and submitted CAD designs for a structure to hold the linear actuator and breadboard and also a structure to hold the actual solenoids.  We put the CAD files and a[ video](https://drive.google.com/file/d/1jELO6MgL8OEqHJt7nowW01g8Ehbm2qYE/view?usp=sharing) of the solenoids working with text input in the repo.

### Current state of project

At this point, we can see the braille patterns pretty accurately and have the LCD display our words at the same time as each letter is translated. We have wired part of the hardware and now need to add the linear actuator so that the motor moves on the x-axis as it prints on paper. We are waiting for the 3D printer to assemble all pieces and verify that they are the expected and can support our hardware accordingly.

### Next week's plan

Next week, we hope to figure out a way to attach a structure onto the tops of the solenoids so that when they move up and down they will stamp braille in the correct standardized dimensions. We also hope to get the linear actuator to work and move correctly as letters are stamped down. If the 3D print jobs are completed, we will also work on assembling the parts together.

We are planning on working together on most goals for next week, getting the linear actuator to work and debug any problems we might find with it should take around 4 hours to get the motor working and 3-5 hours for an attachment to the solenoids depending on how we implement it. Assembling the 3D pieces together should be pretty straightforward, but if we need to fix something or maybe redesign it, it might take up to 3 hours.

If everything works correctly, we also want to implement a feature connecting the braille machine to the blynk app, using the ESP32 interface so that we could try typing in our phones and getting the physical braille output.

## MVP Demo

1. Show a system block diagram & explain the hardware implementation.
2. Explain your firmware implementation, including application logic and critical drivers you've written.
3. Demo your device.
4. Have you achieved some or all of your Software Requirements Specification (SRS)?

   1. Show how you collected data and the outcomes.
5. Have you achieved some or all of your Hardware Requirements Specification (HRS)?

   1. Show how you collected data and the outcomes.
6. Show off the remaining elements that will make your project whole: mechanical casework, supporting graphical user interface (GUI), web portal, etc.
7. What is the riskiest part remaining of your project?

   1. How do you plan to de-risk this?
8. What questions or help do you need from the teaching team?

## Final Project Report

Don't forget to make the GitHub pages public website!
If you’ve never made a GitHub pages website before, you can follow this webpage (though, substitute your final project repository for the GitHub username one in the quickstart guide):  [https://docs.github.com/en/pages/quickstart](https://docs.github.com/en/pages/quickstart)

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

| ID     | Description                                                                                               | Validation Outcome                                                                          |
| ------ | --------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- |
| SRS-01 | The IMU 3-axis acceleration will be measured with 16-bit depth every 100 milliseconds +/-10 milliseconds. | Confirmed, logged output from the MCU is saved to "validation" folder in GitHub repository. |

#### 3.2 Hardware Requirements Specification (HRS) Results

*Based on your quantified system performance, comment on how you achieved or fell short of your expected requirements.*

*Did your requirements change? If so, why? Failing to meet a requirement is acceptable; understanding the reason why is critical!*

*Validate at least two requirements, showing how you tested and your proof of work (videos, images, logic analyzer/oscilloscope captures, etc.).*

| ID     | Description                                                                                                                        | Validation Outcome                                                                                                      |
| ------ | ---------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------- |
| HRS-01 | A distance sensor shall be used for obstacle detection. The sensor shall detect obstacles at a maximum distance of at least 10 cm. | Confirmed, sensed obstacles up to 15cm. Video in "validation" folder, shows tape measure and logged output to terminal. |
|        |                                                                                                                                    |                                                                                                                         |

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
