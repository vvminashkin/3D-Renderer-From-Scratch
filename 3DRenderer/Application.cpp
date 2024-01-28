#include "Application.h"
#include "Controller/Controller.h"
#include <iostream>

void application::Application::Run() {
    model::Model model;
    view::View view;
    controller::Controller controller;
    model.Subscribe(controller.GetRecievePort());
}

void application::HandleException() {

    try {
        throw;
    } catch (std::exception& e) {
        std::cerr << "Application failed with exception:" << e.what();

    } catch (...) {
        std::cerr << "Application failed with unknown exception";
    }
}
