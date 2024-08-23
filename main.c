#include <stdio.h>
#include <stdint.h>

#define SIM_5 		(*((volatile uint32_t *) 0x40048038)) //Direccion del reloj que se debe de activar clk 5
#define PCR_PTB22	(*((volatile uint32_t *) 0x4004A058)) //PIN CONTROL REGISTER_ PTB22
#define PCR_PTB21	(*((volatile uint32_t *) 0X4004A054)) //PIN CONTROL REGISTER_ PTB21
#define PCR_PTE26	(*((volatile uint32_t *) 0x4004D068 )) //PIN CONTROL REGISTER_ PTE26

#define GPIO_B_BASE_ADDR (((volatile uint32_t *) 0x400FF040)) //DIRECCIÓN BASE DE GPIO B
#define GPIO_E_BASE_ADDR (((volatile uint32_t *) 0x400FF100)) //DIRECCIÓN BASE DE GPIO E

#define GPIO_E_PDOR	(*((volatile uint32_t *) 0x400FF100)) //OUTPUT REGISTER
#define GPIO_E_PDDR	(*((volatile uint32_t *) 0x400FF114)) //DIRECTION REGISTER



typedef struct{
	uint32_t PDOR;
	uint32_t PSOR;
	uint32_t PCOR;
	uint32_t PTOR;
	uint32_t PDIR;
	uint32_t PDDR;
} GPIO_t;

#define GPIO_B ((GPIO_t*) GPIO_B_BASE_ADDR)
#define GPIO_E ((GPIO_t*) GPIO_E_BASE_ADDR)

int main(void) {

	SIM_5 = 0x00002400; // Bits 13 y 10 enecendidos para activar el clock e y b
	PCR_PTB22 = 0x00000100; //Pin mux control, ALT1 GPIO
	PCR_PTB21 = 0x00000100; //Pin mux control, ALT1 GPIO
	PCR_PTE26 = 0x00000100; //Pin mux control, ALT1 GPIO

	GPIO_B->PDOR |= (1u << 22) | (1u << 21); // Apaga los LEDs en PTB22 y PTB21, para evitar problemas con PDDR
	GPIO_E->PDOR |= (1u << 26);              // Apaga el LED en PTE26, , para evitar problemas con PDDR

	GPIO_B->PDDR |= (1u << 22) | (1u << 21); //Configura PTB22 y PTB21 como salidas
	GPIO_E->PDDR |= (1u << 26); //Configura PTE26 como salida

    while(1) {

    	//Encender LED ROJO
    	GPIO_B->PDOR &= ~(1u << 22); //K64 tiene una lógica inversa, los leds encienden con 0
    	GPIO_B->PDOR |=  (1u << 21); //LED AZUL OFF
    	GPIO_E->PDOR |=  (1u << 26); //LED VERDE OFF

    	//Encender LED AZUL
    	GPIO_B->PDOR |=  (1u << 22); //LED ROJO OFF
    	GPIO_B->PDOR &= ~(1u << 21); //LED AZUL ON
    	GPIO_E->PDOR |=  (1u << 26); //LED VERDE OFF

    	//Encender LED VERDE
    	GPIO_B->PDOR |=  (1u << 22); //LED ROJO OFF
    	GPIO_B->PDOR |=  (1u << 21); //LED AZUL OFF
    	GPIO_E->PDOR &= ~(1u << 26); //LED VERDE ON
    	GPIO_E->PDOR |=  (1u << 26); //LED VERDE OFF

    	//Enceder LED ROJO,AZUL, VERDE = BLANCO
    	GPIO_B->PDOR &= ~(1u << 22); //LED ROJO ON
    	GPIO_B->PDOR &= ~(1u << 21); //LED AZUL ON
    	GPIO_E->PDOR &= ~(1u << 26); //LED VERDE ON

    	//Apagar todos los LED
    	GPIO_B->PDOR |=  (1u << 22); //LED ROJO OFF
    	GPIO_B->PDOR |=  (1u << 21); //LED AZUL OFF
    	GPIO_E->PDOR |=  (1u << 26); //LED VERDE OFF

    }
    return 0 ;
}
