/*
 
 
 just here for reference really
 
 
 */


#ifdef THIS_WILL_NEVER_COMPILE_EVER

#include "wacom_xinput.h"

//#include "r3_config.h"

#ifdef LR_ENABLE_WACOM_X

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "../../rtypes.h"

	enum
	{
		INPUTEVENT_KEY_PRESS,
		INPUTEVENT_KEY_RELEASE,
		INPUTEVENT_FOCUS_IN,
		INPUTEVENT_FOCUS_OUT,
		INPUTEVENT_BTN_PRESS,
		INPUTEVENT_BTN_RELEASE,
		INPUTEVENT_PROXIMITY_IN,
		INPUTEVENT_PROXIMITY_OUT,
		INPUTEVENT_MOTION_NOTIFY,
		INPUTEVENT_DEVICE_STATE_NOTIFY,
		INPUTEVENT_DEVICE_MAPPING_NOTIFY,
		INPUTEVENT_CHANGE_DEVICE_NOTIFY,
		INPUTEVENT_DEVICE_POINTER_MOTION_HINT,
		INPUTEVENT_DEVICE_BUTTON_MOTION,
		INPUTEVENT_DEVICE_BUTTON1_MOTION,
		INPUTEVENT_DEVICE_BUTTON2_MOTION,
		INPUTEVENT_DEVICE_BUTTON3_MOTION,
		INPUTEVENT_DEVICE_BUTTON4_MOTION,
		INPUTEVENT_DEVICE_BUTTON5_MOTION,

		INPUTEVENT_MAX
	};

	int gnDevListCnt = 0;
	XDeviceInfoPtr gpDevList = NULL;
	int gnLastXError = 0;
	int gnVerbose = 1;
	int gnSuppress = 4;
	int gnInputEvent[INPUTEVENT_MAX] = { 0 };

using namespace std;


LRInputWacomX::LRInputWacomX() : display(NULL) {
	stylus_name = "";
	eraser_name = "";
	verbose = true;
    init();
}


LRInputWacomX::~LRInputWacomX(){
if (gpDevList)
		XFreeDeviceList(gpDevList);

	XUngrabDevice(display,stylus,CurrentTime);
	XUngrabDevice(display,eraser,CurrentTime);

	XCloseDisplay(display);

}

int ErrorHandler(Display* pDisp, XErrorEvent* pEvent)
{
	char chBuf[64];
	XGetErrorText(pDisp,pEvent->error_code,chBuf,sizeof(chBuf));
	fprintf(stderr,"X Error: %d %s\n", pEvent->error_code, chBuf);
	gnLastXError  = pEvent->error_code;
	return 0;
}

int GetLastXError(void)
{
	return gnLastXError;
}

const char* LRInputWacomX::get_event_name(int nType)
{
	static char xchBuf[64];

	switch (nType)
	{
		case KeyPress: return "KeyPress";
		case KeyRelease: return "KeyRelease";
		case ButtonPress: return "ButtonPress";
		case ButtonRelease: return "ButtonRelease";
		case MotionNotify: return "MotionNotify";
		case EnterNotify: return "EnterNotify";
		case LeaveNotify: return "LeaveNotify";
		case FocusIn: return "FocusIn";
		case FocusOut: return "FocusOut";
		case KeymapNotify: return "KeymapNotify";
		case Expose: return "Expose";
		case GraphicsExpose: return "GraphicsExpose";
		case NoExpose: return "NoExpose";
		case VisibilityNotify: return "VisibilityNotify";
		case CreateNotify: return "CreateNotify";
		case DestroyNotify: return "DestroyNotify";
		case UnmapNotify: return "UnmapNotify";
		case MapNotify: return "MapNotify";
		case MapRequest: return "MapRequest";
		case ReparentNotify: return "ReparentNotify";
		case ConfigureNotify: return "ConfigureNotify";
		case ConfigureRequest: return "ConfigureRequest";
		case GravityNotify: return "GravityNotify";
		case ResizeRequest: return "ResizeRequest";
		case CirculateNotify: return "CirculateNotify";
		case CirculateRequest: return "CirculateRequest";
		case PropertyNotify: return "PropertyNotify";
		case SelectionClear: return "SelectionClear";
		case SelectionRequest: return "SelectionRequest";
		case SelectionNotify: return "SelectionNotify";
		case ColormapNotify: return "ColormapNotify";
		case ClientMessage: return "ClientMessage";
		case MappingNotify: return "MappingNotify";

		default:
		if (nType == gnInputEvent[INPUTEVENT_KEY_PRESS])
			return "XIKeyPress";
		else if (nType == gnInputEvent[INPUTEVENT_KEY_RELEASE])
			return "XIKeyRelease";
		else if (nType == gnInputEvent[INPUTEVENT_FOCUS_IN])
			return "XIFocusIn";
		else if (nType == gnInputEvent[INPUTEVENT_FOCUS_OUT])
			return "XIFocusOut";
		else if (nType == gnInputEvent[INPUTEVENT_BTN_PRESS])
			return "XIButtonPress";
		else if (nType == gnInputEvent[INPUTEVENT_BTN_RELEASE])
			return "XIButtonRelease";
		else if (nType == gnInputEvent[INPUTEVENT_PROXIMITY_IN])
			return "XIProximityIn";
		else if (nType == gnInputEvent[INPUTEVENT_PROXIMITY_OUT])
			return "XIProximityOut";
		else if (nType == gnInputEvent[INPUTEVENT_MOTION_NOTIFY])
			return "XIMotionNotify";
		else if (nType == gnInputEvent[INPUTEVENT_DEVICE_STATE_NOTIFY])
			return "XIDeviceStateNotify";
		else if (nType == gnInputEvent[INPUTEVENT_DEVICE_MAPPING_NOTIFY])
			return "XIDeviceMappingNotify";
		else if (nType == gnInputEvent[INPUTEVENT_CHANGE_DEVICE_NOTIFY])
			return "XIChangeDeviceNotify";
		else if (nType == gnInputEvent[INPUTEVENT_DEVICE_POINTER_MOTION_HINT])
			return "XIDevicePointerMotionHint";
		else if (nType == gnInputEvent[INPUTEVENT_DEVICE_BUTTON_MOTION])
			return "XIDeviceButtonMotion";
		else if (nType == gnInputEvent[INPUTEVENT_DEVICE_BUTTON1_MOTION])
			return "XIDeviceButton1Motion";
		else if (nType == gnInputEvent[INPUTEVENT_DEVICE_BUTTON2_MOTION])
			return "XIDeviceButton2Motion";
		else if (nType == gnInputEvent[INPUTEVENT_DEVICE_BUTTON3_MOTION])
			return "XIDeviceButton3Motion";
		else if (nType == gnInputEvent[INPUTEVENT_DEVICE_BUTTON4_MOTION])
			return "XIDeviceButton4Motion";
		else if (nType == gnInputEvent[INPUTEVENT_DEVICE_BUTTON5_MOTION])
			return "XIDeviceButton5Motion";
	}

	snprintf(xchBuf,sizeof(xchBuf),"Event_%d",nType);
	return xchBuf;
}

void LRInputWacomX::init(){
	int res;

	res = init_x();
	
	if ( GetLastXError() != 0 ){

        printf("X errored out after init, aborting\n");
        return;
	}
	
	detect_devices();

	//	see if we're gonna skip this step (at our
	//	own peril)
	std::string hostname = LRIO::get_hostname();
	cout << hostname << endl;
	//printf("host:[%s]\n", hostname.c_str() );
	bool bypass_mode = false;

	if ( hostname == "vs-tabula" || hostname == "vs-sketch"){
		cout << "Tablet-possessing hostname found. skipping wacom safety check."<< endl;
		bypass_mode = true;
	}

	res = GetLastXError();
    if ( res != 0 ){
    	printf("Res: %d\n", res);
        printf("X errored out after device detection., aborting\n");
        if ( !bypass_mode ) 
        	return;
	}
	
	setup_devices();
}

int LRInputWacomX::init_x(){
	//Display* pDisp;
	int nMajor, nFEV, nFER;
	cout << "Opening X display." << endl;

	display = XOpenDisplay(NULL);
	if ( !display){
		cout << "Failed to connect to X server!" << endl;
		return 1;
	}
	XSetErrorHandler(ErrorHandler);
	//XSynchronize(pDisp,1);

	if ( !XQueryExtension( display, INAME, &nMajor, &nFEV, &nFER)){
		cout << "Server does not support XInput extension\n";
		XCloseDisplay(display);
		return 2;
	}
	Window win;
	/*
	win = XCreateSimpleWindow(display, RootWindow(display, 0), 1, 1, 256, 256, \
        0, BlackPixel (display, 0), BlackPixel(display, 0));
*/
	XSelectInput(display ,DefaultRootWindow(display),ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask |
        ButtonPressMask | ButtonReleaseMask  | StructureNotifyMask);
	cout << "Opened ex!\n";

  //  XMapWindow(display, win);
    //XFlush(display);
	return 0;

}

int LRInputWacomX::detect_devices(){
	cout << "Detecting devices.\n";
		int i, j, k;
	XDeviceInfoPtr pDev;
	XAnyClassPtr pClass;

	/* get list of devices */
	if (!gpDevList)
	{
		gpDevList = (XDeviceInfoPtr) XListInputDevices(display, &gnDevListCnt);
		if (!gpDevList)
		{
			fprintf(stderr,"Failed to get input device list.\n");
			return 1;
		}
	}

	cout << "Devices listed.\n";
	for (i=0; i<gnDevListCnt; ++i)
	{
		pDev = gpDevList + i;

		/* if device name is specified, skip other devices */
		//if (pszDeviceName && strcasecmp(pDev->name, pszDeviceName))
		//	continue;
		//	check for stylus
		if (pDev->num_classes){
			//cout << "DEVICE:" <<pDev->name << "<<<" << endl;
			//cout << pDev->name << endl;
			//cout << pDev->name << endl;

			char* pos = strstr(pDev->name, "stylus");
			if ( pos != NULL ) {
				stylus_name = string(pDev->name);

				if ( verbose ) printf("STYLUS FOUND %s\n", pDev->name );
			//}

			//	I'll COME BACK FOR YOU SOME DAY
			//pos = strstr(pDev->name, "eraser");
			//if ( pos != NULL ) {
			//	eraser_name = string(pDev->name);
			//	printf("ERASER FOUND %s\n", pDev->name );
			//}


			//printf("%s\n", pos );
			//printf("%s\n", pDev->name );
			/*
			printf("%-30s %s\n",
				pDev->name,
				(pDev->use == 0) ? "disabled" :
				(pDev->use == IsXKeyboard) ? "keyboard" :
				(pDev->use == IsXPointer) ? "pointer" :
#ifndef WCM_ISXEXTENSIONPOINTER
				(pDev->use == IsXExtensionDevice) ?
#else
				(pDev->use == IsXExtensionDevice ||
				 pDev->use == IsXExtensionKeyboard ||
				 pDev->use == IsXExtensionPointer) ?
#endif
					"extension" : "unknown");
							// skip to next record
				cout << pDev->name << "]]]" << endl;
			*/
			//}

			pClass = pDev->inputclassinfo;
			for (j=0; j<pDev->num_classes; ++j)
			{
				switch (pClass->c_class)
				{
					case ButtonClass:
					{
						//XButtonInfo* pBtn = (XButtonInfo*)pClass;
						//printf("    btn: num=%d\n",pBtn->num_buttons);
						break;
					}

					case FocusClass:
					{
						//printf("  focus:\n");
						break;
					}

					case KeyClass:
					{
						//XKeyInfo* pKey = (XKeyInfo*)pClass;
						//printf("    key: min=%d, max=%d, num=%d\n",
						//		pKey->min_keycode,
						//		pKey->max_keycode,
						//		pKey->num_keys);
						break;
					}

					case ValuatorClass:
					{
						XValuatorInfoPtr pVal = (XValuatorInfoPtr)pClass;


						/*printf("    val: axes=%d mode=%s buf=%ld\n",
								pVal->num_axes,
								pVal->mode == Absolute ? "abs" :
								pVal->mode == Relative ? "rel" : "unk",
								pVal->motion_buffer);
						*/

						for (k=0; k<pVal->num_axes; ++k) {
							printf("k: %d\n", k);
							if ( k == 0 ) {
								canvas_x = pVal->axes[k].max_value;
								if ( verbose ) printf("inputwacomx: x param %d\n",canvas_x);
							}else
							if ( k == 1 ) {
								canvas_y = pVal->axes[k].max_value;
								if ( verbose ) printf("inputwacomx: y param %d\n", canvas_y);
							}else
							if ( k == 2 ) {
								pressure_max = pVal->axes[k].max_value;
								if ( verbose ) printf("Pressure: %d\n", pressure_max);
							}

						}
						break;
					}

					default:
						if ( verbose ) printf("  unknown class\n" );
				}


				pClass = (XAnyClassPtr)((char*)pClass + pClass->length);
			}
			if ( verbose ) printf("\n");
		}
		}
	}

	return 0;
}

int LRInputWacomX::setup_devices(){
    stylus = NULL;
	if ( stylus_name != "" ){
		if ( verbose ) printf("Setting up stylus: %s\n", stylus_name.c_str() );
		stylus_info = get_device_info( stylus_name);
	}
	//if ( eraser_name != "" ){
	//	printf("Setting up eraser.\n");
	//	eraser_info = get_device( eraser_name);
	//}

	if ( verbose ) printf("stylus: %p\n", stylus_info);
	//printf("eraser: %p\n", eraser_info);
    if ( stylus_info == NULL ) {
    	if ( verbose ) printf("Couldn't open stylus info.\n");
        return 1;
    }
 	stylus = XOpenDevice(display, stylus_info->id);
	printf("Stylus opened.\n");
	//eraser = XOpenDevice(display, eraser_info->id);

	//int nRtn;
	//XDevice* pDev;
	//XDeviceInfoPtr pDevInfo;
	//int nEventListCnt = 0;
	//XEventClass eventList[32];
	//XEventClass cls;
	stylus_event_count = 0;

	/* key events */
	DeviceKeyPress(stylus,gnInputEvent[INPUTEVENT_KEY_PRESS],cls);
	if (cls) stylus_events[stylus_event_count++] = cls;
	DeviceKeyRelease(stylus,gnInputEvent[INPUTEVENT_KEY_RELEASE],cls);
	if (cls) stylus_events[stylus_event_count++] = cls;

	/* focus events */
	DeviceFocusIn(stylus,gnInputEvent[INPUTEVENT_FOCUS_IN],cls);
	if (cls) stylus_events[stylus_event_count++] = cls;
	DeviceFocusOut(stylus,gnInputEvent[INPUTEVENT_FOCUS_OUT],cls);
	if (cls) stylus_events[stylus_event_count++] = cls;

	/* button events */
	DeviceButtonPress(stylus,gnInputEvent[INPUTEVENT_BTN_PRESS],cls);
	if (cls) stylus_events[stylus_event_count++] = cls;
	DeviceButtonRelease(stylus,gnInputEvent[INPUTEVENT_BTN_RELEASE],cls);
	if (cls) stylus_events[stylus_event_count++] = cls;

	/* proximity events */
	ProximityIn(stylus,gnInputEvent[INPUTEVENT_PROXIMITY_IN],cls);
	if (cls) stylus_events[stylus_event_count++] = cls;
	ProximityOut(stylus,gnInputEvent[INPUTEVENT_PROXIMITY_OUT],cls);
	if (cls) stylus_events[stylus_event_count++] = cls;

	/* motion events */
	DeviceMotionNotify(stylus,gnInputEvent[INPUTEVENT_MOTION_NOTIFY],cls);
	if (cls) stylus_events[stylus_event_count++] = cls;

	/* device state */
	DeviceStateNotify(stylus,gnInputEvent[INPUTEVENT_DEVICE_STATE_NOTIFY],cls);
	if (cls) stylus_events[stylus_event_count++] = cls;
	DeviceMappingNotify(stylus,
			gnInputEvent[INPUTEVENT_DEVICE_MAPPING_NOTIFY],cls);
	if (cls) stylus_events[stylus_event_count++] = cls;
	ChangeDeviceNotify(stylus,gnInputEvent[INPUTEVENT_CHANGE_DEVICE_NOTIFY],cls);
	if (cls) stylus_events[stylus_event_count++] = cls;

	/* this cuts the motion data down - not sure if this is useful */
	//DevicePointerMotionHint(stylus,
	//		gnInputEvent[INPUTEVENT_DEVICE_POINTER_MOTION_HINT],cls);
	//if (cls) stylus_events[stylus_event_count++] = cls;


	/* button motion */
	DeviceButtonMotion(stylus,
			gnInputEvent[INPUTEVENT_DEVICE_BUTTON_MOTION],cls);
	if (cls) stylus_events[stylus_event_count++] = cls;
	DeviceButton1Motion(stylus,
			gnInputEvent[INPUTEVENT_DEVICE_BUTTON1_MOTION],cls);
	if (cls) stylus_events[stylus_event_count++] = cls;
	DeviceButton2Motion(stylus,
			gnInputEvent[INPUTEVENT_DEVICE_BUTTON2_MOTION],cls);
	if (cls) stylus_events[stylus_event_count++] = cls;
	DeviceButton3Motion(stylus,
			gnInputEvent[INPUTEVENT_DEVICE_BUTTON3_MOTION],cls);
	if (cls) stylus_events[stylus_event_count++] = cls;
	DeviceButton4Motion(stylus,
			gnInputEvent[INPUTEVENT_DEVICE_BUTTON4_MOTION],cls);
	if (cls) stylus_events[stylus_event_count++] = cls;
	DeviceButton5Motion(stylus,
			gnInputEvent[INPUTEVENT_DEVICE_BUTTON5_MOTION],cls);
	if (cls) stylus_events[stylus_event_count++] = cls;

	/* specify which events to report */
	/* XSelectInput(pDisp,wnd,0x00FFFFFF ^ PointerMotionHintMask); */
	/* XSelectExtensionEvent(pDisp,wnd,eventList,nEventListCnt); */

	cout << "Grabbing stylus.\n";
		// grab device - work whether pointer is in active window or not
	XGrabDevice(display,stylus,DefaultRootWindow(display),
			0, // no owner events
			stylus_event_count, stylus_events, //events
			GrabModeAsync, //don't queue, give me whatever you got
			GrabModeAsync, // same
			CurrentTime);


		// grab device - work whether pointer is in active window or not
	/*XGrabDevice(display,eraser,DefaultRootWindow(display),
			0, // no owner events
			nEventListCnt, eventList, // events
			GrabModeAsync, // don't queue, give me whatever you got
			GrabModeAsync, // same
			CurrentTime);
*/

}

XDeviceInfoPtr LRInputWacomX::get_device_info( const std::string& s ){
	int i;
	if ( verbose ) printf("fetching info about a device.\n");
	/* get list of devices */
	if (!gpDevList)
	{
		gpDevList = (XDeviceInfoPtr) XListInputDevices(display, &gnDevListCnt);
		if (!gpDevList)
		{
			fprintf(stderr,"Failed to get input device list.\n");
			return NULL;
		}
	}

	/* find device by name */
	for (i=0; i<gnDevListCnt; ++i)
	{
		if (!strcasecmp(gpDevList[i].name, s.c_str() ) &&
			gpDevList[i].num_classes)
			return gpDevList + i;
	}

	return NULL;
}

void LRInputWacomX::update_device(XDeviceInfo* pdev){


}

void LRInputWacomX::process_events(){


	XEvent event;
	XAnyEvent* pAny;
	struct timeval tv;
	double dStart, dNow;

	gettimeofday(&tv,NULL);
	dStart = tv.tv_sec + (double)tv.tv_usec / 1E6;
	//while (1)
	//{
	//cout << "]";
	while(XPending(display) > 0 ){
		XNextEvent(display,&event);
		//cout << ",";
		pAny = (XAnyEvent*)&event;
		/* printf("event: type=%s\n",GetEventName(pAny->type)); */

		gettimeofday(&tv,NULL);
		dNow = tv.tv_sec + (double)tv.tv_usec / 1E6;
		//printf("%.8f: ",(dNow - dStart));

		if (pAny->type == gnInputEvent[INPUTEVENT_PROXIMITY_IN] || pAny->type == gnInputEvent[INPUTEVENT_PROXIMITY_OUT]){
			//printf("Proximity\n");
			LRMessage m;
			m.domain = LRMSG_INPUT;
			m.type = LRINPUT_WACOM;
			//	this should be bitfield really, not int oh well
			m.i[0] = LRWAC_PROXIMITY;
			m.i[1] = pAny->type == gnInputEvent[INPUTEVENT_PROXIMITY_IN];
			send_msg(m);
		}


		/*else if (pAny->type == gnInputEvent[INPUTEVENT_FOCUS_IN])
			printf("Focus In\n");
		else if (pAny->type == gnInputEvent[INPUTEVENT_FOCUS_OUT])
			printf("Focus Out\n");
		*/
		else if (pAny->type == gnInputEvent[INPUTEVENT_MOTION_NOTIFY])
		{
			//printf("mmm");
			/*
			printf("Motion: x=%+6d y=%+6d p=%4d tx=%+4d ty=%+4d "
				"w=%+5d \n",
					((XDeviceMotionEvent*)pAny)->axis_data[0],
					((XDeviceMotionEvent*)pAny)->axis_data[1],
					((XDeviceMotionEvent*)pAny)->axis_data[2],
					(short)(((XDeviceMotionEvent*)pAny)->axis_data[3]&0xffff),
					(short)(((XDeviceMotionEvent*)pAny)->axis_data[4]&0xffff),
					(short)(((XDeviceMotionEvent*)pAny)->axis_data[5]&0xffff));
			*/
			LRMessage m;
			m.domain = LRMSG_INPUT;
			m.type = LRINPUT_WACOM;
			m.i[0] = LRWAC_MOTION;
			float x = ((XDeviceMotionEvent*)pAny)->axis_data[0];
			float y = ((XDeviceMotionEvent*)pAny)->axis_data[1];
			float p = ((XDeviceMotionEvent*)pAny)->axis_data[2];
			x /= canvas_x;
			y /= canvas_y;
			p /= pressure_max;
			m.f[0] = x;
			m.f[1] = y;
			m.f[2] = p;
			send_msg(m);

		}

		else if ((pAny->type == gnInputEvent[INPUTEVENT_BTN_PRESS]) ||
				(pAny->type == gnInputEvent[INPUTEVENT_BTN_RELEASE]))
		{


			XDeviceButtonEvent* pBtn = (XDeviceButtonEvent*)pAny;
			int sta = pAny->type == gnInputEvent[INPUTEVENT_BTN_PRESS];
			int btn = pBtn->button;

			printf("drvbutton %d %d\n", btn, sta );

			LRMessage m;
			m.domain = LRMSG_INPUT;
			m.type = LRINPUT_WACOM;
			m.i[0] = LRWAC_BUTTON;
			m.i[1] = btn;;

			pAny->type == gnInputEvent[INPUTEVENT_BTN_PRESS] ?
				m.i[2] = 1 : m.i[2] = 0;
			//m.i[2] = gnInputEvent[INPUTEVENT_BTN_PRESS]
			//printf("Button: %d %s\n",pBtn->button,
			//		pAny->type == gnInputEvent[INPUTEVENT_BTN_PRESS] ?
			//			"DOWN" : "UP");
			send_msg(m);
		}
		else if ((pAny->type == gnInputEvent[INPUTEVENT_KEY_PRESS]) ||
				(pAny->type == gnInputEvent[INPUTEVENT_KEY_RELEASE]))
		{
			/*
			XDeviceKeyEvent* pKey = (XDeviceKeyEvent*)pAny;
			printf("Key: %d %s\n", pKey->keycode - 7,
			       (pAny->type == gnInputEvent[INPUTEVENT_KEY_PRESS]) ?
			       "DOWN" : "UP");
			*/
		}
		else
		{
			printf("Event: %s\n", get_event_name(pAny->type));
		}

		/* flush data to terminal */
		fflush(stdout);
	//}
	}


}

void LRInputWacomX::update(){

	process_events();
}
void LRInputWacomX::handle_event(){


}


#endif // LR_ENABLE_WACOM_X


#endif

