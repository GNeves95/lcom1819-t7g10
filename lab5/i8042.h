#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8254
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

#define BIT(n) (0x01<<(n))

#define KBD_IRQ	        1    

/* I/O port addresses */

#define KBD_STATUS	    0x64 /**< @brief Timer 2 count register */
#define KBD_CTRL		0x60 /**< @brief Control register */
#define OUT_BUF         0x60

#define READ_CMD        0x20

#define CHECK_KBC       0xAA

#define CHECK_KBD_INT   0xAB

#define DIS_KBD_INT     0xAD

#define EN_KBD_INT      0xAE

#define OBF             BIT(0)

#define IBF             BIT(1)

#define AUX             BIT(5)  //Mouse Data

#define TO_ERR          BIT(6)

#define PAR_ERR         BIT(7)

#define A_MAKE 0x1E

#define A_BREAK 0x9E

#define B_MAKE 0x30

#define B_BREAK 0xB0

#define C_MAKE 0x2E

#define C_BREAK 0xAE

#define D_MAKE 0x20

#define D_BREAK 0xA0

#define E_MAKE 0x12

#define E_BREAK 0x92

#define F_MAKE 0x21

#define F_BREAK 0xA1

#define G_MAKE 0x22

#define G_BREAK 0xA2

#define H_MAKE 0x23

#define H_BREAK 0xA3

#define I_MAKE 0x17

#define I_BREAK 0x97

#define J_MAKE 0x24

#define J_BREAK 0xA4

#define K_MAKE 0x25

#define K_BREAK 0xA5

#define L_MAKE 0x26

#define L_BREAK 0xA6

#define M_MAKE 0x32

#define M_BREAK 0xB2

#define N_MAKE 0x31

#define N_BREAK 0xB1

#define O_MAKE 0x18

#define O_BREAK 0x98

#define P_MAKE 0x19

#define P_BREAK 0x99

#define Q_MAKE 0x10

#define Q_BREAK 0x90

#define R_MAKE 0x13

#define R_BREAK 0x93

#define S_MAKE 0x1F

#define S_BREAK 0x9F

#define T_MAKE 0x14

#define T_BREAK 0x94

#define U_MAKE 0x16

#define U_BREAK 0x96

#define V_MAKE 0x2F

#define V_BREAK 0xAF

#define W_MAKE 0x11

#define W_BREAK 0x91

#define X_MAKE 0x2D

#define X_BREAK 0xAD

#define Y_MAKE 0x15

#define Y_BREAK 0x95

#define Z_MAKE 0x2C

#define Z_BREAK 0xAC

#define ESC_MAKE 0x01

#define ESC_BREAK 0x81

#define L_CTRL_MAKE 0x1D

#define L_CTRL_BREAK 0x9D

#define L_SHIFT_MAKE 0x2A

#define L_SHIFT_BREAK 0xAA

#define R_SHIFT_MAKE 0x36

#define R_SHIFT_BREAK 0xB6

#define SPACE_MAKE 0x39

#define SPACE_BREAK 0xB9

#define DOUBLE_CODE 0xE0

#define DELAY_US    20000


/**@}*/

#endif /* _LCOM_I8042_H */

