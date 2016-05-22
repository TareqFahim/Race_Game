// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the edX Lab 15
// In order for other students to play your game
// 1) You must leave the hardware configuration as defined
// 2) You must not add/remove any files from the project
// 3) You must add your code only this this C file
// I.e., if you wish to use code from sprite.c or sound.c, move that code in this file
// 4) It must compile with the 32k limit of the free Keil

// April 10, 2014
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Required Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground

// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// 3.3V          (VCC, pin 1) power
// Ground        (GND, pin 2) ground
// SSI0Fss       (SCE, pin 3) connected to PA3
// Reset         (RST, pin 4) connected to PA7
// Data/Command  (D/C, pin 5) connected to PA6
// SSI0Tx        (DN,  pin 6) connected to PA5
// SSI0Clk       (SCLK, pin 7) connected to PA2
// back light    (LED, pin 8) not connected, consists of 4 white LEDs which draw ~80mA total

#include "C:\Keil\Labware\tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include <time.h>
#include <stdlib.h>
#include "GameImage.h"
void Delay100ms(unsigned long count); // time delay in 0.1 seconds

void PortE_Init(void);

int life = 3, flag = 0 , score = 0, level = 10, inc = 1, player_current_track = 22 , car_track[3] = {15, 30, 45};
unsigned long i = 0,j = 2, z = 1, game_car1_new_pos = -5, game_car2_new_pos = -10;
long game_car1_pos , game_car2_pos, speed = 2 ;


int main(void){
  TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
  Random_Init(1);
	PortE_Init();
  Nokia5110_Init();
  Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();      // draw buffer
	//Welcome screen
	do{
			Nokia5110_Clear();
			Nokia5110_SetCursor(4, 1);
			Nokia5110_OutString("Click");
			Nokia5110_SetCursor(4, 2);
			Nokia5110_OutString("To");
			Nokia5110_SetCursor(4, 3);
			Nokia5110_OutString("Start");
			Delay100ms(1);
		
			if((GPIO_PORTE_DATA_R & 0x01)){
				flag = 1;
			}
	}while(flag == 0);
	
	while(1){
	// Create game cars at random positions
	game_car1_pos = -(Random32() % 100);
	game_car2_pos = -(Random32() % 5);
	if(flag == 1){
	for(;;){
		
		Nokia5110_ClearBuffer();
		//Increase Level
		if(score == level){
			level = level + (level * inc++);
			speed++;
		}
		//inc X 
		game_car1_pos += speed;
		game_car2_pos += speed;
		//draw cars
		
		Nokia5110_PrintBMP(game_car1_pos,car_track[i], car, 0);
		Nokia5110_PrintBMP(game_car2_pos, car_track[j], car, 0);
		
	
		//Change Player Position
		//Right
		if((GPIO_PORTE_DATA_R & 0x01)){
			Delay100ms(1);
			//If at Right Donot mins
			if(z != 0)
				z = z - 1;
			//Left
		}else if((GPIO_PORTE_DATA_R & 0x10)){
			Delay100ms(1);
						//If at Left Donot add
			if(z != 2)
				z = z + 1;
		}
		//Print Player
		Nokia5110_PrintBMP(62, car_track[z], car, 0);
		player_current_track = car_track[z];
		
		Nokia5110_DisplayBuffer();     // draw buffer
		Delay100ms(1);              // delay 0.1 sec at 50 MHz
		
		//Check if END OF GAME
		if((game_car1_pos >= 43 && player_current_track == car_track[i]) || (game_car2_pos >= 43 && player_current_track == car_track[j])){
			life = life -1;
			if(life == 0) 
				flag = 2;  //Display GAME OVER screen 
				break;
			
		}else if((game_car1_pos >= 60 && player_current_track != car_track[i])|| (game_car2_pos >= 60 && player_current_track != car_track[j])){
			//increment score 
			score++;
		}
		//New Position for enemys
		if(game_car1_pos >= 60){
			i = Random32() % 3;
			if(i == j)
				i = (i + 1) % 3;
			game_car1_new_pos = Random32() % 100;
			game_car1_pos = -1 * game_car1_new_pos;
		}
		if(game_car2_pos >= 60){
			j = Random32() % 3;
			if(j == i)
				j = (j + 1) % 3;
			game_car2_new_pos = Random32() % 5;
			game_car2_pos = -1 * game_car2_new_pos;
		}
	}
}
	
	//Delay100ms(50);              // delay 5 sec at 50 MHz
  if(flag == 2){
		Nokia5110_Clear();
		Nokia5110_SetCursor(1, 1);
		Nokia5110_OutString("GAME OVER");
		Nokia5110_SetCursor(1, 2);
		Nokia5110_OutString("Score");
		Nokia5110_SetCursor(6, 2);
		Nokia5110_OutUDec(score);
		Nokia5110_SetCursor(1, 3);
		Nokia5110_OutString("Hit Start");
		Nokia5110_SetCursor(1, 4);
		Nokia5110_OutString("To Retry !");
		if((GPIO_PORTE_DATA_R & 0x01)){
			flag = 1;
			z = 1;
			level = 10;
			score = 0;
			speed = 2;
			life = 3;
		}
		Delay100ms(1);
  }
  }

}



void Delay100ms(unsigned long count){unsigned long volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
void PortE_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000010;     // 1) E clock
  delay = SYSCTL_RCGC2_R;     	// delay   
	
  GPIO_PORTE_LOCK_R = 0x4C4F434B;   // 2) unlock PortE PE0  
  GPIO_PORTE_CR_R = 0x1F;           // allow changes to PE4-0       
  GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R = 0xF8;          // 5) PE0,PE1,PE2 input, rest is output   
  GPIO_PORTE_AFSEL_R = 0x00;        // 6) no alternate function
	//GPIO_PORTE_PUR_R = 0x1F;          // enable pullup resistors        
  GPIO_PORTE_DEN_R = 0x1F;          // 7) enable digital pins PE4-PE0        
	//Enable interrupts
	//portEInterrupt();
}
