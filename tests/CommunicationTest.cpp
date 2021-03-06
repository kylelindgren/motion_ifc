#include <motion_ifc/MotionCmd.h>
#include <motion_ifc/RobotCmd.h>
#include <motion_ifc/RobotState.h>
#include <motion_ifc/DataConversion.h>

int main(){
    CommunicationIfc commIfc;
    CommBasePtr interpolte_cp_ifc = commIfc.create_communication_interface("/motion_ifc/servo_cp", OUTGOING);
    CommBasePtr interpolte_jp_ifc = commIfc.create_communication_interface("/motion_ifc/interpolate_jp", INCOMING);

    _cp_data_type cp_data;
    _jp_data_type jp_data;

    DataConversion dataConverter;

    while (ros::ok()){
        ros::spinOnce();
        usleep(100000);
        interpolte_cp_ifc->get_data(cp_data);
        interpolte_cp_ifc->set_data(cp_data);
        dataConverter.serialize(cp_data);
        std::cout << dataConverter.get_x() << std::endl;
        interpolte_jp_ifc->get_data(jp_data);
        dataConverter.serialize(jp_data);
        std::cout << dataConverter.get_x() << std::endl;
    }

    return 0;
}
