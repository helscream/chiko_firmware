#ifndef __CHIKO_PAGE_CONFIGS__
#define __CHIKO_PAGE_CONFIGS__

#include "chiko_displayUI.h"
#include "chiko_joint.h"

const char LOG_TAG_[] = "PAGE_CONFIGS";

void pages::jointCalibrationPage_setup (uint8_t PN){
    SelectedJoint SJ = NONE_SELECTED;
    switch (PN)
    {
    case LEFTLEG_CALIB_PN:
        SJ = LEFTLEG;
        break;
    case LEFTFOOT_CALIB_PN:
        SJ = LEFTFOOT;
        break;
    case RIGHTLEG_CALIB_PN:
        SJ = RIGHTLEG;
        break;
    case RIGHTFOOT_CALIB_PN:
        SJ = RIGHTFOOT;
        break;
    default:
        SJ = NONE_SELECTED;
        break;
    }
    
  strcpy(_pageConfigs[PN].btnFunName, "-");
  strcpy(_pageConfigs[PN].btnPwName, "+");
  _pageConfigs[PN].taskBarVisibility = true;
  _pageConfigs[PN].btnFunVisibility = true;
  _pageConfigs[PN].btnPwVisibility = true;
  _pageConfigs[PN].btnRstVisibility = true;
  _pageConfigs[PN].btnFunClickAction = [this, SJ]() {
            setJointOffset(SJ,getJointOffset(SJ)-1);
            updatePage(true);
        };
  _pageConfigs[PN].btnPwClickAction = [this, SJ]() {
            setJointOffset(SJ,getJointOffset(SJ)+1);
            updatePage(true);
        };
}


void pages::jointCalibrationPage_update (uint8_t PN){
    _title.create(_display,"Joint Calibration",MIDTOP,64,14);
    _textField.create(_display,36,"Offset: ","0");
    _textField.setOuterPadding(10,5,false);
    SelectedJoint SJ = NONE_SELECTED;
    switch (PN)
    {
    case LEFTLEG_CALIB_PN:
        SJ = LEFTLEG;
        _title.create(_display,"Left Leg Joint:",LEFTTOP,10,26);
        break;
    case LEFTFOOT_CALIB_PN:
        _title.create(_display,"Left Foot Joint:",LEFTTOP,10,26);
        SJ = LEFTFOOT;
        break;
    case RIGHTLEG_CALIB_PN:
        _title.create(_display,"Right Leg Joint:",LEFTTOP,10,26);
        SJ = RIGHTLEG;
        break;
    case RIGHTFOOT_CALIB_PN:
        _title.create(_display,"Right Foot Joint:",LEFTTOP,10,26);
        SJ = RIGHTFOOT;
        break;
    default:
        SJ = NONE_SELECTED;
        break;
    }    
    char temp[8];
    sprintf(temp,"%d deg",(int)getJointOffset(SJ));
    _textField.setText(temp,true); 
}

void pages::pageSetup(){

  // Face page configuration
  _pageConfigs[FACE_PN].taskBarVisibility = false;
  _pageConfigs[FACE_PN].btnFunVisibility = false;
  _pageConfigs[FACE_PN].btnPwVisibility = false;
  _pageConfigs[FACE_PN].btnRstVisibility = false;

  // Joint Page configuration
  if(areJointsActive()){
        strcpy(_pageConfigs[JOINT_ENABLE_PN].btnFunName, "Enable");
    }else{
        strcpy(_pageConfigs[JOINT_ENABLE_PN].btnFunName, "Disable");
    }
  _pageConfigs[JOINT_ENABLE_PN].taskBarVisibility = true;
  _pageConfigs[JOINT_ENABLE_PN].btnFunVisibility = true;
  _pageConfigs[JOINT_ENABLE_PN].btnPwVisibility = false;
  _pageConfigs[JOINT_ENABLE_PN].btnRstVisibility = true;
  _pageConfigs[JOINT_ENABLE_PN].btnFunClickAction = [this]() {
    if(areJointsActive()){
        disable_joints();
        strcpy(_pageConfigs[JOINT_ENABLE_PN].btnFunName, "Enable");
    }else{
        enable_joints();
        strcpy(_pageConfigs[JOINT_ENABLE_PN].btnFunName, "Disable");
    }
    updatePage(true);
  };

  // Left Joint Calibration
  jointCalibrationPage_setup(LEFTLEG_CALIB_PN);
  jointCalibrationPage_setup(LEFTFOOT_CALIB_PN);
  jointCalibrationPage_setup(RIGHTLEG_CALIB_PN);
  jointCalibrationPage_setup(RIGHTFOOT_CALIB_PN);

  // Battery Infomation page setup
  _pageConfigs[BATTERY_INFO_PN].taskBarVisibility = true;
  _pageConfigs[BATTERY_INFO_PN].btnFunVisibility = false;
  _pageConfigs[BATTERY_INFO_PN].btnPwVisibility = false;
  _pageConfigs[BATTERY_INFO_PN].btnRstVisibility = false;

  // WiFi Infomation page setup
  _pageConfigs[WIFI_INFO_PN].taskBarVisibility = true;
  _pageConfigs[WIFI_INFO_PN].btnFunVisibility = true;
  _pageConfigs[WIFI_INFO_PN].btnPwVisibility = false;
  _pageConfigs[WIFI_INFO_PN].btnRstVisibility = true;

  // System Clock page setup
  _pageConfigs[CLOCK_PN].taskBarVisibility = true;
  _pageConfigs[CLOCK_PN].btnFunVisibility = false;
  _pageConfigs[CLOCK_PN].btnPwVisibility = false;
  _pageConfigs[CLOCK_PN].btnRstVisibility = true;
  
}


void pages::printPageInfo(){
  switch (_currentPage)
  {
  case JOINT_ENABLE_PN:
    _jointCheckBox.create(_display,"Enable Joints",MIDDLE,64,25);
    _jointCheckBox.setChecked(areJointsActive()); // Check if Joints are active
    _jointCheckBox.update();
    break;
  case LEFTLEG_CALIB_PN:
  case LEFTFOOT_CALIB_PN:
  case RIGHTLEG_CALIB_PN:
  case RIGHTFOOT_CALIB_PN:
    jointCalibrationPage_update(_currentPage);
    break;
  case BATTERY_INFO_PN:
    break;

  case WIFI_INFO_PN:
    break;
  case BLUETOOTH_INFO_PN:
    break;
  case ALARM_INFO_PN:
    break;
  default:
    break;
  }
}




#endif