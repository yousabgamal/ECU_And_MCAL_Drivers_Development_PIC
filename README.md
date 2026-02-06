# Embedded Control System Project

This project is an **embedded control system** implemented for a microcontroller-based application. It is structured in two main layers to separate hardware abstraction from application logic:

- **MCAL_Layer (Microcontroller Abstraction Layer)**  
- **ECU_Layer (Electronic Control Unit Layer)**

---

## 📂 Project Structure

### 1. ECU_Layer
The ECU Layer contains the high-level modules that interact with the hardware through the MCAL layer.

| Folder/File | Description |
|-------------|-------------|
| `7_Segment` | Driver for 7-segment displays. |
| `button` | Button input handling module. |
| `Chr_LCD` | Character LCD driver. |
| `DC_Motor` | DC motor control module. |
| `Keypad` | Keypad interface module. |
| `LED` | LED control module. |
| `Relay` | Relay control module. |
| `ecu_layer_init.c` | Initialization routines for all ECU modules. |

### 2. MCAL_Layer
The MCAL Layer abstracts the microcontroller peripherals, providing a hardware-independent interface for the ECU Layer.

| Folder/File | Description |
|-------------|-------------|
| `ADC` | Analog-to-digital converter driver. |
| `CCP1` | Capture/Compare/PWM module driver. |
| `device_config.c` | Microcontroller configuration settings. |
| `EEPROM` | EEPROM interface module. |
| `GPIO` | General Purpose Input/Output driver. |
| `I2C` | I2C communication driver. |
| `interrupt` | Interrupt service routines. |
| `Timer0`, `Timer1`, `Timer2`, `Timer3` | Timer modules for timing and scheduling tasks. |
| `USART` | Serial communication driver. |

---

## ⚙️ Features
- Modular design with clear separation between hardware and application logic.  
- Support for multiple peripherals: 7-segment, LCD, Keypad, LED, Relay, DC Motor.  
- Configurable timers and communication protocols (USART, I2C).  
- Easy scalability for adding more ECU modules.

---

## 🛠 Setup & Usage
1. Clone the repository:
```bash
git clone https://github.com/username/project-name.git
