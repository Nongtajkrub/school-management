#include "header.h"

ui_head_component_t ui_head_component_make(const char* label) {
	return (ui_head_component_t) {
		.label = label
	};
}
