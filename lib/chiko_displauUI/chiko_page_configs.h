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
  _pageConfigs[PN].taskBarVisibility = false;
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
    _title.create(_display,"Joint Calibration",MIDTOP,64,2);
    _textField.create(_display,24,"Offset: ","0");
    _textField.setOuterPadding(10,5,false);
    SelectedJoint SJ = NONE_SELECTED;
    switch (PN)
    {
    case LEFTLEG_CALIB_PN:
        SJ = LEFTLEG;
        _title.create(_display,"Left Leg Joint:",LEFTTOP,10,14);
        break;
    case LEFTFOOT_CALIB_PN:
        _title.create(_display,"Left Foot Joint:",LEFTTOP,10,14);
        SJ = LEFTFOOT;
        break;
    case RIGHTLEG_CALIB_PN:
        _title.create(_display,"Right Leg Joint:",LEFTTOP,10,14);
        SJ = RIGHTLEG;
        break;
    case RIGHTFOOT_CALIB_PN:
        _title.create(_display,"Right Foot Joint:",LEFTTOP,10,14);
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
        disable_joints();
        strcpy(_pageConfigs[JOINT_ENABLE_PN].btnFunName, "Enable");
    }else{
        enable_joints();
        strcpy(_pageConfigs[JOINT_ENABLE_PN].btnFunName, "Disable");
    }
  _pageConfigs[JOINT_ENABLE_PN].taskBarVisibility = false;
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
//   strcpy(_pageConfigs[LEFTLEFT_CALIB_PN].btnFunName, "-");
//   strcpy(_pageConfigs[LEFTLEFT_CALIB_PN].btnPwName, "+");
//   _pageConfigs[LEFTLEFT_CALIB_PN].taskBarVisibility = false;
//   _pageConfigs[LEFTLEFT_CALIB_PN].btnFunVisibility = true;
//   _pageConfigs[LEFTLEFT_CALIB_PN].btnPwVisibility = true;
//   _pageConfigs[LEFTLEFT_CALIB_PN].btnRstVisibility = true;
//   _pageConfigs[LEFTLEFT_CALIB_PN].btnFunClickAction = [this]() {
//             setJointOffset(LEFTLEG,getJointOffset(LEFTLEG)+1);
//             updatePage(true);
//         };
//   _pageConfigs[LEFTLEFT_CALIB_PN].btnPwClickAction = [this]() {
//             setJointOffset(LEFTLEG,getJointOffset(LEFTLEG)-1);
//             updatePage(true);
//         };
}


void pages::printPageInfo(){
  switch (_currentPage)
  {
  case JOINT_ENABLE_PN:
    _jointCheckBox.create(_display,"Enable Joints",MIDDLE,64,20);
    _jointCheckBox.setChecked(areJointsActive()); // Check if Joints are active
    _jointCheckBox.update();
    break;
  case LEFTLEG_CALIB_PN:
  case LEFTFOOT_CALIB_PN:
  case RIGHTLEG_CALIB_PN:
  case RIGHTFOOT_CALIB_PN:
    jointCalibrationPage_update(_currentPage);
    // _title.create(_display,"Joint Calibration",MIDTOP,64,2);
    // _title.create(_display,"Left Leg Joint:",LEFTTOP,10,14);
    // _textField.create(_display,24,"Offset: ","0");
    // _textField.setOuterPadding(10,5,false);
    // char temp[8];
    // sprintf(temp,"%d deg",(int)getJointOffset(LEFTLEG));
    // _textField.setText(temp,true); 
    // _title.update();
    break;
  default:
    break;
  }
}




#endif