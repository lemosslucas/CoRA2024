# CoRA 2024 (Competição de Robôs Autônomos) - Line Follower

![CoRA2024 Banner](http://cora.cpdee.ufmg.br/img/imgs/banner_cora24.png)

This is a repository containing all the files utilised on the development and tests of our autonomous line follower robot.
The competition challenge participants to prototype, build, program and test the robot from scratch, with the help of tools such as Arduino (and languages alike), and CAD and Simulation softwares.

## File Structure
- `data`: All the relevant data acquired during testing and programs to do so
- `CoRA2024.ino`: Main file in Arduino to compile/run the code and test the robot
- `motores.cpp` and `motores.h`: Specific scripts to implement the motorised functions of the robot
- `desafios.cpp` and `desafios.h`: Specific scripts to tackle the track's challenges with the robot

## Technologies used
### Programming Languages
- [Arduino / C++](https://www.arduino.cc/): Main Framework
- [C](https://learn.microsoft.com/pt-br/cpp/c-language/): Libraries and other implementations
- [Python](https://python.org): Data Analysis
- [MATLAB](https://www.mathworks.com/products/matlab.html): Data Plotting and Analysis

### Hardware
- [2x Yellow DC Motors](https://i0.wp.com/myduino.com/wp-content/uploads/2023/09/2-40.jpg?w=600&ssl=1) - **Specs:** 3V-12VDC; 1:48 gear ratio; 800g/cm max. @ 3VDC; 70mA (250mA max. @ 3VDC)
- [2x Yellow DC Motor Wheels](https://kitsguru.com/cdn/shop/products/tracked-wheel-for-bo-motor-yellow-26mm-width_2048x.jpg?v=1642829900)
- [1x Swivel Caster Wheel](https://www.institutodigital.com.br/wp-content/uploads/2020/10/rodizio-giratorio-25mm-2.png)
- [2x 18650 Lithium-ion battery cells](https://electrostoreshop.com/wp-content/uploads/2024/04/3-7v-2000mah-18650-rechargeable-li-ion-lithium-battery.jpg) - **Specs:** 3.7VDC; 2000mAh
- [1x Arduino Nano](https://store.arduino.cc/products/arduino-nano)
- [1x Breadboard and Jumper Cables](https://images.squarespace-cdn.com/content/v1/556646a4e4b0bda793faf918/1595041751201-D74U6ZYRW93A6ZW6YYMK/BRBRD830J+Breadboard+MB102+w.+65+Jumper+cables.jpg)
- [1x L293D CI Motor Driver](https://www.ti.com/lit/ds/symlink/l293.pdf?HQS=dis-mous-null-mousermode-dsf-pf-null-wwe&ts=1723753701665)
- [7x TCRT5000 Infrared Sensors](https://www.haoyuelectronics.com/Attachment/TCRT5000/tcrt5000.pdf)
- 1x Custom Acrylic Base
- 1x Custom 3D Printed / PLA Chassis

## Developers

<div style="display: flex; justify-content: center; gap: 20px;">
    <div style="border: 1px solid #ddd; border-radius: 8px; padding: 10px; max-width: 200px; text-align: center;">
        <img src="https://github.com/lemosslucas.png" style="margin-bottom: 10px; max-width: 180px;"/>
        <div>
            <a href="https://github.com/lemosslucas" target="_blank" style="text-decoration: none; font-size: 16px; font-weight: bold;">Lucas Lemos Ricaldoni</a><br/>
            <span style="font-size: 14px; color: #555;">Lead Programmer, Principal Technician</span>
        </div>
    </div>
    <div style="border: 1px solid #ddd; border-radius: 8px; padding: 10px; max-width: 200px; text-align: center;">
        <img src="https://github.com/mateusdcp13.png" style="margin-bottom: 10px; max-width: 180px;"/>
        <div>
            <a href="https://github.com/mateusdcp13" target="_blank" style="text-decoration: none; font-size: 16px; font-weight: bold;">Mateus de Carvalho Pedrosa</a><br/>
            <span style="font-size: 14px; color: #555;">Lead Prototyper and Technician</span>
        </div>
    </div>
    <div style="border: 1px solid #ddd; border-radius: 8px; padding: 10px; max-width: 200px; text-align: center;">
        <img src="https://github.com/PauloMendesPVRM.png" style="margin-bottom: 10px; max-width: 180px;"/>
        <div>
            <a href="https://github.com/PauloMendesPVRM" target="_blank" style="text-decoration: none; font-size: 16px; font-weight: bold;">Paulo Vasconcelos Rodrigues Mendes</a><br/>
            <span style="font-size: 14px; color: #555;">Lead Designer and Support Prototyper</span>
        </div>
    </div>
</div>

## License
This project is licensed under the MIT License. See the file [LICENSE](LICENSE) for details.
