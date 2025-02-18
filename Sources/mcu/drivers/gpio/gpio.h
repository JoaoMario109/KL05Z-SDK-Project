/***************************************************************************************
 * M�dulo      : gpio.h
 * Revis�o     : 1.1
 * Data        : 24/05/2023
 * Descri��o   : Arquivo com implementa��es dos M�dulos GPIO da Familia Kinetis KL05.
 * Coment�rios : Nenhum.
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

#ifndef GPIO_DRV_H_
#define GPIO_DRV_H_

/*ARQUIVOS DE CABE�ALHO*/
/*=======================================================================================*/

#include <mcu/common.h>
#include <mcu/drivers/port/port.h>

/*FIM: ARQUIVOS DE CABE�ALHO*/
/*=======================================================================================*/

#if defined(__cplusplus)
extern "C" {
#endif

/*PROT�TIPOS - FUN��ES INLINE*/
/*=======================================================================================*/

static inline void GPIO_SetPin(GPIO_Type *base, uint8_t pin);
static inline void GPIO_ClearPin(GPIO_Type *base, uint8_t pin);
static inline void GPIO_TogglePin(GPIO_Type *base, uint8_t pin);
static inline void GPIO_WritePin(GPIO_Type *base, uint8_t pin, uint8_t value);
static inline void GPIO_SetPinsMask(GPIO_Type *base, uint32_t mask);
static inline void GPIO_ClearPinsMask(GPIO_Type *base, uint32_t mask);
static inline uint8_t GPIO_ReadPin(GPIO_Type *base, uint8_t pin);

/*FIM: PROT�TIPOS - FUN��ES INLINE*/
/*=======================================================================================*/

/*PROTOTIPOS - FUN��ES P�BLICAS*/
/*=======================================================================================*/

/**********************************************************************
 * Fun��o		:	GPIO_InitInputPin
 *
 * Descri��o	:   Inicializa pino de m�dulo GPIO como entrada.
 *
 * Entradas		:   *base - registrador base do perif�rico GPIO;
 *                  pin   - numera��o do pino.
 *
 * Sa�das		:  	Nenhuma.
 *
 * Coment�rios 	: 	Essa fun��o tamb�m realiza a multiplexa��o do
 *                  do pino do PORT relacionado ao GPIO.
 * ********************************************************************/
void GPIO_InitInputPin(GPIO_Type *base, uint8_t pin);

/**********************************************************************
 * Fun��o		:	GPIO_InitOutputPin
 *
 * Descri��o	:   Inicializa pino de m�dulo GPIO como sa�da.
 *
 * Entradas		:   *base - registrador base do perif�rico GPIO;
 *                  pin   - numera��o do pino;
 *                  initVal - valor l�gico inicial do pino (0 ou !0).
 *
 * Sa�das		:  	Nenhuma.
 *
 * Coment�rios 	: 	Essa fun��o tamb�m realiza a multiplexa��o do
 *                  do pino do PORT relacionado ao GPIO.
 * ********************************************************************/
void GPIO_InitOutputPin(GPIO_Type *base, uint8_t pin, uint8_t initVal);

/*FIM: PROTOTIPOS - FUN��ES P�BLICAS*/
/*=======================================================================================*/

/*FUN��ES INLINE*/
/*=======================================================================================*/

/**********************************************************************
 * Fun��o		:	GPIO_SetPinValue
 *
 * Descri��o	:   Coloca o valor l�gico 1 no pino do m�dulo GPIO.
 *
 * Entradas		:   *base - registrador base do perif�rico GPIO;
 *                  pin   - numera��o do pino.
 *
 * Sa�das		:  	Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void GPIO_SetPin(GPIO_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( ( pin >= 0 ) && ( pin < 32 ) );

	base->PSOR = (1 << pin);
}

/**********************************************************************
 * Fun��o		:	GPIO_ClearPinValue
 *
 * Descri��o	:   Coloca o valor l�gico 0 no pino do m�dulo GPIO.
 *
 * Entradas		:   *base - registrador base do perif�rico GPIO;
 *                  pin   - numera��o do pino.
 *
 * Sa�das		:  	Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void GPIO_ClearPin(GPIO_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( ( pin >= 0 ) && ( pin < 32 ) );

	base->PCOR = (1 << pin);
}

/**********************************************************************
 * Fun��o		:	GPIO_TogglePin
 *
 * Descri��o	:   Inverte o valor l�gico no pino do m�dulo GPIO.
 *
 * Entradas		:   *base - registrador base do perif�rico GPIO;
 *                  pin   - numera��o do pino.
 *
 * Sa�das		:  	Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void GPIO_TogglePin(GPIO_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( ( pin >= 0 ) && ( pin < 32 ) );

	base->PTOR = (1 << pin);
}

/**********************************************************************
 * Fun��o		:	GPIO_ReadPin
 *
 * Descri��o	:   Retorna o valor do pino do m�dulo GPIO.
 *
 * Entradas		:   *base - registrador base do perif�rico GPIO;
 *                  pin   - numera��o do pino.
 *
 * Sa�das		:  	0,  valor l�gico 0;
 * 					1,  valor l�gico 1.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline uint8_t GPIO_ReadPin(GPIO_Type *base, uint8_t pin)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( ( pin >= 0 ) && ( pin < 32 ) );

	return (uint8_t)((base->PDIR & (((uint32_t)1) << pin)) != 0);
}

/**********************************************************************
 * Fun��o		:	GPIO_WritePin
 *
 * Descri��o	:   Escreve o valor no pino do m�dulo GPIO.
 *
 * Entradas		:   *base - registrador base do perif�rico GPIO;
 *                  pin   - numera��o do pino;
 *                  value - valor l�gico (0 ou 1) para enviar.
 *
 * Sa�das		:  	Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void GPIO_WritePin(GPIO_Type *base, uint8_t pin, uint8_t value)
{
	SYSTEM_ASSERT(base);
	SYSTEM_ASSERT( ( pin >= 0 ) && ( pin < 32 ) );

	if(pin)
	{
		base->PDOR |= 1 << pin;
	}
	else
	{
		base->PDOR &= ~(1 << pin);
	}
}

/**********************************************************************
 * Fun��o		:	GPIO_SetPinsMask
 *
 * Descri��o	:   Seta os pinos do m�dulo GPIO correspondentes ao bit 1
 *                  na m�scara.
 *
 * Entradas		:   *base - registrador base do perif�rico GPIO;
 *                  mask - m�scara de bits.
 *
 * Sa�das		:  	Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void GPIO_SetPinsMask(GPIO_Type *base, uint32_t mask)
{
	SYSTEM_ASSERT(base);

	base->PSOR = mask;
}

/**********************************************************************
 * Fun��o		:	GPIO_ClearPinsMask
 *
 * Descri��o	:   Limpa os pinos do m�dulo GPIO correspondentes ao bit 1
 *                  na m�scara.
 *
 * Entradas		:   *base - registrador base do perif�rico GPIO;
 *                  mask - m�scara de bits.
 *
 * Sa�das		:  	Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void GPIO_ClearPinsMask(GPIO_Type *base, uint32_t mask)
{
	SYSTEM_ASSERT(base);

	base->PCOR = mask;
}

/*FIM: FUN��ES INLINE*/
/*=======================================================================================*/

#if defined(__cplusplus)
}
#endif

#endif /* GPIO_DRV_H_ */

/***************************************************************************************
 * FIM: M�dulo - gpio.h
 ***************************************************************************************/
