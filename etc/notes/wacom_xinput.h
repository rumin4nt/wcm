

/*
 
 this is a fairly horrible older implementation that I hacked apart from some xinput sample code
 
#ifndef LR_INPUTWACOMX_H_
#define LR_INPUTWACOMX_H_

#include "wacom.h"
#include <vector>
#include <string>

//#include "r3_platform.h"

#ifdef LR_ENABLE_WACOM_X

#include <X11/Xlib.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XIproto.h>
#include <X11/keysym.h>

class RInputWacomX: public RInputWacom{
	friend class RInputWacom;
	public:

	~RInputWacomX();
	void init();

	protected:

	void update();
	void handle_event();

	RInputWacomX();

private:
	XDeviceInfoPtr get_device_info( const std::string& );
	int detect_devices();
	int setup_devices();
	void process_events();
	void update_device(XDeviceInfo* );
 const char* get_event_name(int );

	int init_x();

	Display* display;
	std::vector<std::string> device_names;
	std::string stylus_name;
	std::string eraser_name;

	XDeviceInfoPtr stylus_info;
	XDeviceInfoPtr eraser_info;
	XDevice* stylus;
	XDevice* eraser;


	int stylus_event_count;
	int eraser_event_count;

	XEventClass stylus_events[32];
	XEventClass eraser_events[32];
	
	XEventClass cls;

};
#endif

#endif

*/
