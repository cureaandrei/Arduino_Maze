
## **Overview**

This project is a hybrid hardware–software system that combines embedded electronics with game development. I created a simple Pacman game in Python, where the player controls the character using a physical joystick connected through an Arduino Uno. The Arduino reads the joystick inputs and sends them to the laptop, allowing real-time control of the game.

Additionally, the game scores are transmitted via an I2C communication interface to a local web application, where they are displayed live. The project demonstrates integration between embedded systems, serial communication, Python programming, and basic web technologies.

---

## **Features**

- Playable Pacman game implemented in Python  
- Physical joystick control via Arduino Uno  
- Real-time input transmission from Arduino to PC  
- Live score streaming using I2C communication  
- Local website displaying game scores  
- Hardware–software co-design approach  

---

## **System Architecture**

### **Input Flow**
```text
Joystick → Arduino Uno → Serial Communication → Python Game
```

### **Score Output Flow**
```text
Python Game → I2C Interface → Local Website
```

---

## **Hardware Components**

- Arduino Uno  
- Analog joystick module  
- I2C communication module  
- Connecting wires  
- USB connection to laptop  

---

## **Software Components**

- Python (game implementation)  
- Arduino C/C++ (input handling)  
- Serial communication between Arduino and PC  
- Local web application for score display  

---

## **How It Works**

### **Joystick Input**
The joystick’s analog signals are read by the Arduino. The Arduino converts these readings into directional commands and sends them via serial communication to the Python program running on the laptop.

### **Game Control**
The Python Pacman game listens for incoming commands and moves the character accordingly in real time.

### **Score Streaming**
When the player’s score changes, it is transmitted through an I2C interface to a local website, enabling live score monitoring outside the game window.

---

## **Learning Objectives**

This project was created to explore:

- Embedded systems integration  
- Hardware input for software applications  
- Serial and I2C communication protocols  
- Real-time game control  
- Cross-domain system design  

---

