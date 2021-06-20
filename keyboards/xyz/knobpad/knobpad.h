#pragma once

#include "quantum.h"

#define LAYOUT( \
	K00, K23, K30, K53, K60, K83, \
	K01, K22, K31, K52, K61, K82, \
	K02, K21, K32, K51, K62, K81, \
	K03, K20, K33, K50, K63, K80, \
	K10, K13, K40, K43, K70, K73, \
	K11, K12, K41, K42, K71, K72 \
) { \
	{ K00, K01, K02, K03 }, \
	{ K10, K11, K12, K13 }, \
	{ K20, K21, K22, K23 }, \
	{ K30, K31, K32, K33 }, \
	{ K40, K41, K42, K43 }, \
	{ K50, K51, K52, K53 }, \
	{ K60, K61, K62, K63 }, \
	{ K70, K71, K72, K73 }, \
	{ K80, K81, K82, K83 }, \
}
