#ifndef INIT_H_
#define INIT_H_

#define BUTTONS_PORT PORTD
#define BUTTONS_DDR  DDRD
#define BUTTONS_PIN  PIND

#define BTN_IRQ_PIN_NUM 2
#define BTN1_PIN_NUM    0
#define BTN2_PIN_NUM    1
#define BTN3_PIN_NUM    3
#define BTN4_PIN_NUM    4

#define BTN_IRQ_PIN (1<<BTN_IRQ_PIN_NUM)
#define BTN1_PIN    (1<<BTN1_PIN_NUM)
#define BTN2_PIN    (1<<BTN2_PIN_NUM)
#define BTN3_PIN    (1<<BTN3_PIN_NUM)
#define BTN4_PIN    (1<<BTN4_PIN_NUM)

#define INPUT_PIN_MASK  (BTN1_PIN | BTN2_PIN | BTN3_PIN | BTN4_PIN | BTN_IRQ_PIN)
#define BUTTON_PIN_MASK (BTN1_PIN | BTN2_PIN | BTN3_PIN | BTN4_PIN)

void initGpio();
void initIRQ0();

#endif /* INIT_H_ */