#include"Pair.h"

Pair::Pair(const Z_info& argu_z, const Y_info& argu_y)
{
	z_pair = argu_z;
	y_pair = argu_y;
}

//log
/*ostream& operator<<(ostream& os, Pair& p)
{
	os << "User: " << p.z_pair.z_index << endl;
	os << p.z_pair.radio_condition << " | " << p.z_pair.video_duration << " | " << p.z_pair.z_type << endl;
	os << p.z_pair.flow_number_in_cell << ": \t";
	for (int i_pair = 0; i_pair < number_of_CQI; i_pair++)
		os << p.z_pair.flow_number_in_cell_eachregion[i_pair] << "\t";
	os << endl;
	os << p.z_pair.flow_number_in_starvation << ": \t";
	for (int i_pair = 0; i_pair < number_of_CQI; i_pair++)
		os << p.z_pair.flow_number_in_starvation_eachregion[i_pair] << "\t";
	os << endl;
	os << p.y_pair.y_value << endl;
	os << "time in cell: " << p.y_pair.y_time_along << endl;

	return os;
}*/


//原文14
/*ostream& operator<<(ostream& os, Pair& p)
{
	os << p.y_pair.y_value << " ";
	os << "1:" << p.z_pair.radio_condition << " ";
	os << "2:" << p.z_pair.video_duration << " ";
	os << "3:" << p.z_pair.flow_number_in_cell << " ";
	for (int i_pair = 0; i_pair < number_of_CQI; i_pair++)
		os << i_pair + 4 << ":" << p.z_pair.flow_number_in_cell_eachregion[i_pair] << " ";
	os << "9:" << p.z_pair.flow_number_in_starvation << " ";
	for (int i_pair = 0; i_pair < number_of_CQI; i_pair++)
		os << i_pair + 10 << ":" << p.z_pair.flow_number_in_starvation_eachregion[i_pair] << " ";
	os << endl;

	return os;
}*/


//新加的7
/*ostream& operator<<(ostream& os, Pair& p)
{
	os << p.y_pair.y_value << " ";
	//os << "1:" << p.z_pair.radio_condition << " ";
	//os << "2:" << p.z_pair.video_duration << " ";
	//os << "2:" << p.z_pair.flow_number_in_cell << " ";
	//for (int i_pair = 0; i_pair < number_of_CQI; i_pair++)
	//	os << i_pair + 4 << ":" << p.z_pair.flow_number_in_cell_eachregion[i_pair] << " ";
	//os << "9:" << p.z_pair.flow_number_in_starvation << " ";
	//for (int i_pair = 0; i_pair < number_of_CQI; i_pair++)
	//	os << i_pair + 10 << ":" << p.z_pair.flow_number_in_starvation_eachregion[i_pair] << " ";
	os << "1:" << p.z_pair_value().num_to_starvation << " ";
	os << "2:" << p.z_pair_value().feature_time_to_stay << " ";
	os << "3:" << p.z_pair_value().feature_throughput_in_cell << " ";
	os << "4:" << p.z_pair_value().num_static_in_cell << " ";
	os << "5:" << p.z_pair_value().num_mobile_in_cell << " ";
	os << "6:" << p.z_pair_value().num_fixed_in_cell << " ";
	os << "7:" << p.z_pair_value().num_adaptive_in_cell << " ";

	os << endl;

	return os;
}*/



//14 + 7
/*ostream& operator<<(ostream& os, Pair& p)
{
	os << p.y_pair.y_value << " ";
	os << "1:" << p.z_pair.radio_condition << " ";
	os << "2:" << p.z_pair.video_duration << " ";
	os << "3:" << p.z_pair.flow_number_in_cell << " ";
	for (int i_pair = 0; i_pair < number_of_CQI; i_pair++)
		os << i_pair + 4 << ":" << p.z_pair.flow_number_in_cell_eachregion[i_pair] << " ";
	os << "9:" << p.z_pair.flow_number_in_starvation << " ";
	for (int i_pair = 0; i_pair < number_of_CQI; i_pair++)
		os << i_pair + 10 << ":" << p.z_pair.flow_number_in_starvation_eachregion[i_pair] << " ";
	os << "15:" << p.z_pair_value().num_to_starvation << " ";
	os << "16:" << p.z_pair_value().feature_time_to_stay << " ";
	os << "17:" << p.z_pair_value().feature_throughput_in_cell << " ";
	os << "18:" << p.z_pair_value().num_static_in_cell << " ";
	os << "19:" << p.z_pair_value().num_mobile_in_cell << " ";
	os << "20:" << p.z_pair_value().num_fixed_in_cell << " ";
	os << "21:" << p.z_pair_value().num_adaptive_in_cell << " ";

	os << endl;

	return os;
}*/

//R+N+3
/*ostream& operator<<(ostream& os, Pair& p)
{
	os << p.y_pair.y_value << " ";
	os << "1:" << p.z_pair.radio_condition << " ";
	os << "2:" << p.z_pair.flow_number_in_cell << " ";
	os << "3:" << p.z_pair_value().num_to_starvation << " ";
	os << "4:" << p.z_pair_value().feature_time_to_stay << " ";
	os << "5:" << p.z_pair_value().feature_throughput_in_cell << " ";

	os << endl;

	return os;
}*/

//回归
/*ostream& operator<<(ostream& os, Pair& p)
{
	os << p.y_pair_value().y_num_starvation << " ";
	os << "1:" << p.z_pair.radio_condition << " ";
	os << "2:" << p.z_pair.video_duration << " ";
	os << "3:" << p.z_pair.flow_number_in_cell << " ";
	for (int i_pair = 0; i_pair < number_of_CQI; i_pair++)
		os << i_pair + 4 << ":" << p.z_pair.flow_number_in_cell_eachregion[i_pair] << " ";
	os << "9:" << p.z_pair.flow_number_in_starvation << " ";
	for (int i_pair = 0; i_pair < number_of_CQI; i_pair++)
		os << i_pair + 10 << ":" << p.z_pair.flow_number_in_starvation_eachregion[i_pair] << " ";
	os << endl;

	return os;
}*/

//label 分类
	/*//14, starvation_frequency_label
ostream& operator<<(ostream& os, Pair& p)
{
	os << p.y_pair.y_starvation_frequency_label << " ";
	os << "1:" << p.z_pair.radio_condition << " ";
	os << "2:" << p.z_pair.video_duration << " ";
	os << "3:" << p.z_pair.flow_number_in_cell << " ";
	for (int i_pair = 0; i_pair < number_of_CQI; i_pair++)
		os << i_pair + 4 << ":" << p.z_pair.flow_number_in_cell_eachregion[i_pair] << " ";
	os << "9:" << p.z_pair.flow_number_in_starvation << " ";
	for (int i_pair = 0; i_pair < number_of_CQI; i_pair++)
		os << i_pair + 10 << ":" << p.z_pair.flow_number_in_starvation_eachregion[i_pair] << " ";
	os << endl;

	return os;
}*/

	//14, average_bitrate_label
/*ostream& operator<<(ostream& os, Pair& p)
{
	os << p.y_pair.y_average_bitrate_label << " ";
	os << "1:" << p.z_pair.radio_condition << " ";
	os << "2:" << p.z_pair.video_duration << " ";
	os << "3:" << p.z_pair.flow_number_in_cell << " ";
	for (int i_pair = 0; i_pair < number_of_CQI; i_pair++)
		os << i_pair + 4 << ":" << p.z_pair.flow_number_in_cell_eachregion[i_pair] << " ";
	os << "9:" << p.z_pair.flow_number_in_starvation << " ";
	for (int i_pair = 0; i_pair < number_of_CQI; i_pair++)
		os << i_pair + 10 << ":" << p.z_pair.flow_number_in_starvation_eachregion[i_pair] << " ";
	os << endl;

	return os;
}*/

	/*//14, frequency_switch_label
ostream& operator<<(ostream& os, Pair& p)
{
	os << p.y_pair.y_frequency_bitrate_switch_label << " ";
	os << "1:" << p.z_pair.radio_condition << " ";
	os << "2:" << p.z_pair.video_duration << " ";
	os << "3:" << p.z_pair.flow_number_in_cell << " ";
	for (int i_pair = 0; i_pair < number_of_CQI; i_pair++)
		os << i_pair + 4 << ":" << p.z_pair.flow_number_in_cell_eachregion[i_pair] << " ";
	os << "9:" << p.z_pair.flow_number_in_starvation << " ";
	for (int i_pair = 0; i_pair < number_of_CQI; i_pair++)
		os << i_pair + 10 << ":" << p.z_pair.flow_number_in_starvation_eachregion[i_pair] << " ";
	os << endl;

	return os;
}*/

	//三种metrics
ostream& operator<<(ostream& os, Pair& p)
{
	os << p.z_pair.radio_condition << ",";
	os << p.z_pair.video_duration << ",";
	os << p.z_pair.flow_number_in_cell << ",";
	for (int i_pair = 0; i_pair < number_of_CQI; i_pair++)
		os << p.z_pair.flow_number_in_cell_eachregion[i_pair] << ",";
	os << p.z_pair.flow_number_in_starvation << ",";
	for (int i_pair = 0; i_pair < number_of_CQI; i_pair++)
		os << p.z_pair.flow_number_in_starvation_eachregion[i_pair] << ",";
	

	if (p.y_pair_value().y_value == 1)
		os << "yes" << ",";
	else
		os << "no" << ",";
	os << p.y_pair.y_num_starvation << ",";
	os << p.y_pair.y_starvation_frequency << ",";
	os << p.y_pair.y_starvation_frequency_label << ",";
	os << p.y_pair.y_average_bitrate << ",";
	os << p.y_pair.y_average_bitrate_label << ",";
	os << p.y_pair.y_frequency_bitrate_switch << ",";
	os << p.y_pair.y_frequency_bitrate_switch_label;

	os << endl;

	return os;
}