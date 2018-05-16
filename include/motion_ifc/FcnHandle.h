#ifndef FCN_HANDLE_H
#define FCN_HANDLE_H
#include <motion_ifc/crtkCommon.h>

class FcnHandleBase{
public:
    FcnHandleBase(){}
    void set();
    virtual void operator()(void){}
    virtual void operator()(_cp_data_type&){}
    virtual void operator()(_jp_data_type&){}
};

template<typename D>
class FcnHandle: public FcnHandleBase{
public:
    FcnHandle():_is_set(false){}
    template<typename C>
    FcnHandle(void (C::*method)(D&), C *obj):_is_set(false){
        fcn_handle = boost::bind(method, obj, _1);
    }
    ~FcnHandle(){}

    template<typename C>
    void assign_fcn(void (C::*method)(D&), C *obj){
        fcn_handle = boost::bind(method, obj, _1);
        _is_set = true;
    }

    void operator()(D& data){
        fcn_handle(data);
    }

    boost::function<void (D& data)> fcn_handle;
    bool _is_set;
};


#endif
