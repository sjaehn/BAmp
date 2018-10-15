#ifndef BAMP_H_
#define BAMP_H_

#define AMP_URI "https://www.jahnichen.de/plugins/lv2/BAmp"
#define AMP_GUI_URI "https://www.jahnichen.de/plugins/lv2/BAmp#GUI"
#define AMP_N_PORTS 3

typedef enum {
	AMP_GAIN   = 0,
	AMP_INPUT  = 1,
	AMP_OUTPUT = 2
} PortIndex;

#endif /* BAMP_H_ */
