/**
 * @file chiko_displayUI.cpp
 * @brief Implements the display UI functionalities for Chiko robot, managing pages and button interactions.
 *
 * This file provides the implementation for the 'pages' class, which handles the creation,
 * navigation, and rendering of different display pages on a U8G2-compatible display.
 * It also manages the display of a taskbar, power, reset, and function buttons,
 * and their associated actions.
 */

#include "chiko_displayUI.h"
#include "chiko_page_configs.h"

// Define a tag for logging within this file
const char LOG_TAG[] =  "PAGES";



// =======================================================================================
// Private Task Functions
// =======================================================================================

/**
 * @brief FreeRTOS task to continuously update the Face page.
 *
 * This task runs on a separate core and calls the updatePage() method of the
 * 'pages' object at a regular interval (10ms delay). This ensures the display
 * is refreshed and reflects the current UI state.
 *
 * @param param A pointer to the 'pages' object instance that this task will operate on.
 */
void eyesUpdateTask(void *param) {
  pages *page;
  page = (pages*)param; // Cast the parameter back to a 'pages' object pointer
  initialize_face(page->getDisplayHandel());
  while (page->getSelectedPageNumber() == FACE_PN) {
    
    if (digitalRead(POWER_BUTTON_PIN) || !digitalRead(FUNCTION_BUTTON_PIN)){
      eyes_happy();
    }else if(page->getEyesScanDirection() == LOOK_LEFT){
      eyes_move_left_big();
      delay(EYE_SCAN_DELAY);
      page->setEyesScanDirection(LOOK_STRIGHT);
    }else if(page->getEyesScanDirection() == LOOK_RIGHT){
      eyes_move_right_big();
      delay(EYE_SCAN_DELAY);
      page->setEyesScanDirection(LOOK_STRIGHT);
    }else if(page->getEyesScanDirection() == LOOK_UP){
      eyes_saccade(0,-1);
      delay(EYE_SCAN_DELAY);
      page->setEyesScanDirection(LOOK_STRIGHT);
    }else if(page->getEyesScanDirection() == LOOK_DOWN){
      eyes_saccade(0,1);
      delay(EYE_SCAN_DELAY);
      page->setEyesScanDirection(LOOK_STRIGHT);
    }else if(random(1, 100) > 97){
      eyes_blink(12);
    }else{
      eyes_reset(true);
    }
    delay(100);         // Delay for 10 milliseconds to control update rate
  }
  page->updatePage();
  vTaskDelete(NULL); // Should theoretically not be reached, but good practice
}


void pages::drawFace(){
   // Create a FreeRTOS task to handle continuous page updates
  xTaskCreatePinnedToCore(
      eyesUpdateTask,     // Function that implements the task
      "EyesUpdateTask",   // Name of the task for debugging
      10000,              // Stack size in words (e.g., 10KB)
      this,               // Parameter to pass to the task (this 'pages' object instance)
      2,                  // Priority of the task (higher number = higher priority)
      NULL,               // Task handle (not used here, so NULL)
      1);                 // Core where the task should run (Core 1)
}

// =======================================================================================
// pages Class Implementation
// =======================================================================================

U8G2 pages::getDisplayHandel(){
  return _display;
}


/**
 * @brief Initializes the pages management system.
 *
 * Sets up the display, the number of available pages, and creates a FreeRTOS task
 * for continuous page updates.
 *
 * @param display A reference to the U8G2 display object to be used for rendering.
 * @param pageCount The total number of pages to be managed by the UI.
 */
void pages::intilize(U8G2 &display) {
  _pageCount = NUMBER_OF_PAGES;   // Store the total number of pages
  _display = display;       // Store the U8G2 display object reference
  _pageTaskBar.create(_display, _pageCount); // Initialize the taskbar for page navigation
  
  // Configuering All pages
  pageSetup();

  gotoDefaultPage();
}

/**
 * @brief Sets the configuration for a specific page.
 *
 * @param pageNumber The index of the page to configure.
 * @param config The pageConfig structure containing visibility and action settings for the page.
 */
void pages::setPageConfig(uint8_t pageNumber, pageConfig config) {
  // Ensure the page number is within valid bounds before setting the configuration
  if (pageNumber >= (uint8_t)0 && pageNumber < _pageCount) {
    _pageConfigs[pageNumber] = config;
  } else {
    chikoLog(LOG_TAG, "Attempted to set config for invalid page number: %d", pageNumber);
  }
}

/**
 * @brief Retrieves the configuration for a specific page.
 *
 * @param pageNumber The index of the page to retrieve the configuration for.
 * @return The pageConfig structure for the specified page.
 */
pageConfig pages::getPageConfig(uint8_t pageNumber) {
  // Ensure the page number is within valid bounds before returning the configuration
  if (pageNumber >= (uint8_t)0 && pageNumber < _pageCount) {
    return _pageConfigs[pageNumber];
  } else {
    chikoLog(LOG_TAG, "Attempted to get config for invalid page number: %d", pageNumber);
    // Return a default or empty config to prevent crashes, or handle error appropriately
    return pageConfig();
  }
}

/**
 * @brief Gets the number of the currently selected page.
 *
 * @return The 0-indexed number of the current page.
 */
uint8_t pages::getSelectedPageNumber() {
  return _currentPage;
}

/**
 * @brief Gets the total count of pages configured in the UI.
 *
 * @return The total number of pages.
 */
uint8_t pages::getPageCount() {
  return _pageCount;
}

/**
 * @brief Navigates to the next page in the sequence.
 *        If currently on the last page, it wraps around to the first page.
 */
void pages::gotoNextPage() {
  uint8_t currentPage = getSelectedPageNumber();
  currentPage++; // Increment current page number
  if (currentPage >= _pageCount) { // Check if it exceeds the last page
    _currentPage = 0; // Wrap around to the first page
  } else {
    _currentPage = currentPage; // Go to the next page
  }
  updatePage();
  chikoLog(LOG_TAG, "Navigated to next page: %d", _currentPage);
}

/**
 * @brief Navigates to the previous page in the sequence.
 *        If currently on the first page, it wraps around to the last page.
 */
void pages::gotoPreviousPage() {
  uint8_t currentPage = getSelectedPageNumber();
  if (currentPage == 0) { // Check if it's the first page
    _currentPage = _pageCount - 1; // Wrap around to the last page
  } else {
    currentPage--; // Decrement current page number
    _currentPage = currentPage; // Go to the previous page
  }
  updatePage();
  chikoLog(LOG_TAG, "Navigated to previous page: %d", _currentPage);
}

/**
 * @brief Navigates directly to a specified page number.
 *        If the page number is invalid, it defaults to the first page (0).
 *
 * @param pageNumber The 0-indexed number of the page to navigate to.
 */
void pages::gotoPageNumber(uint8_t pageNumber) {
  // Validate the requested page number
  if (pageNumber < _pageCount) {
    _currentPage = pageNumber; // Set the current page to the specified number
    chikoLog(LOG_TAG, "Navigated to page number: %d", _currentPage);
  } else {
    _currentPage = 0; // Default to the first page if the number is out of bounds
    chikoLog(LOG_TAG, "Invalid page number %d, defaulting to page 0.", pageNumber);
  }
  updatePage();
}

/**
 * @brief Navigates to the default page, which is typically the first page (index 0).
 */
void pages::gotoDefaultPage() {
  _currentPage = 0; // Set the current page to the first page
  updatePage();
  chikoLog(LOG_TAG, "Navigated to default page: %d", _currentPage);
}

/**
 * @brief Updates the display to show the current page and its associated UI elements.
 *
 * This function clears the display and redraws the taskbar, power, reset, and function
 * buttons based on the configuration of the currently selected page. It only redraws
 * if the page has changed or if a forceUpdate is requested.
 *
 * @param forceUpdate If true, the display will be updated even if the page hasn't changed.
 */
void pages::updatePage(bool forceUpdate) {
  // Only update the display if the page has changed or if a force update is requested
  if ((_previousPage != _currentPage && _currentPage != FACE_PN) || forceUpdate) {
    _display.clearDisplay(); // Clear the entire display buffer

    // Handle Task Bar visibility and update
    if (!_pageConfigs[_currentPage].taskBarVisibility) {
      _pageTaskBar.clear(); // Clear the taskbar if not visible
    } else {
      _pageTaskBar.select(_currentPage); // Highlight the current page in the taskbar
      _pageTaskBar.update();             // Redraw the taskbar
    }


    _power.create(_display, _pageConfigs[_currentPage].btnPwName, MIDBOTTOM, 64, 62);

    // Handle Power Button visibility and update
    if (!_pageConfigs[_currentPage].btnPwVisibility) {
      _power.clear(); // Clear the power button if not visible
    } else {
      // Create and update the power button with its configured name and position
      
      _power.update();
    }

    _reset.create(_display, _pageConfigs[_currentPage].btnRstName, RIGHTBOTTOM, 126, 62);
    // Handle Reset Button visibility and update
    if (!_pageConfigs[_currentPage].btnRstVisibility) {
      _reset.clear(); // Clear the reset button if not visible
    } else {
      // Create and update the reset button with its configured name and position
      _reset.update();
    }

    _fun.create(_display, _pageConfigs[_currentPage].btnFunName, LEFTBOTTOM, 2, 62);
    // Handle Function Button visibility and update
    if (!_pageConfigs[_currentPage].btnFunVisibility) {
      _fun.clear(); // Clear the function button if not visible
    } else {
      // Create and update the function button with its configured name and position
      _fun.update();
    }

    printPageInfo();
  
  }else if(_currentPage == FACE_PN){
    drawFace();
  }
  _previousPage = _currentPage; // Store the current page as previous for the next update cycle
}



/**
 * @brief Executes the action associated with the Function button for the current page.
 *        Logs a message if no action is assigned.
 */
void pages::btnFunctionAction() {
  // Check if a function click action is assigned for the current page
  if (_pageConfigs[_currentPage].btnFunClickAction != nullptr) {
    chikoLog(LOG_TAG_, "Executing Function button action linked to page # %d", _currentPage);
    _pageConfigs[_currentPage].btnFunClickAction(); // Execute the assigned action
  } else {
    chikoLog(LOG_TAG_, "No Function button action assigned for page # %d!", _currentPage);
  }
}

/**
 * @brief Executes the action associated with the Power button for the current page.
 *        Logs a message if no action is assigned.
 */
void pages::btnPowerAction() {
  // Check if a power button click action is assigned for the current page
  if (_pageConfigs[_currentPage].btnPwClickAction != nullptr) {
    chikoLog(LOG_TAG_, "Executing Power button action linked to page # %d", _currentPage);
    _pageConfigs[_currentPage].btnPwClickAction(); // Execute the assigned action
  } else {
    chikoLog(LOG_TAG_, "No Power button action assigned for page # %d!", _currentPage);
  }
}

/**
 * @brief Selects or deselects the Power button on the display.
 *        Only applies if the power button is configured to be visible.
 *
 * @param select True to select (highlight) the button, false to deselect.
 */
void pages::btnPowerSelect(bool select) {
  if (_pageConfigs[_currentPage].btnPwVisibility == true) {
    _power.select(select); // Call the select method of the power button object
  }
}

/**
 * @brief Selects or deselects the Function button on the display.
 *        Only applies if the function button is configured to be visible.
 *
 * @param select True to select (highlight) the button, false to deselect.
 */
void pages::btnFunctionSelect(bool select) {
  if (_pageConfigs[_currentPage].btnFunVisibility == true) {
    _fun.select(select); // Call the select method of the function button object
  }
}

void pages::setEyesScanDirection(eyesConfig ESD){
  _eyesScanDirection = ESD;
}

eyesConfig pages::getEyesScanDirection(){
  return _eyesScanDirection;
}

