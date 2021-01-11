
#ifndef __CONFIG_INFO_H__
#define __CONFIG_INFO_H__


#define INFO_STRING_LEN 15
#define INFO_NUM_LEN 6
#define INFO_LENGTH_SHIFT 2

typedef struct {
//general info
	char model[INFO_STRING_LEN+1];
	char serial_no[INFO_STRING_LEN+1];
	char vendor_name[INFO_STRING_LEN+1];
	char installation_date[INFO_STRING_LEN+1];
	int weight_max; //kg
	int range_max; //0.1m
	int error_ratio_convert; //kg
	int range_max_weight_max; //0.1m

//calibration
	int range_begin; //0.1m
	int range_end; //0.1m
	int height_begin; //0.1m
	int height_end; //0.1m
	int weight_calibration_zero; //kg
	int weight_calibration_standard; //kg
	int angle_begin; //deg
	int angle_end; //deg

//control
	int control_switch;
	int range_stop_out; //0.1m
	int range_stop_in; //0.1m
	int height_stop_up; //0.1m
	int height_stop_down; //0.1m
	int angle_stop_clockwise; //deg
	int angle_stop_anti_clockwise; //deg
	int angle_stop_pre; //deg

//communication
	char phone_num1[INFO_STRING_LEN+1];
	char phone_num2[INFO_STRING_LEN+1];
	char phone_num3[INFO_STRING_LEN+1];
	char address1[INFO_STRING_LEN+1];
	char address2[INFO_STRING_LEN+1];
	char address3[INFO_STRING_LEN+1];

//system info
	int password_menu;
	int password_setup;
	int user_defined_no;
	char date[INFO_STRING_LEN+1];
	char time[INFO_STRING_LEN+1];
	int pre_warning_ratio;
	int warning_ratio;
	int impacting_eliminate_time; //s
	int ratio;
	int cord_weight_per_meter;  //0.001kg 
	int cord_diameter;  //0.001m
	int cord_layer;
	int height_maintenance_interval;
	int range_maintenance_interval;
	int angle_maintenance_interval;

} CONFIG_INFO;


#endif /*__CONFIG_INFO_H__*/

