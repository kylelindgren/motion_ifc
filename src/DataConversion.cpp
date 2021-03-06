#include <motion_ifc/DataConversion.h>

///
/// \brief DataConversion::DataConversion
///
DataConversion::DataConversion(){

}

///
/// \brief DataConversion::resize
/// \param size
///
void DataConversion::resize_ss(const uint &size){
    if(pva.x.rows() != size){
        pva.x.resize(size);
        pva.dx.resize(size);
        pva.ddx.resize(size);
    }
}

///
/// \brief DataConversion::set_x
/// \param x
///
void DataConversion::set_x(VectorXd &x){
    resize_ss(x.rows());
    pva.x = x;
}

///
/// \brief DataConversion::set_dx
/// \param dx
///
void DataConversion::set_dx(VectorXd &dx){
    resize_ss(dx.rows());
    pva.dx = dx;
}

///
/// \brief DataConversion::set_ddx
/// \param ddx
///
void DataConversion::set_ddx(VectorXd &ddx){
    resize_ss(ddx.rows());
    pva.ddx = ddx;
}

template<>
///
/// \brief DataConversion::serialize<sensor_msgs::JointState>
/// \param data
/// \return
///
StateSpace* DataConversion::serialize<sensor_msgs::JointState>(sensor_msgs::JointState &data){
    resize_ss(data.position.size());
    for (int i = 0 ; i < data.position.size(); i++){
        pva.x[i] = data.position[i];
        pva.dx[i] = 0;
        pva.ddx[i] = 0;
    }
    return &pva;
}

template<>
///
/// \brief DataConversion::serialize<std::vector<double> >
/// \param data
/// \return
///
StateSpace* DataConversion::serialize<std::vector<double> >(std::vector<double> &data){
    resize_ss(data.size());
    for (int i = 0 ; i < data.size(); i++){
        pva.x[i] = data[i];
        pva.dx[i] = 0;
        pva.ddx[i] = 0;
    }
    return &pva;
}

template<>
///
/// \brief DataConversion::serialize<geometry_msgs::TransformStamped>
/// \param data
/// \return
///
StateSpace* DataConversion::serialize<geometry_msgs::TransformStamped>(geometry_msgs::TransformStamped &data){
    tf::Quaternion quat;
    tf::Matrix3x3 mat;
    tf::quaternionMsgToTF(data.transform.rotation, quat);
    mat.setRotation(quat);
    double r, p, y;
    mat.getRPY(r, p, y);
    resize_ss(6);
    pva.x << data.transform.translation.x, data.transform.translation.y, data.transform.translation.z, r, p, y;
    pva.dx << 0, 0, 0, 0, 0, 0;
    pva.ddx << 0, 0, 0, 0, 0, 0;
    return &pva;
}

template<>
///
/// \brief DataConversion::serialize<geometry_msgs::PoseStamped>
/// \param data
/// \return
///
StateSpace* DataConversion::serialize<geometry_msgs::PoseStamped>(geometry_msgs::PoseStamped &data){
    tf::Quaternion quat;
    tf::Matrix3x3 mat;
    tf::quaternionMsgToTF(data.pose.orientation, quat);
    mat.setRotation(quat);
    double r, p, y;
    mat.getRPY(r, p, y);
    resize_ss(6);
    pva.x << data.pose.position.x, data.pose.position.y, data.pose.position.z, r, p, y;
    pva.dx << 0, 0, 0, 0, 0, 0;
    pva.ddx << 0, 0, 0, 0, 0, 0;
    return &pva;
}

template<>
///
/// \brief DataConversion::serialize<geometry_msgs::Pose>
/// \param data
/// \return
///
StateSpace* DataConversion::serialize<geometry_msgs::Pose>(geometry_msgs::Pose &data){
    tf::Quaternion quat;
    tf::Matrix3x3 mat;
    tf::quaternionMsgToTF(data.orientation, quat);
    mat.setRotation(quat);
    double r, p, y;
    mat.getRPY(r, p, y);
    resize_ss(6);
    pva.x << data.position.x, data.position.y, data.position.z, r, p, y;
    pva.dx << 0, 0, 0, 0, 0, 0;
    pva.ddx << 0, 0, 0, 0, 0, 0;
    return &pva;
}

template<>
///
/// \brief DataConversion::deserialize<sensor_msgs::JointState>
/// \param data
/// \param pva
///
void DataConversion::deserialize<sensor_msgs::JointState>(sensor_msgs::JointState *data, StateSpace* pva){
    data->position.resize(pva->x.rows());
    for (int i = 0 ; i < pva->x.rows(); i++){
        data->position[i] = pva->x[i];
    }
    data->velocity.resize(pva->dx.rows());
    for (int i = 0 ; i < pva->dx.rows(); i++){
        data->velocity[i] = pva->dx[i];
    }
}

template<>
///
/// \brief DataConversion::deserialize<std::vector<double> >
/// \param data
/// \param pva
///
void DataConversion::deserialize<std::vector<double> >(std::vector<double> *data, StateSpace* pva){
    data->resize(pva->x.rows());
    for (int i = 0 ; i < pva->x.rows(); i++){
        data->at(i) = pva->x[i];
    }
}

template<>
///
/// \brief DataConversion::deserialize<geometry_msgs::TransformStamped>
/// \param data
/// \param pva
///
void DataConversion::deserialize<geometry_msgs::TransformStamped>(geometry_msgs::TransformStamped *data, StateSpace* pva){
    data->transform.translation.x = pva->x[0];
    data->transform.translation.y = pva->x[1];
    data->transform.translation.z = pva->x[2];
    tf::Quaternion quat;
    quat.setRPY(pva->x[3], pva->x[4], pva->x[5]);
    tf::quaternionTFToMsg(quat, data->transform.rotation);
}

template<>
///
/// \brief DataConversion::deserialize<geometry_msgs::PoseStamped>
/// \param data
/// \param pva
///
void DataConversion::deserialize<geometry_msgs::PoseStamped>(geometry_msgs::PoseStamped *data, StateSpace* pva){
    data->pose.position.x = pva->x[0];
    data->pose.position.y = pva->x[1];
    data->pose.position.z = pva->x[2];
    tf::Quaternion quat;
    quat.setRPY(pva->x[3], pva->x[4], pva->x[5]);
    tf::quaternionTFToMsg(quat, data->pose.orientation);
}

template<>
///
/// \brief DataConversion::deserialize<geometry_msgs::Pose>
/// \param data
/// \param pva
///
void DataConversion::deserialize<geometry_msgs::Pose>(geometry_msgs::Pose *data, StateSpace* pva){
    data->position.x = pva->x[0];
    data->position.y = pva->x[1];
    data->position.z = pva->x[2];
    tf::Quaternion quat;
    quat.setRPY(pva->x[3], pva->x[4], pva->x[5]);
    tf::quaternionTFToMsg(quat, data->orientation);
}
